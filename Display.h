#include "Config.h"

#if DISPLAY_ENABLED

#include <U8x8lib.h>
U8X8_SSD1306_128X32_UNIVISION_HW_I2C u8x8(/* reset=*/ U8X8_PIN_NONE);   // Adafruit ESP8266/32u4/ARM Boards + FeatherWing OLED

class DisplayThread: public Thread {
public:

  const char *string_list = 
  "Exit";
  uint8_t current_selection = 0;

  bool menu = false;

  void showMenu() {
    menu = false;
    current_selection = u8x8_UserInterfaceSelectionList(u8x8.getU8x8(), "\tSetup", current_selection, string_list);

    if (current_selection == 0) {
      u8x8.clear();
      showData();
      return;
    }

    u8x8_UserInterfaceMessage(u8x8.getU8x8(), "\tSelection:", 
      u8x8_GetStringLineStart(current_selection, string_list ),
      "", " Ok \n Cancel ");
  }

  void showData(){
    u8x8.setCursor(0, 0);
    u8x8.println("T: " + String(sensorData.temp) + "C H: " + String(sensorData.hum) + "%");
    u8x8.println("T2: " + String(sensorData.outTemp) + "C");
    //u8x8.println("L: " + String(sensorData.locked, HEX));


    
    u8x8.println(sensorData.lastCard);
  }

  void run(){
    // Reads the analog pin, and saves it localy

    if (menu) {
      showMenu();
    } else {
      showData();
    }
      
    runned();
  }

  void setup() {
    u8x8.setMenuHomePin(BUTTON_A);
    u8x8.setMenuSelectPin(BUTTON_B);
    u8x8.setMenuNextPin(BUTTON_C);
  //  u8x8.setMenuPrevPin(14);
  //  u8x8.setMenuUpPin(12);
  //  u8x8.setMenuDownPin(13);
    
    u8x8.begin();
    u8x8.setFont(u8x8_font_chroma48medium8_r);
    u8x8.drawString(0,1,"Initializing...");
    //u8x8.refreshDisplay();    // for SSD1606  
    u8x8.clear();

    if (!digitalRead(BUTTON_A)) {
      menu = true;
      delay(30);
    }
  }

};

#endif
