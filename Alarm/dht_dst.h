
#include "DHT.h"
DHT dht;

#include "DST.h"
DST dst;

OneWire oneWire(ONE_WIRE_BUS);

void update_dht(SchedulerTimer *timer) {
  int16_t res = dht_update(&dht);
  if (res == DHTLIB_OK) {
    sensorData.temp = dht.temperature;
    sensorData.hum  = dht.humidity;
  } else if (res > 0) {
    scheduler_reset_timeout(timer, res);
  } else {
    DEBUG_PRINTLN("res " + String(res));
  }
}

void setup_dht() {
  dht_setup(&dht, DHT_PIN, DHT_TYPE);
}

void update_dst(SchedulerTimer *timer) {
  uint16_t newTimeout = dst_update(&dst, &oneWire, &sensorData.outTemp);
  scheduler_reset_timeout(timer, newTimeout);
  
//  if (t < -100) {
//    DEBUG_PRINTLN("Failed to read from DST sensor: " + String(t));
//    return;
//  }
}

void setup_dst() {
  if (!dst_setup(&dst, &oneWire)) {
    DEBUG_PRINTLN("DST not found");
  }
}


