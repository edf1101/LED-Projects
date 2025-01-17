/*
 * Created by Ed Fillingham on 15/01/2025.
*/

#ifndef LED_PROJECTS_RADIALWAVE_H
#define LED_PROJECTS_RADIALWAVE_H

#include "Effects/Effect.h"
#include "Set/Set2D.h"

class RadialWave : public Effect {
public:
    explicit RadialWave(std::string name, Set2D *effectSet, float centerX = 0.5, float centerY = 0.5, float speed = 1);

    void renderEffect(std::vector<uint32_t> &output) override;

private:
    Set2D *effectSet;
    float centerX;
    float centerY;
};



#endif //LED_PROJECTS_RADIALWAVE_H
