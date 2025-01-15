/*
 * Created by Ed Fillingham on 15/01/2025.
*/

#include "AngularWave.h"
#include "lib/helpers.h"
#define EFFECT_LENGTH 2

AngularWave::AngularWave(std::string name, Set2D *effectSet, float centerX, float centerY, float speed) : Effect(std::move(name)) {
  this->effectSet = effectSet;
  this->speed = speed;
  this->centerX = centerX;
  this->centerY = centerY;
}

void AngularWave::renderEffect(vector<uint32_t> &output) {
  timeValue = millis(); // Current time in milliseconds
  long effectLength = long(1000 * float(EFFECT_LENGTH) / speed); // Duration of one wave cycle
  float offset = float(timeValue % effectLength) / float(effectLength); // Normalized offset [0, 1]

  float minX = effectSet->getMinX();
  float maxX = effectSet->getMaxX();
  float minY = effectSet->getMinY();
  float maxY = effectSet->getMaxY();

  for(auto pair : effectSet->getPairings()){
    float pairX = helpers::floatMap(pair.xPos, minX, maxX, 0, 1);
    float pairY = helpers::floatMap(pair.yPos, minY, maxY, 0, 1);

    // create a wave based on the angle from the center
    float angle = atan2(pairY - centerY, pairX - centerX);
    float wave = sin(angle + offset * 2 * PI);
    wave = wave / 2 + 0.5;

    // set the pixel colour
    uint32_t col = Gradient::rainbowGradient.getColor(wave * 255);
    pair.group->setLeds(col, output);
  }
}