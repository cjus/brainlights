
#include <Arduino.h>
#include <string.h>
#include <math.h>
#include <Wire.h>
#include <Adafruit_NeoPixel.h>

#define DEBUG
#define LEDPIN 4
#define BUTTONPIN 3
#define RGBW  false // is 4 LED Neopixel? RGB+White
#define LEDS 2 // total number of LEDS in strip or ring

Adafruit_NeoPixel strip = Adafruit_NeoPixel(LEDS, LEDPIN, ((RGBW) ? NEO_GRBW : NEO_GRB) + NEO_KHZ800);

uint8_t brightness = 255;
uint8_t toggle = false;
uint32_t color = 0;
uint8_t colorIndex = -1;
uint16_t elapsedTime = 0;


void setup() {
#ifdef DEBUG    
  while (!Serial);
  Serial.begin(9600);
  Serial.println("Serial connect");
#endif  
  pinMode(BUTTONPIN, INPUT_PULLUP);  
  color = strip.Color(255, 255, 255);
  strip.setBrightness(brightness);
  strip.begin();
  elapsedTime = millis();
}

void setMatrix(uint32_t color, uint8_t count) {
  uint32_t black = strip.Color(0, 0, 0);
  for (int i = 0; i < LEDS; i++) {
    strip.setPixelColor(i, (i < count) ? color : black);
  }
  strip.show();
}

void loop() {
  if (millis() - elapsedTime > 500) {
    if (digitalRead(BUTTONPIN) == LOW) {
#ifdef DEBUG    
      Serial.println("Button down");
#endif
      colorIndex++;
      if (colorIndex > 3) {
        colorIndex = 0;
      }
      switch (colorIndex) {
        case 0:
          color = strip.Color(255, 255, 255);
          break;
        case 1:
          color = strip.Color(255, 0, 0);
          break;
        case 2:
          color = strip.Color(0, 255, 0);
          break;
        case 3:
          color = strip.Color(0, 0, 255);
          break;
        default:
          break;
      }
      elapsedTime = millis();
    }      
  }
  toggle = !toggle;
  if (toggle) {
    setMatrix(color, LEDS);  
  } else {
    setMatrix(strip.Color(0, 0, 0), LEDS);  
  }
  strip.show();
  delay(12.5);
}

