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
  maxX = 0;
  minX = 0;
  for (auto &pair: pairings) {
    if (pair.iPos > maxX) {
      maxX = pair.iPos;
    }
    if (pair.iPos < minX) {
      minX = pair.iPos;
    }
  }
  this->maxX = maxX;
  this->minX = minX;
}

/**
 * Get the size of the set.
 *
 * @return the size of the set.
 */
int Set1D::getSetSize() {
  return pairings.size();
}

/**
 * Get the group / location at a given index.
 * @param i The index of the group / location to get.
 * @return The group / location at the given index.
 */
Set1DPair Set1D::getPair(int i) {
  return pairings[i];
}

/**
 * Get the maximum x value of the set.
 *
 * @return the maximum x value of the set.
 */
int Set1D::getMaxX() const {
  return maxX;
}

/**
 * Get the minimum x value of the set.
 *
 * @return the minimum x value of the set.
 */
int Set1D::getMinX() {
  return minX;
}

