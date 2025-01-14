/*
 * Created by Ed Fillingham on 06/01/2025.
 *
 * This class represents a project. Ie nano-leafs, led cube, Light grid etc.
*/

#include "Project.h"

/**
 * Constructor
 *
 * @param numLeds number of leds in the project
 * @param dataPin the data pin for the project
 * @param type the type of leds used (GRB, RGB, etc)
 */
Project::Project(int numLeds, short dataPin, neoPixelType type)
        : numLeds(numLeds), dataPin(dataPin), type(type), leds(numLeds) {
  // set all leds to black on start
  for (int i = 0; i < numLeds; i++) {
    leds[i] = Adafruit_NeoPixel::Color(0, 0, 0);
  }
}

/**
 * Draw the leds to the strip
 *
 * @param strip the strip to draw the leds to
 */
void Project::drawLeds(Adafruit_NeoPixel *strip) {
  for (int i = 0; i < numLeds; i++) {
    strip->setPixelColor(i, leds[i]);
  }
  strip->show();
}

/**
 * Set the colour of a led
 *
 * @param ledIndex the index of the led to set
 * @param colour the colour to set the led to
 */
void Project::setLed(int ledIndex, uint32_t colour) {
  leds[ledIndex] = colour;
}
