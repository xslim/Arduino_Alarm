
#define TINY_GSM_MODEM_SIM800
#include <TinyGsmClient.h>
#include <SoftwareSerial.h>

SoftwareSerial fonaSS(FONA_TX, FONA_RX);
TinyGsm gsmModem(fonaSS);
TinyGsmClient netClient(gsmModem);

#if MQTT_ENABLED
#include <PubSubClient.h>
PubSubClient mqtt(netClient);
#endif

class MQTTThread: public Thread {
public:

  void run(){    
    
    getBattVoltage();
    
#if MQTT_ENABLED
    mqtt_loop();
#endif
    
    runned();
  }

  void setup(){
    fonaSS.begin(4800);
    delay(3000);
    
    DEBUG_PRINTLN("Initializing modem...");
    gsmModem.restart();

    DEBUG_PRINTLN("Waiting for network...");
    if (!gsmModem.waitForNetwork()) {
      DEBUG_PRINTLN(" fail");
      while (true);
    }
    DEBUG_PRINTLN(" OK");
  
    DEBUG_PRINTLN("Connecting to APN");
    if (!gsmModem.gprsConnect(FONA_APN, FONA_USERNAME, FONA_PASSWORD)) {
      DEBUG_PRINTLN(" fail");
      while (true);
    }
    DEBUG_PRINTLN(" OK");
    
    getBattVoltage();

#if MQTT_ENABLED
    mqtt_setup();
#endif
  }

  void getBattVoltage() {
    gsmOperator = gsmModem.getOperator();
    sensorData.batt = gsmModem.getBattVoltage() / 100;
    
    DEBUG_PRINTLN(gsmOperator);
    DEBUG_PRINTLN(sensorData.batt);
  }

#if MQTT_ENABLED
  void mqtt_setup() {
    // MQTT Broker setup
    mqtt.setServer(MQTT_SERVER, 1883);
    //mqtt.setCallback(mqttCallback);
  }

  void mqtt_loop() {
    // Check if MQTT client has connected else reconnect
    if (!mqtt.connected()) {
      mqtt_reconnect();
    }
    // Call the loop continuously to establish connection to the server
    mqtt.loop();
    
    mqtt_publish();
  }

  void mqtt_reconnect() {
    // Loop until we're reconnected
    while (!mqtt.connected())  {
      DEBUG_PRINTLN("Attempting MQTT connection...");
      // Connect to the MQTT broker
      if (mqtt.connect("xxx"))  {
        DEBUG_PRINTLN("connected");
      } else {
        DEBUG_PRINT("failed, rc=");
        // Print to know why the connection failed
        // See http://pubsubclient.knolleary.net/api.html#state for the failure code and its reason
        DEBUG_PRINTLN(mqtt.state());
        DEBUG_PRINTLN(" try again in 5 seconds");
        // Wait 5 seconds before retrying to connect again
        delay(5000);
      }
    }
  }

  void mqtt_publish() {
    if (sensorData.hum == 0) {
      return;
    }

    // Create data string to send to ThingSpeak
    char buf[50];
    int n = sprintf(buf, "field1=%d&field2=%d&field3=%d&field4=%d.%d", 
      sensorData.temp, sensorData.hum, sensorData.outTemp,
      (sensorData.batt / 10), (sensorData.batt % 10));
    DEBUG_PRINTLN(buf);
    mqtt.publish("channels/" MQTT_CHANNEL_ID "/publish/" MQTT_CHANNEL_APIKEY, buf, n);
  }
#endif
};

