/*----------------------------wifi------------------------------*/
void setupWifiManager() 
{
  //WiFiManager, Local intialization. Once its business is done, there is no need to keep it around
  WiFiManager wm;
  //AsyncWiFiManager wm;
  //AsyncWiFiManager wm(&_webServer,&dns);  //ESPAsyncWiFiManager
  
//  wm.resetSettings();                         // reset settings - wipe credentials for testing
  
  // Automatically connect using saved credentials,
  // if connection fails, it starts an access point with the specified name ( "AutoConnectAP"),
  // if empty will auto generate SSID, if password is blank it will be anonymous AP (wm.autoConnect())
  // then goes into a blocking loop awaiting configuration and will return success result

  if (!DEBUG_WIFI_HERE) { wm.setDebugOutput(false); }  // Debug is enabled by default. This will turn off Wifi Manager debug output
  
  bool res;
  // res = wm.autoConnect(); // auto generated AP name from chipid
  // res = wm.autoConnect("AutoConnectAP"); // anonymous ap
  res = wm.autoConnect(_apName, _apPassword); // password protected ap
  
  if(!res) {
    _wifiAvailable = false;
    if (DEBUG_WIFI_HERE) { Serial.println("Failed to connect to router."); }
    // ESP.restart();
  } 
  else {
    //if you get here you have connected to the WiFi   
    _wifiAvailable = true; 
    if (DEBUG_WIFI_HERE) { Serial.println("connected to wifi router.)"); }
  }

  _ip = WiFi.localIP();
}
