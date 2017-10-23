// Thing
// All objects that are physically present in the world (UI excluded) : NPC, Tiles, Main Character...
// # position : where is the object
// # spriteSize : used for drawing sprite
// # myMask : used for collision
// # mySprite : current sprite of the object
// # mySheet : the current spritesheet
// # sheetPaths : 0 : iddle, 1 : walk
// # visible : if it has to be drawn
// # solid : if collision
// # dir : Down=0, Up=1, Left=2, Right=3
// # animationId : which sprite of the line of the spritesheet
// # animation :  x and y in the sprite sheet
// # animationSpeed : change sprite every ? frame, 0 if not animated
// # facing : where the character is looking (Down=0, Up=1, Left=2, Right=3)
// # zLevel : -3, -2,-1,0, 1, 2, 3 are the floor levels, normal height : 0
// # thingsToColide : all objects on map except him
// # myLeafs : all the leafs of the quadtree he is in

#include "Thing.h"
#include <SFML/Graphics.hpp>
#include <iostream>
#include <set>

Thing::Thing() {
	// Auto-generated constructor
}

Thing::Thing(std::vector<std::string> sheetPaths_v) {
	// Constructor, seldom used
	// # sheetPaths_v : the path to the spritesheet
	this->sheetPaths = sheetPaths_v;
	this->solid = true;
	this->visible = true;
}

Thing::~Thing() {
	// Auto-generated destructor
}

void Thing::draw_thing(sf::RenderWindow &w) {
	// Draw the sprite at the right position
	// # w : game window

	if (visible)
	{
		this->mySprite.setPosition(this->position.x,this->position.y);
		this->mySprite.setOrigin(this->spriteSize.x/2, this->spriteSize.y/2);
		w.draw(this->mySprite);
	}
}

void Thing::set_animation() {
	// Sets which frame should be displayed

	this->mySheet.loadFromFile(this->sheetPaths[this->animationId - 1]); //load image as sprite
	if ((this->animation.x + 1)*this->spriteSize.x>=this->mySheet.getSize().x and animationSpeed != 0){
			this->animation.x=0;
	}
    this->mySprite.setTexture(this->mySheet);
    this->mySprite.setTextureRect(sf::IntRect(this->animation.x*this->spriteSize.x,this->animation.y*this->spriteSize.y,
                                              this->spriteSize.x,this->spriteSize.y));
    if (this->animationId>0 and this->animationSpeed !=0){
            this->animation.x++;
            if ((this->animation.x + 1)*this->spriteSize.x>=this->mySheet.getSize().x){
                this->animation.x=0;
            }
    }
}

sf::Vector2f Thing::getPosition() {
    return this->position;
}
int Thing::getSpriteH() {
    return this->spriteSize.y;
}
int Thing::getSpriteW() {
    return this->spriteSize.x;
}

bool Thing::sortingVector(const Thing& key1, const Thing& key2) {
	// Compares the objects according to their level position, used to compare the objects Thing in vector
	// # key1 : first object
	// # key2 : second object

    return key1.getLevel() < key2.getLevel();
}

bool Thing::sortingVectorPoint(const Thing* key1, const Thing* key2) {
	// Compares the objects pointer according to their level position, used to compare the pointers Thing in vector
	// # key1 : first object
	// # key2 : second object

	Thing v1 = *key1;
	Thing v2 = *key2;
	return v1.getLevel() < v2.getLevel();
}

void Thing::drawEverything(sf::RenderWindow &w, std::vector<Thing*> &things) {
	// Draw all objects (pointer version)
	// # w : game window
	// # things : all objects to draw, pointers

	unsigned int sizeThings =  things.size();
	for (int i = 0; i < sizeThings; ++i) {
		(*(things[i])).draw_thing(w);
	}
}

void Thing::drawEverything(sf::RenderWindow &w, std::multiset<Thing*, Thing> &things) {
	// Draw all objects (pointer version)
	// # w : game window
	// # things : all objects to draw, pointers

	//std::cout<< things.size() <<std::endl;
	unsigned int sizeThings =  things.size();
	std::multiset<Thing*>::iterator it = things.begin();

	for (int i = 0; i < sizeThings; ++i) {
		(*(*it++)).draw_thing(w);
	}
}

void Thing::drawEverything2(sf::RenderWindow &w, std::vector<Thing> things) {
	// Draw all objects (object version)
	// # w : game window
	// # things : all objects to draw, not pointers

	unsigned int sizeThings =  things.size();
	for (int i = 0; i < sizeThings; ++i) {
		((things[i])).draw_thing(w);
	}
}

std::vector<Thing*> Thing::generateCollisionList() {
	// Generates the collision list from the leaf list (from quadtree)

	std::vector<Thing*> result;
	for (int i = 0; i < myLeafs.size(); ++i) {
		std::vector<Thing*> candidate = myLeafs[i]->getObjectsInside();
		for (int j = 0; j < candidate.size(); ++j) {
			if (candidate[j] != this && std::find(result.begin(),result.end(),candidate[j])==result.end()){
				result.push_back(candidate[j]);
			}
		}
	}
	return result;
}

void Thing::eraseFromTree() {
	// Erase object from the quadtree, uses the list myLeafs

	for (int i = 0; i < myLeafs.size(); ++i) {
		myLeafs[i]->eraseFromTree(this);
	}
}

void Thing::prepareCollision(QuadTree* root) {
	// All steps to prepare the collision list
	// # root : the main quadtree

	thingsToColide = root->retrieve(this);
	std::set<Thing*> s( thingsToColide.begin(), thingsToColide.end() );
	thingsToColide.assign( s.begin(), s.end() );
}

void Thing::resetTreePosition(QuadTree* root) {
	// Erase from the quadtree to then re-insert it
	// # root : main quadtree

	this->eraseFromTree();
	this->emptyMyLeafs();
	root->insertOneObject(this);
}

void Thing::purifyLeafList(){
	// Takes the quadtree leaf list and removes the leaf parents if any

	std::vector<QuadTree*> parents;
	for (int i = 0; i < myLeafs.size(); ++i) {
		QuadTree* p = myLeafs[i]->getParent();
		parents.push_back(p);
	}
	std::vector<QuadTree*> newLeaf;
	for (int i = 0; i < myLeafs.size(); ++i) {
		if (std::find(parents.begin(),parents.end(),myLeafs[i])==parents.end()){
			newLeaf.push_back(myLeafs[i]);
		}
	}
	this->setMyLeafs(newLeaf);
}
