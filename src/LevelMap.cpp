// LevelMap
// A map with objects
// # colision_objects : all the solid objects of the level
// # tiles : list of the tiles used in the map
// # size : width and height of the map (TODO : get rid of it)
// # start_point : position of the main character
// # image : small image representing the level
// # width : half width of the map
// # height : half height of the map
// # myTiles : all the tiles represented by integers on the map
// # sizeArrayTile : size of myTiles

#include "LevelMap.h"
#include <SFML/Graphics.hpp>
#include "SolidSquare.h"
#include "Thing.h"
#include <iostream>

LevelMap::LevelMap() {
	// Basic initialisation
}

LevelMap::LevelMap(std::vector<Thing> colision_objects_in) {
	// Basic initialization, enter objects with collisions
	// # colision_objects_in : objects linked to the map
	this->colision_objects = colision_objects_in;

}

LevelMap::LevelMap(std::vector<Thing> colision_objects_in, int w, int h) {
	// More current initialization, enter objects with collisions
	// # colision_objects_in : objects linked to the map
	// # w : half width of the map
	// # h : half height of the map
	this->colision_objects = colision_objects_in;
	this->height=h;// half height in reality since [-height;height]
	this->width=w; // half width in reality since [-width;width]
	this->size.x=width;
	this->size.y=height;
	// access to a tile with : [x/tileSize+y/tileSize*RealWidth]    here RealWidth = 2*width
	// with x = obj.x + width to include negative    (obj.x is in [-width;width])
	// and y = obj.y + height for same reason
	//
	// so tile(x,y) is represented on the map on x-width,y-height
	// with tile.x = (position in myTiles)%width * tileSize - tileSize/2   (% is modulo)
	// and tile.y = (position in myTiles)/width * tileSize - tileSize/2
	this->sizeArrayTile = (this->width*2)/tileSize * (this->height*2)/tileSize;

	// print informations
	std::cout<<"nb :"<<std::endl;
	std::cout<<this->sizeArrayTile<<std::endl;
	std::cout<<width*2/tileSize<<std::endl;
	std::cout<<height*2/tileSize<<std::endl;
	std::cout<<"i=0 x,y:"<<std::endl;
	std::cout<<0%(2*width/tileSize) * tileSize - tileSize/2 - width<<std::endl;
	std::cout<<0/(2*width/tileSize) * tileSize - tileSize/2 - height<<std::endl;
	std::cout<<"i=24999 x,y:"<<std::endl;
	std::cout<<24999%(2*width/tileSize) * tileSize - tileSize/2 - width<<std::endl;
	std::cout<<24999/(2*width/tileSize) * tileSize - tileSize/2 - height<<std::endl;

	// build map
	for (int i = 0; i < (this->sizeArrayTile); ++i) {
		if(i%(2*width/tileSize) * tileSize - tileSize/2 - width < -100||i%(2*width/tileSize) * tileSize - tileSize/2 - width > 600
				|| i/(2*width/tileSize) * tileSize - tileSize/2 - height<-300|| i/(2*width/tileSize) * tileSize - tileSize/2 - height>500
				|| i == positionInVector(100,300)){
			this->myTiles.push_back(1);
		}
		else{
			this->myTiles.push_back(0);
		}
	}
}

LevelMap::LevelMap(std::vector<Thing> colision_objects_in, int w, int h, std::vector<MapTile> usedTiles) {
	// More current initialization, enter objects with collisions
	// # colision_objects_in : objects linked to the map
	// # w : half width of the map
	// # h : half height of the map
	this->colision_objects = colision_objects_in;
	this->height=h;// half height in reality since [-height;height]
	this->width=w; // half width in reality since [-width;width]
	this->size.x=width;
	this->size.y=height;
	this->tiles = usedTiles;
	// access to a tile with : [x/tileSize+y/tileSize*RealWidth]    here RealWidth = 2*width
	// with x = obj.x + width to include negative    (obj.x is in [-width;width])
	// and y = obj.y + height for same reason
	//
	// so tile(x,y) is represented on the map on x-width,y-height
	// with tile.x = (position in myTiles)%width * tileSize - tileSize/2   (% is modulo)
	// and tile.y = (position in myTiles)/width * tileSize - tileSize/2
	this->sizeArrayTile = (this->width*2)/tileSize * (this->height*2)/tileSize;

	// print informations
	std::cout<<"nb :"<<std::endl;
	std::cout<<this->sizeArrayTile<<std::endl;
	std::cout<<width*2/tileSize<<std::endl;
	std::cout<<height*2/tileSize<<std::endl;
	std::cout<<"i=0 x,y:"<<std::endl;
	std::cout<<0%(2*width/tileSize) * tileSize - tileSize/2 - width<<std::endl;
	std::cout<<0/(2*width/tileSize) * tileSize - tileSize/2 - height<<std::endl;
	std::cout<<"i=24999 x,y:"<<std::endl;
	std::cout<<24999%(2*width/tileSize) * tileSize - tileSize/2 - width<<std::endl;
	std::cout<<24999/(2*width/tileSize) * tileSize - tileSize/2 - height<<std::endl;

	// build map
	for (int i = 0; i < (this->sizeArrayTile); ++i) {
		if(i%(2*width/tileSize) * tileSize - tileSize/2 - width < -900||i%(2*width/tileSize) * tileSize - tileSize/2 - width > 900
				|| i/(2*width/tileSize) * tileSize - tileSize/2 - height<-900|| i/(2*width/tileSize) * tileSize - tileSize/2 - height>960
				|| i == positionInVector(100,300)
				|| (positionInCoordinate(i).y==positionInCoordinate(positionInVector(100,300)).y && positionInCoordinate(i).x <200)){
			this->myTiles.push_back(1);
		}
		else{
			this->myTiles.push_back(0);
		}
	}

}

