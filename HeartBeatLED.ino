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
const uint64_t HR_NUMBERS[] = {
  0x1824242424242418,
  0x0808080808080808,
  0x3c0404043c20203c,
  0x1c2020201c20201c,
  0x2020203c24242424,
  0x1c2020201c04043c,
  0x182424241c040438,
  0x101010101010223e,
  0x1824242418242418,
  0x1824202038242418
};
const int HR_NUMBERS_LEN = sizeof(HR_NUMBERS)/8;

//int pulseSensor = A0;
//double alpha = 0.75;
//int period = 200;
//double change = 0.0;

void setup() {
//  pinMode(LED_BUILTIN, OUTPUT);
//  Serial.begin(9600);
  
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

void sampleNums(uint64_t HR_NUMBERS) {
  for (int i = 0; i < 8; i++) {
    byte row = (HR_NUMBERS >> i * 8) & 0xFF;
    for (int j =0; j < 8; j++) {
      lc.setLed(1, i, j, bitRead(row, j));
    }
  }
  for (int i = 0; i < 8; i++) {
    byte row = (HR_NUMBERS >> i * 8) & 0xFF;
    for (int j =0; j < 8; j++) {
      lc.setLed(2, i, j, bitRead(row, j));
    }
  }
}

int i = 0;

void loop() {
  
  showHRchars(HR_CHARS[i]);
  if (++i >= HR_CHARS_LEN) {
    i = 0;
  }

  sampleNums(HR_NUMBERS[i]);
  if (++i >= HR_NUMBERS_LEN) {
    i = 0;
  }
    
  showHeart(HEART_SPRITE[i]);
  if (++i >= HEART_SPRITE_LEN) {
    i = 0;
  }
}
