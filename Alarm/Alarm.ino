#include <Arduino.h>

#include <Thread.h>
#include <ThreadController.h>

#include "Config.h"

#include "SensorData.h"
tSensorData sensorData;





#if DHT_ENABLED
#include "DHTThread.h"
DHTThread dhtThread = DHTThread();
#endif

#if MQTT_ENABLED
#include "MQTTThread.h"
MQTTThread mqttThread = MQTTThread();
#endif

#if DISPLAY_ENABLED
#include "Display.h"
DisplayThread displayThread = DisplayThread();
#endif

#if RFID_ENABLED
#include "RFIDThread.h"
RFIDThread rfidThread = RFIDThread();
#endif

ThreadController controller = ThreadController();

void setup() {
  // put your setup code here, to run once:
  delay(500);
//#ifdef DEBUG
  Serial.begin(115200);
//#endif
 // DEBUG_PRINTLN("Initializing");

#if DISPLAY_ENABLED
  pinMode(BUTTON_A, INPUT_PULLUP);
  pinMode(BUTTON_B, INPUT_PULLUP);
  pinMode(BUTTON_C, INPUT_PULLUP);
#endif

#if DISPLAY_ENABLED
  displayThread.setup();
  displayThread.setInterval(1000);
  controller.add(&displayThread);
#endif

#if DHT_ENABLED
  dhtThread.setup();
  dhtThread.setInterval(5000);
  controller.add(&dhtThread);
#endif
  
#if MQTT_ENABLED
  mqttThread.setup();
  mqttThread.setInterval(MQTT_POSTING_INTERVAL);
  controller.add(&mqttThread);
#endif

#if RFID_ENABLED
    rfidThread.setup();
    rfidThread.setInterval(100);
    controller.add(&rfidThread);
#endif

//  DEBUG_PRINTLN("Running");
  delay(500);
}



void loop() {
  // put your main code here, to run repeatedly:

  controller.run();
}
