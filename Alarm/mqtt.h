

#include <PubSubClient.h>
PubSubClient mqtt;

void mqtt_reconnect();
void mqtt_publish();

void setup_mqtt() {
  mqtt.setClient(netClient);
  // MQTT Broker setup
  mqtt.setServer(MQTT_SERVER, 1883);
  //mqtt.setCallback(mqttCallback);
}

void update_mqtt(SchedulerTimer *timer) {

  if (sensorData.hum == 0) {
    return;
  }
  
  fona_wakeup();
  update_fona(NULL);
  
  // Check if MQTT client has connected else reconnect
  if (!mqtt.connected()) {
    mqtt_reconnect();
  }
  // Call the loop continuously to establish connection to the server
  mqtt.loop();
  
  mqtt_publish();

  led_blink(2);
  
  fona_sleap();
}

void mqtt_reconnect() {
  // Loop until we're reconnected
  while (!mqtt.connected())  {
    DEBUG_PRINTLN("Attempting MQTT connection...");
    // Connect to the MQTT broker
    if (mqtt.connect("X"))  {
      DEBUG_PRINTLN("connected");
    } else {
      DEBUG_PRINT("failed, rc=");
      // Print to know why the connection failed
      // See http://pubsubclient.knolleary.net/api.html#state for the failure code and its reason
      DEBUG_PRINTLN(mqtt.state());
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
  //DEBUG_PRINTLN(buf);
  mqtt.publish("channels/" MQTT_CHANNEL_ID "/publish/" MQTT_CHANNEL_APIKEY, buf);
}

