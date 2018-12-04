NodeContainer clients;
NodeContainer aps;
clients.Create(10);
aps.Create(2);

//set position with mobility model left out for brevity

SSid ssid = Ssid ("default-ssid");
NqosWifiMacHelper wifiMac = NqosWifiMacHelper::Default();
wifiMac.SetType("ns3::ApWifiMac", "Ssid", "SsidValue"(ssid));
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
