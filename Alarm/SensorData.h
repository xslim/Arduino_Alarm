#ifndef _SensorData_H_
#define _SensorData_H_

typedef struct tSensorData {

  int8_t temp, outTemp;
  uint8_t hum;
  uint8_t batt;
  bool locked;
  

public:
  tSensorData() {
    temp     = 0;
    outTemp  = 0;
    hum      = 0;
    batt     = 0;
    locked   = false;
  };
};

#endif
