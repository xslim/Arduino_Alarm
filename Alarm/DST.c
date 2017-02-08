//
//  Created by Taras Kalapun on 2/8/17.
//
//

#include "DST.h"
#include <stdlib.h>

int dst_rand_between(int min, int max){
    return min + rand() / (RAND_MAX / (max - min + 1) + 1);
}

#define DST_INITIALIZED		0x01
#define DST_REQUESTED       0x02

void dst_setup(DST* sensor, uint8_t pin) {
    sensor->state = DST_INITIALIZED;
}



uint16_t dst_request_value(DST* sensor) {
    sensor->state |= DST_REQUESTED;
    uint16_t timeout = 1000;
    return timeout;
}

uint8_t dst_read_value(DST* sensor, int8_t* temperature) {
    sensor->state &= ~DST_REQUESTED;
    
    *temperature = dst_rand_between(1, 30);
    
    return 1;
}

uint16_t dst_update(DST* sensor, int8_t* temperature) {
    
    if (!(sensor->state & DST_INITIALIZED)) {
        return 0;
    }
    
    if (sensor->state & DST_REQUESTED) {
        dst_read_value(sensor, temperature);
        return 0;
    } else {
        // Request value
        uint16_t timeout = dst_request_value(sensor);
        return timeout;
    }
    
}
