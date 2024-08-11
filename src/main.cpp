#include <Arduino.h>

#include <FastLED.h>

// How many leds in your strip?
#define NUM_LEDS 72

// For led chips like WS2812, which have a data line, ground, and power, you just
// need to define DATA_PIN.  For led chipsets that are SPI based (four wires - data, clock,
// ground, and power), like the LPD8806 define both DATA_PIN and CLOCK_PIN
// Clock pin only needed for SPI based chipsets when not using hardware SPI
#define DATA_PIN 4

// Define the array of leds
CRGB leds[NUM_LEDS];

void setup() {
// write your initialization code here
  FastLED.addLeds<NEOPIXEL, DATA_PIN>(leds, NUM_LEDS);  // GRB ordering is assumed

}

void loop() {
// write your code here
// Turn the LED on, then pause
for (int i = 0; i < NUM_LEDS; i++) {
  leds[i] = CRGB::Red;
}
  FastLED.show();
  delay(500);
  // Now turn the LED off, then pause
  for (int i = 0; i < NUM_LEDS; i++) {
    leds[i] = CRGB::Black;
  }
  FastLED.show();
  delay(500);
}