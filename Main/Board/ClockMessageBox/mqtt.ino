/*----------------------------mqtt------------------------------*/

void mqttSubscribe() { 
  mqttClient.subscribe(D_SUB_CLKMSGBOX1);
}

void mqttReconnect() { for (int attempt = 0; attempt < 3; ++attempt)
  {
    //Loop until we're reconnected
    if (DEBUG_MQTT) { Serial.print(F("Attempting MQTT connection...")); }
    const String clientId = "MlC_Clkmsgbox1";
    // Attempt to connect
    if (true == mqttClient.connect(clientId.c_str(), _username, _password, "house/clkmsgbox1/available", 0, true, "offline"))
    {
      if (DEBUG_MQTT) { Serial.println(F("connected")); }
      mqttClient.publish(D_CD1_CLKMSGBOX1, D_ONLINE);
      mqttSubscribe();
      break;
    }
    else
    {
      if (DEBUG_MQTT) { 
        Serial.print(F("failed, rc="));
        Serial.print(mqttClient.state());
        Serial.println(F(" try again in 5 seconds"));
      }
      delay(5000);              // Wait 5 seconds before retrying
    }
  }
}

void setupMqtt() { 
  if (_wifiAvailable) 
  { 
    if (DEBUG_MQTT) { 
      Serial.print(F("MQTT Server: "));
      Serial.println(MQTT_BROKER_IP); //mqtt_server ???
      Serial.print(F("MQTT Port: "));
      Serial.println(_mqtt_port);
      Serial.print(F("MQTT Username: "));
      Serial.println(_username);
    }

    const int mqttPort = atoi(_mqtt_port);
    mqttClient.setServer(_mqtt_server, mqttPort);
    mqttClient.setCallback(receiveWifiMessage);

    mqttReconnect();
  }
}

void loopMqtt() {
  if (_wifiAvailable) 
  { 
    mqttClient.loop();
    
    // Reconnect if there is an issue with the MQTT connection
    const unsigned long mqttConnectionMillis = millis();
    if ( (false == mqttClient.connected()) && (_mqttConnectionInterval <= (mqttConnectionMillis - _mqttConnectionPreviousMillis)) )
    {
      _mqttConnectionPreviousMillis = mqttConnectionMillis;
      mqttReconnect();
    }
  }
}

/*----------------------------WIFI received messages - get sent straight to MQTT broker----------------------------*/
void receiveWifiMessage(char* p_topic, byte* p_payload, unsigned int p_length) {
  
  if (DEBUG_MQTT) { Serial.println("mqttCallback - receive message"); }
  // concat the payload into a string
  String payload;
  for (uint8_t i = 0; i < p_length; i++) {
    payload.concat((char)p_payload[i]);
  }
  
  // handle message topics
  if (String(MQTT_MSG_TOPIC_COMMAND).equals(p_topic)) { receiveMessage(payload); } 

  if (DEBUG_MQTT) { Serial.print(p_topic); Serial.print(" - "); Serial.println(payload); }
}
