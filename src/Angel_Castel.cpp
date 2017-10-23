// Angel_Castle
// Main Loop

#include <SFML/Graphics.hpp>
#include "Collision.h"
#include "Hero.h"
#include "SolidSquare.h"
#include "Npc.h"
#include "Thing.h"
#include "LevelMap.h"
#include "MapTile.h"
#include "Npc.h"
#include "QuadTree.h"

sf::RenderWindow windowH;
bool fps = true;

sf::Clock timed;
float fps_c=0;
float fps_switch=400;//400  //100
float fps_speed=800;

// walk npc
bool check = true;
float check_time=0;
float check_time_lim=8;

sf::View view;
int screenH=500;
int screenW=800;

// regenerate tree
const int regTreeTimeMax = 1240;//120
int regTreeTime = regTreeTimeMax - 1;

int main()
{
	//-------------------- create first level
	int colision_obj_number_lv1 = 4;

    windowH.create(sf::VideoMode(screenW, screenH), "Angel Castle");
    // Howl's Castle!
    sf::CircleShape shape(100.f);
    shape.setFillColor(sf::Color::Green);

    // create hero that the player controls
    std::vector<std::string> sheetPaths_hero;
    sheetPaths_hero.push_back("angelbaby1.png");
	sheetPaths_hero.push_back("angelbaby1.png");
	std::vector<std::string> sheetPaths_v;
	sheetPaths_v.push_back("mario1.png");
	sheetPaths_v.push_back("mario2.png");
    Hero monHero(sheetPaths_hero, 4, 0, 0, 47, 64, 1, 47, 64);  //pas mettre les ()
    std::vector<sf::Vector3f> positionsToGo;
    sf::Vector3f n;
    n.x=-800;
    n.y=-800;
    //n.x=-100;
    //n.y=-100;
    positionsToGo.push_back(n);
    n.x=800;
    n.y=800;
    //n.x=500;
    //n.y=200;
	positionsToGo.push_back(n);
	n.x=300;
	n.y=300;
	positionsToGo.push_back(n);
	n.x=0;
	n.y=0;
	positionsToGo.push_back(n);

    std::vector<Npc*> dummies; //npc list
    Npc myDummy(sheetPaths_v, 1, 100, 50, 47, 64, 1, 47, 64, true, positionsToGo, 0);
    dummies.push_back(&myDummy);

    //Npc clone(sheetPaths_v, 1, -100, -450, 47, 64, 1, 47, 64, true, positionsToGo, 0);
    //dummies.push_back(&clone);

    /*
    Npc myDummy2(sheetPaths_v, 1, 300, 100, 47, 64, 1, 47, 64, true, positionsToGo, 0);
    dummies.push_back(&myDummy2);

    Npc myDummy3(sheetPaths_v, 1, 10, 100, 47, 64, 1, 47, 64, true, positionsToGo, 0);
    dummies.push_back(&myDummy3);

	Npc myDummy4(sheetPaths_v, 1, 400, 100, 47, 64, 1, 47, 64, true, positionsToGo, 0);
	dummies.push_back(&myDummy4);
	*/

	/*Npc myDummy5(sheetPaths_v, 1, 800, 100, 47, 64, 1, 47, 64, true, positionsToGo, 0);
	dummies.push_back(&myDummy5);
	Npc myDummy6(sheetPaths_v, 1, 700, 100, 47, 64, 1, 47, 64, true, positionsToGo, 0);
	dummies.push_back(&myDummy6);
	Npc myDummy7(sheetPaths_v, 1, 400, 300, 47, 64, 1, 47, 64, true, positionsToGo, 0);
	dummies.push_back(&myDummy7);
	Npc myDummy8(sheetPaths_v, 1, -400, 100, 47, 64, 1, 47, 64, true, positionsToGo, 0);
	dummies.push_back(&myDummy8);*/




    //-----------------------test-begin
    std::vector<Thing> colision_objects_lv1;


	//tile => std::string path_tile, int x, int y, int sizeX, int sizeY, bool collision, bool visible, int animation_speed, int level,int sheetX, int sheetY

    std::vector<MapTile> tileSet;
    tileSet.push_back(MapTile("old_tiles.png", 100, 100, 16, 16, true, false, 0, -2, 0, 1));
    tileSet.push_back(MapTile("old_tiles.png", 100, 100, 16, 16, true, true, 0, -2, 3, 1));



    /*
	MapTile myTile1 = MapTile("old_tiles.png", 100, 100, 64, 64, true, true, 0, -1, 0, 1);

	MapTile myTile2 = MapTile("old_tiles.png", 100, 200, 32, 32, true, true, 0, 2, 0, 0);

	MapTile myTile3 = MapTile("old_tiles.png", 400, 300, 32, 64, true, true, 0, 2, 0, 0);


	colision_objects_lv1.push_back(myTile1);
	colision_objects_lv1.push_back(myTile2);
	colision_objects_lv1.push_back(myTile3);

	MapTile myTile4 = MapTile("old_tiles.png", 800, 600, 32, 32, true, true, 0, 2, 0, 0);

	MapTile myTile5 = MapTile("old_tiles.png", 900, 300, 32, 64, true, true, 0, 2, 0, 0);
	MapTile myTile6 = MapTile("old_tiles.png", 400, 800, 32, 32, true, true, 0, 2, 0, 0);

	MapTile myTile7 = MapTile("old_tiles.png", 400, 900, 32, 64, true, true, 0, 2, 0, 0);

	colision_objects_lv1.push_back(myTile4);
	colision_objects_lv1.push_back(myTile5);
	colision_objects_lv1.push_back(myTile6);
	colision_objects_lv1.push_back(myTile7);

	MapTile myTile8 = MapTile("old_tiles.png", -400, 900, 32, 64, true, true, 0, 2, 0, 0);
	colision_objects_lv1.push_back(myTile8);
	MapTile myTile9 = MapTile("old_tiles.png", -400, 500, 32, 64, true, true, 0, 2, 0, 0);
	colision_objects_lv1.push_back(myTile9);
	MapTile myTile10 = MapTile("old_tiles.png", -400, 200, 32, 64, true, true, 0, 2, 0, 0);
	colision_objects_lv1.push_back(myTile10);
	MapTile myTile11 = MapTile("old_tiles.png", -400, 100, 32, 64, true, true, 0, 2, 0, 0);
	colision_objects_lv1.push_back(myTile11);
	MapTile myTile12 = MapTile("old_tiles.png", -500, 100, 32, 64, true, true, 0, 2, 0, 0);
	colision_objects_lv1.push_back(myTile12);
	MapTile myTile13 = MapTile("old_tiles.png", -300, 300, 32, 64, true, true, 0, 2, 0, 0);
	colision_objects_lv1.push_back(myTile13);
	MapTile myTile14 = MapTile("old_tiles.png", -300, -200, 32, 64, true, true, 0, 2, 0, 0);
	colision_objects_lv1.push_back(myTile14);
	*/


	LevelMap first_level = LevelMap(colision_objects_lv1,1600,1000, tileSet);

	// ---- THINGS TO DRAW
	std::vector<Thing*> things_to_draw;

	// add npc to draw
	for (int ii = 0;ii < dummies.size(); ++ii) {
		things_to_draw.push_back(dummies[ii]);
	}
	// add player to draw
	things_to_draw.push_back(&monHero);

	// add other objects to draw
	for (int j = 0; j < first_level.getColisionObjects().size(); ++j) {
		things_to_draw.push_back((first_level.getColisionObjectsAdd(j)));
	}


	//___________________

	std::cout<<("tableau avant")<<std::endl;

	// sort the objects according to layers
	sort(things_to_draw.begin(), things_to_draw.end(), Thing::sortingVectorPoint); //sorts according to appearance order :  zLevel

	std::cout<<("tableau trie")<<std::endl;

	std::vector<Thing*> things_to_collide;
	// get objects to collide with the hero
	for (int i = 0; i < things_to_draw.size(); ++i) {
		bool isNpc = false;
		// check if is an npc
		for (int ii = 0;ii < dummies.size(); ++ii) {
			if (things_to_draw[i]==dummies[ii]){
				isNpc = true;
			}
		}
		if (things_to_draw[i]->isSolid() ){
			things_to_collide.push_back(things_to_draw[i]);
		}
	}

    //set current map
    LevelMap current_map = first_level;

    // second test with quadtree
    QuadTree* treeTest2 = new QuadTree(0,current_map.getWidth()*2, current_map.getHeight()*2, 0, 0);

    std::cout<<("nb of collision obj")<<std::endl;
    std::cout<<(things_to_collide.size())<<std::endl;

    // collect info from map
   int colision_obj_number;
   colision_obj_number = current_map.getColisionObjects().size();

   std::vector<Thing> colision_objects = current_map.getColisionObjects();


   std::vector<sf::Vector2f> colision_positions(things_to_collide.size());
   std::vector<sf::Vector2f> colision_sizes(things_to_collide.size());
   for (int col_counter = 0; col_counter < things_to_collide.size(); col_counter++)
	{
		colision_positions[col_counter] = (*things_to_collide[col_counter]).getPosition();
		colision_sizes[col_counter] = (*things_to_collide[col_counter]).getMysize();
	}

   //std::vector<Thing*> all_things_to_draw;
   //auto f = [](Thing* a, Thing* b) { return *a->getLevel() < *b->getLevel(); };
   std::multiset<Thing*, Thing> all_things_to_draw;

   std::multiset<Thing*, Thing> objectsToDraw;
   for (int j = 0; j < things_to_draw.size(); ++j) {
	   objectsToDraw.insert(things_to_draw[j]);
	}

   //std::set<Thing*> all_things_to_draw;

   std::cout<<"before main loop :"<<std::endl;
   //Main Loop Start
   windowH.setFramerateLimit(60);
    while (windowH.isOpen())
    {
        sf::Event event;
        while (windowH.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                windowH.close();
        }

        monHero.set_animation();
		for (int ii = 0;ii < dummies.size(); ++ii) {
			(*dummies[ii]).set_animation();
		}
		//std::cout<<"end dummies animation"<<std::endl;
        int mapRadius = current_map.getWidth();
        int mapRadius2 = current_map.getHeight();

        //check buttons

        // second test quadtreee 2
        //std::cout<<"begin tree :"<<std::endl;
		treeTest2->clearChildren();
		treeTest2->buildFromScratch(things_to_collide);
		//std::cout<<"end tree, begin hero press button"<<std::endl;

		monHero.press_button(treeTest2, &current_map);
		//std::cout<<"begin dummy move :"<<std::endl;
		for (int ii = 0;ii < dummies.size(); ++ii) {
			//(*dummies[ii]).move(treeTest2, &current_map);
			(*dummies[ii]).moveAccordingly(treeTest2, &current_map);
		}
		//std::cout<<"end dummy move"<<std::endl;

		//set view camera
		view.reset(sf::FloatRect(0,0,screenW,screenH)); // window size
		sf::Vector2f position(screenW/2,screenH/2);
		position.x= monHero.getPosition().x + monHero.getSpriteW()/2 - screenW/2;
		position.y= monHero.getPosition().y + monHero.getSpriteH()/2 - screenH/2;

		if (position.y <(-mapRadius2)){
			position.y=-mapRadius2;
		}
		if (position.y + screenH>mapRadius2-current_map.getTileSize()){
			position.y=mapRadius2 -current_map.getTileSize() - screenH;
		}
		if (position.x <(-mapRadius)){
			position.x=-mapRadius;
		}
		if (position.x + screenW>mapRadius){
			position.x=mapRadius - screenW;
		}
		view.reset(sf::FloatRect(position.x,position.y,screenW,screenH));
		windowH.setView(view);
		//std::cout<<"draw window clear "<<std::endl;
		//draw
		windowH.clear();
		//treeTest.drawLeafTree(windowH);
		treeTest2->drawLeafTree(windowH);
		all_things_to_draw = objectsToDraw;
		//std::cout<<"refresh position"<<std::endl;
		current_map.refreshPositions(windowH, position.x + screenW/2 - current_map.getTileSize(),position.y + screenH/2,screenW,screenH);
		//std::cout<<"loop all things"<<std::endl;
		for (int j = 0; j < first_level.getTiles().size(); ++j) {
			all_things_to_draw.insert(current_map.getTileModel(j));
		}
		//std::cout<<"draw all"<<std::endl;
		Thing::drawEverything(windowH, all_things_to_draw);
		windowH.display();


    }

    return 0;
}

