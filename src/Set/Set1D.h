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
    Set1D(vector<Set1DPair> pairings);

    int getSetSize();

    Set1DPair getPair(int i);

    int getMaxX() const;

    int getMinX();

private:
    vector<Set1DPair> pairings;
    int maxX;
    int minX;

};


#endif //LED_PROJECTS_SET1D_H
