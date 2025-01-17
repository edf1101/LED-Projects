/*
 * Created by Ed Fillingham on 15/01/2025.
*/

#ifndef LED_PROJECTS_PERLINEFFECT_H
#define LED_PROJECTS_PERLINEFFECT_H

#include "Effect.h"
#include "Set/Set2D.h"

class PerlinEffect : public Effect{
public:
    explicit PerlinEffect(std::string name, Set2D *effectSet,float frequency = 2.0, float speed = 1);

    void renderEffect(std::vector<uint32_t>& output) override;
private:
    Set2D* effectSet;
    float frequency;
};


#endif //LED_PROJECTS_PERLINEFFECT_H
