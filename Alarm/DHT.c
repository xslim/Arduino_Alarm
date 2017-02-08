
#include "DHT.h"

#define DHT_INITIALIZED   0x01
#define DHT_REQUESTED     0x02



int8_t dht_read11(DHT* sensor)
{
  uint8_t bits[5] = {0};;
  int8_t result = dht_readSensor(sensor->pin, DHTLIB_DHT11_WAKEUP, DHTLIB_DHT11_LEADING_ZEROS, &bits);

    // these bits are always zero, masking them reduces errors.
    bits[0] &= 0x7F;
    bits[2] &= 0x7F;

    // CONVERT AND STORE
    sensor->humidity    = bits[0];  // bits[1] == 0;
    sensor->temperature = bits[2];  // bits[3] == 0;

    // TEST CHECKSUM
    // bits[1] && bits[3] both 0
    uint8_t sum = bits[0] + bits[2];
    if (bits[4] != sum)
    {
        return DHTLIB_ERROR_CHECKSUM;
    }
    return result;
}

int8_t dht_read22(DHT* sensor)
{
    uint8_t bits[5] = {0};
    int8_t result = dht_readSensor(sensor->pin, DHTLIB_DHT_WAKEUP, DHTLIB_DHT_LEADING_ZEROS, &bits);

    // these bits are always zero, masking them reduces errors.
    bits[0] &= 0x03;
    bits[2] &= 0x83;

    // CONVERT AND STORE
    sensor->humidity = (bits[0]*256 + bits[1]) /10;
    sensor->temperature = ((bits[2] & 0x7F)*256 + bits[3]) /10;
    if (bits[2] & 0x80)  // negative temperature
    {
        sensor->temperature = - sensor->temperature;
    }

    // TEST CHECKSUM
    uint8_t sum = bits[0] + bits[1] + bits[2] + bits[3];
    if (bits[4] != sum)
    {
        return DHTLIB_ERROR_CHECKSUM;
    }


    //DEBUG_PRINTLN("DHT t "+String(temperature) +" H "+String(humidity) + " r " + String(result));
    return result;
}


int8_t dht_readSensor(uint8_t pin, uint8_t wakeupDelay, uint8_t leadingZeroBits,  uint8_t (*bits)[5])
{
    // INIT BUFFERVAR TO RECEIVE DATA
    uint8_t mask = 128;
    uint8_t idx = 0;

    uint8_t data = 0;
    uint8_t state = LOW;
    uint8_t pstate = LOW;
    uint16_t zeroLoop = DHTLIB_TIMEOUT;
    uint16_t delta = 0;

    leadingZeroBits = 40 - leadingZeroBits; // reverse counting...

    // replace digitalRead() with Direct Port Reads.
    // reduces footprint ~100 bytes => portability issue?
    // direct port read is about 3x faster
    uint8_t bit = digitalPinToBitMask(pin);
    uint8_t port = digitalPinToPort(pin);
    volatile uint8_t *PIR = portInputRegister(port);

    // REQUEST SAMPLE
    pinMode(pin, OUTPUT);
    digitalWrite(pin, LOW); // T-be
    
    delayMicroseconds(wakeupDelay * 1000UL);
    
    digitalWrite(pin, HIGH); // T-go
    pinMode(pin, INPUT);

    uint16_t loopCount = DHTLIB_TIMEOUT * 2;  // 200uSec max
    // while(digitalRead(pin) == HIGH)
    while ((*PIR & bit) != LOW )
    {
        if (--loopCount == 0) return DHTLIB_ERROR_CONNECT;
    }

    // GET ACKNOWLEDGE or TIMEOUT
    loopCount = DHTLIB_TIMEOUT;
    // while(digitalRead(pin) == LOW)
    while ((*PIR & bit) == LOW )  // T-rel
    {
        if (--loopCount == 0) return DHTLIB_ERROR_ACK_L;
    }

    loopCount = DHTLIB_TIMEOUT;
    // while(digitalRead(pin) == HIGH)
    while ((*PIR & bit) != LOW )  // T-reh
    {
        if (--loopCount == 0) return DHTLIB_ERROR_ACK_H;
    }

    loopCount = DHTLIB_TIMEOUT;

    // READ THE OUTPUT - 40 BITS => 5 BYTES
    for (uint8_t i = 40; i != 0; )
    {
        // WAIT FOR FALLING EDGE
        state = (*PIR & bit);
        if (state == LOW && pstate != LOW)
        {
            if (i > leadingZeroBits) // DHT22 first 6 bits are all zero !!   DHT11 only 1
            {
                zeroLoop = min(zeroLoop, loopCount);
                delta = (DHTLIB_TIMEOUT - zeroLoop)/4;
            }
            else if ( loopCount <= (zeroLoop - delta) ) // long -> one
            {
                data |= mask;
            }
            mask >>= 1;
            if (mask == 0)   // next byte
            {
                mask = 128;
                (*bits)[idx] = data;
                idx++;
                data = 0;
            }
            // next bit
            --i;

            // reset timeout flag
            loopCount = DHTLIB_TIMEOUT;
        }
        pstate = state;
        // Check timeout
        if (--loopCount == 0)
        {
            return DHTLIB_ERROR_TIMEOUT;
        }

    }
    pinMode(pin, OUTPUT);
    digitalWrite(pin, HIGH);

    return DHTLIB_OK;
}

int8_t dht_setup(DHT* sensor, uint8_t pin, uint8_t type) {
  sensor->pin = pin;
  sensor->type = type;

//  #ifdef __AVR
//    sensor->_bit = digitalPinToBitMask(pin);
//    sensor->_port = digitalPinToPort(pin);
//  #endif
//  // set up the pins!
//  pinMode(_pin, INPUT_PULLUP);
}

uint16_t dst_request_value(DHT* sensor) {
//  sensor->state |= DHT_REQUESTED;
//
//  // Go into high impedence state to let pull-up raise data line level and
//  // start the reading process.
//  digitalWrite(sensor->pin, HIGH);
//  
  return 250;
}

uint8_t dht_read_value(DHT* sensor) {
  sensor->state &= ~DHT_REQUESTED;

//  *temperature = ;
    
  return 0;
}

int16_t dht_update(DHT* sensor) {
    
    if (!(sensor->state & DHT_INITIALIZED)) {
        return DHTLIB_ERROR_CONNECT;
    }

    if (sensor->type == DHT11) {
      return dht_read11(sensor);
    } else if (sensor->type == DHT22) {
      return dht_read22(sensor);
    }
    return DHTLIB_ERROR_CONNECT;
    
//    if (sensor->state & DHT_REQUESTED) {
//        dst_read_value(sensor, temperature, humidity);
//        return 0;
//    } else {
//        // Request value
//        uint16_t timeout = dht_request_value(sensor);
//        return timeout;
//    }

  
    
}
