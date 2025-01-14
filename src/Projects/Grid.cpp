/*
 * Created by Ed Fillingham on 07/01/2025.
*/

#include "Grid.h"

Grid::Grid(short dataPin, neoPixelType type, short gridWidth, short gridHeight) : Project(128, dataPin, type) {
  this->gridWidth = gridWidth;
  this->gridHeight = gridHeight;
}

void Grid::createGroups() {
  // create the single pixel groups
  for (int i = 0; i < numLeds; i++) {
    std::vector<int> groupLeds = {i};
    Group *group = new Group(this, groupLeds);
    std::string name = "Base" + std::to_string(i);
    groups.insert({name, group});
  }

  // create groups of rows
  for (int row = 0; row < gridHeight; row++) {
    std::vector<int> groupLeds;
    for (int x = 0; x < gridWidth; x++) {
      groupLeds.push_back(gridToPix(x, row));
    }
    auto *group = new Group(this, groupLeds);
    std::string name = "Row" + std::to_string(row);
    groups.insert({name, group});
  }

  // create groups of columns
  for (int i = 0; i < gridWidth; i++) {
    std::vector<int> groupLeds;
    for (int j = 0; j < gridHeight; j++) {
      groupLeds.push_back(gridToPix(i, j));
    }
    auto *group = new Group(this, groupLeds);
    std::string name = "Col" + std::to_string(i);
    groups.insert({name, group});
  }
}

void Grid::init() {
  createGroups();

}

int Grid::gridToPix(int x, int y) {
  int n = x * 8;
  if (x % 2 == 0)
    n += y;
  else
    n += 7 - y;
  return n;
}
