// QuadTree
// Store the objects for collision purpose in a smart way (4 directions and one center = a leaf)
// # limitation : max number of objects we want in quadtree leaf
// # parent : previous quadtree (TODO : delete this)
// # level : 0 is root
// # nW : what is contained in the upper left side of the square
// # nE : what is contained in the upper right side of the square
// # sW : what is contained in the down left side of the square
// # sE : what is contained in the down right side of the square
// # objectsInside : all objects in the Quadtree (objects in children excluded)
// # height : height of square
// # width : width of square
// # center : center of the square

#include "QuadTree.h"
#include <SFML/Graphics.hpp>
#include "Thing.h"
#include <iostream>
#include "Collision.h"

QuadTree::QuadTree() {
	// Auto-generated constructor
	nW = (new QuadTree(-1));
	sW =  (new QuadTree(-1));
	nE =  (new QuadTree(-1));
	sE =  (new QuadTree(-1));
	parent = (new QuadTree(-1));
	std::cout<<("init tree")<<std::endl;
}

QuadTree::QuadTree(int lv, int width, int height, int x, int y) {
	// Usual constructor
	nW = (new QuadTree(-1));
	sW =  (new QuadTree(-1));
	nE =  (new QuadTree(-1));
	sE =  (new QuadTree(-1));
	level = lv;
	this->height = height;
	this->width = width;
	this->center.x = x;
	this->center.y = y;
	//std::cout<<("init tree2")<<std::endl;
}

QuadTree::QuadTree(int notIni) {
	// put impossible values to show it is not initialized
	height = -1;
	width = -1;
	//std::cout<<("init tree3")<<std::endl;
}

QuadTree::~QuadTree() {
	// Auto-generated destructor
}

//----test2 BEGIN

void QuadTree::clearChildren(){
	// Clears the quadtree

	objectsInside.clear();
	if (this->height!=-1){ // if we arn't the last branch
		nW->clearChildren();
		sW->clearChildren();
		nE->clearChildren();
		sE->clearChildren();
		if(level==0){
			nW->setHeight(-1);
			nE->setHeight(-1);
			sW->setHeight(-1);
			sE->setHeight(-1);
		}
		else{
			delete(this->nE);
			delete(this->nW);
			delete(this->sE);
			delete(this->sW);
		}
	}
}

void QuadTree::split(){
	// Splits the branch into 4 small children

	delete(this->nE);
	this->nE= new QuadTree(level + 1, this->width/2, this->height/2, this->center.x + this->width/4, this->center.y - this->height/4);
	delete(this->nW);
	this->nW=new QuadTree(level + 1, this->width/2, this->height/2, this->center.x - this->width/4, this->center.y - this->height/4);
	delete(this->sE);
	this->sE=new QuadTree(level + 1, this->width/2, this->height/2, this->center.x + this->width/4, this->center.y + this->height/4);
	delete(this->sW);
	this->sW=new QuadTree(level + 1, this->width/2, this->height/2, this->center.x - this->width/4, this->center.y + this->height/4);
}

std::vector<int> QuadTree::intersectsWith(Thing* obj){
	// Tells which of the 4 children is/are in contact with the object
	// # obj : tells whith which branch it intersects

	sf::Vector2f sqSizeSmall = sf::Vector2f(width/2, height/2);
	sf::Vector2f sqCenterSE = sf::Vector2f(center.x + width/4, center.y + height/4);
	sf::Vector2f sqCenterSW = sf::Vector2f(center.x - width/4, center.y + height/4);
	sf::Vector2f sqCenterNE = sf::Vector2f(center.x + width/4, center.y - height/4);
	sf::Vector2f sqCenterNW = sf::Vector2f(center.x - width/4, center.y - height/4);

	sf::Vector2f myPosition = obj->getPosition();
	sf::Vector2f mySize = obj->getMysize();
	std::vector<int> result; // [nw (0) ,ne (1) , sw (2) , se (3)]
	if  (Collision::square_collision(myPosition, sqCenterNW ,mySize, sqSizeSmall)){
		result.push_back(0);
	}
	if  (Collision::square_collision(myPosition, sqCenterNE ,mySize, sqSizeSmall)){
		result.push_back(1);
	}
	if  (Collision::square_collision(myPosition, sqCenterSW ,mySize, sqSizeSmall)){
		result.push_back(2);
	}
	if  (Collision::square_collision(myPosition, sqCenterSE ,mySize, sqSizeSmall)){
		result.push_back(3);
	}

	return result;
}

