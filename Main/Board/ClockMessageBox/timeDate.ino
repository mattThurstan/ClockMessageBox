/*----------------------------timeDate--------------------------*/
void setupTimeDate() 
{
  rtcClock.begin();

  if (_wifiAvailable) { 
    configTime(MY_TZ, MY_NTP_SERVER);
    uint32_t startTime = millis();

    if (DEBUG_TIME) {
      Serial.print("NTP wait for first valid timestamp "); 
    }
    while (time(nullptr) < 100000ul) {
      if (DEBUG_TIME) { Serial.printf("."); }
      delay(10);
    }
    if (DEBUG_TIME) {
      uint32_t runtimeMillis = millis() - startTime;
      Serial.printf("\ntime synced in %d ms.\n", runtimeMillis);
    }
    _firstDNS = true;

    updateTimeDate(); 
  }
}

/*
 * Update time and date.
 * Uses ESP8266 internal libs.
 * uint8_t
 */
void updateTimeDate() 
{  
  time(&nowTime); //read
  localtime_r(&nowTime, &tmTime); // update tm

  uint8_t day = tmTime.tm_mday;
  uint8_t mon = tmTime.tm_mon + 1;
  uint8_t yer = tmTime.tm_year;
  uint8_t hor = tmTime.tm_hour;
  uint8_t min = tmTime.tm_min;
  uint8_t sec = tmTime.tm_sec;

  if (DEBUG_TIME) {
    Serial.println("Time received from NTP: "); 
    Serial.print(" Hour: ");
    Serial.print(hor);
    Serial.print(" Mins: ");
    Serial.print(min);
    Serial.print(" Secs: ");
    Serial.print(sec);
    Serial.print(" Day: ");
    Serial.print(day);
    Serial.print(" Weekday: ");
    Serial.print(tmTime.tm_wday);
    Serial.print(" Month: ");
    Serial.print(mon);
    Serial.print(" Year: ");
    Serial.print(yer + 1900);
    Serial.print(" Daylight saving: ");
    if (tmTime.tm_isdst == 1) {Serial.print("DST");}
    else                  {Serial.print("Standard");}
    Serial.println(""); 
  } 

  DateTime curTimestamp;
  
  curTimestamp.Day    = day;
  curTimestamp.Month  = mon;
  curTimestamp.Year   = yer - 100; //26; //yer - 2000; // yer+1900-2000
  curTimestamp.Hour   = hor;
  curTimestamp.Minute = min;
  curTimestamp.Second = sec;
  
  rtcClock.write(curTimestamp);

  delay(10);
  
  if (DEBUG_TIME) {
    Serial.print("RTC has been set to: ");
    rtcClock.printTo(Serial);
    Serial.println();
  }
}
