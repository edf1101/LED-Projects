#ifndef LED_PROJECTS_GRADIENT_H
#define LED_PROJECTS_GRADIENT_H

#include "Arduino.h"
#include <lib/Adafruit_NeoPixel/Adafruit_NeoPixel.h>

struct GradientStop {
    uint8_t position; // Position in the gradient (0-255)
    uint8_t red, green, blue, white; // Color at the position
};

class Gradient {
public:
    Gradient(GradientStop *stops, uint8_t numStops, bool isRGBW = false)
            : _stops(stops), _numStops(numStops), _isRGBW(isRGBW) {}

    // Interpolate the gradient to get the color at a specific position
    uint32_t getColor(uint8_t position);

    // Set a new array of gradient stops
    void setStops(GradientStop *stops, uint8_t numStops);

    // Update a specific stop by index
    void updateStop(uint8_t index, GradientStop stop);

    // Get the number of stops
    uint8_t getNumStops() const { return _numStops; }

    // Predefined rainbow stops (declared here, defined later)
    static const GradientStop rainbowStops[4];

    // Predefined rainbow gradient (declared here, defined later)
    static Gradient rainbowGradient;

private:
    GradientStop *_stops;  // Pointer to gradient stops
    uint8_t _numStops;     // Number of gradient stops
    bool _isRGBW;          // Whether the strip is RGBW
};

#endif //LED_PROJECTS_GRADIENT_H
