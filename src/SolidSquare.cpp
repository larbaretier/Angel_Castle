// SolidSquare
// # mysize : width and height of the square
// # position_solid_square : position of the square
// # shape : shape of the square (TODO : delete this)

#include "SolidSquare.h"
#include <SFML/Graphics.hpp>

SolidSquare::SolidSquare(int x, int y, int w, int h) {
	// Constructor
	// # x : coordinate of the object
	// # y : coordinate of the object
	// # w : width of the square
	// # h : height of the square
    this->mysize.y=h;
    this->mysize.x=w;
    this->position_solid_square.x=x;
    this->position_solid_square.y=y;
    this->solid = true;
    this->visible = true;
}

SolidSquare::~SolidSquare() {
    // Destructor
}

void SolidSquare::draw_thing(sf::RenderWindow &w) {
	// Draw the square object
	// # w : game window

	if (this->visible)
	{
		this->shape.setPosition(this->position_solid_square.x, this->position_solid_square.y);
		this->shape.setOrigin(this->mysize.x/2, this->mysize.y/2);
		this->shape.setSize(this->mysize);
		this->shape.setFillColor(sf::Color::Blue);
		w.draw(this->shape);
	}
}

sf::Vector2f SolidSquare::getPosition() {
    return this->position_solid_square;
}

sf::Vector2f SolidSquare::getMysize() {
    return this->mysize;
}
