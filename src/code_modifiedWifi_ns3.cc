/*
*
*This code was based in the third.cc example presented by ns3 and add almost entirely the code wrote in the 
*article: 
*
*ERNST, J.B.; RODRIGUES, J.; KREMER, S. A Wi-Fi Simulation Model Which Supports Channel Scanning across Multiple
*Non-overlapping Channels in NS3. IEEE 28th International Conference on Advanced Information Networking and Applications. 2014
*
* As the third.cc example code has this GNU header, I think its important to reply it also in this code:
*
* This program is free software; you can redistribute it and/or modify
* it under the terms of the GNU General Public License version 2 as
* published by the Free Software Foundation;
*
* This program is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
* GNU General Public License for more details.
*
* You should have received a copy of the GNU General Public License
* along with this program; if not, write to the Free Software
* Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
*/

#include "ns3/core-module.h"
#include "ns3/point-to-point-module.h"
#include "ns3/network-module.h"
#include "ns3/applications-module.h"
#include "ns3/wifi-module.h"
#include "ns3/mobility-module.h"
#include "ns3/csma-module.h"
#include "ns3/internet-module.h"

using std::string;
using namespace ns3;

NS_LOG_COMPONENT_DEFINE ("EP");

int main (int argc, char *argv[]) {

CommandLine cmd;
  cmd.Parse (argc, argv);

Time::SetResolution (Time::NS);
LogComponentEnable ("UdpEchoClientApplication", LOG_LEVEL_INFO);
LogComponentEnable ("UdpEchoServerApplication", LOG_LEVEL_INFO);

NodeContainer clients;
NodeContainer aps;
  clients.Create(10);
  aps.Create(2);

Ssid ssid = Ssid ("default-ssid");

NqosWifiMacHelper wifiMac = NqosWifiMacHelper::Default();
  wifiMac.SetType("ns3::ApWifiMac", "Ssid", SsidValue(ssid));

YansWifiChannelHelper wifiChannel = YansWifiChannelHelper::Default();

YansWifiPhyHelper wifiPhy1 = YansWifiPhyHelper::Default();
  wifiPhy1.SetPcapDataLinkType(YansWifiPhyHelper::DLT_IEEE802_11_RADIO);
  wifiPhy1.SetChannel(wifiChannel.Create());

YansWifiPhyHelper wifiPhy2 = YansWifiPhyHelper::Default();
  wifiPhy2.SetPcapDataLinkType(YansWifiPhyHelper::DLT_IEEE802_11_RADIO);

WifiHelper wifiAP = WifiHelper::Default();
  string phyMode("DsssRate1Mbps");
  string phyModeFast("ErpOfdmRate54Mbps");

NetDeviceContainer apWifiDevs;
  wifiAP.SetStandard(WIFI_PHY_STANDARD_80211g);
  wifiAP.SetRemoteStationManager("ns3::ConstantRateWifiManager", "DataMode", StringValue(phyModeFast), "ControlMode", StringValue(phyMode));
  apWifiDevs.Add(wifiAP.Install(wifiPhy1, wifiMac,aps.Get(0)));

WifiHelper wifi = WifiHelper::Default();
wifi.SetStandard(WIFI_PHY_STANDARD_80211g);

NetDeviceContainer clientDevs;
  clientDevs.Add(wifi.Install(wifiPhy1, wifiMac,clients));
  clientDevs.Add(wifi.Install(wifiPhy2, wifiMac, clients));

for(unsigned int i=0; i < clientDevs.GetN()/2; i++){
  Ptr <WifiNetDevice> d1 = clientDevs.Get(i)->GetObject<WifiNetDevice>();
  Ptr <WifiNetDevice> d2 = clientDevs.Get(i + (clientDevs.GetN()/2))->GetObject<WifiNetDevice>();
  Ptr<StaWifiMac> mac1 = DynamicCast<StaWifiMac>(d1->GetMac());
  Ptr<StaWifiMac> mac2 = DynamicCast<StaWifiMac>(d2->GetMac());
}

MobilityHelper mobility;
  mobility.SetPositionAllocator ("ns3::GridPositionAllocator",
    "MinX", DoubleValue (0.0),
    "MinY", DoubleValue (0.0),
    "DeltaX", DoubleValue (5.0),
    "DeltaY", DoubleValue (10.0),
    "GridWidth", UintegerValue (3),
    "LayoutType", StringValue ("RowFirst"));
  
  mobility.SetMobilityModel ("ns3::RandomWalk2dMobilityModel",
    "Bounds", RectangleValue (Rectangle (-50, 50, -50, 50)));
  
  mobility.Install(clients);
  mobility.SetMobilityModel ("ns3::ConstantPositionMobilityModel");
  mobility.Install(aps);

InternetStackHelper stack;
  stack.Install (clients);
  stack.Install (aps);

Ipv4AddressHelper address;
  address.SetBase ("10.1.3.0", "255.255.255.0");

Ipv4InterfaceContainer interfaces_aps = address.Assign (apWifiDevs);
Ipv4InterfaceContainer interfaces_clients =  address.Assign (clientDevs);

UdpEchoServerHelper echoServer (9);

ApplicationContainer serverApps = echoServer.Install (aps.Get (0));
  serverApps.Start (Seconds (1.0));
  serverApps.Stop (Seconds (10.0));

UdpEchoClientHelper echoClient (interfaces_aps.GetAddress (0), 9);
  echoClient.SetAttribute ("MaxPackets", UintegerValue (1));
  echoClient.SetAttribute ("Interval", TimeValue (Seconds (1.0)));
  echoClient.SetAttribute ("PacketSize", UintegerValue (1024));

ApplicationContainer clientApps = echoClient.Install (clients.Get (0));
  serverApps.Start (Seconds (1.0));
  serverApps.Stop (Seconds (10.0));

UdpEchoClientHelper echoClient (interfaces_aps.GetAddress (0), 9);
  echoClient.SetAttribute ("MaxPackets", UintegerValue (1));
  echoClient.SetAttribute ("Interval", TimeValue (Seconds (1.0)));
  echoClient.SetAttribute ("PacketSize", UintegerValue (1024));

ApplicationContainer clientApps = echoClient.Install (clients.Get (0));
  clientApps.Start (Seconds (2.0));
  clientApps.Stop (Seconds (10.0));

Ipv4GlobalRoutingHelper::PopulateRoutingTables ();

Simulator::Stop (Seconds (10.0));
Simulator::Run ();
Simulator::Destroy ();

return 0;
}