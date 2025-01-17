/*
 * Created by Ed Fillingham on 15/01/2025.
*/

#ifndef LED_PROJECTS_WAVE2D_H
#define LED_PROJECTS_WAVE2D_H

#include "Effect.h"
#include "Set/Set2D.h"

class Wave2D : public Effect{
public:
    explicit Wave2D(std::string name, Set2D *effectSet,float angle, float speed = 1);

    void renderEffect(std::vector<uint32_t>& output) override;
private:
    Set2D* effectSet;
    float angle;
};


#endif //LED_PROJECTS_WAVE2D_H
