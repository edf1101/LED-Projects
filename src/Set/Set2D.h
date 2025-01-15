/*
 * Created by Ed Fillingham on 14/01/2025.
*/

#ifndef LED_PROJECTS_SET2D_H
#define LED_PROJECTS_SET2D_H

#include "vector"
#include "Group/Group.h"
#include "Set.h"

using namespace std;

struct Set2DPair {
    Group *group;
    float xPos;
    float yPos;
};

class Set2D : public Set {
public:
    explicit Set2D(vector<Set2DPair> pairings);

    int getSetSize();

    vector<Set2DPair> getPairings(){return pairings;};

    float getMaxX() const { return maxX; }
    float getMaxY() const { return maxY; }
    float getMinX() const { return minX; }
    float getMinY() const { return minY; }

private:
    vector<Set2DPair> pairings;
    float maxX = -infinityf();
    float maxY = -infinityf();
    float minX = infinityf();
    float minY = infinityf();

};


#endif //LED_PROJECTS_SET2D_H
