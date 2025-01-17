#include "BouncingBallsEffect.h"
#include "Lib/Gradient.h"
#include <utility>

BouncingBallsEffect::BouncingBallsEffect(std::string name, Set2D *effectSet, int numBalls, float speed, float ballSize)
        : Effect(std::move(name),speed) , effectSet(effectSet), numBalls(numBalls), ballSize(ballSize) {
  initializeBalls();
}

/**
 * This function initialises the balls with random positions and velocities
 */
void BouncingBallsEffect::initializeBalls() {
  lastUpdateTime = millis();
  float maxX = effectSet->getMaxX();
  float maxY = effectSet->getMaxY();
  float minX = effectSet->getMinX();
  float minY = effectSet->getMinY();

  for (int i = 0; i < numBalls; i++) {
    Ball ball;
    ball.x = random(minX, maxX);
    ball.y = random(minY, maxY);

    // create velocity
    vector2d direction = normalise({(float) random(-10, 10), (float) random(-10, 10)});
    float randomSpeed = random(6, 14) / 10.0f;
    ball.vx = direction.x * randomSpeed;
    ball.vy = direction.y * randomSpeed;

    ball.colorInd = random(0, 255);
    balls.push_back(ball);
  }
}

void BouncingBallsEffect::renderEffect(std::vector<uint32_t> &output) {
  calculateBallPositions();

  for (auto pair : effectSet->getPairings()) {
    float posX = pair.xPos;
    float posY = pair.yPos;

    uint8_t finalR = 0, finalG = 0, finalB = 0;

    // Iterate over all balls
    for (auto &ball : balls) {
      float dx = ball.x - posX;
      float dy = ball.y - posY;
      float distance = sqrt(dx * dx + dy * dy);

      if (distance < ballSize) {
        // Calculate fade factor based on distance
        float fadeFactor = 1.0f - (distance / ballSize); // Fade factor between 0 and 1

        // Extract ball color
        uint32_t orgColor = Gradient::sampleGradient(ball.colorInd);
        uint8_t r = (orgColor >> 16) & 0xFF;
        uint8_t g = (orgColor >> 8) & 0xFF;
        uint8_t b = orgColor & 0xFF;

        // Apply fade factor to the ball's color
        r = static_cast<uint8_t>(r * fadeFactor);
        g = static_cast<uint8_t>(g * fadeFactor);
        b = static_cast<uint8_t>(b * fadeFactor);

        // Blend with existing color in this pixel
        finalR = std::min(255, finalR + r);
        finalG = std::min(255, finalG + g);
        finalB = std::min(255, finalB + b);
      }
    }

    // Write the blended color to the pixel
    uint32_t blendedColor = Adafruit_NeoPixel::Color(finalR, finalG, finalB);
    pair.group->setLeds(blendedColor, output);
  }
}

/**
 * This function calculates the new position of the balls
 */
void BouncingBallsEffect::calculateBallPositions() {
  unsigned long currentTime = millis();
  float deltaTime = (currentTime - lastUpdateTime) / 1000.0f; // Convert to seconds
  lastUpdateTime = currentTime;

  float maxX = effectSet->getMaxX();
  float maxY = effectSet->getMaxY();
  float minX = effectSet->getMinX();
  float minY = effectSet->getMinY();

  float speedFactor = 2;
  for (auto &ball : balls) {
    ball.x += ball.vx * speed * deltaTime * speedFactor;
    ball.y += ball.vy * speed * deltaTime * speedFactor;

    // Check for collision with the walls and adjust position/velocity
    if (ball.x < minX) {
      // Ball is outside on the left
      ball.x = 2 * minX - ball.x; // Reflect position inside
      ball.vx = fabs(ball.vx);    // Ensure velocity is positive

      // Add a small random deviation
      ball.vy += ((float)(rand() % 20 - 10) / 100.0f); // Range [-0.1, 0.1]
    } else if (ball.x > maxX) {
      // Ball is outside on the right
      ball.x = 2 * maxX - ball.x; // Reflect position inside
      ball.vx = -fabs(ball.vx);   // Ensure velocity is negative

      // Add a small random deviation
      ball.vy += ((float)(rand() % 20 - 10) / 100.0f); // Range [-0.1, 0.1]
    }

    if (ball.y < minY) {
      // Ball is outside on the bottom
      ball.y = 2 * minY - ball.y; // Reflect position inside
      ball.vy = fabs(ball.vy);    // Ensure velocity is positive

      // Add a small random deviation
      ball.vx += ((float)(rand() % 20 - 10) / 100.0f); // Range [-0.1, 0.1]
    } else if (ball.y > maxY) {
      // Ball is outside on the top
      ball.y = 2 * maxY - ball.y; // Reflect position inside
      ball.vy = -fabs(ball.vy);   // Ensure velocity is negative

      // Add a small random deviation
      ball.vx += ((float)(rand() % 20 - 10) / 100.0f); // Range [-0.1, 0.1]
    }

    // Ensure velocity does not become too small or too large
    float magnitude = std::sqrt(ball.vx * ball.vx + ball.vy * ball.vy);
    if (magnitude < 0.1f) {
      ball.vx *= 1.1f;
      ball.vy *= 1.1f;
    } else if (magnitude > 2.0f) {
      ball.vx *= 0.9f;
      ball.vy *= 0.9f;
    }
  }
}



BouncingBallsEffect::vector2d BouncingBallsEffect::normalise(BouncingBallsEffect::vector2d v) {
  {
    float magnitude = std::sqrt(v.x * v.x + v.y * v.y);

    // Avoid division by zero
    if (magnitude == 0.0f) {
      return {0.0f, 0.0f}; // Return a zero vector if the magnitude is zero
    }

    return {v.x / magnitude, v.y / magnitude};
  }
}
