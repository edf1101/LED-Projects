/*
 * Created by Ed Fillingham on 15/01/2025.
*/

#include "PerlinEffect.h"
#include "lib/Noise/SimplexNoise.h"
#include "lib/helpers.h"

#define EFFECT_LENGTH (1000*60*2)

PerlinEffect::PerlinEffect(std::string name, Set2D *effectSet, float frequency, float speed) : Effect(std::move(name)),
                                                                                               effectSet(effectSet),
                                                                                               speed(speed),
                                                                                               frequency(frequency) {}

/**
 * Render the effect to the output buffer
 *
 * @param output led buffer to render the effect to
 */
void PerlinEffect::renderEffect(vector<uint32_t> &output) {

  timeValue = millis();
  // get min and max x and y values

  float minPos = min(effectSet->getMinX(), effectSet->getMinY());
  float maxPos = max(effectSet->getMaxX(), effectSet->getMaxY());

  float zValue = 0.25f * speed * (float(timeValue) / 1000);

  for (auto pair: effectSet->getPairings()) {
    float xPos = helpers::floatMap(pair.xPos, minPos, maxPos, 0.0f, 1.0f) * frequency;
    float yPos = helpers::floatMap(pair.yPos, minPos, maxPos, 0.0f, 1.0f) * frequency;

    // get the noise value at the current position

    // This bit remaps the noise value from -1 to 1 to 0 to 1 except it puts a little more emphasis on the extremeties
    float noiseValue = helpers::floatMap(SimplexNoise::noise(xPos, yPos, zValue), -1.0, 1.0, -0.05, 1.05);
    noiseValue = min(1.0f, max(0.0f, noiseValue));

    // set the pixel colour
    uint32_t col = Gradient::rainbowGradient.getColor(noiseValue * 255);
    pair.group->setLeds(col, output);

  }

}


