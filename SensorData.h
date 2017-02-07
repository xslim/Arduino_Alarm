#ifndef _SensorData_H_
#define _SensorData_H_

typedef struct tSensorData {

  int temp, outTemp, hum;
  int uploaded;
  String lastCard;
  bool locked;
  float batt;

public:
  tSensorData() {
    temp     = 0;
    outTemp  = 0;
    hum      = 0;
    uploaded = -1;
    lastCard = "";
    locked   = false;
    batt = 0.0;
  };
};

#endif
