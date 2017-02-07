

#define TINY_GSM_MODEM_SIM800
#include <TinyGsmClient.h>
#include <SoftwareSerial.h>

#include <PubSubClient.h>

SoftwareSerial fonaSS(FONA_TX, FONA_RX);
TinyGsm gsmModem(fonaSS);
TinyGsmClient netClient(gsmModem);
PubSubClient mqtt(netClient);

class MQTTThread: public Thread {
public:

  void run(){    
    // Check if MQTT client has connected else reconnect
    if (!mqtt.connected()) {
      reconnect();
    }
    // Call the loop continuously to establish connection to the server
    mqtt.loop();

    getBattVoltage();
    
    mqttpublish();
    
    runned();
  }

  void setup(){
    fonaSS.begin(4800);
    delay(3000);
    
    Serial.println("Initializing modem...");
    gsmModem.restart();

    Serial.print("Waiting for network...");
    if (!gsmModem.waitForNetwork()) {
      Serial.println(" fail");
      while (true);
    }
    Serial.println(" OK");
  
    Serial.print("Connecting to APN");
    if (!gsmModem.gprsConnect(FONA_APN, FONA_USERNAME, FONA_PASSWORD)) {
      Serial.println(" fail");
      while (true);
    }
    Serial.println(" OK");
    
    getBattVoltage();
  
    // MQTT Broker setup
    mqtt.setServer(MQTT_SERVER, 1883);
    //mqtt.setCallback(mqttCallback);
  }

  void getBattVoltage() {
    //uint16_t v = gsmModem.getBattVoltage();
    //sensorData.batt = v/1000.0;
  }

  void reconnect() {
    // Loop until we're reconnected
    while (!mqtt.connected())  {
      Serial.print("Attempting MQTT connection...");
      // Connect to the MQTT broker
      if (mqtt.connect("xxx"))  {
        Serial.println("connected");
      } else {
        Serial.print("failed, rc=");
        // Print to know why the connection failed
        // See http://pubsubclient.knolleary.net/api.html#state for the failure code and its reason
        Serial.print(mqtt.state());
        Serial.println(" try again in 5 seconds");
        // Wait 5 seconds before retrying to connect again
        delay(5000);
      }
    }
  }

  void mqttpublish() {

    if (sensorData.hum == 0) {
      return;
    }

    // Create data string to send to ThingSpeak
    String data = String("field1=" + String(sensorData.temp) 
                      + "&field2=" + String(sensorData.hum) 
                      + "&field3=" + String(sensorData.outTemp) 
                      + "&field4=" + String(sensorData.batt, 2));
    // Get the data string length
    int length = data.length();
    char msgBuffer[length];
    // Convert data string to character buffer
    data.toCharArray(msgBuffer,length+1);
    Serial.println(msgBuffer);

    mqtt.publish("channels/" MQTT_CHANNEL_ID "/publish/" MQTT_CHANNEL_APIKEY, msgBuffer);
  }
  
};

