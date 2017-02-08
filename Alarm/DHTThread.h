//#include <DHT.h>
//DHT dht(DHT_PIN, DHT_TYPE);

#include "dht.h"
dht DHT;

#include "DSTThread.h"

class DHTThread: public Thread {
public:

  void readDHT() {
    // Reading temperature or humidity takes about 250 milliseconds!
    // Sensor readings may also be up to 2 seconds 'old' (its a very slow sensor)

    int8_t r = DHT.read22(DHT_PIN);
    
//    float h = dht.readHumidity();
//    float t = dht.readTemperature();

    float h = DHT.humidity;
    float t = DHT.temperature;
    
    // Check if any reads failed and exit early (to try again).
    if (isnan(h) || isnan(t)) {
      DEBUG_PRINTLN("Failed to read from DHT sensor!");
      return;
    }

    DEBUG_PRINTLN("T: " + String(t) + "C H: " + String(h) + "%");
    sensorData.hum = h;
    sensorData.temp = t;
  
    // Compute heat index in Celsius (isFahreheit = false)
    //float hic = dht.computeHeatIndex(t, h, false);
  }

  void readDST(){
    int16_t t = dst_getTemp(&oneWire, dstAddress);
    
    if (t < -100) {
      DEBUG_PRINTLN("Failed to read from DST sensor: " + String(t));
      return;
    }
    DEBUG_PRINTLN("T2: " + String(t) + "C");
    sensorData.outTemp = t;
  }

  void run(){
    readDHT();
    readDST();

    DEBUG_PRINTLN("T: " + String(sensorData.temp) + "C "
      + "H: " + String(sensorData.hum) + "% "
      + "T2: " + String(sensorData.outTemp) + "C");
  
    runned();
  }

  void setup(){
    if (!dst_search(&oneWire, dstAddress)) {
      DEBUG_PRINTLN("DST not found");
    }
  }
};

