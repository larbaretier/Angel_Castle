// Collision
// Group of functions linked to the collision of objects

#include "Collision.h"
#include <SFML/Graphics.hpp>
#include <iostream>
#include <vector>

Collision::Collision() {
	// Not used

}

Collision::~Collision() {
	// Not used
}

bool Collision::collisions(Thing* main, std::vector<Thing> stuffs, sf::Vector2f mainMove){
	// Returns true if there is a collision with objects, false otherwise (version with list of objects, not address)
	// # main : object which needs to have its collision checked
	// # stuffs : objects that could potentially collide with the main object
	// # mainMove : the movement of the main object : this will determine the future position to check, put (0,0) to
	//				check current position

	int stuffNb = stuffs.size();
	Thing mainObject = *main;

	sf::Vector2f newPosition = mainObject.getPosition();
	sf::Vector2f newSize = mainObject.getMysize();
	newPosition.x = newPosition.x + mainMove.x;
	newPosition.y = newPosition.y + mainMove.y;
	Thing compareObject;
	for (int a = 0; a < stuffNb; a ++){
		compareObject = stuffs[a];
		if (square_collision(newPosition, compareObject.getPosition(), newSize, compareObject.getMysize() ))
		{
			return true;
		}
	}
	return false;
}

bool Collision::map_collisions(Thing* main, sf::Vector2f mainMove, LevelMap* level){
	// Returns true if there is a collision with solid tiles, false otherwise
	// # main : object which needs to have its collision checked
	// # mainMove : the movement of the main object : this will determine the future position to check, put (0,0) to
	//				check current position
	// # level : current level map with the tiles that could potentially collide

	// TODO : divide height and width by tilesize/2 to know how many points to have checked on each side
	int x = main->getPosition().x;
	int y = main->getPosition().y;
	int sizeX = main->getMysize().x;
	int sizeY = main->getMysize().y;
	int numberWidth = sizeX/level->getTileSize() + 1;
	int numberHeight = sizeY/level->getTileSize() + 1;
	for (int i = 0; i < numberWidth; ++i) {
		if(mainMove.y>0 and (level->checkTileCollision(x + sizeX/2 - i*sizeX/numberWidth, y + sizeY/2 + mainMove.y) ) ) {
			return true;
		}
		else if(mainMove.y<0 and (level->checkTileCollision(x + sizeX/2 - i*sizeX/numberWidth, y - sizeY/2 + mainMove.y) ) ){
			return true;
		}
	}

	for (int i = 0; i < numberHeight; ++i) {
		if(mainMove.x>0 and (level->checkTileCollision(x + sizeX/2 + mainMove.x, y + sizeY/2 - i*sizeY/numberHeight)  ) ){
			return true;
		}
		else if(mainMove.x<0 and (level->checkTileCollision(x - sizeX/2 + mainMove.x, y + sizeY/2 - i*sizeY/numberHeight) ) ){
			return true;
		}
	}


	if(mainMove.y>0 and (level->checkTileCollision(x, y + sizeY/2 + mainMove.y) or level->checkTileCollision(x + sizeX/2, y + sizeY/2 + mainMove.y)
			or level->checkTileCollision(x - sizeX/2, y + sizeY/2 + mainMove.y) ) ){
		return true;
	}
	else if(mainMove.y<0 and (level->checkTileCollision(x, y - sizeY/2 + mainMove.y) or level->checkTileCollision(x + sizeX/2, y - sizeY/2 + mainMove.y)
			or level->checkTileCollision(x - sizeX/2, y - sizeY/2 + mainMove.y) ) ){
		return true;
	}

	if(mainMove.x>0 and (level->checkTileCollision(x + sizeX/2 + mainMove.x, y) or level->checkTileCollision(x + sizeX/2 + mainMove.x, y + sizeY/2)
			or level->checkTileCollision(x + sizeX/2  + mainMove.x, y - sizeY/2) ) ){
		return true;
	}
	else if(mainMove.x<0 and (level->checkTileCollision(x - sizeX/2 + mainMove.x, y) or level->checkTileCollision(x - sizeX/2 + mainMove.x, y + sizeY/2)
			or level->checkTileCollision(x - sizeX/2  + mainMove.x, y - sizeY/2) ) ){
		return true;
	}
	return false;
}

