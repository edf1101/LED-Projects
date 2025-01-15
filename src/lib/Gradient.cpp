#include "Gradient.h"

// Define the static rainbowStops
const GradientStop Gradient::rainbowStops[4] = {
        {0,   255, 0,   0,   0},  // Red at position 0
        {85,  0,   255, 0,   0},  // Green at position 85
        {170, 0,   0,   255, 0},  // Blue at position 170
        {255, 255, 0,   0,   0}   // Red at position 255
};

// Define the static rainbowGradient
Gradient Gradient::rainbowGradient(
        const_cast<GradientStop*>(Gradient::rainbowStops),
        4,
        false
);

// Gradient methods
uint32_t Gradient::getColor(uint8_t position) {
  // Find the two gradient stops surrounding the position
  GradientStop start, end;
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

void Gradient::setStops(GradientStop *stops, uint8_t numStops) {
  _stops = stops;
  _numStops = numStops;
}

void Gradient::updateStop(uint8_t index, GradientStop stop) {
  if (index < _numStops) {
    _stops[index] = stop;
  }
}
