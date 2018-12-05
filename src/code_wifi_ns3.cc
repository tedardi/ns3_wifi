#include "ns3/core-module.h"
#include "ns3/network-module.h"
#include "ns3/internet-module.h"
#include "ns3/point-to-point-module.h"
#include "ns3/applications-module.h"
#include "ns3/wifi-module.h"
#include "ns3/simulator.h"
#include "ns3/mobility-module.h"
#include "ns3/flow-monitor.h"
#include "ns3/flow-monitor-module.h"
#include "ns3/flow-monitor-helper.h"
#include "ns3/olsr-helper.h"
#include "stdlib.h"
include <iostream>
#include <sstream>
#include <fstream>
#include <cstdlib>
#include <string>



using namespace ns3;

NS_LOG_COMPONENT_DEFINE ("Running Code Wifi NS3");

int main(){

NodeContainer clients;
NodeContainer aps;
clients.Create(10);
aps.Create(2);

//set position with mobility model left out for brevity

Ssid ssid = Ssid ("default-ssid");
NqosWifiMacHelper wifiMac = NqosWifiMacHelper::Default();
wifiMac.SetType("ns3::ApWifiMac", "Ssid", SsidValue(ssid));
YansWifiChannelHelper wifiChannel = YansWifiChannelHelper::Default();

YansWifiPhyHelper wifiPhy1 = YansWifiPhyHelper::Default();
wifiPhy1.SetPcapDataLinkType(YansWifiPhyHelper::DLT_IEEE802_11_RADIO);
wifiPhy1.SetChannel(wifiChannel.Create());

WifiHelper wifiAP = WifiHelper::Default();
string phyMode("DsssRate1Mbps");

NetDevice Container apWifiDevs;
wifiAP.SetStandard(WIFI_PHY_STANDARD_80211g);
wifiAP.SetRemoteStationManager("ns3::ConstantRateWifiManager", "DataMode", StringValue (phyMode), "ControlMode", StringValue(phyMode));

apWifiDevs.Add(wifiAp.Install(wifiPhy1, wifiMac, aps));

WifiHelper wifi = WifiHelper::Default();
wifi.SetStandard(WIFI_PHY_STANDARD_80211g);
NetDeviceContainer clientDevs;
clientDevs.Add(wifi.Install(wifiPhy1, wifiMac, clients));

}