bool Collision::map_collisions_at_position(Thing* main, int x, int y, LevelMap* level){
	// Returns true if there is a collision with solid tiles, false otherwise
	// # main : object which needs to have its collision checked
	// # mainMove : the movement of the main object : this will determine the future position to check, put (0,0) to
	//				check current position
	// # level : current level map with the tiles that could potentially collide

	// TODO : divide height and width by tilesize/2 to know how many points to have checked on each side

	int sizeX = main->getMysize().x;
	int sizeY = main->getMysize().y;
	int numberWidth = sizeX/level->getTileSize() + 1;
	int numberHeight = sizeY/level->getTileSize() + 1;

	for (int i = 0; i < numberWidth; ++i) {
		if((level->checkTileCollision(x + sizeX/2 - i*sizeX/numberWidth, y + sizeY/2) ) ) {
			return true;
		}
		if((level->checkTileCollision(x + sizeX/2 - i*sizeX/numberWidth, y - sizeY/2) ) ) {
			return true;
		}
	}

	for (int i = 0; i < numberHeight; ++i) {
		if((level->checkTileCollision(x + sizeX/2, y + sizeY/2 - i*sizeY/numberHeight)  ) ){
			return true;
		}
		if((level->checkTileCollision(x - sizeX/2, y + sizeY/2 - i*sizeY/numberHeight)  ) ){
			return true;
		}
	}

	if((level->checkTileCollision(x, y + sizeY/2 ) or level->checkTileCollision(x , y - sizeY/2) or level->checkTileCollision(x , y)
			or level->checkTileCollision(x + sizeX/2, y) or level->checkTileCollision(x - sizeX/2, y)
			or level->checkTileCollision(x - sizeX/2, y - sizeY/2 ) or level->checkTileCollision(x + sizeX/2, y + sizeY/2 )
			or level->checkTileCollision(x + sizeX/2, y - sizeY/2 ) or level->checkTileCollision(x - sizeX/2, y + sizeY/2 )
	) ){
		return true;
	}
	return false;
}

bool Collision::tiles_collisions_at_position(Thing* main, int x, int y, LevelMap* level, std::vector<int> tempSolidTiles){
	// Returns true if there is a collision with a temporary solid tile, false otherwise
	// # main : object which needs to have its collision checked
	// # mainMove : the movement of the main object : this will determine the future position to check, put (0,0) to
	//				check current position
	// # level : current level map with the tiles that could potentially collide
	// # tempSolidTiles : numbers of the tiles to consider as solid here

	int sizeX = main->getMysize().x;
	int sizeY = main->getMysize().y;
	int numberWidth = sizeX/level->getTileSize() + 1;
	int numberHeight = sizeY/level->getTileSize() + 1;

	for (int i = 0; i < numberWidth; ++i) {

		for (int j = 0; j < tempSolidTiles.size(); ++j) {
			if( (level->positionInVector(x + sizeX/2 - i*sizeX/numberWidth, y + sizeY/2)== tempSolidTiles[j]) ) {
				return true;
			}

			if( (level->positionInVector(x + sizeX/2 - i*sizeX/numberWidth, y - sizeY/2)== tempSolidTiles[j]) ) {
				return true;
			}
		}
	}

	for (int i = 0; i < numberHeight; ++i) {
		for (int j = 0; j < tempSolidTiles.size(); ++j) {
			if((level->positionInVector(x + sizeX/2, y + sizeY/2 - i*sizeY/numberHeight)== tempSolidTiles[j]  ) ){
				return true;
			}
			if((level->positionInVector(x - sizeX/2, y + sizeY/2 - i*sizeY/numberHeight)== tempSolidTiles[j]  ) ){
				return true;
			}
		}
	}
	for (int j = 0; j < tempSolidTiles.size(); ++j) {
		if((level->positionInVector(x, y + sizeY/2 ) == tempSolidTiles[j] or level->positionInVector(x , y - sizeY/2)== tempSolidTiles[j] or level->positionInVector(x , y)== tempSolidTiles[j]
					or level->positionInVector(x + sizeX/2, y)== tempSolidTiles[j] or level->positionInVector(x - sizeX/2, y)== tempSolidTiles[j]
					or level->positionInVector(x - sizeX/2, y - sizeY/2 )== tempSolidTiles[j] or level->positionInVector(x + sizeX/2, y + sizeY/2 )== tempSolidTiles[j]
					or level->positionInVector(x + sizeX/2, y - sizeY/2 )== tempSolidTiles[j] or level->positionInVector(x - sizeX/2, y + sizeY/2 )== tempSolidTiles[j]
		) ){
			return true;
		}
	}
	return false;
}

