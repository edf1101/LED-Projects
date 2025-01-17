/*
 * Created by Ed Fillingham on 15/01/2025.
*/

#include "Wave1D.h"

#include <utility>
#include "lib/helpers.h"

#define EFFECT_LENGTH 5

/**
 * Constructor for the wave1D class
 *
 * @param name The name of the effect
 * @param effectSet The set to apply the effect to
 * @param speed The speed multiplier of the wave. Default is 1
 */
Wave1D::Wave1D(std::string name, Set1D *effectSet, float speed) : Effect(std::move(name), speed) {
  this->effectSet = effectSet;
}

void Wave1D::renderEffect(vector<uint32_t> &output) {

  timeValue = millis(); // Current time in milliseconds
  long effectLength = long(1000 * float(EFFECT_LENGTH) / speed); // Duration of one wave cycle
  float offset = float(timeValue % effectLength) / float(effectLength); // Normalized offset [0, 1]
  float effectWavelength = 1; // Wavelength of the wave
  // Get the number of LEDs in the set
  float minX = effectSet->getMinX();
  float maxX = effectSet->getMaxX();

  // Loop through the groups in the set
  for (auto pair: effectSet->getPairings()) {
    // Map each group's position to a range of 0-1 and add the offset
    float pos = helpers::floatMap(pair.iPos, minX, maxX, 0, 1.0f / effectWavelength);
    pos += offset;

    // Ensure pos stays in the range [0, 1] by wrapping around
    pos = fmod(pos, 1.0f);

    // Generate the color based on the position
    uint32_t colour = Gradient::sampleGradient(pos * 255);

    // Set the LEDs for the group
    pair.group->setLeds(colour, output);
  }
}



