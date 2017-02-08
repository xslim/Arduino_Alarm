//
//  Created by Taras Kalapun on 2/8/17.
//
//

#ifndef DST_h
#define DST_h

#include <stdio.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef struct {
    uint8_t pin;
    uint16_t address;
    uint8_t state;
} DST;

void dst_setup(DST* sensor, uint8_t pin);

uint16_t dst_update(DST* sensor, int8_t* temperature);

#ifdef __cplusplus
}
#endif

#endif /* DST_h */
