/*
 * MapTile.h
 *
 *  Created on: 8 mars 2017
 *      Author: Louis
 */

#ifndef MAPTILE_H_
#define MAPTILE_H_
#include <SFML/Graphics.hpp>
#include "Thing.h"

class MapTile : public Thing {
public:
	MapTile();
	MapTile(std::string path_tile, int x, int y, int sizeX, int sizeY, bool collision, bool visible, int animation_speed, int level, int sheetX, int sheetY);
	virtual ~MapTile();
	void draw_thing(sf::RenderWindow &w) override;
	void addTilePosition(int x, int y);
	void emptyTilePosition();
private:
	std::vector<sf::Vector2f> tilePositions;
};

#endif /* MAPTILE_H_ */
