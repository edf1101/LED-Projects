#include <Arduino.h>
#include <lib/Adafruit_NeoPixel/Adafruit_NeoPixel.h>
#include "Projects/Grid.h"

// Create an instance of the project
Project *myProject = new Grid();

// Create an instance of the Adafruit NeoPixel library with project values
Adafruit_NeoPixel strip(myProject->getNumLeds(),
                        myProject->getDataPin(),
                        myProject->getType());

void setup() {
  Serial.begin(115200);
  myProject->init();
  // Initialize all pixels to 'off'
  strip.begin();
  strip.clear();
  strip.show();
}

void loop() {
  myProject->drawLeds(&strip);
  delay(1);
}