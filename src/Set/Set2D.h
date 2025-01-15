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
    Set2D(vector<Set2DPair> pairings);

    int getSetSize();

    Set2DPair getPair(int i);

    int getMaxX() const { return maxX; }
    int getMaxY() const { return maxY; }
    int getMinX() const { return minX; }
    int getMinY() const { return minY; }

private:
    vector<Set2DPair> pairings;
    int maxX = 0;
    int maxY = 0;
    int minX = 0;
    int minY = 0;

};


#endif //LED_PROJECTS_SET2D_H
