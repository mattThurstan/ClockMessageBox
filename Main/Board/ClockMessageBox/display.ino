/*----------------------------display---------------------------*/
void setupDisplay() 
{
  _p.begin();                             // Initialise the display
  _p.setIntensity(_intensity);                      
  _p.setInvert(false);
  _p.displaySuspend(false);
  _p.displayClear();

  _p.displayText(_text, _textAlign[0], SPEED_TIME, PAUSE_TIME, _effect[0], PA_NO_EFFECT); // center, print
}

void displayText( const char *theText)
{
  // If message is active then display message, else show the clock.
  if (_p.displayAnimate())              // animates and returns true when an animation is completed
  {
    _p.setTextBuffer(theText);
    
    if (_msgActive || _showIpActive) {
      _p.setTextEffect(_effect[1], PA_NO_EFFECT);     // scroll left
      _p.setTextAlignment(_textAlign[1]);             // align left
    } else {
      _p.setTextEffect(_effect[0], PA_NO_EFFECT);     // print
      _p.setTextAlignment(_textAlign[0]);             // align center
    }
    
    _p.displayReset();
  }
}

/*
 * Set the display text (excluding message).
 * Checks if message is active or not, 
 * if not checks and sets text to Date&Temp (IP) or current time.
 */
void setDisplayText() 
{
  DateTime timeStamp;               // Create a variable to hold the data 
  timeStamp = rtcClock.read();      // Ask the clock for the data.
  
  if (_msgActive == true) { 
    _showTempActive = false;  // bit of a hack
    if (timeStamp.Hour >= _msgTimeoutNextHr && timeStamp.Minute >= _msgTimeoutNextMin) {
      cancelMessage();
      if (DEBUG_DISPLAY) { Serial.println("Message canceled by timeout"); }
    } else {
      checkMsgCancelBt();
    }
  } 
  else if (_showIpActive == true) {
    // Using this to show Date and Temperature instead of IP.
    _showTempActive = false;  // bit of a hack
    
    String tDa = String(timeStamp.Day);
    String tMo = String(timeStamp.Month);
    String tYr = String(timeStamp.Year);   //2000-1970=30

    byte  tTp;
    tTp = rtcClock.getTemperature();
    String temp = String(tTp);

    String ipStr = " " + temp + "c" + " " + tDa + "-" + tMo + "-" + tYr + " ";

    strcpy(_text, ipStr.c_str());
    _length = strlen(_text);
  } 
  else {
    _showTempActive = false;  // bit of a hack
    String tHr = String(timeStamp.Hour);
    String tMin = String(timeStamp.Minute);
    if (timeStamp.Hour < 10) { tHr = "0" + String(timeStamp.Hour); }
    if (timeStamp.Minute < 10) { tMin = "0" + String(timeStamp.Minute); }
    String timeString = tHr + ":" + tMin;
    strcpy(_text, timeString.c_str());
    _length = strlen(_text);
  }

}

/*
 * Set the inital display text (IP of WIFI portal 192.168.4.1).
 */
void setInitDisplayText() 
{
  String ipStr = "..68.4.1"; //doesn't fit on screen
  if (DEBUG_DISPLAY) { Serial.print("IP displayed - 192.168.4.1"); }
  strcpy(_text, ipStr.c_str());
  _length = strlen(_text);
}