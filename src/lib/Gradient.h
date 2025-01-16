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

    ~Gradient() {
      // Free the dynamically allocated stops if they exist
      if (_stops) {
        delete[] _stops;
        _stops = nullptr;
      }
    }

    uint32_t getColor(uint8_t position); // Interpolate the gradient to get the color at a specific position
    void updateStops(GradientStop *stops, uint8_t numStops); // Update gradient stops

    void setRGBW(bool isRGBW) { _isRGBW = isRGBW; } // Set RGBW flag

    static void setCurrentGradient(Gradient *gradient);
    static void gradientSwitchLoop();
    static uint32_t sampleGradient(uint8_t position);

private:
    GradientStop *_stops;  // Pointer to gradient stops
    uint8_t _numStops;     // Number of gradient stops
    bool _isRGBW;          // Whether the strip is RGBW

    static Gradient *currentGradient;
    static Gradient *switchToGradient;
    static float gradientRatio; // 1 = fully currentGradient, 0 = fully switchToGradient
};

#endif //LED_PROJECTS_GRADIENT_H
