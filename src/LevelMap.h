/*
 * LevelMap.h
 *
 *  Created on: 25 oct. 2016
 *      Author: Louis
 */

#ifndef LEVELMAP_H_
#define LEVELMAP_H_
#include <SFML/Graphics.hpp>
#include "SolidSquare.h"
#include "MapTile.h"
#include "MapWall.h"
#include "Thing.h"

class LevelMap {
public:
	const static int tileSize = 16; // 16*16
	LevelMap();
	LevelMap(std::vector<Thing> colision_objects_in);
	LevelMap(std::vector<Thing> colision_objects_in, int w, int h);
	LevelMap(std::vector<Thing> colision_objects_in, int w, int h, std::vector<MapTile> usedTiles);
	bool checkTileCollision(int x, int y);
	virtual ~LevelMap();
	void drawMap(sf::RenderWindow &w, int xView, int yView, int wView, int hView);
	void drawVisibleTile(sf::RenderWindow &w, int x, int y);
	void refreshPositions(sf::RenderWindow &w, int xView, int yView, int wView, int hView);
	int positionInVector(int x, int y);
	sf::Vector3f positionInCoordinate(int positionInVector);

	const std::vector<Thing>& getColisionObjects() const {
		return colision_objects;
	}

	Thing* getColisionObjectsAdd(int i) {
			return &(colision_objects[i]);
	}

	MapTile* getTileModel(int i) {
		return &(tiles[i]);
	}

	void setColisionObjects(const std::vector<Thing>& colisionObjects) {
		colision_objects = colisionObjects;
	}

	const sf::Vector2f& getStartPoint() const {
		return start_point;
	}

	void setStartPoint(const sf::Vector2f& startPoint) {
		start_point = startPoint;
	}

	const sf::Vector2f& getSize() const {
		return size;
	}

	void setSize(const sf::Vector2f& size) {
		this->size = size;
	}

	const std::vector<MapTile>& getTiles() const {
		return tiles;
	}

	void setTiles(const std::vector<MapTile>& tiles) {
		this->tiles = tiles;
	}

	int getHeight() const {
		return height;
	}

	void setHeight(int height) {
		this->height = height;
	}

	int getWidth() const {
		return width;
	}

	void setWidth(int width) {
		this->width = width;
	}

	const std::vector<int>& getMyTiles() const {
		return myTiles;
	}

	void setMyTiles(const std::vector<int>& myTiles) {
		this->myTiles = myTiles;
	}

	int getSizeArrayTile() const {
		return sizeArrayTile;
	}

	void setSizeArrayTile(int sizeArrayTile) {
		this->sizeArrayTile = sizeArrayTile;
	}

	int getTileSize() const {
		return tileSize;
	}

private:
	std::vector<Thing> colision_objects;
	std::vector<MapTile> tiles;
	sf::Vector2f size;
	sf::Vector2f start_point;
	sf::Texture image;
	int width;
	int height;
	std::vector<int> myTiles;
	int sizeArrayTile;
};

#endif /* LEVELMAP_H_ */
