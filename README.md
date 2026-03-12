# Clock Message Box
LED Matrix and ESP8266 project. <br> 
https://hackaday.io/project/173256-clock-message-box

### Latest update
March 2026 <br> 
The previous build had several issues, the worst being periodic rebooting. All libraries have now been updated and all appears to be working as intended. <br> 
The program has also been simplified. This is a result of usage, or non-usage in this case. <br> 
It was all very nice having a fancy web interface with lots of options and buttons to press, but what I really needed was a simple message when I got home from work reminding me to put out the recycling. 

### Equipment
#### Hardware
- WeMos D1 (R2 & mini) (ESP8266), 80 MHz, 115200 baud, 4M (3M)
- Max7219 LED matrix
- DS3231 Clock (RTC with temperature and memory. battery will last about 8 years)

#### Extra Arduino Libraries
- SPI.h
- time.h
- https://github.com/tzapu/WiFiManager
- ttps://github.com/bblanchon/ArduinoJson
- https://github.com/knolleary/pubsubclient
- https://github.com/sleemanj/DS3231_Simple
- https://github.com/MajicDesigns/MD_MAX72XX
- https://github.com/MajicDesigns/MD_Parola

### Licence
- Written by MTS Standish (Thurstan|mattKsp) 2026
- Released under GNU GPLv3 (see LICENCE file)

This software is provided on an "AS-IS-BASIS"

Originally based on project by aapicella (mostly replaced now).
- https://github.com/aapicella/WiFi-enables-LED-Matrix
- https://www.instructables.com/id/WIFI-Enabled-LED-Matrix/

### Instructions to setup device 
- Power on.  
- Connect your phone/tablet/computer to Wifi device "ClockMessageBox".  
- Connection password is "password".  
- When successfull open browser and goto 192.168.4.1  
- On some devices the auto-capture will work differently. For example, on my Android device it asks me if I want to sign in to this device, this then takes me to the config page. 
- Select the Wifi device you want it connected to (probably your home router).  
- Enter the password for that device.  
- Read the instructions on the page to tell if it was successfull or not.   
- If not repeat.   
- When successfull the Clock Message Box will save the connection and (attempt to) automatically connect to this every time it powers on.   
- On power-up the Clock Message Box should display (a truncated version of) the IP it has been assigned.   
- Messages are sent over WiFi using MQTT (from HomeAssistant in my usage).  

- ( To display the IP again touch the front button. )  
- ( To clear a message touch the front button. )  
- ( Message will timeout and revert to clock (currently) after 1 hour. ) 
- ( Clock Message Box will attempt to update date and time everytime it powers on. )
- ( Clock Message Box automatically adjusts for UK daylight savings. )

### Reference
https://www.w3schools.com/tags/ref_httpmethods.asp <br>
https://randomnerdtutorials.com/esp32-web-server-spiffs-spi-flash-file-system/ <br> 
https://randomnerdtutorials.com/esp8266-web-server-spiffs-nodemcu/ <br> 
https://randomnerdtutorials.com/esp32-esp8266-input-data-html-form/ <br> 
https://forum.arduino.cc/index.php?topic=590802.0 <br> 



