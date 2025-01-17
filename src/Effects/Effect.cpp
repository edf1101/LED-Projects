/*
 * Created by Ed Fillingham on 15/01/2025.
*/

#include "Effect.h"

#include <utility>

Effect::Effect(std::string name,float speed) {
  this->name = std::move(name);
  this->timeValue = 0;
    this->speed = speed;

}
