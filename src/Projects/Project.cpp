/*
 * Created by Ed Fillingham on 06/01/2025.
 *
 * This class represents a project. Ie nano-leafs, led cube, Light grid etc.
*/

#include "Project.h"

#define BLENDING_DURATION 700.0

/**
 * Constructor
 *
 * @param numLeds number of leds in the project
 * @param dataPin the data pin for the project
 * @param type the type of leds used (GRB, RGB, etc)
 */
Project::Project(int numLeds, short dataPin, neoPixelType colourType, neoPixelType otherData)
        : numLeds(numLeds), dataPin(dataPin), colourType(colourType), otherData(otherData), leds(numLeds) {
  // set all leds to black on start
  for (int i = 0; i < numLeds; i++) {
    leds[i] = Adafruit_NeoPixel::Color(0, 0, 0);
  }

  if (colourType == NEO_RGB || colourType == NEO_RBG || colourType == NEO_GRB || colourType == NEO_GBR
      || colourType == NEO_BRG || colourType == NEO_BGR)
    hasWComponents = false;
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
 * This is called each frame and does stuff like rendering / handling effects
 */
void Project::loop() {
  static unsigned long lastUpdateTime = millis(); // Track the last update time
  float weightStep = (1.0 / BLENDING_DURATION) * (millis() - lastUpdateTime);
  lastUpdateTime = millis();

  // Render the effects according to their weight
  int currentEffectSize = (int) currentEffects.size();
  // Clear the LED buffer
  for (int i = 0; i < numLeds; i++) {
    leds[i] = 0;
  }

  if (currentEffectSize == 1) {
    // Single effect fully applied
    currentEffects[0].weight = 1;
    currentEffects[0].effect->renderEffect(leds);
  } else if (currentEffectSize > 1) {
    // Adjust weights dynamically based on the time elapsed
    currentEffects[0].weight -= weightStep;
    currentEffects[1].weight += weightStep;

    // Clamp weights to valid range [0, 1]
    if (currentEffects[0].weight < 0) currentEffects[0].weight = 0;
    if (currentEffects[1].weight > 1) currentEffects[1].weight = 1;

    // Remove the first effect if its weight reaches 0
    if (currentEffects[0].weight <= 0.03) {
      currentEffects.erase(currentEffects.begin());
      currentEffects[0].effect->renderEffect(leds);
      currentEffects[0].weight = 1;
      return;
    }

    // Interpolate between effects
    std::vector<uint32_t> output(numLeds, 0); // Temporary buffer for effect output
    for (int i = 0; i < currentEffectSize; i++) {
      output = std::vector<uint32_t>(numLeds, 0); // Clear the buffer
      currentEffects[i].effect->renderEffect(output); // Render the effect

      for (int j = 0; j < numLeds; j++) {
        uint32_t color = output[j];
        uint8_t r = (color >> 16) & 0xFF;
        uint8_t g = (color >> 8) & 0xFF;
        uint8_t b = color & 0xFF;
        uint8_t w = hasWComponents ? ((color >> 24) & 0xFF) : 0;

        // Clamp and blend the values
        r = std::min(255, (int)((leds[j] >> 16 & 0xFF) + r * currentEffects[i].weight));
        g = std::min(255, (int)((leds[j] >> 8 & 0xFF) + g * currentEffects[i].weight));
        b = std::min(255, (int)((leds[j] & 0xFF) + b * currentEffects[i].weight));
        w = hasWComponents
            ? std::min(255, (int)((leds[j] >> 24 & 0xFF) + w * currentEffects[i].weight))
            : 0;

        leds[j] = Adafruit_NeoPixel::Color(r, g, b, w);
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
void Project::setEffect(const std::string &effectName) {
  if (effects.find(effectName) == effects.end()) {
    return;
  }
  if (currentEffects.size() > 2)
    return;

  if (!currentEffects.empty() && currentEffects[0].effect->getName() == effectName)
    return;

  auto effect = effects[effectName];
  currentEffects.push_back({effect, 0});
}

/**
 * Get the names of the effects in the project
 *
 * @return a vector of strings containing the names of the effects
 */
std::vector<std::string> Project::getEffectNames() {
  std::vector<std::string> effectNames;
  for (const auto &effect: effects) {
    effectNames.push_back(effect.first);
  }
  return effectNames;
}

/**
 * Get the name of the current effect
 *
 * @return the name of the current effect
 */
std::string Project::getCurrentEffectName() {
  if (currentEffects.empty())
    return "None";
  return currentEffects[0].effect->getName();
}

/**
 * The base function adds just the audio analysis class,
 * sub classes will call this then add effects.
 */
void Project::addAudioAnalysis(int sck, int ws, int sd, bool leftChannel) {
  if (audioAnalysis == nullptr) {
    audioAnalysis = new SpectrumAnalysis();
    audioAnalysis->setupAudio(sck, ws, sd, leftChannel);
  }
}

/**
 * Add a wifi connection to the project
 *
 * @param ssid SSID of the wifi network
 * @param password Password of the wifi network
 */
void Project::addWifi(const char *ssid, const char *password) {
  webServer = new Web();
  webServer->setup(this, ssid, password);

}
