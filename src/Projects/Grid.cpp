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

void Grid::createSets() {

  // create the horizontal row set
  std::vector<Set1DPair> rowPairings;
  for (int i = 0; i < gridHeight; i++) {
    std::string name = "Row" + std::to_string(i);
    rowPairings.push_back({groups[name], 0});
  }
  auto *rowSet = new Set1D(rowPairings);
  sets.insert({"AllRowSet", rowSet});

  // create the vertical column set
  std::vector<Set1DPair> colPairings;
  for (int i = 0; i < gridWidth; i++) {
    std::string name = "Col" + std::to_string(i);
    colPairings.push_back({groups[name], 0});
  }
  auto *colSet = new Set1D(colPairings);
  sets.insert({"AllColSet", colSet});


  // create the individual column sets
  for (int i = 0; i < gridWidth; i++) {
    std::vector<Set1DPair> pairings;
    for (int j = 0; j < gridHeight; j++) {
      std::string name = "Base" + std::to_string(gridToPix(i, j));
      pairings.push_back({groups[name], 0});
    }
    auto *set = new Set1D(pairings);
    sets.insert({"IndivCol" + std::to_string(i), set});
  }

  // create the grid set
  std::vector<Set2DPair> gridPairings;
  for (int i = 0; i < gridWidth; i++) {
    for (int j = 0; j < gridHeight; j++) {
      std::string name = "Base" + std::to_string(gridToPix(i, j));
      gridPairings.push_back({groups[name], (float) i, (float) j});
    }
  }

}

void Grid::init() {
  createGroups();
  createSets();
}

int Grid::gridToPix(int x, int y) {
  int n = x * 8;
  if (x % 2 == 0)
    n += y;
  else
    n += 7 - y;
  return n;
}