void QuadTree::insertObject(Thing* obj){
	// Inserts the object in the quadtree, if too much objects and enough levels create children
	// # obj : the object to insert in the quadtree

	std::vector<int> intersections;
	if (this->sE->getHeight()!=-1){ // if branches already initialised
		intersections = intersectsWith(obj);
		for (int j = 0; j < intersections.size(); j++) {
			switch (intersections[j]){
			case 0: nW->insertObject(obj);
						break;
			case 1: nE->insertObject(obj);
						break;
			case 2: sW->insertObject(obj);
						break;
			case 3: sE->insertObject(obj);
						break;
			default:;
			}
		}
	}
	if (intersections.size()==0){
		objectsInside.push_back(obj);
		if(objectsInside.size()>maxPerLeaf && level<maxLevel){
			if (this->sE->getHeight()==-1){ // branches not initialised
				this->split();
			}
			int i = 0;
			while (i < objectsInside.size()) {
				intersections = intersectsWith(obj);
				int length = intersections.size();
				for (int j = 0; j < intersections.size(); j++) {
					switch (intersections[j]){
					case 0: nW->insertObject(objectsInside[i]);
								break;
					case 1: nE->insertObject(objectsInside[i]);
								break;
					case 2: sW->insertObject(objectsInside[i]);
								break;
					case 3: sE->insertObject(objectsInside[i]);
								break;
					default:;
					}
				}
				//objectsInside.erase(objectsInside.begin());
				if (length==0){ // if no intersections, leave the object
					i++;
				}
				else{ // if intersection delete object from tab after sending it to the children
					objectsInside.erase(objectsInside.begin() + i);
				}
			}
		}

	}
}

std::vector<Thing*> QuadTree::retrieve(Thing* obj){
	// Returns all objects that might collide with the given object
	// # obj : the object that might collide with other objects from the quadtree

	std::vector<Thing*> result;
	if (this->sE->getHeight()!=-1){ // branches initialised
		std::vector<int> intersections = intersectsWith(obj);
		std::vector<Thing*> temp;
		for (int j = 0; j < intersections.size(); j++) {
			switch (intersections[j]){
			case 0:
				temp.clear();
				temp = nW->retrieve(obj);
				result.insert(result.end(), temp.begin(), temp.end());
						break;
			case 1:
				temp.clear();
				temp = nE->retrieve(obj);
					result.insert(result.end(), temp.begin(), temp.end());
						break;
			case 2:
				temp.clear();
				temp = sW->retrieve(obj);
				result.insert(result.end(), temp.begin(), temp.end());
						break;
			case 3:
				temp.clear();
				temp = sE->retrieve(obj);
				result.insert(result.end(), temp.begin(), temp.end());
						break;
			default:;
			}
		}
	}
	for (int i = 0; i < objectsInside.size(); ++i) {
		if (objectsInside[i]!=obj && std::find(result.begin(), result.end(), objectsInside[i]) == result.end()){
			result.push_back(objectsInside[i]);
		}
	}
	//result.insert(result.end(), objectsInside.begin(), objectsInside.end());
	return result;
}

