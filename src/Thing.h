/*
 * Thing.h
 *
 *  Created on: 24 oct. 2016
 *      Author: Louis
 */

#ifndef THING_H_
#define THING_H_
#include <SFML/Graphics.hpp>
#include "QuadTree.h"
#include <iostream>
#include <set>

class Thing {
	public:

		Thing();
		Thing(std::vector<std::string> sheetPaths_v);
		virtual ~Thing();
		virtual void draw_thing(sf::RenderWindow &w);
		void set_animation();
		sf::Vector2f getPosition();
		int getSpriteW();
		int getSpriteH();
		std::vector<Thing*> generateCollisionList();
		void purifyLeafList();
		void eraseFromTree();
		void resetTreePosition(QuadTree* root);
		void prepareCollision(QuadTree* root);
		static void drawEverything2(sf::RenderWindow &w, std::vector<Thing> things);

		static void drawEverything(sf::RenderWindow &w, std::multiset<Thing*, Thing> &things);


		static void drawEverything(sf::RenderWindow &w, std::vector<Thing*> &things);
		static bool sortingVectorPoint(const Thing* key1, const Thing* key2);
		static bool sortingVector(const Thing & key1, const Thing & key2);


		bool operator()(const Thing *comp1, const Thing *comp2) const {
			return comp1->getLevel() < comp2->getLevel();
		}

		const sf::Vector2i& getAnimation() const {
			return animation;
		}

		sf::Vector2f getMysize(){
			sf::Vector2f result = this->myMask.getSize();
		    return result;
		}

		void setAnimation(const sf::Vector2i& animation) {
			this->animation = animation;
		}

		void setAnimation(int x, int y) {
					sf::Vector2i po = sf::Vector2i();
					po.x = x;
					po.y = y;
					this->animation = po;
		}

		int getAnimationId() const {
			return animationId;
		}

		void setAnimationId(int animationId) {
			this->animationId = animationId;
		}

		int getFacing() const {
			return facing;
		}

		void setFacing(int facing) {
			this->facing = facing;
		}

		const sf::RectangleShape& getMyMask() const {
			return myMask;
		}

		void setMyMask(const sf::RectangleShape& myMask) {
			this->myMask = myMask;
		}

		const sf::Texture& getMySheet() const {
			return mySheet;
		}

		void setMySheet(const sf::Texture& mySheet) {
			this->mySheet = mySheet;
		}

		const sf::Sprite& getMySprite() const {
			return mySprite;
		}

		void setMySprite(const sf::Sprite& mySprite) {
			this->mySprite = mySprite;
		}

		void setPosition(const sf::Vector2f& position) {
			this->position = position;
		}
		void setPosition(int x, int y) {
				sf::Vector2f po = sf::Vector2f();
				po.x = x;
				po.y = y;
				this->position = po;
		}

		bool isSolid() const {
			return solid;
		}

		void setSolid(bool solid) {
			this->solid = solid;
		}

		const sf::Vector2f& getSpriteSize() const {
			return spriteSize;
		}
		void setSpriteSize(int x, int y) {
					sf::Vector2f po = sf::Vector2f();
					po.x = x;
					po.y = y;
					this->spriteSize = po;
		}

		void setSpriteSize(const sf::Vector2f& spriteSize) {
			this->spriteSize = spriteSize;
		}

		bool isVisible() const {
			return visible;
		}

		void setVisible(bool visible) {
			this->visible = visible;
		}
		int getLeft() const {
			return Left;
		}
		int getRight() const {
			return Right;
		}
		int getUp() const {
			return Up;
		}
		int getDown() const {
			return Down;
		}

		int getLevel() const {
			return zLevel;
		}

		void setLevel(int level) {
			zLevel = level;
		}

		const std::vector<Thing*>& getThingsToColide() const {
			return thingsToColide;
		}

		void setThingsToColide(std::vector<Thing*> thingsToColide) {
			this->thingsToColide = thingsToColide;
		}

		void addToThingsToColide(Thing* t) {
				this->thingsToColide.push_back(t);
		}

	const std::vector<QuadTree*>& getMyLeafs() const {
		return myLeafs;
	}

	void setMyLeafs(const std::vector<QuadTree*>& myLeafs) {
		this->myLeafs = myLeafs;
	}

	void addToMyLeafs(QuadTree* l) {
		this->myLeafs.push_back(l);
	}

	void emptyMyLeafs() {
		this->myLeafs.clear();
	}

	    protected:
	        sf::Vector2f position;
	        sf::Vector2f spriteSize; // used for drawing sprite
	        sf::RectangleShape myMask; // used for collision
	        //to get the size of the mask, use myMask.getSize() ->sf::Vector2f
	        sf::Sprite mySprite;
	        sf::Texture mySheet;
	        std::vector<std::string> sheetPaths;
	        bool visible;
	        bool solid;
	        enum dir{Down, Up, Left, Right};
	        int animationId;
	        sf::Vector2i animation; // x and y in the sprite sheet
	        int animationSpeed;//if 0 not animated
	        int facing;
	        int zLevel;// -3, -2,-1,0, 1, 2, 3 are the floor levels, normal height : 0
	        std::vector<Thing*> thingsToColide; // all objects on map except him
	        std::vector<QuadTree*> myLeafs; // all the leafs he is in
};

#endif /* THING_H_ */
