#ifndef LED_PROJECTS_RIPPLEEFFECT_H
#define LED_PROJECTS_RIPPLEEFFECT_H

#include "Effect.h"
#include "Set/Set2D.h"
#include <vector>

class RippleEffect : public Effect {
public:
    explicit RippleEffect(std::string name, Set2D *effectSet, float speed = 1);

    void renderEffect(std::vector<uint32_t> &output) override;

private:
    unsigned long lastSpawnCheck = 0;
    Set2D *effectSet;

    struct Ripple {
        float centerX;
        float centerY;
        float endRadius;
        float currentRadius;
        float intensity;
        uint32_t colour;
    };

    int maxRipples = 3;
    std::vector<Ripple> ripples;

    void spawnRipple();
};

#endif //LED_PROJECTS_RIPPLEEFFECT_H