void QuadTree::buildFromScratch(std::vector<Thing*> objects){
	// Build the quadtree with the given objects
	// # objects : objects to have in the quadtree

	nW = (new QuadTree(1));
	sW =  (new QuadTree(1));
	nE =  (new QuadTree(1));
	sE =  (new QuadTree(1));
	sf::Vector2f sqSize = sf::Vector2f(width, height);
	sf::Vector2f sqCenter = sf::Vector2f(center.x, center.y);
	sf::Vector2f sqSizeSmall = sf::Vector2f(width/2, height/2);
	sf::Vector2f sqCenterSE = sf::Vector2f(center.x + width/4, center.y + height/4);
	sf::Vector2f sqCenterSW = sf::Vector2f(center.x - width/4, center.y + height/4);
	sf::Vector2f sqCenterNE = sf::Vector2f(center.x + width/4, center.y - height/4);
	sf::Vector2f sqCenterNW = sf::Vector2f(center.x - width/4, center.y - height/4);

	std::vector<Thing*> nEa;
	std::vector<Thing*> nWe;
	std::vector<Thing*> sEa;
	std::vector<Thing*> sWe;
	// go deeper only if needed
	if(objects.size() > this->maxPerLeaf and this->maxLevel> this->level and this->minArea<this->height*this->width){ //TODO : make level condition
		for (int i = 0; i < objects.size(); ++i) {
			sf::Vector2f myPosition = objects[i]->getPosition();
			sf::Vector2f mySize = objects[i]->getMysize();
			if  (Collision::square_collision(myPosition, sqCenterNW ,mySize, sqSizeSmall)){
				nWe.push_back(objects[i]);
			}
			if  (Collision::square_collision(myPosition, sqCenterNE ,mySize, sqSizeSmall)){
				nEa.push_back(objects[i]);
			}
			if  (Collision::square_collision(myPosition, sqCenterSW ,mySize, sqSizeSmall)){
				sWe.push_back(objects[i]);
			}
			if  (Collision::square_collision(myPosition, sqCenterSE ,mySize, sqSizeSmall)){
				sEa.push_back(objects[i]);
			}

		}
	}
	else{
		// each object knows where he is
		this->objectsInside = objects;
	}
	// see if any new branch is created

	if (nEa.size() != 0 || nWe.size() != 0 || sEa.size() != 0 || sEa.size() != 0){
		split();
		this->nE->buildFromScratch(nEa);

		this->nW->buildFromScratch(nWe);

		this->sE->buildFromScratch(sEa);

		this->sW->buildFromScratch(sWe);
	}

}

//----test2 END

std::vector<QuadTree*> QuadTree::getLeaf(Thing* object){
	// Gets the leaf with the given object
	// # object : object to find

	sf::Vector2f myPosition = object->getPosition();
	if (myPosition.x < this->center.x){
		// West : sW or nW
		if (myPosition.y < this->center.y){
			//n
		}
		else {
			//s
		}
	} else {
		// East : sE or nE
		if (myPosition.y < this->center.y){
			//n
		}
		else {
			//s
		}
	}
}

void QuadTree::emptyLeafs(std::vector<Thing*> objects){
	// Empty the leaf list of the given objects
	// # objects : objects that have to have their leaf list emptied

	for (int i = 0; i < objects.size(); ++i) {
		objects[i]->emptyMyLeafs();
	}
}


QuadTree* QuadTree::getRoot(){
	// Get the root of the Quadtree

	if(this->parent->getHeight() == -1){
		// root is only one with parent not initialized (beside undefined upper branches)
		std::cout<<"yes"<<std::endl;
		return this;
	}
	else{
		std::cout<<"no"<<std::endl;
		return this->parent->getRoot();
	}
}

