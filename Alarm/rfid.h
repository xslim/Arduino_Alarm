#include <SPI.h>
#include <MFRC522.h>

MFRC522 mfrc522(RFID_SS_PIN, RFID_RST_PIN);

class RFIDThread: public Thread {
public:

//  byte readCard[4];

  void run(){
    if (getID()) {
      
    }
    runned();
  }

  void setup(){
    SPI.begin();
    mfrc522.PCD_Init();
    Serial.println("RFID inited");
  }

  uint8_t getID() {
    // Getting ready for Reading PICCs
    if ( !mfrc522.PICC_IsNewCardPresent() || !mfrc522.PICC_ReadCardSerial() ) {
      return 0;
    }
    
//    for ( uint8_t i = 0; i < 4; i++) {  //
//      readCard[i] = mfrc522.uid.uidByte[i];
//    }
    sensorData.lastCard = uidToString(mfrc522.uid.uidByte, mfrc522.uid.size);
    mfrc522.PICC_HaltA(); // Stop reading
    return 1;
  }

  String uidToString(byte *buf, byte bufferSize) {
    String s = "";
    for (byte i = 0; i < bufferSize; i++) {
      s += String(buf[i] < 0x10 ? "0" : "") + String(buf[i], HEX);
    }
    //Serial.println(s);
    return s;
  }
  
};
