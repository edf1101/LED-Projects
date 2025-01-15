/*
 * Created by Ed Fillingham on 14/01/2025.
*/

#include "Set2D.h"

#include <utility>

/**
 * Constructor for the Set1D class
 *
 * @param pairings the pairings of groups and their positions
 */
Set2D::Set2D(vector<Set2DPair> pairings) {
  this->pairings = std::move(pairings);

  // calculate the max and min x and y values
  for (auto &pair: this->pairings) {
    if (pair.xPos > maxX) {
      maxX = pair.xPos;
    }
    if (pair.xPos < minX) {
      minX = pair.xPos;
    }
    if (pair.yPos > maxY) {
      maxY = pair.yPos;
    }
    if (pair.yPos < minY) {
      minY = pair.yPos;
    }
  }

}

/**
 * Get the size of the set.
 *
 * @return the size of the set.
 */
int Set2D::getSetSize() {
  return (int) pairings.size();
}

