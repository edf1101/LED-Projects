/*
 * Created by Ed Fillingham on 07/01/2025.
*/

#include "Grid.h"

#include "Effects/Wave1D.h"
#include "Effects/Wave2D.h"
#include "Effects/PerlinEffect.h"
#include "Effects/RadialWave.h"
#include "Effects/AngularWave.h"

Grid::Grid(short dataPin, neoPixelType colourType, neoPixelType otherData, short gridWidth, short gridHeight) : Project(
        128, dataPin, colourType, otherData) {
  this->gridWidth = gridWidth;
  this->gridHeight = gridHeight;
}

void Grid::createGroups() {
  // create the single pixel groups
  for (int i = 0; i < numLeds; i++) {
    std::vector<int> groupLeds = {i};
    Group *group = new Group(groupLeds);
    std::string name = "Base" + std::to_string(i);
    groups.insert({name, group});
  }

  // create groups of rows
  for (int row = 0; row < gridHeight; row++) {
    std::vector<int> groupLeds;
    for (int x = 0; x < gridWidth; x++) {
      groupLeds.push_back(gridToPix(x, row));
    }
    auto *group = new Group(groupLeds);
    std::string name = "Row" + std::to_string(row);
    groups.insert({name, group});
  }

  // create groups of columns
  for (int i = 0; i < gridWidth; i++) {
    std::vector<int> groupLeds;
    for (int j = 0; j < gridHeight; j++) {
      groupLeds.push_back(gridToPix(i, j));
    }
    auto *group = new Group(groupLeds);
    std::string name = "Col" + std::to_string(i);
    groups.insert({name, group});
  }
}

void Grid::createSets() {

  // create the horizontal row set
  std::vector<Set1DPair> rowPairings;
  for (int i = 0; i < gridHeight; i++) {
    std::string name = "Row" + std::to_string(i);
    rowPairings.push_back({groups[name], float(i)});
  }
  auto *rowSet = new Set1D(rowPairings);
  sets.insert({"AllRowSet", rowSet});

  // create the vertical column set
  std::vector<Set1DPair> colPairings;
  for (int i = 0; i < gridWidth; i++) {
    std::string name = "Col" + std::to_string(i);
    colPairings.push_back({groups[name], float(i)});
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
  sets.insert({"GridSet", new Set2D(gridPairings)});

}

void Grid::createEffects() {

  // create the rainbow effect
  auto *wave1D = new Wave1D("1D Wave", (Set1D *) sets["AllColSet"]);
  effects.insert({wave1D->getName(), wave1D});

  auto *wave2D = new Wave2D("2D Wave", (Set2D *) sets["GridSet"], 135);
  effects.insert({wave2D->getName(), wave2D});

  auto *perlinEffect = new PerlinEffect("Perlin Effect", (Set2D *) sets["GridSet"], 1, 1);
  effects.insert({perlinEffect->getName(), perlinEffect});

  auto *radialWave = new RadialWave("Radial Wave", (Set2D *) sets["GridSet"]);
  effects.insert({radialWave->getName(), radialWave});

  auto *angularWave = new AngularWave("Angular Wave", (Set2D *) sets["GridSet"], 0.5, 0.5, 1);
  effects.insert({angularWave->getName(), angularWave});
}

void Grid::init() {
  createGroups();
  createSets();
  createEffects();

  // Initial gradient
  GradientStop *initialStops = new GradientStop[3]{
          {0,   255, 255, 0,   0},
          {128, 0,   255, 255, 0},
          {255, 255, 255, 0,   0},
  };
  Gradient *initialGradient = new Gradient(initialStops, 3, false);
  Gradient::setCurrentGradient(initialGradient);
}

int Grid::gridToPix(int x, int y) {
  int n = x * gridHeight;
  if (x % 2 == 0)
    n += y;
  else
    n += gridHeight - 1 - y;
  return n;
}
