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

/**
 * This is called each frame and does stuff like rendering / handling effects
 */
void Project::loop() {
  // render the effects according to their weight
  int currentEffectSize = (int) currentEffects.size();
  for (int i = 0; i < numLeds; i++) {
    leds[i] = 0;
  }
  if (currentEffectSize == 1) {
    currentEffects[0].effect->renderEffect(leds);
  } else if (currentEffectSize > 1) {
    // interpolate between the effects based on weight
    std::vector<uint32_t> output(numLeds);
    for (int i = 0; i < currentEffectSize; i++) {
      currentEffects[i].effect->renderEffect(output);
      for (int j = 0; j < numLeds; j++) {
        leds[j] += output[j] * currentEffects[i].weight;
      }
    }
  }
}

/**
 * Set the effect to be used
 *
 * @param effectName The name of the effect in the effects dictionary
 * @param weight The weight of the effect
 */
void Project::setEffect(const std::string& effectName, float weight) {
  auto effect = effects[effectName];
  currentEffects.clear();
  currentEffects.push_back({effect, weight});

}
