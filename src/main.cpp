#include <Arduino.h>
#include <lib/Adafruit_NeoPixel/Adafruit_NeoPixel.h>
#include "Projects/Grid.h"
#include "Lib/Gradient.h"

// Create an instance of the project
Project *myProject = new Grid(4, NEO_GRB, NEO_KHZ800, 16, 8);

// Create an instance of the Adafruit NeoPixel library with project values
Adafruit_NeoPixel strip(myProject->getNumLeds(),
                        myProject->getDataPin(),
                        myProject->getColourType() + myProject->getOtherData());

void setup() {
  Serial.begin(115200);
  myProject->init();
  // Initialize all pixels to 'off'
  strip.begin();
  strip.clear();
  strip.show();
  strip.setBrightness(100);
  myProject->setEffect("2D Wave");
}

void loop() {
  Gradient::gradientSwitchLoop();
  myProject->loop();
  myProject->drawLeds(&strip);
  delay(1);

}