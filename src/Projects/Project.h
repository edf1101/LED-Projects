/*
 * Created by Ed Fillingham on 06/01/2025.
 *
 * This class represents a project. Ie nano-leafs, led cube, Light grid etc.
*/

#ifndef LED_PROJECTS_PROJECT_H
#define LED_PROJECTS_PROJECT_H

#include <Arduino.h>
#include "lib/Audio/SpectrumAnalysis.h"
#include <unordered_map>
#include "lib/Adafruit_NeoPixel/Adafruit_NeoPixel.h"

#include "Group/Group.h"

#include "Set/Set.h"
#include "Set/Set1D.h"
#include "Set/Set2D.h"

#include "Effects/Effect.h"

#include "string"
#include "vector"

/**
 * This class represents a project. Ie nano-leafs, led cube, Light grid etc.
 */
class Project {
public:
    Project(int numLeds, short dataPin, neoPixelType type);

    virtual void init() = 0; // So we don't call subclasses' virtual functions in the constructor
    void loop(); // This loops through the effects and renders them

    short getDataPin() const { return dataPin; }

    int getNumLeds() const { return numLeds; }

    neoPixelType getType() const { return type; }

    void drawLeds(Adafruit_NeoPixel *strip);

    void setLed(int ledIndex, uint32_t colour);

    void setEffect(const std::string& effectName, float weight);

protected:
    const int numLeds;
    const short dataPin;
    const neoPixelType type;

    /**
     * A group is a collection of leds, store the groups here. Ie vertical bars, horizontal bars, etc
     */
    std::unordered_map<std::string, Group *> groups;

    /**
     * A set is an ordered collection of groups, store the sets here.
     * Ie all the vertical bars, all the horizontal bars, etc
     */
    std::unordered_map<std::string, Set *> sets;

    /**
     * A map of all the effects in the project
     */
    std::unordered_map<std::string, Effect *> effects;
    /**
     * This hold the colour of each led in the project
     */
    std::vector<uint32_t> leds;

    virtual void createGroups() = 0;

    virtual void createSets() = 0;

    virtual void createEffects() = 0;

private:
    struct EffectWeight {
        Effect *effect;
        float weight;
    };
    std::vector<EffectWeight> currentEffects;

};


#endif //LED_PROJECTS_PROJECT_H
