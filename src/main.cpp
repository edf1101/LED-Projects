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
  randomSeed(analogRead(0));

  myProject->init();
  myProject->addAudioAnalysis(14, 15, 32, false);
  myProject->addWifi("SKYF93G6", "CSZt6Q9x4rDJ");

  // Initialize all pixels to 'off'
  strip.begin();
  strip.clear();
  strip.show();
  strip.setBrightness(100);
  myProject->setEffect("Ripple Effect");
}


void loop() {
  Gradient::gradientSwitchLoop();
  myProject->loop();
  myProject->drawLeds(&strip);
  delay(1);
//  Serial.println("looping");

}
