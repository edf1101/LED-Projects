/*
 * Created by Ed Fillingham on 06/01/2025.
*/

#include "Group.h"

#include <utility>
#include "Projects/Project.h"

/**
 * This is the constructor for the Group class. It is used when you have a base group of just LEDs.
 *
 * @param myProject The reference to the project that the group is part of so we can later write to its LED strip.
 * @param ledIndices The indices of the LEDs that are part of this group.
 */
Group::Group(Project *myProject, std::vector<int> ledIndices) {
  this->myProject = myProject;
  this->ledIndices = std::move(ledIndices);
}

/**
 * This is the constructor for the Group class. It is used when you have a group made of other groups.
 *
 * @param myProject The reference to the project that the group is part of so we can later write to its LED strip.
 * @param subGroups The subgroups that make up this group.
 */
Group::Group(Project *myProject, std::vector<Group*> subGroups) {
  this->myProject = myProject;
  this->subGroups = std::move(subGroups);
}

/**
 * This function is used to draw the LEDs of the group and its subgroups.
 *
 * @param colour The colour that the LEDs should be set to.
 */
void Group::setLeds(uint32_t colour) {
  for (int ledIndice: ledIndices) {
    myProject->setLed(ledIndice, colour);
  }
  for (Group* &subGroup: subGroups) {
    subGroup->setLeds(colour);
  }
}

