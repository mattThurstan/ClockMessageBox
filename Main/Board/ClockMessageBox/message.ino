/*----------------------------message---------------------------*/

/*
 * Receive the message and setup.
 * Call from MQTT
 */
void receiveMessage(String message)
{
  message += "    ";
  strcpy(_text, message.c_str());   // Copy received message to _text
  _length=strlen(_text);

  DateTime timeStamp;               // Create a variable to hold the data 
  timeStamp = rtcClock.read();      // Ask the clock for the data.
  _msgTimeoutNextHr = timeStamp.Hour + _msgTimeoutHr; // The hour at which to revert
  _msgTimeoutNextMin = timeStamp.Minute;            // The minute at which to revert

  _p.displayClear();
  _msgActive = true;
  _showIpActive = false;                // Just in case
  
  if (DEBUG_MQTT) { Serial.println("Message active"); Serial.println(_text); }
}

/*
 * Cancel the message.
 * Called from checkMsgCancelBt and setDisplayText.
 */
void cancelMessage() 
{
  _p.displayClear();
  _msgActive = false;                 // Cancel the message
  setBtLock();
}