std::vector<int> Collision::object_to_map_tiles(Thing* main, LevelMap* level){
	// Returns list of tile place that collides with the given object
	// # main : object which needs to have its collision checked
	// # mainMove : the movement of the main object : this will determine the future position to check, put (0,0) to
	//				check current position
	// # level : current level map with the tiles that could potentially collide

	int sizeX = main->getMysize().x;
	int sizeY = main->getMysize().y;
	int numberWidth = sizeX/level->getTileSize() + 1;
	int numberHeight = sizeY/level->getTileSize() + 1;
	std::vector<int> result;

	for (int i = 0; i < numberWidth; ++i) {
		result.push_back(level->positionInVector(main->getPosition().x + sizeX/2 - i*sizeX/numberWidth, main->getPosition().y + sizeY/2));
		result.push_back(level->positionInVector(main->getPosition().x + sizeX/2, main->getPosition().y + sizeY/2 - i*sizeY/numberHeight));

		result.push_back(level->positionInVector(main->getPosition().x + sizeX/2 - i*sizeX/numberWidth, main->getPosition().y - sizeY/2));
		result.push_back(level->positionInVector(main->getPosition().x - sizeX/2, main->getPosition().y + sizeY/2 - i*sizeY/numberHeight));
	}

	result.push_back(level->positionInVector(main->getPosition().x + sizeX/2 , main->getPosition().y + sizeY/2));
	result.push_back(level->positionInVector(main->getPosition().x - sizeX/2 , main->getPosition().y + sizeY/2));
	result.push_back(level->positionInVector(main->getPosition().x - sizeX/2 , main->getPosition().y - sizeY/2));
	result.push_back(level->positionInVector(main->getPosition().x + sizeX/2 , main->getPosition().y - sizeY/2));

	return result;
}

std::vector<int> Collision::objects_to_map_tiles(std::vector<Thing*> stuffs, LevelMap* level){
	// Returns list of tiles place that collides with the given objects
	// # stuffs : objects which needs to have its collision checked
	// # mainMove : the movement of the main object : this will determine the future position to check, put (0,0) to
	//				check current position
	// # level : current level map with the tiles that could potentially collide

	int sizeX;
	int sizeY;
	std::vector<int> result;
	std::vector<int> temp;
	for (int i = 0; i < stuffs.size(); ++i) {
		sizeX = stuffs[i]->getMysize().x;
		sizeY = stuffs[i]->getMysize().y;
		temp = object_to_map_tiles(stuffs[i],level);
		result.insert( result.end(), temp.begin(), temp.end() );
	}
	result.erase( unique( result.begin(), result.end() ), result.end() );
	return result;
}

bool Collision::collisionsMapIncluded(Thing* main, std::vector<Thing*> stuffs, sf::Vector2f mainMove, LevelMap* level){
	// Returns true if there is a collision with solid tiles or a solid tile, false otherwise
	// # main : object which needs to have its collision checked
	// # stuffs : objects that could potentially collide with the main object
	// # mainMove : the movement of the main object : this will determine the future position to check, put (0,0) to
	//				check current position
	// # level : current level map with the tiles that could potentially collide

	bool result = map_collisions(main, mainMove, level) or collisions(main, stuffs, mainMove);
	return result;
}

bool Collision::collisionsMapIncluded_at_position(Thing* main, std::vector<Thing*> stuffs, int x, int y, LevelMap* level){
	// Returns true if there is a collision with solid tiles or a solid tile, false otherwise
	// # main : object which needs to have its collision checked
	// # stuffs : objects that could potentially collide with the main object
	// # mainMove : the movement of the main object : this will determine the future position to check, put (0,0) to
	//				check current position
	// # level : current level map with the tiles that could potentially collide

	bool result = map_collisions_at_position(main, x, y, level) or collisions_at_position(main, stuffs, x, y);
	return result;
}

bool Collision::collisions(Thing* main, std::vector<Thing*> stuffs, sf::Vector2f mainMove){
	// Returns true if there is a collision with objects, false otherwise (version with list of address of objects)
	// # main : object which needs to have its collision checked
	// # stuffs : objects that could potentially collide with the main object
	// # mainMove : the movement of the main object : this will determine the future position to check, put (0,0) to
	//				check current position

	int stuffNb = stuffs.size();
	Thing mainObject = *main;
	sf::Vector2f newPosition = mainObject.getPosition();
	sf::Vector2f newSize = mainObject.getMysize();
	newPosition.x = newPosition.x + mainMove.x;
	newPosition.y = newPosition.y + mainMove.y;
	Thing compareObject;
	for (int a = 0; a < stuffNb; a ++){
		if(main != stuffs[a])
		{
			compareObject = *stuffs[a];
			if (square_collision(newPosition, compareObject.getPosition(), newSize, compareObject.getMysize() ))
			{
				return true;
			}
		}
	}
	return false;
}

