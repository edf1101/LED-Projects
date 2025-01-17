#include "RippleEffect.h"
#include "lib/helpers.h"
#include <cmath>

RippleEffect::RippleEffect(std::string name, Set2D *effectSet, float speed)
        : Effect(std::move(name), speed), effectSet(effectSet) {}

void RippleEffect::spawnRipple() {
  // Spawn a new ripple at a random position with a random intensity
  Ripple ripple = {
          .centerX = float(random(0, 100)) / 100.0f, // Random position
          .centerY = float(random(0, 100)) / 100.0f,
          .endRadius = 1.0f, // Normalized max radius
          .currentRadius = 0.0f,
          .intensity = 0.7f, // Random intensity
          .colour = Gradient::sampleGradient(random(0, 255))
  };
  ripples.push_back(ripple);
}

void RippleEffect::renderEffect(std::vector<uint32_t> &output) {
  float minPos = min(effectSet->getMinX(), effectSet->getMinY());
  float maxPos = max(effectSet->getMaxX(), effectSet->getMaxY());

  // Normalize positions to a 0-1 range
  for (auto &pair: effectSet->getPairings()) {
    float xPos = helpers::floatMap(pair.xPos, minPos, maxPos, 0, 1);
    float yPos = helpers::floatMap(pair.yPos, minPos, maxPos, 0, 1);

    // Initialize LED color
    uint8_t r = 0, g = 0, b = 0;

    // Calculate ripple effects
    for (auto &ripple: ripples) {
      float distance = sqrt(pow(xPos - ripple.centerX, 2) + pow(yPos - ripple.centerY, 2));

      // Check if the distance falls within the ripple's radius
      if (distance < ripple.currentRadius && distance > ripple.currentRadius - 0.1f) {
        float brightness = ripple.intensity * (1.0f - ripple.currentRadius / ripple.endRadius);
        // get the colour of the ripple
        uint8_t rippleR = (ripple.colour >> 16) & 0xFF;
        uint8_t rippleG = (ripple.colour >> 8) & 0xFF;
        uint8_t rippleB = ripple.colour & 0xFF;

        r += brightness * rippleR;
        g += brightness * rippleG;
        b += brightness * rippleB;
      }
    }
    r = min(255, (int) r);
    g = min(255, (int) g);
    b = min(255, (int) b);
    pair.group->setLeds(Adafruit_NeoPixel::Color(r, g, b), output);

  }

  // Update ripple states
  for (auto &ripple: ripples) {
    ripple.currentRadius += speed * 0.005f; // Expand ripple
  }

  // Remove ripples that have reached their max radius
  ripples.erase(std::remove_if(ripples.begin(), ripples.end(),
                               [](const Ripple &r) { return r.currentRadius >= r.endRadius; }),
                ripples.end());

  // Spawn new ripples periodically
  if (millis() - lastSpawnCheck > 100) {
    lastSpawnCheck = millis();
    if (ripples.size() < maxRipples && random(0, 100) < 10) { // 5% chance to spawn a new ripple per .1s
      spawnRipple();
    }
  }
}
