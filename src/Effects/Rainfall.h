/*
 * Created by Ed Fillingham on 16/01/2025.
*/

#ifndef LED_PROJECTS_RAINFALL_H
#define LED_PROJECTS_RAINFALL_H


#include "Effect.h"
#include "Set/Set1D.h"
#include "vector"

class Rainfall : public Effect {
public:
    explicit Rainfall(std::string name, vector<Set1D *> effectSets, float speed = 1);

    void renderEffect(std::vector<uint32_t> &output) override;

private:
    vector<Set1D *> effectSets;
    float speed;
    int maxDrops = 1;
    int effectHeight = 1;
    // Structure to hold each raindrop's status
    struct Drop {
        int x;       // X-position will always be in a set column
        float y;     // Y-position (float for smooth motion)
        bool active; // True if the drop is currently visible
        uint32_t color; // Color of the drop
        float speed; // Speed at which the drop falls
        float trailLength = 1.5;
    };

    vector<Drop> drops; // Vector to hold all the raindrops

    void spawnDrop();

    unsigned long lastSpawnCheck = 0;
};


#endif //LED_PROJECTS_RAINFALL_H
