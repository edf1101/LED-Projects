/*
 * Created by Ed Fillingham on 14/01/2025.
*/

#include "Set1D.h"

#include <utility>

/**
 * Constructor for the Set1D class
 *
 * @param pairings the pairings of groups and their positions
 */
Set1D::Set1D(vector<Set1DPair> pairings) {
  this->pairings = std::move(pairings);

  // calculate the max x value
  maxX = -infinityf();
  minX = infinityf();
  for (auto &pair: this->pairings) {
    if (pair.iPos > maxX) {
      maxX = pair.iPos;
    }
    if (pair.iPos < minX) {
      minX = pair.iPos;
    }
  }
}

/**
 * Get the size of the set.
 *
 * @return the size of the set.
 */
int Set1D::getSetSize() {
  return (int)pairings.size();
}

/**
 * Get the maximum x value of the set.
 *
 * @return the maximum x value of the set.
 */
float Set1D::getMaxX() {
  return maxX;
}

/**
 * Get the minimum x value of the set.
 *
 * @return the minimum x value of the set.
 */
float Set1D::getMinX() {
  return minX;
}

