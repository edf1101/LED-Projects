/*
 * Created by Ed Fillingham on 16/01/2025.
*/

#include "Rainfall.h"

/**
 * Construct the effect class
 *
 * @param name The name of the effect
 * @param effectSets The sets that the rain operates on. Ie the columns to rain down.
 * @param speed The speed of the rain
 */
Rainfall::Rainfall(std::string name, vector<Set1D *> effectSets, float speed) : Effect(std::move(name),speed) {
  this->effectSets = std::move(effectSets);
  maxDrops = (int) this->effectSets.size() + 2;
  effectHeight = this->effectSets[0]->getSetSize();

  drops = vector<Drop>(maxDrops);
  for (auto &drop: drops) {
    drop.active = false;
  }
}

/**
 * Render the rain effect
 *
 * @param output The output buffer to render to
 */
void Rainfall::renderEffect(vector<uint32_t> &output) {
  if (millis() - lastSpawnCheck > 100) {
    lastSpawnCheck = millis();
    if (random(0, 100) < 15) {  // 15% chance of spawning each .1s
      spawnDrop();
    }
  }

  // Move the drops down
  for (auto &drop: drops) {
    if (drop.active) {
      drop.y -= drop.speed * speed; // Move the drop down
      if (drop.y <= -drop.trailLength) {
        drop.active = false; // Deactivate if it reaches the bottom
      }
    }
  }

  for (int col = 0; col < effectSets.size(); col++) {
    Set1D *set = effectSets[col];
    // Set is a column, the group is pixels in the column
    for (int y = 0; y < set->getSetSize(); y++) {
      auto pairing = set->getPairings()[y];

      float totalBrightnessR = 0.0;
      float totalBrightnessG = 0.0;
      float totalBrightnessB = 0.0;

      // Check distance to all active drops in the same column
      for (const auto &drop: drops) {
        if (drop.active && drop.x == col) {
          float distance = fabs(drop.y - y); // Absolute distance to the drop
          if (distance <= drop.trailLength) {
            // Calculate brightness factor (linear fade from 1.0 to 0.0)
            float brightnessFactor = 1.0 - (distance / drop.trailLength);

            // Decompose drop color into RGB components
            uint8_t r = (drop.color >> 16) & 0xFF;
            uint8_t g = (drop.color >> 8) & 0xFF;
            uint8_t b = drop.color & 0xFF;

            // Add scaled brightness to total brightness for each color channel
            totalBrightnessR += r * brightnessFactor / 255.0;
            totalBrightnessG += g * brightnessFactor / 255.0;
            totalBrightnessB += b * brightnessFactor / 255.0;
          }
        }
      }
      // Cap total brightness to 1.0
      totalBrightnessR = min(1.0f, totalBrightnessR);
      totalBrightnessG = min(1.0f, totalBrightnessG);
      totalBrightnessB = min(1.0f, totalBrightnessB);

      // Scale brightness back to 0-255 range
      uint8_t r = (uint8_t) (totalBrightnessR * 255);
      uint8_t g = (uint8_t) (totalBrightnessG * 255);
      uint8_t b = (uint8_t) (totalBrightnessB * 255);

      // set the group colour
      uint32_t col = Adafruit_NeoPixel::Color(r, g, b);
      pairing.group->setLeds(col, output);


    }


  }
}

void Rainfall::spawnDrop() {
  for (auto &drop: drops) {
    if (!drop.active) {
      drop.x = random(0, (int) effectSets.size()); // Random column
      drop.y = (float)effectHeight;                // Start at the top
      drop.active = true;
      int i = random(0, 255);
      drop.color = Gradient::sampleGradient(i); // Random color
      drop.speed = (float(effectHeight) / 8.0) *float(random(20, 80)) / 1000.0; // Random speed
      drop.trailLength = (float(effectHeight) / 8.0) * float(random(10, 19)) / 10.0f;
      return; // Spawn one drop per frame
    }
  }
}





