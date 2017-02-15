#include <Arduino.h>

#include "config.h"

void led_blink(uint8_t num);

#include "Scheduler.h"

#include "SensorData.h"
tSensorData sensorData = { 0, 0, 0, 0 };



#if DHT_ENABLED
#include "dht_dst.h"
#endif

#if FONA_ENABLED
#include "fona.h"
#endif

#if MQTT_ENABLED
#include "mqtt.h"
#endif

#if DISPLAY_ENABLED
#include "display.h"
#endif

#if RFID_ENABLED
#include "rfid.h"
#endif

void setup() {

  // initialize digital pin LED_BUILTIN as an output.
  pinMode(LED_BUILTIN, OUTPUT);

#ifdef DEBUG
  Serial.begin(115200);
  //while (!Serial);
  DEBUG_PRINTLN("Initializing");
#endif

  scheduler_setup(millis());

  led_blink(3);

#if DISPLAY_ENABLED
//  pinMode(BUTTON_A, INPUT_PULLUP);
//  pinMode(BUTTON_B, INPUT_PULLUP);
//  pinMode(BUTTON_C, INPUT_PULLUP);
#endif

#if DISPLAY_ENABLED
  setup_display();
  scheduler_add(&update_display, 1000, false);
#endif

#if DHT_ENABLED
  setup_dst();
  setup_dht();
  scheduler_add(&update_dht, DST_INTERVAL, true);
  scheduler_add(&update_dst, DST_INTERVAL, true);
#endif

#if FONA_ENABLED
  setup_fona();
//  scheduler_add(&update_fona, MQTT_POSTING_INTERVAL);

#if MQTT_ENABLED
  setup_mqtt();
  scheduler_add(&update_mqtt, MQTT_POSTING_INTERVAL, true);
#endif
#endif

#if RFID_ENABLED
    rfidThread.setup();
    rfidThread.setInterval(100);
    controller.add(&rfidThread);
#endif

  DEBUG_PRINTLN("Running");
  led_blink(3);
}



void loop() {
  // put your main code here, to run repeatedly:

  scheduler_update(millis());
}


//void sleep_1s() {
//  //this next bit creates a 1 second WDT delay during the DS18b20 temp conversion
//  //The time needed between the CONVERT_T command and the READ_SCRATCHPAD command has to be at least
//  //750 millisecs (but can be shorter if using a D18B20 type with resolutions < 12 bits)
//  MCUSR = 0; // clear various “reset” flags
//  WDTCSR = bit (WDCE) | bit (WDE); // allow changes, disable reset
//  // set interrupt mode and an interval
//  WDTCSR = bit (WDIE) | bit (WDP2) | bit (WDP1); //a 1 sec timer
//  wdt_reset(); // pat the dog
//  set_sleep_mode (SLEEP_MODE_PWR_DOWN);
//  sleep_enable();
//  sleep_cpu ();
//  sleep_disable(); // cancel sleep after wakeup as a precaution
//}

void led_blink(uint8_t num) {
  for (uint8_t i = num; i != 0; --i) {
    digitalWrite(LED_BUILTIN, HIGH);
    delay(200);
    digitalWrite(LED_BUILTIN, LOW);
    delay(200);
  }
}
