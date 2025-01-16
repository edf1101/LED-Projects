/*
 * Created by Ed Fillingham on 07/01/2025.
*/

#ifndef LED_PROJECTS_GRID_H
#define LED_PROJECTS_GRID_H

#include "Project.h"

class Grid : public Project {
public:
    explicit Grid(short dataPin = 4, neoPixelType colourType = NEO_GRB, neoPixelType otherData = NEO_KHZ800, short gridWidth = 16,
                  short gridHeight = 8);

    void init();
    void addAudioAnalysis(int sck, int ws, int sd, bool leftChannel) override;
private:
    void createGroups() override;

    void createSets() override;

    void createEffects() override;

    int gridWidth;
    int gridHeight;

    int gridToPix(int x, int y); // Convert grid coordinates to pixel index
};


#endif //LED_PROJECTS_GRID_H
