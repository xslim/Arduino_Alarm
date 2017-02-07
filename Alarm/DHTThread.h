#if DHT_ENABLED

#include <DHT.h>
DHT dht(DHT_PIN, DHT_TYPE);

#include "DSTThread.h"

class DHTThread: public Thread {
public:

  void readDHT() {
    // Reading temperature or humidity takes about 250 milliseconds!
    // Sensor readings may also be up to 2 seconds 'old' (its a very slow sensor)
    
    float h = dht.readHumidity();
    float t = dht.readTemperature();
    
    // Check if any reads failed and exit early (to try again).
    if (isnan(h) || isnan(t)) {
      Serial.println("Failed to read from DHT sensor!");
      return;
    }

    DEBUG_PRINTLN("T: " + String(t) + "C H: " + String(h) + "%");
    sensorData.hum = (int)h;
    sensorData.temp = (int)t;
  
    // Compute heat index in Celsius (isFahreheit = false)
    //float hic = dht.computeHeatIndex(t, h, false);
  }

  void readDST(){
    int16_t t = dst_getTemp(&oneWire, dstAddr);
    
    if (t < -100) {
      Serial.println("Failed to read from DST sensor: " + String(t));
      return;
    }
    DEBUG_PRINTLN("T2: " + String(t) + "C");
    sensorData.outTemp = (int)t;
  }

  void run(){
    readDHT();
    readDST();

//    Serial.println("T: " + String(sensorData.temp) + "C "
//      + "H: " + String(sensorData.hum) + "% "
//      + "T2: " + String(sensorData.outTemp) + "C");
  
    runned();
  }

  void setup(){
    dht.begin();                  // temperature and humidity

    //dst.begin();
    //if (!dst.getAddress(dstAddress, 0)) Serial.println("Unable to find address for Device 0"); 
    dst_search(&oneWire, dstAddr);
  }
};



#endif
