/*
 * Created by Ed Fillingham on 07/01/2025.
*/

#include "Grid.h"

#include <utility>

#include "Effects/Wave1D.h"
#include "Effects/Wave2D.h"
#include "Effects/PerlinEffect.h"
#include "Effects/RadialWave.h"
#include "Effects/AngularWave.h"
#include "Effects/Rainfall.h"
#include "Effects/RippleEffect.h"
#include "Effects/Spectrometer.h"
#include "Effects/BouncingBallsEffect.h"

Grid::Grid(std::string name, short dataPin, neoPixelType colourType, neoPixelType otherData, short width, short height,
           Rotation gridRotation, GridWiring gridWiring) : Project(
        std::move(name), 128, dataPin, colourType, otherData) {
  // Keep the *original* physical dims in separate variables:
  this->origWidth = width;
  this->origHeight = height;

  // Then do the swap in constructor so "gridWidth" and "gridHeight"
  // become the rotated dimension:
  if (gridRotation == ROTATION_90 || gridRotation == ROTATION_270) {
    this->gridWidth = height; // swapped
    this->gridHeight = width;
  } else {
    this->gridWidth = width;
    this->gridHeight = height;
  }

  this->gridRotation = gridRotation;
  this->gridWiring = gridWiring;
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
  auto *wave2D = new Wave2D("2D Wave", (Set2D *) sets["GridSet"], 135);
  effects.insert({wave2D->getName(), wave2D});

  auto *perlinEffect = new PerlinEffect("Perlin Effect", (Set2D *) sets["GridSet"], 1, 1);
  effects.insert({perlinEffect->getName(), perlinEffect});

  auto *radialWave = new RadialWave("Radial Wave", (Set2D *) sets["GridSet"]);
  effects.insert({radialWave->getName(), radialWave});

  auto *angularWave = new AngularWave("Angular Wave", (Set2D *) sets["GridSet"], 0.5, 0.5, 1);
  effects.insert({angularWave->getName(), angularWave});

  vector<Set1D *> indivCols = {};
  for (int i = 0; i < gridWidth; i++) {
    indivCols.push_back((Set1D *) sets["IndivCol" + std::to_string(i)]);
  }
  auto *rainEffect = new Rainfall("Rainfall", indivCols, 1);
  effects.insert({rainEffect->getName(), rainEffect});

  auto *rippleEffect = new RippleEffect("Ripple Effect", (Set2D *) sets["GridSet"], 1);
  effects.insert({rippleEffect->getName(), rippleEffect});

  auto *bouncingBallsEffect = new BouncingBallsEffect("Bouncing Balls", (Set2D *) sets["GridSet"], 3, 1, 2);
  effects.insert({bouncingBallsEffect->getName(), bouncingBallsEffect});


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
  auto *initialGradient = new Gradient(initialStops, 3, false);
  Gradient::setCurrentGradient(initialGradient);

}

int Grid::gridToPix(int x, int y) {
  int col, row; // "old" column and row in the *physical*, unrotated sense
  switch (gridRotation) {
    case ROTATION_90:
      col = y;
      row = (origHeight - 1) - x;
      break;
    case ROTATION_180:
      col = (origWidth - 1) - x;
      row = (origHeight - 1) - y;
      break;
    case ROTATION_270:
      col = (origWidth - 1) - y;
      row = x;
      break;
    default: // ROTATION_0
      col = x;
      row = y;
  }

  // Now do the physical zigzag:
  if (gridWiring == ROW_MAJOR) {
    int base = col * origHeight;
    if (col % 2 == 0) {
      return base + row;
    } else {
      return base + (origHeight - 1 - row);
    }
  } else { // COLUMN_MAJOR
    int index = row * origWidth;
    if (row % 2 == 0) {
      index += col;
    } else {
      index += (origWidth - 1 - col);
    }
    return index;
  }
}


void Grid::addAudioAnalysis(int sck, int ws, int sd, bool leftChannel) {
  Project::addAudioAnalysis(sck, ws, sd, leftChannel);

//   add effects that require audio analysis
  vector<Set1D *> indivCols = {};
  for (int i = 0; i < gridWidth; i++) {
    indivCols.push_back((Set1D *) sets["IndivCol" + std::to_string(i)]);
  }
  auto *spectrometer = new Spectrometer("Spectrometer", indivCols, 1);
  effects.insert({spectrometer->getName(), spectrometer});

}
