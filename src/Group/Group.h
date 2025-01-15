/*
 * Created by Ed Fillingham on 06/01/2025.
*/

#ifndef LED_PROJECTS_GROUP_H
#define LED_PROJECTS_GROUP_H

#include "Arduino.h"
#include "lib/Adafruit_NeoPixel/Adafruit_NeoPixel.h"
#include "vector"

class Project; // forward declaration

/**
 * Group class.
 */
class Group {
public:
    Group(std::vector<int> ledIndices); // constructor for when you have a base group of just LEDs
    Group(std::vector<Group*> subGroups); // constructor for when you have a group made of other groups

    void setLeds(uint32_t colour, std::vector<uint32_t>& output);

private:
    Project* myProject;

    std::vector<int> ledIndices;
    std::vector<Group*> subGroups;
};


#endif //LED_PROJECTS_GROUP_H
