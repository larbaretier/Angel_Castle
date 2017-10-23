// Character
// Parent of the Npc and Hero
// # speed : how fast the object moves (pixel per frame)
// # name : name of the character for dialogue/UI purpose

#include "Character.h"
#include <string>
#include <iostream>
#include "Thing.h"

Character::Character() {
	// Basic initialization, no sprite
	this->speed = 0;
	this->zLevel = 0;
	this->solid = true;
	this->visible = true;

}
Character::Character(std::vector<std::string> sheetPaths_v) {
	// More current initialization, with sprite
	// # sheetPaths_v : a vector with the main spritesheets, first is for idle, second for walk
	// (a spritesheet is composed of 4 lines, one for each direction : down, up, left, right)
	this->speed = 0;
	this->sheetPaths = sheetPaths_v;
	this->zLevel = 0;
	this->solid = true;
	this->visible = true;

}
Character::~Character() {
	// Auto-generated destructor
}

