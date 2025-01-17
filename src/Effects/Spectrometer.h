#ifndef LED_PROJECTS_SPECTROMETER_H
#define LED_PROJECTS_SPECTROMETER_H

#include "Effect.h"
#include "Set/Set1D.h"
#include "vector"

class Spectrometer : public Effect {
public:
    explicit Spectrometer(std::string name, std::vector<Set1D *> effectSets, float speed = 1, float smoothingFactor = 0.1f);

    void renderEffect(std::vector<uint32_t> &output) override;

private:
    std::vector<Set1D *> effectSets;
    std::vector<float> smoothedFFT; // Stores the smoothed FFT values
    int effectHeight;
    float smoothingFactor; // Smoothing factor for rolling average

    void applySmoothing();
};

#endif //LED_PROJECTS_SPECTROMETER_H
