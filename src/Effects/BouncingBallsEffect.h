#ifndef LED_PROJECTS_BOUNCINGBALLSEFFECT_H
#define LED_PROJECTS_BOUNCINGBALLSEFFECT_H

#include "Effect.h"
#include "Set/Set2D.h"
#include <vector>

class BouncingBallsEffect : public Effect {
public:
    explicit BouncingBallsEffect(std::string name, Set2D *effectSet, int numBalls, float speed, float ballSize);

    void renderEffect(std::vector<uint32_t> &output) override;

    void reset() override{
      Effect::reset();
      balls.clear();
      initializeBalls();
    }


private:
    struct Ball {
        float x, y;       // Current position
        float vx, vy;     // Velocity
        int colorInd;   // Ball color
    };

    unsigned long lastUpdateTime;
    Set2D *effectSet;
    int numBalls;
    float ballSize;
    std::vector<Ball> balls;

    void initializeBalls();

    void calculateBallPositions();


    struct vector2d {
        float x, y;
    };

    static vector2d normalise(vector2d v);

};

#endif // LED_PROJECTS_BOUNCINGBALLSEFFECT_H
