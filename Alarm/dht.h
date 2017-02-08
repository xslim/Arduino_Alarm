#ifndef DHT_h
#define DHT_h

#if ARDUINO < 100
#include <WProgram.h>
#include <pins_arduino.h>  // fix for broken pre 1.0 version - TODO TEST
#else
#include <Arduino.h>
#endif

#ifdef __cplusplus
extern "C" {
#endif

#define DHT11 1
#define DHT22 2

#define DHTLIB_OK                   0
#define DHTLIB_ERROR_CHECKSUM       -1
#define DHTLIB_ERROR_TIMEOUT        -2
#define DHTLIB_ERROR_CONNECT        -3
#define DHTLIB_ERROR_ACK_L          -4
#define DHTLIB_ERROR_ACK_H          -5

#define DHTLIB_DHT11_WAKEUP         18
#define DHTLIB_DHT_WAKEUP           1

#define DHTLIB_DHT11_LEADING_ZEROS  1
#define DHTLIB_DHT_LEADING_ZEROS    6

// max timeout is 100 usec.
// For a 16 Mhz proc 100 usec is 1600 clock cycles
// loops using DHTLIB_TIMEOUT use at least 4 clock cycli
// so 100 us takes max 400 loops
// so by dividing F_CPU by 40000 we "fail" as fast as possible
#ifndef F_CPU
#define DHTLIB_TIMEOUT 1000  // ahould be approx. clock/40000
#else
#define DHTLIB_TIMEOUT (F_CPU/40000)
#endif

typedef struct {
  uint8_t state;
  uint8_t pin, type;
  uint8_t _bit, _port;
  //uint8_t _bits[5];
  int8_t temperature;
  uint8_t humidity;
} DHT;

int8_t dht_setup(DHT* sensor, uint8_t pin, uint8_t type);

int16_t dht_update(DHT* sensor);

// private
int8_t dht_readSensor(uint8_t pin, uint8_t wakeupDelay, uint8_t leadingZeroBits, uint8_t (*bits)[5]);

    // return values:
    // DHTLIB_OK
    // DHTLIB_ERROR_CHECKSUM
    // DHTLIB_ERROR_TIMEOUT
    // DHTLIB_ERROR_CONNECT
    // DHTLIB_ERROR_ACK_L
    // DHTLIB_ERROR_ACK_H

//    uint8_t bits[5];  // buffer to receive data

#ifdef __cplusplus
}
#endif

#endif

