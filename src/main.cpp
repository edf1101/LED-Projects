#include <Arduino.h>

#include <FastLED.h>

#include "Audio/SpectrumAnalysis.h"

// How many leds in your strip?
#define NUM_LEDS 128
#define DATA_PIN 4

DEFINE_GRADIENT_PALETTE( heatmap_gp ) {
        0,     50,  0,  0,   //black
        128,   255,  0,  0,   //red
        224,   255,255,  0,   //bright yellow
        255,   255,255,255 }; //full white
CRGBPalette16 myPal = heatmap_gp;

// Define the array of leds
CRGB leds[NUM_LEDS];
SpectrumAnalysis spectrumAnalysis;

int pixelAt(int x, int y) {
  int n = x * 8;
  if (x % 2 == 0)
    n += y;
  else
    n += 7 - y;
  return n;
}

void setup() {
// write your initialization code here
  Serial.begin(115200);
  FastLED.addLeds<NEOPIXEL, DATA_PIN>(leds, NUM_LEDS);  // GRB ordering is assumed
  spectrumAnalysis.setupAudio();
}

void loop() {
  FastLED.clear();
  for (int row = 0; row < 16; row++) {
    int result =  8.0* (float(spectrumAnalysis.fftResult[row]) /255.0);
    float frac = 8.0* (float(spectrumAnalysis.fftResult[row]) /255.0) - result;
    for (int height = 0; height < result; height++) {
      int pix = pixelAt(row, height);
      CRGB basecol = ColorFromPalette( myPal, 255.0*(float(row)/16.0));
      CRGB lightCol = basecol * frac;
      CRGB col = (height == result-1 )? lightCol : basecol;
      leds[pix] = col;
    }
  }
  // Serial.println(FFT_Magnitude);


  // FastLED.setBrightness(brightness);
  FastLED.show();
}