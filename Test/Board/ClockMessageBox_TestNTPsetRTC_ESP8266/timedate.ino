/*----------------------------timeDate--------------------------*/
void setupTimeDate() 
{
  rtcClock.begin();

  if (_wifiAvailable) { 
    configTime(MY_TZ, MY_NTP_SERVER);
    
    delay(1);
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

    if (DEBUG_TIME) {
      Serial.println("Time received from NTP: "); 
      Serial.print(" Hour: ");
      Serial.print(tmTime.tm_hour);
      Serial.print(" Mins: ");
      Serial.print(tmTime.tm_min);
      Serial.print(" Secs: ");
      Serial.print(tmTime.tm_sec);
      Serial.print(" Day: ");
      Serial.print(tmTime.tm_mday);
      Serial.print(" Weekday: ");
      Serial.print(tmTime.tm_wday);
      Serial.print(" Month: ");
      Serial.print(tmTime.tm_mon + 1);
      Serial.print(" Year: ");
      Serial.print(tmTime.tm_year + 1900);
      Serial.print(" Daylight saving: ");
      if (tmTime.tm_isdst == 1) {Serial.print("DST");}
      else                  {Serial.print("Standard");}
      Serial.println(""); 
    } 

  DateTime MyTimestamp;
  
  MyTimestamp.Hour   = tmTime.tm_hour;
  MyTimestamp.Minute = tmTime.tm_min;
  MyTimestamp.Second = tmTime.tm_sec;
  MyTimestamp.Day    = tmTime.tm_mday;
  MyTimestamp.Month  = tmTime.tm_mon + 1;
  MyTimestamp.Year   = tmTime.tm_year + 1900 - 2000; 
  
  rtcClock.write(MyTimestamp);
  
  if (DEBUG_TIME) {
    Serial.print("RTC has been set to: ");
    rtcClock.printTo(Serial);
    Serial.println();
  }
}
