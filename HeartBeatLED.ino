#include "LedControlMS.h"

/*
 LED Matrix array pinout:
 Pin 12 is connected to DataIn 
 Pin 11 is connected to CLK 
 Pin 10 is connected to LOAD 
 4 MAX72XX devices in array
 */
LedControl lc=LedControl(12,11,10,4);

/* Use heartrate sensor to obtain this value */
// unsigned long delaytime=500;

/*  Consts for a full 8x8 display of whatever sprite you choose to use make.
 *   
 *  I used: https://xantorohara.github.io/led-matrix-editor/#
 *    to create 8x8 drawings and obtain either the byte array values
 *    OR their hex values since you can store them as unsigned long ints.
*/
const uint64_t HEART_SPRITE[] = {0x183c7effffffff66};
const int HEART_SPRITE_LEN = sizeof(HEART_SPRITE)/8;
const uint64_t HR_CHARS[] {0x00959555f7957500};
const int HR_CHARS_LEN = sizeof(HR_CHARS)/8;

void setup() {
  // init all devices in a loop
  int devices=lc.getDeviceCount();
  for(int address=0;address<devices;address++) {
    /*The MAX72XX is in power-saving mode on startup*/
    lc.shutdown(address,false);
    /* Set the brightness to a medium values */
    lc.setIntensity(address,1);
    /* and clear the display */
    lc.clearDisplay(address);
  }
}

// TODO: Change delays to delaytime variable obtained by sensor
void showHeart(uint64_t HEART_SPRITE) {
  for (int i = 0; i < 8; i++) {
    byte row = (HEART_SPRITE >> i * 8) & 0xFF;
    for (int j =0; j < 8; j++) {
      lc.setLed(0, i, j, bitRead(row, j));
    }
  }
  delay(200);
  lc.clearDisplay(0);
  delay(200);
  for (int i = 0; i < 8; i++) {
    byte row = (HEART_SPRITE >> i * 8) & 0xFF;
    for (int j =0; j < 8; j++) {
      lc.setLed(0, i, j, bitRead(row, j));
    }
  }
  delay(750);
  lc.clearDisplay(0);
}

void showHRchars(uint64_t HR_CHARS) {  
  for (int i = 0; i < 8; i++) {
    byte row = (HR_CHARS >> i * 8) & 0xFF;
    for (int j =0; j < 8; j++) {
      lc.setLed(3, i, j, bitRead(row, j));
    }
  }
}

int i = 0;

void loop() {
  showHRchars(HR_CHARS[i]);
  if (++i >= HR_CHARS_LEN) {
    i = 0;
  }
    
  showHeart(HEART_SPRITE[i]);
  if (++i >= HEART_SPRITE_LEN) {
    i = 0;
  }
}
