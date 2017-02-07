#if DST_ENABLED

#include <OneWire.h>
OneWire oneWire(ONE_WIRE_BUS);

byte dstAddr[8];

bool dst_search(OneWire* wire, const uint8_t* addr) {
  // while ?
  if ( !wire->search(addr)) {
    // No more addresses.
    wire->reset_search();
    delay(250);
    return false;
  }

  if (OneWire::crc8(addr, 7) != addr[7]) {
      // CRC is not valid
      return false;
  }
  return true;
}

int8_t dst_getTemp(OneWire* wire, const uint8_t *addr) {
  byte type_s;
  byte data[2]; //byte data[12];
  
  // the first ROM byte indicates which chip
//  switch (addr[0]) {
//    case 0x10:
//      type_s = 1; //DS18S20 or old DS1820
//      break;
//    case 0x28:
//      type_s = 0; // DS18B20
//      break;
//    case 0x22:
//      type_s = 0; // DS1822
//      break;
//    default:
//      // Device is not a DS18x20 family device
//      return 0;
//  } 

  wire->reset();
  wire->select(addr);
  wire->write(0x44); //wire.write(0x44, 1);        // start conversion, with parasite power on at the end
  
  delay(1000);     // maybe 750ms is enough, maybe not
  // we might do a ds.depower() here, but the reset will take care of it.
  
  wire->reset();
  wire->select(addr);    
  wire->write(0xBE);         // Read Scratchpad

//  for ( i = 0; i < 9; i++) {           // we need 9 bytes
//    data[i] = wire.read();
//  }

    for (int i = 0; i < 2; i++) { // Only read the bytes you need? there is more there
      data[i] = wire->read();
    }

  // Convert the data to actual temperature
  // because the result is a 16 bit signed integer, it should
  // be stored to an "int16_t" type, which is always 16 bits
  // even when compiled on a 32 bit processor.
  int16_t raw = (data[1] << 8) | data[0];
//  if (type_s) {
//    raw = raw << 3; // 9 bit resolution default
//    if (data[7] == 0x10) {
//      // "count remain" gives full 12 bit resolution
//      raw = (raw & 0xFFF0) + 12 - data[6];
//    }
//  } else {
//    byte cfg = (data[4] & 0x60);
//    // at lower res, the low bits are undefined, so let's zero them
//    if (cfg == 0x00) raw = raw & ~7;  // 9 bit resolution, 93.75 ms
//    else if (cfg == 0x20) raw = raw & ~3; // 10 bit res, 187.5 ms
//    else if (cfg == 0x40) raw = raw & ~1; // 11 bit res, 375 ms
//    //// default is 12 bit resolution, 750 ms conversion time
//  }



  // sensor defaults to a reading of 85 if you read it too soon after a reset!

  //celsius = (float)raw / 16.0;
  return raw / 16;
}

void sleep_1s() {
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
}


#endif
