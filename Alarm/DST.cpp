//
//  Created by Taras Kalapun on 2/8/17.
//
//

#include "DST.h"
#include <stdlib.h>

#define DST_INITIALIZED		0x01
#define DST_REQUESTED     0x02

uint8_t dst_setup(DST* sensor, OneWire* wire) {
  if(dst_search(wire, sensor->address)) {
    sensor->state = DST_INITIALIZED;
    return 1;
  }
  return 0;
}

bool dst_search(OneWire* wire, uint8_t* addr) {
  // while ?
  if ( !wire->search(addr)) {
    wire->reset_search(); // No more addresses.
    return false;
  }

  if (OneWire::crc8(addr, 7) != addr[7]) {
    return false; // CRC is not valid
  }

//  if (addr[0] != 0x10 && addr[0] != 0x28 && addr[0] != 0x22) {
//    return false; // Device is not recognized
//  }
  return true;
}


uint16_t dst_request_value(DST* sensor, OneWire* wire) {
  sensor->state |= DST_REQUESTED;
   
  // the first ROM byte indicates which chip
//  byte type_s;
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
  wire->select(sensor->address);
  wire->write(0x44); //wire.write(0x44, 1);        // start conversion, with parasite power on at the end

  return 800; // request in x ms
}

uint8_t dst_read_value(DST* sensor, OneWire* wire, int8_t* temperature) {
  sensor->state &= ~DST_REQUESTED;

  wire->reset();
  wire->select(sensor->address);    
  wire->write(0xBE);         // Read Scratchpad

  byte data[2]; //byte data[12];

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
  //DEBUG_PRINTLN("DST raw "+String(raw));
  
  *temperature = raw / 16;
    
  return 0;
}

uint16_t dst_update(DST* sensor, OneWire* wire, int8_t* temperature) {
    
    if (!(sensor->state & DST_INITIALIZED)) {
        return 0;
    }
    
    if (sensor->state & DST_REQUESTED) {
        dst_read_value(sensor, wire, temperature);
        return 0;
    } else {
        // Request value
        uint16_t timeout = dst_request_value(sensor, wire);
        return timeout;
    }
    
}
