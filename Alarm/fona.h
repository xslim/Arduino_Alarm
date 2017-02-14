
#define TINY_GSM_DEBUG Serial
#define TINY_GSM_MODEM_SIM800
#include <TinyGsmClient.h>
#include <SoftwareSerial.h>

SoftwareSerial fonaSS(FONA_TX, FONA_RX);
TinyGsm gsmModem(fonaSS);
TinyGsmClient netClient(gsmModem);

void update_fona(SchedulerTimer *timer);
void fona_wakeup();
void fona_sleap();

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

    
  
  update_fona(NULL);

 
  
//

  fona_wakeup();
  fona_sleap();

   // Auto sleep
  //AT+CSCLK=2
  gsmModem.setSlowClock(2);
  
}

void update_fona(SchedulerTimer *timer) {
//#if DISPLAY_ENABLED
//  gsmOperator = gsmModem.getOperator();
//#endif

  sensorData.batt = gsmModem.getBattVoltage() / 100;
  
  //DEBUG_PRINTLN(gsmOperator);
  DEBUG_PRINTLN(sensorData.batt);
}

void fona_wakeup() {
  DEBUG_PRINTLN("Connecting to APN");
  if (!gsmModem.gprsConnect(FONA_APN, FONA_USERNAME, FONA_PASSWORD)) {
    DEBUG_PRINTLN(" fail");
    while (true);
  }
  
}



void fona_sleap() {
  DEBUG_PRINTLN("Fona sleap");
  delay(500);
  netClient.stop();
  gsmModem.gprsOff();
}
