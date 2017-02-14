#ifndef _Config_H_
#define _Config_H_

#include "private.h"

#define DEBUG

#define DISPLAY_ENABLED 1
#define RFID_ENABLED 0
#define DHT_ENABLED 1
#define DST_ENABLED 1


#define MQTT_ENABLED 1
#define FONA_ENABLED 1

#define DHT_TYPE DHT22     // DHT 22

#define FONA_APN       "internet"
#define FONA_USERNAME  ""
#define FONA_PASSWORD  ""

#define MQTT_SERVER "mqtt.thingspeak.com"
#define MQTT_POSTING_INTERVAL 300000UL
#define MQTT_CHANNEL_ID "189404"

#ifndef MQTT_CHANNEL_APIKEY
#define MQTT_CHANNEL_APIKEY ""
#endif

//LED_BUILTIN
#if defined(ESP8266)
  #define BUTTON_A 0
  #define BUTTON_B 16
  #define BUTTON_C 2
  #define LED_PIN  0
  #define LED2     2
  #define DHTPIN   2
  #define ONE_WIRE_BUS 0 //4 15?
  #define RFID_RST_PIN  15
  #define RFID_SS_PIN   16
#elif defined(ARDUINO_STM32F2_FEATHER)
  #define BUTTON_A PA15
  #define BUTTON_B PC7
  #define BUTTON_C PC5
  #define LED_PIN      PB5
#elif defined(TEENSYDUINO)
  #define BUTTON_A 4
  #define BUTTON_B 3
  #define BUTTON_C 8
  #define LED_PIN  13
#else 
  #define BUTTON_A 9
  #define BUTTON_B 6
  #define BUTTON_C 5
  #define LED_PIN  13
//  #define RX_PIN   0
//  #define TX_PIN   1
  // I2C SDA 2, SCL 3  
//  #define DHTPIN   5
//  #define ONE_WIRE_BUS 6
  #define FONA_RX  9
  #define FONA_TX  8
  #define FONA_RST 4
  #define DHT_PIN      12
  #define ONE_WIRE_BUS 13
#endif

#ifdef DEBUG 
 #define DEBUG_PRINT(x)    Serial.print (x)
 #define DEBUG_PRINTLN(x)  Serial.println (x)
#else
 #define DEBUG_PRINT(x)
 #define DEBUG_PRINTLN(x) 
#endif

#endif
