/*
 * Created by Ed Fillingham on 07/01/2025.
*/

#ifndef LED_PROJECTS_GRID_H
#define LED_PROJECTS_GRID_H

#include "Project.h"

enum Rotation {
    ROTATION_0 = 0,
    ROTATION_90 = 90,
    ROTATION_180 = 180,
    ROTATION_270 = 270
};
enum GridWiring {
    ROW_MAJOR,
    COLUMN_MAJOR
};
class Grid : public Project {
public:
    explicit Grid(std::string name ,short dataPin = 4, neoPixelType colourType = NEO_GRB, neoPixelType otherData = NEO_KHZ800, short width = 16,
                  short height = 8, Rotation gridRotation = ROTATION_0,GridWiring gridWiring = ROW_MAJOR);

    void init();
    void addAudioAnalysis(int sck, int ws, int sd, bool leftChannel) override;
private:
    void createGroups() override;

    void createSets() override;

    void createEffects() override;

    int gridWidth;
    int gridHeight;

    int gridToPix(int x, int y); // Convert grid coordinates to pixel index

    Rotation gridRotation;
    GridWiring gridWiring;

    short origWidth;
    short origHeight;
};


#endif //LED_PROJECTS_GRID_H
