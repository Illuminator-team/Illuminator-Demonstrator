#include "DigiCDC.h"
#include <Adafruit_NeoPixel.h>

#define LED_PIN 0   // WS2812B Data Line on PB0 (Pin 0), since Pin 0 is connected to DIN of LED strip
#define LED_COUNT 20 // Number of LEDs, Change this for longer strips
#define ID 0 // ID needs to be shared among ATtiny boards of the same LED strip and unique compared to other LED strips


Adafruit_NeoPixel strip(LED_COUNT, LED_PIN, NEO_GRB + NEO_KHZ800);

int animationSpeed = -1;  // Default speed
uint8_t r = 10;
uint8_t g = 10;
uint8_t b = 10;

bool direction = 0;
uint8_t i = 0;

void setup() {
    SerialUSB.begin();  // Initialize USB Serial
    strip.begin();
    strip.show();
}

void loop() {
    requestSpeedFromPi();  // Ask Raspberry Pi for speed
    cyanFlowAnimation(animationSpeed, r, g, b, direction);  // Run animation
}

void requestSpeedFromPi() {
    char c1 = 'a';
    char c2 = 'b';
    char c3 = 'c';

    if (SerialUSB.available()) {
        //SerialUSB.println("1");
        //SerialUSB.flush();
        //delay(50);
        //int s = SerialUSB.parseInt();
        c1 = SerialUSB.read();
        c2 = SerialUSB.read();
        c3 = SerialUSB.read();
        while (SerialUSB.available()){
          char c = SerialUSB.read();
        }

        switch (c1) {
          case '0':
            animationSpeed = 10;
            c2 = 'o';
            //animationSpeed = 1000;
            break;
          case '1':
            animationSpeed = 500;
            break;
          case '2':
            animationSpeed = 100;
            break;
          case '3':
            animationSpeed = 50;
            break;
          case '4':
            animationSpeed = 10;
            break;
          default:
            animationSpeed = -1;
            break;
        }

        switch (c2) {
          case 'r':
            r = 254;
            g = 0;
            b = 0;
            break;
          case 'g':
            r = 0;
            g = 254;
            b = 0;
            break;
          case 'b':
            r = 0;
            g = 0;
            b = 254;
            break;
          default:
            r = 10;
            g = 10;
            b = 10;
            break;
        }

        switch (c3) {
          case '0':
            direction = 0;
            break;
          case '1':
            direction = 1;
            break;
          default:
            direction = 0;
            break;
        }
        
        SerialUSB.delay(10);
        SerialUSB.print(ID);
        SerialUSB.delay(10);
        // SerialUSB.print(animationSpeed);
        // SerialUSB.write(c2);
        // SerialUSB.println("'!!!");
        // SerialUSB.delay(100);
        //animationSpeed = (s > 0 && s < 500) ? s : 50;



        //SerialUSB.flush();
    }
    else{
      SerialUSB.refresh();
    }
    SerialUSB.delay(10);
}


// Flow Animation with dynamic speed
void cyanFlowAnimation(int wait, uint8_t r, uint8_t g, uint8_t b, bool direction) {

  if (wait == -1){
    for (uint8_t pos=0; pos < LED_COUNT; pos++){
      strip.setPixelColor(pos, strip.Color(r, g, b));
    }
    strip.show();
    delay(10);
    return;
  }
  
  //for (int i = 0; i < LED_COUNT; i++){
  int pos = 0;
  if (direction == 0){
    pos = LED_COUNT - i - 1;
  }
  else{
    pos = i;
  }
  strip.clear();
  strip.setPixelColor(pos, strip.Color(r, g, b));  // Cyan color
  strip.show();
  delay(wait);
  //}

  i++;
  if (i > LED_COUNT){
    i = 0;
  }
}
 