#include "Spectrometer.h"
#include <utility>
#include "lib/Audio/SpectrumAnalysis.h"

Spectrometer::Spectrometer(std::string name, std::vector<Set1D *> effectSets, float speed, float smoothingFactor)
        : Effect(std::move(name)), smoothingFactor(smoothingFactor) {
  this->effectSets = std::move(effectSets);
  this->speed = speed;
  this->effectHeight = this->effectSets[0]->getSetSize();

  // Initialize smoothed FFT with zeros
  smoothedFFT.resize(16, 0.0f); // 16 bands, initialized to 0
}

void Spectrometer::applySmoothing() {
  for (int i = 0; i < 16; i++) {
    smoothedFFT[i] = (smoothingFactor * SpectrumAnalysis::fftResult[i]) +
                     ((1.0f - smoothingFactor) * smoothedFFT[i]);
  }
}

void Spectrometer::renderEffect(std::vector<uint32_t> &output) {
  // Apply smoothing to the FFT results
  applySmoothing();

  // Determine the number of columns and map them to the 16 FFT bands
  int numColumns = effectSets.size();
  for (int col = 0; col < numColumns; col++) {
    // Map column to the FFT range [0, 15]
    float bandPosition = float(col) * (15.0f / float(numColumns - 1));
    int bandIndexLow = floor(bandPosition);
    int bandIndexHigh = ceil(bandPosition);
    float bandWeight = bandPosition - bandIndexLow;

    // Interpolate the FFT values
    float interpolatedValue = (1.0f - bandWeight) * smoothedFFT[bandIndexLow] +
                              bandWeight * smoothedFFT[bandIndexHigh];

    // Map interpolated value to range [0, effectHeight]
    float rangeVal = (interpolatedValue * (float(effectHeight) / 255.0f));

    Set1D *set = effectSets[col];
    for (int y = 0; y < effectHeight; y++) {
      auto pairing = set->getPairings()[y];
      uint32_t colour = Gradient::sampleGradient(int(255 * float(col) / float(effectSets.size())));

      if (y < static_cast<int>(rangeVal)) {
        // Fully light up LEDs below the integer part of rangeVal
        pairing.group->setLeds(colour, output);
      } else if (y == static_cast<int>(rangeVal)) {
        // Partially light up the LED at the fractional part of rangeVal
        float fractional = rangeVal - static_cast<int>(rangeVal);
        uint8_t r = (colour >> 16) & 0xFF;
        uint8_t g = (colour >> 8) & 0xFF;
        uint8_t b = colour & 0xFF;

        r *= fractional;
        g *= fractional;
        b *= fractional;

        // make sure r,b,g are between 0 and 255
        r = std::min(255, (int)std::max(0, (int)r));
        g = std::min(255, (int)std::max(0, (int)g));
        b = std::min(255, (int)std::max(0, (int)b));

        uint32_t partialColour = Adafruit_NeoPixel::Color(r, g, b);
        pairing.group->setLeds(partialColour, output);
      }
    }
  }
}
