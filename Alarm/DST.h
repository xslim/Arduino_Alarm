//
//  Created by Taras Kalapun on 2/8/17.
//
//

#ifndef DST_h
#define DST_h

#include <stdio.h>
#include <Arduino.h>

//#define ONEWIRE_CRC 1
//#define ONEWIRE_CRC16 0
//#define ONEWIRE_CRC8_TABLE 0
#include <OneWire.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef struct {
  uint8_t state;
  byte address[8];
} DST;

uint8_t dst_setup(DST* sensor, OneWire* wire);

uint16_t dst_update(DST* sensor, OneWire* wire, int8_t* temperature);

//private
bool dst_search(OneWire* wire, uint8_t* addr);

#ifdef __cplusplus
}
#endif

#endif /* DST_h */