bool Collision::collisions_at_position(Thing* main, std::vector<Thing*> stuffs, int x, int y){
	// Returns true if there is a collision with objects, false otherwise (version with list of address of objects)
	// # main : object which needs to have its collision checked
	// # stuffs : objects that could potentially collide with the main object
	// # mainMove : the movement of the main object : this will determine the future position to check, put (0,0) to
	//				check current position

	int stuffNb = stuffs.size();
	Thing mainObject = *main;
	sf::Vector2f newPosition;
	sf::Vector2f newSize = mainObject.getMysize();
	newPosition.x = x;
	newPosition.y = y;
	Thing compareObject;
	for (int a = 0; a < stuffNb; a ++){
		if(main != stuffs[a])
		{
			compareObject = *stuffs[a];
			if (square_collision(newPosition, compareObject.getPosition(), newSize, compareObject.getMysize() ))
			{
				return true;
			}
		}
	}
	return false;
}

bool Collision::square_collision(sf::Vector2f object1_coordinates, sf::Vector2f object2_coordinates, sf::Vector2f object1_size, sf::Vector2f object2_size){
	// Returns true if there is a square collision between the two objects, false otherwise
	// # object1_coordinates : (x,y) of the first object
	// # object2_coordinates : (x,y) of the second object
	// # object1_size : (width, height) of the first object
	// # object2_size : (width, height) of the second object

	//bool result = true;
	/*
	if ((not ((object1_coordinates.x - object1_size.x/2 < object2_coordinates.x - object2_size.x/2) &&
			(object1_coordinates.x + object1_size.x/2 < object2_coordinates.x - object2_size.x/2))
			&&
			not ((object1_coordinates.x - object1_size.x/2 > object2_coordinates.x + object2_size.x/2) &&
			(object1_coordinates.x + object1_size.x/2 > object2_coordinates.x + object2_size.x/2))
			)
		&&
		(not ((object1_coordinates.y - object1_size.y/2 < object2_coordinates.y - object2_size.y/2) &&
					(object1_coordinates.y + object1_size.y/2 < object2_coordinates.y - object2_size.y/2))
					&&
					not ((object1_coordinates.y - object1_size.y/2 > object2_coordinates.y + object2_size.y/2) &&
					(object1_coordinates.y + object1_size.y/2 > object2_coordinates.y + object2_size.y/2))
		))
	{
		//std::cout<<"collision ! "<<std::endl;
	}
	else
	{
		result = false;
	}*/
	if( object1_coordinates.x +object1_size.x < object2_coordinates.x || object1_coordinates.x > object2_coordinates.x+object2_size.x ) return false;
	if( object1_coordinates.y +object1_size.y < object2_coordinates.y || object1_coordinates.y > object2_coordinates.y+object2_size.y ) return false;

	return true;
}

bool Collision::many_square_collisions(sf::Vector2f object1_coordinates, sf::Vector2f object1_size, std::vector<sf::Vector2f> positions, std::vector<sf::Vector2f> sizes){
	// Check square collision with all given objects : returns true if any, false otherwise
	// # object1_coordinates : (x,y) of the main object
	// # object1_size : (width, height) of the main object
	// # positions : list of (x,y) of all objects that could potentially collide with the main object
	// # sizes : list of (width, height) of all objects that could potentially collide with the main object

	for (int a = 0; a < positions.size(); a ++){
		if (square_collision(object1_coordinates, positions[a], object1_size, sizes[a]))
		{
			return true;
		}
	}
	return false;
}

int Collision::distanceSquare(int x1, int y1, int x2, int y2){
	// Gives the distance between the two points
	// # x1 : coordinate of first point
	// # y1 : coordinate of first point
	// # x1 : coordinate of second point
	// # y2 : coordinate of second point

	int result = (x1-x2)*(x1-x2) + (y1-y2)*(y1-y2);
	return result;
}


bool Collision::first_inside_second(sf::Vector2f object1_coordinates, sf::Vector2f object2_coordinates, sf::Vector2f object2_size){
	// Check if the first position is inside the square zone defined by the second object info
	// # object1_coordinates : (x,y) of the first object
	// # object2_coordinates : (x,y) of the second object
	// # object2_size : (width, height) of the second object

	bool result = true;

	if (object1_coordinates.x<object2_coordinates.x + object2_size.x/2 && object1_coordinates.x>object2_coordinates.x - object2_size.x/2
			&& object1_coordinates.y<object2_coordinates.y + object2_size.y/2 && object1_coordinates.y>object2_coordinates.y - object2_size.y/2)
	{
		//std::cout<<"collision ! "<<std::endl;
	}
	else
	{
		result = false;
	}

	return result;
}