void QuadTree::buildFromScratch(std::vector<Thing*> objects, float fheight, float fwidth, float x, float y){
	// Build the quadtree
	// # objects : the objects to put in the tree
	// # fheight : height of the branch to build
	// # fwidth : width of the branch to build
	// # x : coordinate of the position of the branch
	// # y : coordinate of the position of the branch

	nW = (new QuadTree(1));
	sW =  (new QuadTree(1));
	nE =  (new QuadTree(1));
	sE =  (new QuadTree(1));
	sf::Vector2f sqSize = sf::Vector2f(fwidth, fheight);
	sf::Vector2f sqSizeSmall = sf::Vector2f(fwidth/2, fheight/2);
	sf::Vector2f sqCenter = sf::Vector2f(x, y);
	sf::Vector2f sqCenterSE = sf::Vector2f(x + fwidth/4, y + fheight/4);
	sf::Vector2f sqCenterSW = sf::Vector2f(x - fwidth/4, y + fheight/4);
	sf::Vector2f sqCenterNE = sf::Vector2f(x + fwidth/4, y - fheight/4);
	sf::Vector2f sqCenterNW = sf::Vector2f(x - fwidth/4, y - fheight/4);

	this->width = fwidth;
	this->height = fheight;
	//std::cout<<fheight<<std::endl;
	this->center = sf::Vector2f(x, y);
	this->objectsInside = objects;
	std::vector<Thing*> nEa;
	std::vector<Thing*> nWe;
	std::vector<Thing*> sEa;
	std::vector<Thing*> sWe;
	// go deeper only if needed
	if(objects.size() > this->maxPerLeaf){
		for (int i = 0; i < objects.size(); ++i) {
			sf::Vector2f myPosition = objects[i]->getPosition();
			sf::Vector2f mySize = objects[i]->getMysize();
			if  (Collision::square_collision(myPosition, sqCenterNW ,mySize, sqSizeSmall)){
				nWe.push_back(objects[i]);
			}
			if  (Collision::square_collision(myPosition, sqCenterNE ,mySize, sqSizeSmall)){
				nEa.push_back(objects[i]);
			}
			if  (Collision::square_collision(myPosition, sqCenterSW ,mySize, sqSizeSmall)){
				sWe.push_back(objects[i]);
			}
			if  (Collision::square_collision(myPosition, sqCenterSE ,mySize, sqSizeSmall)){
				sEa.push_back(objects[i]);
			}

		}
	}
	else{
		// each object knows where he is
		for (int i = 0; i < objects.size(); ++i) {
			objects[i]->addToMyLeafs(this);
		}
	}
	// see if any new branch is created
	bool createNE = false;
	bool createSE = false;
	bool createNW = false;
	bool createSW = false;

	if (nEa.size() != 0 || nWe.size() != 0 || sEa.size() != 0 || sEa.size() != 0){
		this->nE->setParent(this);
		this->nE->buildFromScratch(nEa, this->height/2, this->width/2, this->center.x + this->width/4, this->center.y - this->height/4);
		createNE = true;

		this->nW->setParent(this);
		this->nW->buildFromScratch(nWe, this->height/2, this->width/2, this->center.x - this->width/4, this->center.y - this->height/4);
		createNW = true;

		this->sE->setParent(this);
		this->sE->buildFromScratch(sEa, this->height/2, this->width/2, this->center.x + this->width/4, this->center.y + this->height/4);
		createSE = true;

		this->sW->setParent(this);
		this->sW->buildFromScratch(sWe, this->height/2, this->width/2, this->center.x - this->width/4, this->center.y + this->height/4);
		createSW = true;
	}
}

