/*
 * Created by Ed Fillingham on 14/01/2025.
*/

#ifndef LED_PROJECTS_SET1D_H
#define LED_PROJECTS_SET1D_H

#include "vector"
#include "Group/Group.h"
#include "Set.h"

using namespace std;

struct Set1DPair {
    Group *group;
    float iPos;
};

class Set1D : public Set {
public:
    explicit Set1D(vector<Set1DPair> pairings);

    int getSetSize();

    vector<Set1DPair> getPairings(){return pairings;};
    float getMaxX();

    float getMinX();

private:
    vector<Set1DPair> pairings;
    float maxX;
    float minX;

};


#endif //LED_PROJECTS_SET1D_H
