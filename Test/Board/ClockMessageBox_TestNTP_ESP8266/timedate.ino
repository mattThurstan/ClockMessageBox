/*----------------------------timeDate--------------------------*/

/*
 * Update time and date.
 * Spins up an NTP client, connects to world clock and sets the current time.
 * Called during setup if WIFI is available.
 */
void updateTimeDate() 
{  
    if (DEBUG_TIME) { 
      time(&nowTime); //read
      localtime_r(&nowTime, &tmTime); // update tm

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
}
