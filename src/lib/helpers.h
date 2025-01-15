/*
 * Created by Ed Fillingham on 15/01/2025.
*/

#ifndef LED_PROJECTS_HELPERS_H
#define LED_PROJECTS_HELPERS_H

#include "Arduino.h"

class helpers {
public:
    static float floatMap(float x, float in_min, float in_max, float out_min, float out_max) {
      return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
    };
};


#endif //LED_PROJECTS_HELPERS_H
