string phyModeFast("ErpOfdmRate54Mbps");

NetDeviceVontainer apWifiDevs;
wifiAP.SetStandard(WIFI_PHY_STANDARD_80211g);
wifiAP.SetRemoteStationManager("ns3::ConstantRateWifiManager", "DataMode", StringValue(phyModeFast), "ControlMode", StringValue(phyMode));
apWifiDevs.Add(wifiAP.Install(wifiPhy1, wifiMac,aps.get(0));

WifiHelper wifi = WifiHelper::Default();
wifi.SetStandard(WIFI_PHY_STANDARD_80211g);
NetDeviceContainer clientDevs;

clientDevs.Add(wifi.Install(wifiPhy1, wifiMac,clients));
clientDevs.Add(wifi.Install(wifiPhy2, wifiMac, clients));

for(unsigned int i=0; i < clientDevs.GetN()/2; i++){
  Ptr <WifiNetDevice> d1 = clientDevs.Get(i)->GetObject<WifiNetDevice>();
  Ptr <WifiNetDevice> d2 = clientDevs.Get(i + (clientDevs.GetN()/2))->GetObject<WifiNetDevice>();

  Ptr<StaWifiMacJason> mac1 = DynamicCast<StaWifiMacJason>(d1->GetMac());
  Ptr<StaWifiMacJason> mac2 = DynamicCast<StaWifiMacJason>(d2->getMac());

  if((mac1 != NULL) && (mac2 != NULL)){
    mac1->SetOtherInterface(mac2);
    mac2->SetOtherInterface(mac1);
  }
}
