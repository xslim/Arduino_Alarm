#ifndef _SensorData_H_
#define _SensorData_H_

typedef struct  {

  int8_t temp, outTemp;
  uint8_t hum;
  uint8_t batt;
  bool locked;
  
} tSensorData;

#endif
