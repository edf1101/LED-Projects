/*
 * Created by Ed Fillingham on 06/01/2025.
*/

#include "Group.h"

#include <utility>
#include "Projects/Project.h"

/**
 * This is the constructor for the Group class. It is used when you have a base group of just LEDs.
 *
 * @param ledIndices The indices of the LEDs that are part of this group.
 */
Group::Group(std::vector<int> ledIndices) {
  this->myProject = myProject;
  this->ledIndices = std::move(ledIndices);
}

/**
 * This is the constructor for the Group class. It is used when you have a group made of other groups.
 *
 * @param subGroups The subgroups that make up this group.
 */
Group::Group(std::vector<Group*> subGroups) {
  this->subGroups = std::move(subGroups);
}

/**
 * This function is used to draw the LEDs of the group and its subgroups.
 *
 * @param colour The colour that the LEDs should be set to.
 */
void Group::setLeds(uint32_t colour, std::vector<uint32_t>& output) {
  for (int ledIndice: ledIndices) {
    output[ledIndice] = colour;
  }
  for (Group* &subGroup: subGroups) {
    subGroup->setLeds(colour, output);
  }
}

