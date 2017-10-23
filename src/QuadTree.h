/*
 * QuadTree.h
 *
 *  Created on: 16 mai 2017
 *      Author: Louis
 */

#ifndef QUADTREE_H_
#define QUADTREE_H_
class Thing;
#include <SFML/Graphics.hpp>
#include <iostream>

class QuadTree {
public:
	QuadTree();
	QuadTree(int notIni);
	QuadTree(int lv, int width, int height, int x, int y);
	virtual ~QuadTree();
	void addObject(Thing obj);
	void buildFromScratch(std::vector<Thing*> objects, float height, float width, float x, float y);
	void buildFromScratch(std::vector<Thing*> objects);
	std::vector<QuadTree*> getLeaf(Thing* objects);
	void drawLeafTree(sf::RenderWindow &w);
	void emptyLeafs(std::vector<Thing*> objects);
	void eraseFromTree(Thing* object);
	void insertOneObject(Thing* object);
	void clearChildren();
	void split();
	std::vector<int> intersectsWith(Thing* obj);
	void insertObject(Thing* obj);
	std::vector<Thing*> retrieve(Thing* obj);
	QuadTree* getRoot();
	static const int maxPerLeaf = 2;//3
	static const int maxLevel = 10;//5
	static const int minArea = 200;// in pixel*pixel
	QuadTree* getParent() {
		return (this->parent);
	}

	void setParent(QuadTree* parent) {
		this->parent = parent;
	}

	const std::vector<Thing*>& getObjectsInside() const {
		return objectsInside;
	}

	void setObjectsInside(const std::vector<Thing*>& objectsInside) {
		this->objectsInside = objectsInside;
	}

	float getHeight() const {
		return height;
	}

	void setHeight(float height) {
		this->height = height;
	}

	float getWidth() const {
		return width;
	}

	void setWidth(float width) {
		this->width = width;
	}

private:
	int limitation;//max number of objects we want in quadtree leaf
	QuadTree* parent;
	int level; // 0 is root
	//QuadTree* branches[4]; //= {QuadTree(1),QuadTree(1),QuadTree(1),QuadTree(1)}; // branches = [nw,ne,sw,se]
	QuadTree* nW;//what is contained in the upper left side of the square
	QuadTree* nE;//what is contained in the upper right side of the square
	QuadTree* sW;//what is contained in the down left side of the square
	QuadTree* sE;//what is contained in the down right side of the square
	std::vector<Thing*> objectsInside;
	float height;//middle of square h
	float width;//middle of square w
	sf::Vector2f center;//center of the square


};

#endif /* QUADTREE_H_ */
