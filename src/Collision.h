/*
 * Collision.h
 *
 *  Created on: 10 oct. 2016
 *      Author: Louis
 */

#include <SFML/Graphics.hpp>
#ifndef COLLISION_H_
#define COLLISION_H_
#include <iostream>
#include <vector>
#include "Thing.h"
#include "LevelMap.h"

class Collision {
public:
	Collision();
	virtual ~Collision();
	static bool collisions(Thing* main, std::vector<Thing*> stuffs, sf::Vector2f mainMove);
	static bool collisions(Thing* main, std::vector<Thing> stuffs, sf::Vector2f mainMove);
	static bool square_collision(sf::Vector2f object1_coordinates, sf::Vector2f object2_coordinates, sf::Vector2f object1_size, sf::Vector2f object2_size);
	static bool many_square_collisions(sf::Vector2f object1_coordinates, sf::Vector2f object1_size, std::vector<sf::Vector2f> positions, std::vector<sf::Vector2f> sizes);
	static bool first_inside_second(sf::Vector2f object1_coordinates, sf::Vector2f object2_coordinates, sf::Vector2f object2_size);
	static bool map_collisions(Thing* main, sf::Vector2f mainMove, LevelMap* level);
	static bool collisionsMapIncluded(Thing* main, std::vector<Thing*> stuffs, sf::Vector2f mainMove, LevelMap* level);
	static int distanceSquare(int x1, int y1, int x2, int y2);
	static bool map_collisions_at_position(Thing* main, int x, int y, LevelMap* level);
	static bool collisions_at_position(Thing* main, std::vector<Thing*> stuffs, int x, int y);
	static bool collisionsMapIncluded_at_position(Thing* main, std::vector<Thing*> stuffs, int x, int y, LevelMap* level);
	static std::vector<int> object_to_map_tiles(Thing* main, LevelMap* level);
	static std::vector<int> objects_to_map_tiles(std::vector<Thing*> stuffs, LevelMap* level);
	static bool tiles_collisions_at_position(Thing* main, int x, int y, LevelMap* level, std::vector<int> tempSolidTiles);
};

#endif /* COLLISION_H_ */