LevelMap::~LevelMap() {
	// Auto-generated destructor
}

bool LevelMap::checkTileCollision(int x, int y) {
	// Returns true if the tile at the given position is solid, false otherwise
	// # x : x coord position to check for collision
	// # y : y coord position to check for collision

	int myTileType = this->myTiles[positionInVector(x,y)]; //    x/tileSize+y/tileSize*RealWidth
	switch (myTileType){
		case 0: return false;
					break;
		case 1: return true; //collision !
					break;
		case 2: return false;
					break;
		case 3: return false;
					break;
		default: return false;;
	}

}

void LevelMap::refreshPositions(sf::RenderWindow &w, int xView, int yView, int wView, int hView){
	// Adds the tiles' position
	// # w : game window
	// # xView : the x coordinate of the camera's position
	// # yView : the y coordinate of the camera's position
	// # wView : the width of the camera
	// # hView : the height of the camera
	//std::cout<<"reset tiles"<<std::endl;
	// reset every tiles
	for (int i = 0; i < this->tiles.size(); ++i) {
		tiles[i].emptyTilePosition();
	}
	//std::cout<<"init"<<std::endl;

	int nbLineV = wView/tileSize + 2;
	int nbLineH = hView/tileSize + 2;
	int max = positionInVector(xView + wView/2, yView + hView/2);
	int i = positionInVector(xView - wView/2, yView - hView/2);
	while(i<0){
		i+=2*width/tileSize;
	}
	//int i = (xView - wView/2 + width)/tileSize + (yView - hView/2 + height)/tileSize*(2*width/tileSize) -1;
	int lineCounter = 0;
	int columnCounter = 0;
	int x =0;
	int y =0;
	//std::cout<<nbLineH<<std::endl;
	//std::cout<<i<<std::endl;
	//std::cout<<"begin loop refresh"<<std::endl;
	// add only the tile positions that are inside the view
	while(columnCounter<nbLineH && i<sizeArrayTile){//<max
		i+=1;
		lineCounter+=1;
		x = i%(2*width/tileSize) * tileSize - tileSize/2 - width;
		y = i/(2*width/tileSize) * tileSize - tileSize/2 - height;
		tiles[myTiles[i]].addTilePosition(x, y);
		if(lineCounter>nbLineV){
			columnCounter+=1;
			//std::cout<<columnCounter<<std::endl;
			i= i + 2*width/tileSize - lineCounter;
			lineCounter = 0;
		}

	}
}

void LevelMap::drawMap(sf::RenderWindow &w, int xView, int yView, int wView, int hView){
	// Draw the tiles
	// # w : game window
	// # xView : the x coordinate of the camera's position
	// # yView : the y coordinate of the camera's position
	// # wView : the width of the camera
	// # hView : the height of the camera

	int nbLineV = wView/tileSize + 5;
	int nbLineH = hView/tileSize + 5;
	int max = (xView + wView/2 + width)/tileSize + (yView + hView/2 + height)/tileSize*(2*width/tileSize);
	int i = (xView - wView/2 + width)/tileSize + (yView - hView/2 + height)/tileSize*(2*width/tileSize) -1;
	int lineCounter = 0;
	int columnCounter = 0;
	while(columnCounter<nbLineH){//<max
		i+=1;
		lineCounter+=1;
		switch (this->myTiles[i]){
			case 0:
						break;
			case 1: drawVisibleTile(w,i%(2*width/tileSize) * tileSize - tileSize/2 - width, i/(2*width/tileSize) * tileSize - tileSize/2 - height); //collision !
						break;
			case 2:
						break;
			case 3:
						break;
			default: ;
		}

		if(lineCounter>nbLineV){
			columnCounter+=1;
			i= i + 2*width/tileSize - lineCounter;
			lineCounter = 0;
		}

	}

}

int LevelMap::positionInVector(int x, int y){
	int result = ((x)+width + tileSize)/tileSize + ((y)+height + tileSize)/tileSize*(2*width/tileSize);
	if(result<0||result>sizeArrayTile){
		result = -1;
	}
	return result;
}

sf::Vector3f LevelMap::positionInCoordinate(int positionInVector){
	sf::Vector3f result;
	result.x = positionInVector%(2*width/tileSize) * tileSize - tileSize/2 - width;
	result.y = positionInVector/(2*width/tileSize) * tileSize - tileSize/2 - height;
	return result;
}

void LevelMap::drawVisibleTile(sf::RenderWindow &w, int x, int y){
	// Draw the given tile at the given position
	// # w : game window
	// # x : x coordinate of the position of the tile to draw
	// # y : y coordinate of the position of the tile to draw

	sf::RectangleShape rect;
	sf::Vector2f sizee = sf::Vector2f(tileSize, tileSize);
	rect.setSize(sizee);
	rect.setOutlineThickness(1);
	//rect.setOrigin(0,0);
	rect.setOrigin(tileSize/2, tileSize/2);
	rect.setPosition(sf::Vector2f(x, y));
	rect.setOutlineColor(sf::Color::Red);
	rect.setFillColor(sf::Color::Yellow);
	rect.setScale(1,1);
	w.draw(rect);
}

