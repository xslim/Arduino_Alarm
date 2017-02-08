#include <Arduino.h>

#include "Config.h"
#include "Scheduler.h"

#include "SensorData.h"
tSensorData sensorData = { 0, 0, 0, 0 };

String gsmOperator = "";

#if DHT_ENABLED
#include "DHTThread.h"
#endif

#if FONA_ENABLED
#include "MQTTThread.h"
#endif

#if DISPLAY_ENABLED
#include "Display.h"
#endif

#if RFID_ENABLED
#include "RFIDThread.h"
RFIDThread rfidThread = RFIDThread();
#endif

//ThreadController controller = ThreadController();

void setup() {
  // put your setup code here, to run once:
  delay(500);

#ifdef DEBUG
  Serial.begin(115200);
  while (!Serial);
  DEBUG_PRINTLN("Initializing");
#endif

  scheduler_setup(millis());

#if DISPLAY_ENABLED
//  pinMode(BUTTON_A, INPUT_PULLUP);
//  pinMode(BUTTON_B, INPUT_PULLUP);
//  pinMode(BUTTON_C, INPUT_PULLUP);
#endif

#if DISPLAY_ENABLED
  setup_display();
  scheduler_add(&update_display, 1000);
#endif

#if DHT_ENABLED
  setup_dst();
  scheduler_add(&update_dht_dst, 5000);
#endif

#if FONA_ENABLED
  setup_fona();
  scheduler_add(&update_fona, MQTT_POSTING_INTERVAL);
#endif

#if MQTT_ENABLED
  setup_mqtt();
  scheduler_add(&update_mqtt, MQTT_POSTING_INTERVAL);
#endif
  

#if RFID_ENABLED
    rfidThread.setup();
    rfidThread.setInterval(100);
    controller.add(&rfidThread);
#endif

  DEBUG_PRINTLN("Running");
  delay(500);
}



void loop() {
  // put your main code here, to run repeatedly:

  scheduler_update(millis());
}
