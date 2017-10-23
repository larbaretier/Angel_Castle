#ifndef Hero_H
#define Hero_H
#include <SFML/Graphics.hpp>
#include "Collision.h"
#include "Character.h"
#include <iostream>
#include <vector>

class Hero : public Character {
	public:
		Hero();
		Hero(std::vector<std::string>& sheetPaths_v, int speed_h_v, int x_v, int y_v, int sprx, int spry, int facing, int sizex, int sizey);
		virtual ~Hero();
		//void draw_char(sf::RenderWindow &w);
		void press_button(std::vector<sf::Vector2f> positions, std::vector<sf::Vector2f> sizes);
		void press_button(QuadTree* root, LevelMap* level);


	private:

};

#endif // Hero_H
