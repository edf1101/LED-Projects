/*
 * Created by Ed Fillingham on 15/01/2025.
*/

#ifndef LED_PROJECTS_EFFECT_H
#define LED_PROJECTS_EFFECT_H

#include "lib/Adafruit_NeoPixel/Adafruit_NeoPixel.h"

#include "Arduino.h"
#include "string"
#include "vector"
#include "Lib/Gradient.h"

class Effect {

public:
    explicit Effect(std::string name);

    std::string getName() { return name; };

    virtual void renderEffect(std::vector<uint32_t> &output) = 0;

    void reset() { timeValue = 0; };
protected:
    std::string name = "Effect";
    unsigned long timeValue;

};


#endif //LED_PROJECTS_EFFECT_H
