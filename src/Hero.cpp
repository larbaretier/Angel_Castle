// Hero
// Playable character

#include "Hero.h"
#include <SFML/Graphics.hpp>
#include "Collision.h"
#include <iostream>
#include <vector>
#include <set>

Hero::Hero() {
    // Basic initialization for the Hero, no sprite
	this->zLevel = 0;
    this->setPosition(0,0);
    this->speed = 1;
    this->setAnimationId(1);//idle
    this->animationSpeed = 1;
    this->setFacing(0);//down
    this->setSpriteSize(47, 64); //(121, 135)
    this->myMask = sf::RectangleShape(sf::Vector2f(47, 64));
    this->thingsToColide = {};
}
Hero::Hero(std::vector<std::string>& sheetPaths_v, int speed_h_v, int x_v, int y_v, int sprx,
		int spry, int facing, int sizex, int sizey) {
	// More current initialization, with sprite
	// # sheetPaths_v : a vector with the main spritesheets, first is for idle, second for walk
	// (a spritesheet is composed of 4 lines, one for each direction : down, up, left, right)
	// # speed_h_v : speed of the hero walk in pixel per frame
	// # x_v : x position of the hero
	// # y_v : y position of the hero
	// # sprx : sprite width
	// # spry : sprite height
	// # facing : the face position of the hero : Down = 0, Up = 1, Left = 2, Right = 3
	// # sizex : collision mask width
	// # sizey : collision mask height
    this->setPosition(x_v,y_v);
    this->zLevel = 0;
    this->animationSpeed = 1;
    this->solid = true;
    this->visible = true;
    this->setAnimationId(1);//idle
    this->setFacing(facing);//down
    this->setSpriteSize(sprx, spry); //(121, 135)
    this->speed = speed_h_v;
    this->myMask = sf::RectangleShape(sf::Vector2f(sizex, sizey));
    this->sheetPaths=sheetPaths_v;
    this->thingsToColide = {};
}


void Hero::press_button(QuadTree* root, LevelMap* level) {
	// Detects which important button for the hero is pressed and acts according to it (move, set sprite)
	// # root : the quadtree to check the collisions with other solid objects in case the hero wants to move
	// # level : the current level map to check the collisions with the solid tiles in case the hero wants to move

	sf::Vector2f mainMove;
	mainMove.x = 0;
	mainMove.y = 0;
    int a = this->getAnimation().y;
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up)){
    	thingsToColide.clear();
    	this->prepareCollision(root);

    	mainMove.y = mainMove.y - this->speed;
    	if (!Collision::collisionsMapIncluded(this, this->thingsToColide, mainMove, level))
    	{
			this->setPosition(this->getPosition().x, this->getPosition().y - this->speed);
			this->setAnimation(this->getAnimation().x, this->getUp());
			if (a!= this->getAnimation().y)
			{
				this->set_animation();
			}
    	}
        this->setAnimationId(2);
        this->setFacing(this->getUp());
    }else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down)){
    	thingsToColide.clear();
    	this->prepareCollision(root);
    	mainMove.y = mainMove.y + this->speed;
		if (!Collision::collisionsMapIncluded(this, this->thingsToColide, mainMove, level))
		{
			this->setPosition(this->getPosition().x, this->getPosition().y + this->speed);
			this->setAnimation(this->getAnimation().x, this->getDown());
			if (a!= this->getAnimation().y){
				this->set_animation();
			}
			this->setAnimationId(2);
			this->setFacing(this->getDown());
		}
    }else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right)){
    	thingsToColide.clear();
    	this->prepareCollision(root);
    	mainMove.x = mainMove.x + this->speed;
		if (!Collision::collisionsMapIncluded(this, this->thingsToColide, mainMove, level))
		{
			this->setPosition(this->getPosition().x + this->speed, this->getPosition().y);
			this->setAnimation(this->getAnimation().x, this->getRight());
			if (a!= this->getAnimation().y){
				this->set_animation();
			}
			this->setAnimationId(2);
			this->setFacing(this->getRight());
		}
    }else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left)){
    	thingsToColide.clear();
    	this->prepareCollision(root);
    	mainMove.x = mainMove.x - this->speed;
		if (!Collision::collisionsMapIncluded(this, this->thingsToColide, mainMove, level))
		{
			this->setPosition(this->getPosition().x - this->speed, this->getPosition().y);
			this->setAnimation(this->getAnimation().x, this->getLeft());
			if (a!= this->getAnimation().y){
				this->set_animation();
			}
			this->setAnimationId(2);
			this->setFacing(this->getLeft());
		}
    }
    else{
        //if no button pressed, idle
        this->setAnimationId(1);
    }
}

Hero::~Hero() {
	// nothing
}
