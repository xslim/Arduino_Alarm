#include <U8x8lib.h>
U8X8_SSD1306_128X32_UNIVISION_HW_I2C u8x8(/* reset=*/ U8X8_PIN_NONE);   // Adafruit ESP8266/32u4/ARM Boards + FeatherWing OLED

class DisplayThread: public Thread {
public:

  void showData(){
    u8x8.setCursor(0, 0);
    char buf[30];
    int n = sprintf(buf, "T: %dC H: %d%%\nOT: %d C B: %d.%d", 
      sensorData.temp, sensorData.hum, sensorData.outTemp,
      (sensorData.batt / 10), (sensorData.batt % 10));
      
    u8x8.println(buf);
    u8x8.println(gsmOperator);
    
//    u8x8.println("T: " + String(sensorData.temp) + "C H: " + String(sensorData.hum) + "%");
//    u8x8.println("T2: " + String(sensorData.outTemp) + "C");
    //u8x8.println("L: " + String(sensorData.locked, HEX));
    
    //u8x8.println(sensorData.lastCard);
  }

  void run(){
    showData();      
    runned();
  }

  void setup() {
//    u8x8.setMenuHomePin(BUTTON_A);
//    u8x8.setMenuSelectPin(BUTTON_B);
//    u8x8.setMenuNextPin(BUTTON_C);
  //  u8x8.setMenuPrevPin(14);
  //  u8x8.setMenuUpPin(12);
  //  u8x8.setMenuDownPin(13);
    
    u8x8.begin();
    u8x8.setFont(u8x8_font_chroma48medium8_r);
    u8x8.drawString(0,0,"Initializing...");
    //u8x8.refreshDisplay();    // for SSD1606  
    //u8x8.clear();

//    if (!digitalRead(BUTTON_A)) {
//      menu = true;
//      delay(30);
//    }
  }

};