void QuadTree::insertOneObject(Thing* object){
	// Insert the object in the quadtree
	// # object : object to insert

	sf::Vector2f sqSize = sf::Vector2f(width, height);
	sf::Vector2f sqSizeSmall = sf::Vector2f(width/2, height/2);
	sf::Vector2f sqCenter = sf::Vector2f(center.x, center.y);
	sf::Vector2f sqCenterSE = sf::Vector2f(center.x + width/4, center.y + height/4);
	sf::Vector2f sqCenterSW = sf::Vector2f(center.x - width/4, center.y + height/4);
	sf::Vector2f sqCenterNE = sf::Vector2f(center.x + width/4, center.y - height/4);
	sf::Vector2f sqCenterNW = sf::Vector2f(center.x - width/4, center.y - height/4);
	std::vector<Thing*> nEa;
	std::vector<Thing*> nWe;
	std::vector<Thing*> sEa;
	std::vector<Thing*> sWe;
	bool createNE = false;
	bool createSE = false;
	bool createNW = false;
	bool createSW = false;
	sf::Vector2f myPosition = object->getPosition();
	sf::Vector2f mySize = object->getMysize();

	objectsInside.push_back(object);

	if(objectsInside.size() > this->maxPerLeaf){
		if  (Collision::square_collision(myPosition, sqCenterNW ,mySize, sqSizeSmall)){
			createNW = true;
		}
		if  (Collision::square_collision(myPosition, sqCenterNE ,mySize, sqSizeSmall)){
			createNE = true;
		}
		if  (Collision::square_collision(myPosition, sqCenterSW ,mySize, sqSizeSmall)){
			createSW = true;
		}
		if  (Collision::square_collision(myPosition, sqCenterSE ,mySize, sqSizeSmall)){
			createSE = true;
		}

		for (int i = 0; i < objectsInside.size(); ++i) {
			//TODO : make conditions to only loop if needed ?
			myPosition = objectsInside[i]->getPosition();
			mySize = objectsInside[i]->getMysize();
			if  (Collision::square_collision(myPosition, sqCenterNW ,mySize, sqSizeSmall)){
				nWe.push_back(objectsInside[i]);
			}
			if  (Collision::square_collision(myPosition, sqCenterNE ,mySize, sqSizeSmall)){
				nEa.push_back(objectsInside[i]);
			}
			if  (Collision::square_collision(myPosition, sqCenterSW ,mySize, sqSizeSmall)){
				sWe.push_back(objectsInside[i]);
			}
			if  (Collision::square_collision(myPosition, sqCenterSE ,mySize, sqSizeSmall)){
				sEa.push_back(objectsInside[i]);
			}

		}
	}
	else{
		object->addToMyLeafs(this);
	}
	// see if any new branch is created

	if (this->nE->getHeight()==-1 && nEa.size()!=0){
		//not init
		this->nE->setParent(this);
		this->nE->buildFromScratch(nEa, this->height/2, this->width/2, this->center.x + this->width/4, this->center.y - this->height/4);
		createNE = true;
	}
	else if (createNE && objectsInside.size() > this->maxPerLeaf){
		this->nE->insertOneObject(object);
	}

	if (this->nW->getHeight()==-1 && nWe.size()!=0){
		//not init
		this->nW->setParent(this);
		this->nW->buildFromScratch(nWe, this->height/2, this->width/2, this->center.x - this->width/4, this->center.y - this->height/4);
		createNW = true;
	}
	else if (createNW && objectsInside.size() > this->maxPerLeaf){
		this->nW->insertOneObject(object);
	}

	if (this->sE->getHeight()==-1 && sEa.size()!=0){
		//not init
		this->sE->setParent(this);
		this->sE->buildFromScratch(sEa, this->height/2, this->width/2, this->center.x + this->width/4, this->center.y + this->height/4);
		createSE = true;
	}
	else if (createSE && objectsInside.size() > this->maxPerLeaf){
		this->sE->insertOneObject(object);
	}

	if (this->sW->getHeight()==-1 && sWe.size()!=0){
		//not init
		this->sW->setParent(this);
		this->sW->buildFromScratch(sWe, this->height/2, this->width/2, this->center.x - this->width/4, this->center.y + this->height/4);
		createSW = true;
	}
	else if (createSW && objectsInside.size() > this->maxPerLeaf){
		this->sW->insertOneObject(object);
	}

}

void QuadTree::eraseFromTree(Thing* object){
	// Erase object from the tree
	// # object : object to erase

	ptrdiff_t pos = find(objectsInside.begin(), objectsInside.end(), object) - objectsInside.begin();
	if (pos < objectsInside.size()){
		objectsInside.erase(objectsInside.begin() + pos);
		parent->eraseFromTree(object);
	}
}

void QuadTree::drawLeafTree(sf::RenderWindow &w) {
	// Draw the quadtree
	// # w : game window

	sf::RectangleShape rect;
	sf::Vector2f sizee = sf::Vector2f(this->width, this->height);
	rect.setSize(sizee);
	rect.setOutlineThickness(2);
	//rect.setOrigin(0,0);
	rect.setOrigin(this->width/2, this->height/2);
	rect.setPosition(this->center);
	rect.setOutlineColor(sf::Color::Green);
	rect.setFillColor(sf::Color::Transparent);
	rect.setScale(1,1);
	w.draw(rect);

	if (this->nE->getHeight() != -1){
		this->nE->drawLeafTree(w);
	}
	if (this->nW->getHeight() != -1){
		this->nW->drawLeafTree(w);
	}
	if (this->sW->getHeight() != -1){
		this->sW->drawLeafTree(w);
	}
	if (this->sE->getHeight() != -1){
		this->sE->drawLeafTree(w);
	}

}
