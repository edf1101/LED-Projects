#include "Gradient.h"

#define TRANSISTION_LENGTH 1000 // Time in milliseconds for a gradient transition

float Gradient::gradientRatio = 1.0f;
Gradient *Gradient::currentGradient = nullptr;
Gradient *Gradient::switchToGradient = nullptr;

uint32_t Gradient::getColor(uint8_t position) {
  GradientStop start, end;

  // Find the two gradient stops surrounding the position
  for (int i = 0; i < _numStops - 1; i++) {
    if (position >= _stops[i].position && position <= _stops[i + 1].position) {
      start = _stops[i];
      end = _stops[i + 1];
      break;
    }
  }

  // Interpolate between the start and end stops
  float ratio = float(position - start.position) / float(end.position - start.position);
  uint8_t red = start.red + ratio * (end.red - start.red);
  uint8_t green = start.green + ratio * (end.green - start.green);
  uint8_t blue = start.blue + ratio * (end.blue - start.blue);
  uint8_t white = start.white + ratio * (end.white - start.white);

  // Pack the color based on RGB or RGBW
  return _isRGBW ? Adafruit_NeoPixel::Color(red, green, blue, white)
                 : Adafruit_NeoPixel::Color(red, green, blue);
}

void Gradient::updateStops(GradientStop *stops, uint8_t numStops) {
  // Free the existing stops
  if (_stops) {
    delete[] _stops;
  }

  // Assign new stops
  _stops = stops;
  _numStops = numStops;
}

void Gradient::gradientSwitchLoop() {
  static unsigned long lastUpdateTime = millis(); // Track the last update time

  // Calculate the time delta and update `gradientRatio`
  unsigned long currentTime = millis();
  float weightStep = (1.0f / TRANSISTION_LENGTH) * (currentTime - lastUpdateTime);
  lastUpdateTime = currentTime;

  if (switchToGradient == nullptr) {
    gradientRatio = 1.0f;
    return;
  }

  // Reduce `gradientRatio` by the calculated weightStep
  gradientRatio -= weightStep;

  // Clamp `gradientRatio` to valid range
  if (gradientRatio < 0.0f) {
    gradientRatio = 0.0f;
  }

  // If transition is complete, finalize the switch
  if (gradientRatio <= 0.0f) {
    if (currentGradient != nullptr && currentGradient != switchToGradient) {
      delete currentGradient; // Free the old gradient
    }
    currentGradient = switchToGradient;
    switchToGradient = nullptr;
    gradientRatio = 1.0f; // Reset for the next transition
  }
}


void Gradient::setCurrentGradient(Gradient *gradient) {
  if (gradient == currentGradient || switchToGradient != nullptr) {

    delete gradient; // Free the unused gradient
    return;
  }

  switchToGradient = gradient;
}

uint32_t Gradient::sampleGradient(uint8_t position) {
  if (currentGradient == nullptr) {
    return 0;
  }
  if (switchToGradient == nullptr) {
    return currentGradient->getColor(position);
  }

  uint32_t currentColor = currentGradient->getColor(position);
  uint32_t switchToColor = switchToGradient->getColor(position);

  // Extract RGB(W) components from current and target gradients
  uint8_t currentRed = (currentColor >> 16) & 0xFF;
  uint8_t currentGreen = (currentColor >> 8) & 0xFF;
  uint8_t currentBlue = currentColor & 0xFF;
  uint8_t currentWhite = currentGradient->_isRGBW ? ((currentColor >> 24) & 0xFF) : 0;

  uint8_t switchRed = (switchToColor >> 16) & 0xFF;
  uint8_t switchGreen = (switchToColor >> 8) & 0xFF;
  uint8_t switchBlue = switchToColor & 0xFF;
  uint8_t switchWhite = currentGradient->_isRGBW ? ((switchToColor >> 24) & 0xFF) : 0;

  // Interpolate each channel
  uint8_t blendedRed = currentRed * gradientRatio + switchRed * (1.0f - gradientRatio);
  uint8_t blendedGreen = currentGreen * gradientRatio + switchGreen * (1.0f - gradientRatio);
  uint8_t blendedBlue = currentBlue * gradientRatio + switchBlue * (1.0f - gradientRatio);
  uint8_t blendedWhite = currentWhite * gradientRatio + switchWhite * (1.0f - gradientRatio);

  return currentGradient->_isRGBW
         ? Adafruit_NeoPixel::Color(blendedRed, blendedGreen, blendedBlue, blendedWhite)
         : Adafruit_NeoPixel::Color(blendedRed, blendedGreen, blendedBlue);
}
