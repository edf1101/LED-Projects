/*
 * Created by Ed Fillingham on 15/01/2025.
*/

#ifndef LED_PROJECTS_WAVE1D_H
#define LED_PROJECTS_WAVE1D_H

#include "Effect.h"
#include "Set/Set1D.h"

class Wave1D : public Effect{
public:
    explicit Wave1D(std::string name, Set1D *effectSet, float speed = 1);

    void renderEffect(std::vector<uint32_t>& output) override;
private:
    Set1D* effectSet;
    float speed;
};


#endif //LED_PROJECTS_WAVE1D_H
