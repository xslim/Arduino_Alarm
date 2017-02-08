
#define TINY_GSM_MODEM_SIM800
#include <TinyGsmClient.h>
#include <SoftwareSerial.h>

SoftwareSerial fonaSS(FONA_TX, FONA_RX);
TinyGsm gsmModem(fonaSS);
TinyGsmClient netClient(gsmModem);

void update_fona(SchedulerTimer *timer);

void setup_fona(){
  fonaSS.begin(4800);
  delay(3000);
  
  DEBUG_PRINTLN("Initializing modem...");
  gsmModem.restart();

  DEBUG_PRINTLN("Waiting for network...");
  if (!gsmModem.waitForNetwork()) {
    DEBUG_PRINTLN(" fail");
    while (true);
  }

  DEBUG_PRINTLN("Connecting to APN");
  if (!gsmModem.gprsConnect(FONA_APN, FONA_USERNAME, FONA_PASSWORD)) {
    DEBUG_PRINTLN(" fail");
    while (true);
  }
  
  update_fona(NULL);
}

void update_fona(SchedulerTimer *timer) {
  gsmOperator = gsmModem.getOperator();
  sensorData.batt = gsmModem.getBattVoltage() / 100;
  
  DEBUG_PRINTLN(gsmOperator);
  DEBUG_PRINTLN(sensorData.batt);
}

