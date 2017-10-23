// MapTile
// Tile objects (child of Thing)

#include "MapTile.h"
#include <SFML/Graphics.hpp>

MapTile::MapTile() {
	// Auto-generated constructor

}

MapTile::MapTile(std::string path_tile, int x, int y, int sizeX, int sizeY,
			bool collision, bool visible, int animation_speed, int level,
			int sheetX, int sheetY) {
	// More current constructor
	// # path_tile : spritesheet of the tile
	// # x : coordinate of the tile
	// # y : coordinate of the tile
	// # sizeX : width of the tile
	// # sizeY : height of the tile
	// # collision : if it is solid
	// # visible : if the tile is visible
	// # animation_speed : for animated tiles, speed image per frame
	// # level : the higher, the later it will be drawn
	// # sheetX : the position of the tile on the spritesheet
	// # sheetY : the position of the tile on the spritesheet
	this->setPosition(x,y);
	this->zLevel = level;
	this->solid = collision;
	this->visible = visible;
	this->animationSpeed = animation_speed;
	this->setAnimationId(1);//idle
	this->setFacing(facing);//down
	this->setSpriteSize(sizeX, sizeY);
	std::vector<std::string> sheetPaths_v;
	sheetPaths_v.push_back(path_tile);
	this->sheetPaths=sheetPaths_v;
	sf::Vector2f mysize;
	mysize.x = sizeX;
	mysize.y = sizeY;
	this->animation.x = sheetX;
	this->animation.y = sheetY;
	this->myMask = sf::RectangleShape(mysize);
	this->set_animation();
}

void MapTile::draw_thing(sf::RenderWindow &w) {
	// Draw the tiles at the right position
	// # w : game window

	//std::cout<<tilePositions.size()<<std::endl;
	if (visible) {
		for (int i = 0; i < tilePositions.size(); ++i) {
			this->mySprite.setPosition(tilePositions[i].x, tilePositions[i].y);
			this->mySprite.setOrigin(this->spriteSize.x/2, this->spriteSize.y/2);
			w.draw(this->mySprite);
		}
	}
}

void MapTile::addTilePosition(int x, int y) {
	// Add a tile
	// # x : coordinate of the position of the new tile
	// # y : coordinate of the position of the new tile

	tilePositions.push_back(sf::Vector2f(x, y));
}

void MapTile::emptyTilePosition() {
	// Empty the vector

	tilePositions.clear();
}

MapTile::~MapTile() {
	// Auto-generated destructor
}

