/*
 * Created by Ed Fillingham on 15/01/2025.
*/

#include "Wave2D.h"

#include <utility>
#include "lib/helpers.h"

#define EFFECT_LENGTH 5

/**
 * Constructor for the wave2D class
 *
 * @param name The name of the effect
 * @param effectSet The set to apply the effect to
 * @param angle The angle of the wave in degrees
 * @param speed The speed multiplier of the wave. Default is 1
 */
Wave2D::Wave2D(std::string name, Set2D *effectSet, float angle, float speed) : Effect(std::move(name)) {
  this->effectSet = effectSet;
  this->speed = speed;
  this->angle = angle;
}

void Wave2D::renderEffect(vector<uint32_t> &output) {
  timeValue = millis(); // Current time in milliseconds
  long effectLength = long(1000 * float(EFFECT_LENGTH) / speed); // Duration of one wave cycle
  float offset = float(timeValue % effectLength) / float(effectLength); // Normalized offset [0, 1]

  float minX = effectSet->getMinX();
  float maxX = effectSet->getMaxX();
  float minY = effectSet->getMinY();
  float maxY = effectSet->getMaxY();

  // Convert angle to radians and compute wave direction vector
  float angleRad = angle * (M_PI / 180.0); // Convert to radians
  float waveDirX = cos(angleRad);
  float waveDirY = sin(angleRad);

  // Calculate the effective length of the wave across the display
  // This is the projected distance from (minX, minY) to (maxX, maxY) along the wave direction
  float displayWidth = (maxX - minX);
  float displayHeight = (maxY - minY);
  float effectiveLength = fabs(displayWidth * waveDirX) + fabs(displayHeight * waveDirY);

  // Adjust the wavelength to ensure the gradient fits across the display
  float effectWavelength = effectiveLength;

  // Loop through the groups in the set
  for (auto pair : effectSet->getPairings()) {
    float xPos = pair.xPos;
    float yPos = pair.yPos;

    // Project the position onto the wave direction
    float wavePosition = ((xPos - minX) * waveDirX + (yPos - minY) * waveDirY) / effectWavelength;

    // Add the time-based offset to create animation
    wavePosition += offset;

    // Wrap the wave position to the range [0, 1]
    wavePosition = fmod(wavePosition, 1.0f);
    if (wavePosition < 0) wavePosition += 1.0f;

    // Map wavePosition to color using the gradient
    uint32_t colour = Gradient::rainbowGradient.getColor(255 * wavePosition);

    // Assign color to the group's LEDs
    pair.group->setLeds(colour, output);
  }
}




