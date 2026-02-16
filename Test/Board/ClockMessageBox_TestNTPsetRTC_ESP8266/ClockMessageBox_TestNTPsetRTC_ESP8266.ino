/*
    'ClockMessageBox_TestNTPsetRTC_ESP8266' by Thurstan. Wifi connected box with LED Matrix showing clock and messages.
    Copyright (C) 2026  MTS Standish (Thurstan|mattKsp)
    
    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.
    
    This program is distributed in the hope that it will be usefull,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.
    
    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
    
    https://github.com/mattThurstan/

    WeMos D1 (R2 & mini) (ESP8266), 80 MHz, 115200 baud, 4M (3M)
    Max7219 LED matrix
    DS3231 Clock

    Original project by aapicella.
    https://github.com/aapicella/WiFi-enables-LED-Matrix
    https://www.instructables.com/id/WIFI-Enabled-LED-Matrix/
*/

/*
 * Changes to IDE in 2019 mean that NTP time is nowTime embedded. 
 * This is to test this.
 */

/*----------------------------libraries-------------------------*/
#include <MT_LightControlComms.h>
#include <MT_LightControlDefines.h>
#include <ESP8266WiFi.h>
// Web portal @ web page @ 192.168.4.1
#include <WiFiManager.h>                  // https://github.com/tzapu/WiFiManager - clash with ESPAsyncWebServer
#include <SPI.h>
#include <MD_Parola.h>                    // https://github.com/MajicDesigns/MD_Parola 
#include <MD_MAX72xx.h>                   // https://github.com/MajicDesigns/MD_MAX72XX

#include <DS3231_Simple.h>
//#include <Wire.h>

#include <time.h>
#include <ArduinoJson.h>                  // https://github.com/bblanchon/ArduinoJson
#include <PubSubClient.h>                 // https://github.com/knolleary/pubsubclient

/*----------------------------system----------------------------*/
const String _progName = "ClockMessageBox_TestNTPsetRTC_ESP8266";
const String _progVers = "1.6";           // Test NTP and set RTC
#define DEBUG 1                           // 0 or 1 - remove later - SERIAL OUTPUT
#define DEBUG_WIFI_HERE 1                 // 
#define DEBUG_DISPLAY 0                   // 
#define DEBUG_TIME 1                      //
#define DEBUG_BT 0                        // 
#define DEBUG_MQTT 1                      // 

/*----------------------------pins------------------------------*/
// Max7219 to Wemos D1 Mini (SPI - Serial) - 5V
#define DATA_PIN  13                      // DIN to D7 (MOSI - GPIO 13) orange
#define CS_PIN    12                      // CS  to D6 (MISO - GPIO 12) yellow
#define CLK_PIN   14                      // CLK to D5 (SCLK - GPIO 14) green
// DS3231 Clock to Wemos D1 Mini (I2C) - 5V
// ground = orange
// +3.3v = red
#define SCL_PIN   5                       // SCL to D1 (GPIO5) green
#define SDA_PIN   4                       // SDA to D2 (GPIO4) yellow
// button
#define BT_PIN    16                      // BT to D0 (GPIO16) with external 10K pullup / touch bt is active low

/*-----------------------------WIFI-----------------------------*/
WiFiClient espClient;
DNSServer dns;                            // ??? notactually used, but needed for ESP Async WIFI manager setup
IPAddress _ip;
bool _wifiAvailable = false;              // Is wifi available for use?
const char* _apName = "ClockMessageBox";
const char* _apPassword = "password";

/*-----------------------------MQTT-----------------------------*/
//PubSubClient mqttClient(MQTT_BROKER_IP, MQTT_BROKER_PORT, mqttCallback, espClient);
PubSubClient mqttClient(espClient);

unsigned long _mqttConnectionPreviousMillis = millis();
const long _mqttConnectionInterval = 60000;

char _mqtt_server[] = MQTT_BROKER_IP;
char _mqtt_port[] = "1883"; //MQTT_BROKER_PORT;
char _workgroup[] = WORKGROUP_NAME;
char _username[] = MQTT_BROKER_USERNAME;
char _password[] = MQTT_BROKER_PASSWORD;

const uint8_t MSG_BUFFER_SIZE = 50;
char m_msg_buffer[MSG_BUFFER_SIZE];           // buffer used to send/receive data with MQTT

// receive
const PROGMEM char* MQTT_MSG_TOPIC_COMMAND = "house/clkmsgbox1/msg/set";              // receive switch
// send

/*-----------------------------Show message and IP--------------*/
volatile boolean _msgActive = false;      // Is the message currently being shown?
int _msgTimeoutHr = 1;                    // Message timeout in hours - Change this!
//int _msgTimeoutMin = 0;                   // Message timeout in minutes - not in use
int _msgTimeoutNextHr = 0;                // Message timeout saved hour to clear msg 
int _msgTimeoutNextMin = 0;               // Message timeout saved minute to clear msg

volatile boolean _showIpActive = false;   // Is the IP currently being shown?
unsigned long _showIpSaveTime = 0;        // Save the current time
long _showIpDisplayInterval = 10000;      // Amount of time to display the IP in milliseconds

volatile boolean _btLock = false;         // Button lock
unsigned long _btLockSaveTime = 0;        // Save the current time
long _btLockInterval = 1000;              // Amount of time to lock the button in milliseconds

volatile boolean _showTempActive = false; // Is the temperature currently being shown?

/*----------------------------LED Matrix------------------------*/
#define HARDWARE_TYPE MD_MAX72XX::GENERIC_HW 
#define MAX_DEVICES 5
MD_Parola _p = MD_Parola(HARDWARE_TYPE, DATA_PIN, CLK_PIN, CS_PIN, MAX_DEVICES);

uint8_t _intensity = 1;                   // 0-15
#define SPEED_TIME  50                    // 25 - higher is slower. Change to variable int later.
#define PAUSE_TIME  1000
textEffect_t _effect[] = { PA_PRINT, PA_SCROLL_LEFT, };
textPosition_t _textAlign[] = { PA_CENTER, PA_LEFT, }; 

#define BUF_SIZE  512
char _text[BUF_SIZE] = " Hello ";         // Marquee text
int _length = strlen(_text); 
const int _animDelay = 100; //75;         // frameDelay ???

/*----------------------------time-------------------------------*/
time_t nowTime; // epoch. secs since 1970
tm tmTime; // convenience struct

/*----------------------------clock-------------------------------*/
DS3231_Simple rtcClock;

/*----------------------------NTP-------------------------------*/
//char _daysOfTheWeek[7][12] = {"Sunday", "Monday", "Tuesday", "Wednesday", "Thursday", "Friday", "Saturday"};
//String _months[12]={"January", "February", "March", "April", "May", "June", "July", "August", "September", "October", "November", "December"};
#define MY_NTP_SERVER "uk.pool.ntp.org"
#define MY_TZ "TZ_Europe_London"  // TimeZone

/*----------------------------daylight savings------------------*/


/*----------------------------main------------------------------*/
void setup() 
{
  if (DEBUG) {
    Serial.begin(115200);
    
    Serial.println();
    Serial.print(_progName);
    Serial.print(" v.");
    Serial.print(_progVers);
    Serial.println();
    Serial.print("..");
    Serial.println();
  }

  pinMode(BT_PIN, INPUT);                 // Set button pin as input (with external 10K pullup)
  
  setupWifiManager();
  delay(200);

  setupTimeDate();
}

void loop() 
{

}
