// Npc
// Non playable characters, controlled by patterns
// # positionsAreGlobal : if true go to the map point, if not go according to last point
// # isGoingToGoal : not the case if a 'mood' is triggered (ex : wants to battle instead)
// # positionsToGo : list of positions to go (z is not important right now)
// # currentPositionGoal : current position in vector
// # lastGoalPosition : from where you started to go to the current goal
// # readBackwardPositions : if in the end you read the instructions backward (TODO : delete this)
// # toDoAfter : 0 stop after all positions, 1 loop by going to 1rst, 2 loop by going backward
// # currentMoods : ids of 'moods' (attack when sees something, follows character...)

#include "Npc.h"
#include <iostream>
#include <vector>
#include "Character.h"
#include "Collision.h"

Npc::Npc() {
	// Auto-generated constructor

}

Npc::Npc(std::vector<std::string>& sheetPaths_v, int speed_h_v, int x_v, int y_v, int sprx,
		int spry, int facing, int sizex, int sizey,
		bool posGlobal, std::vector<sf::Vector3f> positionsToGoLater, int toDoAfter) {
	// More current constructor
	// # sheetPaths_v : a vector with the main spritesheets, first is for idle, second for walk
	// (a spritesheet is composed of 4 lines, one for each direction : down, up, left, right)
	// # speed_h_v : speed of the hero walk in pixel per frame
	// # x_v : x position of the hero
	// # y_v : y position of the hero
	// # sprx : sprite width
	// # spry : sprite height
	// # facing : the face position of the hero : Down = 0, Up = 1, Left = 2, Right = 3
	// # sizex : collision mask width
	// # sizey : collision mask height
	// # posGlobal : if the npc has to move to the coordinates globaly or
	// # positionToGo : positions that the npc has to follow
	// # toDoAfter : wether to loop the actions at the end from the beginning, end or stop

	sf::Vector3f po = sf::Vector3f();
	po.x = x_v;
	po.y = y_v;
	po.z = 0;

	sf::Vector3f lastGoalPosition=po;
	this->isGoingToGoal = true;
	int currentPositionGoal=0;
	this->positionsAreGlobal = posGlobal;
	this->positionsToGoLater = positionsToGoLater;
	this->positionsToGo.clear();
	this->toDoAfter = toDoAfter;
    this->setPosition(x_v,y_v);
    this->zLevel = 0;
    this->animationSpeed = 1;
    this->solid = true;
    this->visible = true;
    this->setAnimationId(1);//idle
    this->setFacing(facing);//down
    this->setSpriteSize(sprx, spry); //(121, 135)
    this->sheetPaths=sheetPaths_v;
    this->speed = 2;
    this->myMask = sf::RectangleShape(sf::Vector2f(sizex, sizey));
    this->currentPositionGoal =0;
    this->currentPositionGoalLater =0;
    finishedAStar = 0;
}


void Npc::move(QuadTree* root, LevelMap* level){
	// How the npc has to move
	// # root : quadtree with all objects that might collide with the npc
	// # level : the map with the tiles that might collide with the npc

	thingsToColide.clear();
	this->prepareCollision(root);

	// go up and down
	sf::Vector2f mainMove;
	mainMove.x = 0;
	mainMove.y = 0;
	int a = this->getAnimation().y;

	if (this->getFacing()==this->getUp()){
		if(this->getPosition().y<=0){
			this->setFacing(this->getDown());
		}
	}
	else if(this->getFacing()==this->getDown()){
		if(this->getPosition().y>=1000){
			this->setFacing(this->getUp());
		}
	}

	if (this->getFacing()==this->getUp() ){
		mainMove.y = mainMove.y - this->getSpeed();
		if (!Collision::collisionsMapIncluded(this, this->thingsToColide, mainMove, level))
		{
			this->setPosition(this->getPosition().x, this->getPosition().y - this->getSpeed());
			this->setAnimation(this->getAnimation().x, this->getUp());
			if (a!= this->getAnimation().y)
			{
				this->set_animation();
			}
		}
		else{
			this->setFacing(this->getDown());
		}
		this->setAnimationId(2);
	}else{
		mainMove.y += (this->getSpeed());
		if (!Collision::collisionsMapIncluded(this, this->thingsToColide, mainMove, level))
		{
			this->setPosition(this->getPosition().x, this->getPosition().y + this->getSpeed());
			this->setAnimation(this->getAnimation().x, this->getDown());
			if (a!= this->getAnimation().y){
				this->set_animation();
			}
		}
		else{
			this->setFacing(this->getUp());
		}
		this->setAnimationId(2);
	}

}

void Npc::moveAccordingly(QuadTree* root, LevelMap* level){
	// How the npc has to move
	// # root : quadtree with all objects that might collide with the npc
	// # level : the map with the tiles that might collide with the npc


	//std::cout<<((void *)root)<<std::endl;
	if (level->positionInVector(position.x,position.y) == level->positionInVector(positionsToGoLater[currentPositionGoalLater].x,positionsToGoLater[currentPositionGoalLater].y)){
		finishedAStar = 1;
		//std::cout<<("finished true")<<std::endl;
	}
	else if (!positionsToGo.empty() and positionsToGo.size()-1==currentPositionGoal){
		finishedAStar = 2;
		//std::cout<<("finished false")<<std::endl;
	}
	else{
		finishedAStar = 0;
		//std::cout<<("still doing ")<<std::endl;
	}

	if(currentPositionGoal <positionsToGo.size()){
		//std::cout<<(turnIddle)<<std::endl;
		thingsToColide.clear();
		this->prepareCollision(root);
		sf::Vector2f mainMove = nextMove(positionsToGo[currentPositionGoal].x, positionsToGo[currentPositionGoal].y);
		if (!Collision::collisionsMapIncluded(this, this->thingsToColide, mainMove, level))
		{
			//std::cout<<("really move")<<std::endl;
			turnIddle =0;
			this->setPosition(this->getPosition().x + mainMove.x, this->getPosition().y + mainMove.y);

			if(mainMove.x>0){
				this->setAnimation(this->getAnimation().x, this->getRight());
			}
			else if(mainMove.x<0){
				this->setAnimation(this->getAnimation().x, this->getLeft());
			}
			else if(mainMove.y>0){
				this->setAnimation(this->getAnimation().x, this->getDown());
			}
			else if(mainMove.y<0){
				this->setAnimation(this->getAnimation().x, this->getUp());
			}
			//std::cout<<("end move")<<std::endl;
		}
		else{
			//std::cout<<("iddle")<<std::endl;
			turnIddle+=1;
		}
		//iddle
		if(turnIddle>=Npc::maxIddleTurn){
			//std::cout<<("begin iddle")<<std::endl;
			thingsToColide.clear();
			this->prepareCollision(root);
			// it is the case if it didnt reach the goal due to loop limit settings
			currentPositionGoal=0;
			positionsToGo.clear();
			/*
			if (finishedAStar){
				if(currentPositionGoalLater + 1 <positionsToGoLater.size()){
					currentPositionGoalLater +=1;
					std::cout<<(currentPositionGoalLater)<<std::endl;
				}
				else{
					currentPositionGoalLater=0;
				}
			}*/
			std::vector<int> tempVector = generateAStarSearch(root, level, positionsToGoLater[currentPositionGoalLater].x, positionsToGoLater[currentPositionGoalLater].y);
			for (int tempVar = 0; tempVar < tempVector.size(); ++tempVar) {
				sf::Vector3f v = level->positionInCoordinate(tempVector[tempVar]);
				positionsToGo.push_back(v);
			}
			//std::cout<<("A* because iddle")<<std::endl;
			/*
			std::cout<<(positionsToGo.size())<<std::endl;
			std::cout<<( positionsToGoLater[currentPositionGoalLater].x)<<std::endl;
			std::cout<<( positionsToGoLater[currentPositionGoalLater].y)<<std::endl;
			std::cout<<(position.x)<<std::endl;
			std::cout<<(position.y)<<std::endl;*/
		}
		//-endiddle
		if(position.x==positionsToGo[currentPositionGoal].x && position.y==positionsToGo[currentPositionGoal].y){
			//std::cout<<("begin increase or restart")<<std::endl;
			if(currentPositionGoal + 1<positionsToGo.size()){
				currentPositionGoal +=1;
				/*
				std::cout<<("current (not later)")<<std::endl;
				std::cout<<(currentPositionGoal)<<std::endl;
				std::cout<<(positionsToGo[currentPositionGoal].x)<<std::endl;
				std::cout<<(positionsToGo[currentPositionGoal].y)<<std::endl;*/

			}
			else{
				//std::cout<<("A* begin")<<std::endl;
				currentPositionGoal=0;
				positionsToGo.clear();
				if (finishedAStar==1){
					if(currentPositionGoalLater + 1 <positionsToGoLater.size()){
						currentPositionGoalLater +=1;
						//std::cout<<(currentPositionGoalLater)<<std::endl;
					}
					else{
						currentPositionGoalLater=0;
					}
				}
				//std::cout<<("A* 1")<<std::endl;
				std::vector<int> tempVector = generateAStarSearch(root, level, positionsToGoLater[currentPositionGoalLater].x, positionsToGoLater[currentPositionGoalLater].y);
				//std::cout<<("A* 2")<<std::endl;
				for (int tempVar = 0; tempVar < tempVector.size(); ++tempVar) {
					sf::Vector3f v = level->positionInCoordinate(tempVector[tempVar]);
					positionsToGo.push_back(v);
				}
				//std::cout<<("A*")<<std::endl;
			}
			//std::cout<<("end no begin")<<std::endl;
		}
	}else{
		if (positionsToGo.empty() and finishedAStar==1){ // it is the case for the first move
			//std::cout<<("begin empty")<<std::endl;
			currentPositionGoal=0;
			positionsToGo.clear();
			currentPositionGoalLater=0;

			//std::cout<<("A* if empty ")<<std::endl;
			/*
			std::cout<<( positionsToGoLater[currentPositionGoalLater].x)<<std::endl;
			std::cout<<( positionsToGoLater[currentPositionGoalLater].y)<<std::endl;
			std::cout<<(position.x)<<std::endl;
			std::cout<<(position.y)<<std::endl;
			*/
			std::vector<int> tempVector = generateAStarSearch(root, level, positionsToGoLater[currentPositionGoalLater].x, positionsToGoLater[currentPositionGoalLater].y);
			for (int tempVar = 0; tempVar < tempVector.size(); ++tempVar) {
				sf::Vector3f v = level->positionInCoordinate(tempVector[tempVar]);
				positionsToGo.push_back(v);
			}

			/*for (int tempVar = 0; tempVar < positionsToGo.size(); ++tempVar) {
				std::cout<<("--")<<std::endl;
				std::cout<<(tempVar)<<std::endl;
				std::cout<<(positionsToGo[tempVar].x)<<std::endl;
				std::cout<<(positionsToGo[tempVar].y)<<std::endl;
			}*/
			//std::cout<<(tempVector.size())<<std::endl;
		}
		else{
			// it is the case if it didnt reach the goal due to loop limit settings
			//std::cout<<("begin incomplete")<<std::endl;
			currentPositionGoal=0;
			positionsToGo.clear();
			if (finishedAStar==1){
				if(currentPositionGoalLater + 1 <positionsToGoLater.size()){
					currentPositionGoalLater +=1;
					//std::cout<<(currentPositionGoalLater)<<std::endl;
				}
				else{
					currentPositionGoalLater=0;
				}
			}
			std::vector<int> tempVector = generateAStarSearch(root, level, positionsToGoLater[currentPositionGoalLater].x, positionsToGoLater[currentPositionGoalLater].y);
			for (int tempVar = 0; tempVar < tempVector.size(); ++tempVar) {
				sf::Vector3f v = level->positionInCoordinate(tempVector[tempVar]);
				positionsToGo.push_back(v);
			}
			//std::cout<<("A* because incomplete")<<std::endl;
			/*
			std::cout<<(positionsToGo.size())<<std::endl;
			std::cout<<( positionsToGoLater[currentPositionGoalLater].x)<<std::endl;
			std::cout<<( positionsToGoLater[currentPositionGoalLater].y)<<std::endl;
			std::cout<<(position.x)<<std::endl;
			std::cout<<(position.y)<<std::endl;*/
		}

	}
	//std::cout<<("real end")<<std::endl;

}

sf::Vector2f Npc::nextMove(int goalX, int goalY){
	// Decides which will be the move
	// # goalX : coordinate of the wanted position
	// # goalY : coordinate of the wanted position

	sf::Vector2f result;
	int difX = position.x - goalX;
	int difY = position.y - goalY;
	if(abs(difX) + abs(difY) <= speed){
		result.x = -1*difX;
		result.y = -1*difY;
	}
	else if(abs(difX)>abs(difY)){
		result.x = -1*difX/abs(difX)*speed;
		result.y = 0;
	}
	else{
		result.y = -1*difY/abs(difY)*speed;
		result.x = 0;
	}
	return result;
}

std::vector<int> Npc::generateAStarSearch(QuadTree* root, LevelMap* level, int goalX, int goalY){
	// returns path from start to goal using the level's tiles
	// # root : quadtree with all objects that might collide with the npc
	// # level : the map with the tiles that might collide with the npc
	// # goalX : coordinate of the wanted position
	// # goalY : coordinate of the wanted position

	std::vector<int> result;

	// get level info
	int tileSize = level->getTileSize();
	int width = level->getWidth();
	int height = level->getHeight();
	int sizeArrayTile = level->getSizeArrayTile();

	// initialize
	int start = level->positionInVector(this->position.x, this->position.y);
	int startX = level->positionInCoordinate(start).x;
	int startY = level->positionInCoordinate(start).y;
	int goal = level->positionInVector(goalX, goalY);
	goalX = level->positionInCoordinate(goal).x;
	goalY = level->positionInCoordinate(goal).y;
	std::vector<int> alreadyVisited;
	std::vector<int> toVisit;
	std::vector<int> startDistance;
	std::vector<int> goalDistance;
	std::vector<bool> visited;
	std::vector<bool> seenAtLeastOnce;
	std::vector<int> previous;
	for (int i = 0; i < (sizeArrayTile); ++i) {
		startDistance.push_back(-1);
		goalDistance.push_back(-1);
		previous.push_back(-1);
		visited.push_back(false);
		seenAtLeastOnce.push_back(false);
	}

	startDistance[start]=0;
	goalDistance[start]= Collision::distanceSquare(goalX, goalY, startX, startY);
	startDistance[goal]=Collision::distanceSquare(goalX, goalY, startX, startY);
	goalDistance[goal]= 0;

	int currentX = level->positionInCoordinate(start).x;
	int currentY = level->positionInCoordinate(start).y;
	int currentI = level->positionInVector(currentX, currentY);
	int visitingPotential =0;
	int min = -1;
	int minPlace = -1;


	alreadyVisited.push_back(start);
	visited[start] = true;
	seenAtLeastOnce[start] = true;

	std::vector<int> forbidden;
	if (turnIddle!=0){
		std::vector<int> temp = Collision::objects_to_map_tiles(this->thingsToColide, level);
		forbidden = temp;
		std::cout<<(forbidden.size())<<std::endl;
	}

	int nbLoop = 0;
	while (currentI != goal and nbLoop<Npc::maxLoopForAStar){
		nbLoop+=1;

		// south -
		visitingPotential = level->positionInVector(currentX, currentY + tileSize);
		if(visitingPotential!=-1 and !visited[visitingPotential] and !seenAtLeastOnce[visitingPotential] and
				(turnIddle==0 or !Collision::tiles_collisions_at_position(this, currentX, currentY + tileSize, level, forbidden)) and
				!Collision::map_collisions_at_position(this, currentX, currentY + tileSize, level)
		){
		toVisit.push_back(visitingPotential);
		goalDistance[visitingPotential]= Collision::distanceSquare(goalX, goalY, currentX, currentY + tileSize);
		startDistance[visitingPotential]= Collision::distanceSquare(startX, startY, currentX, currentY + tileSize);
		previous[visitingPotential] = currentI;
		seenAtLeastOnce[visitingPotential] = true;
		}


		// north -
		visitingPotential = level->positionInVector(currentX, currentY - tileSize);
		if(visitingPotential!=-1 and !visited[visitingPotential] and !seenAtLeastOnce[visitingPotential] and
			(turnIddle==0 or !Collision::tiles_collisions_at_position(this, currentX, currentY - tileSize, level, forbidden)) and
			!Collision::map_collisions_at_position(this, currentX, currentY - tileSize, level)){
		toVisit.push_back(visitingPotential);
		goalDistance[visitingPotential]= Collision::distanceSquare(goalX, goalY, currentX, currentY - tileSize);
		startDistance[visitingPotential]= Collision::distanceSquare(startX, startY, currentX, currentY - tileSize);
		previous[visitingPotential] = currentI;
		seenAtLeastOnce[visitingPotential] = true;
		}


		// east -
		visitingPotential = level->positionInVector(currentX + tileSize, currentY);
		if(visitingPotential!=-1 and !visited[visitingPotential] and !seenAtLeastOnce[visitingPotential] and
			(turnIddle==0 or !Collision::tiles_collisions_at_position(this, currentX + tileSize, currentY, level, forbidden)) and
			!Collision::map_collisions_at_position(this, currentX + tileSize, currentY, level)){
		toVisit.push_back(visitingPotential);
		goalDistance[visitingPotential]= Collision::distanceSquare(goalX, goalY, currentX + tileSize, currentY);
		startDistance[visitingPotential]= Collision::distanceSquare(startX, startY, currentX + tileSize, currentY);
		previous[visitingPotential] = currentI;
		seenAtLeastOnce[visitingPotential] = true;
		}

		// west -
		visitingPotential = level->positionInVector(currentX - tileSize, currentY);
		if(visitingPotential!=-1 and !visited[visitingPotential] and !seenAtLeastOnce[visitingPotential] and
			(turnIddle==0 or !Collision::tiles_collisions_at_position(this, currentX - tileSize, currentY, level, forbidden)) and
			!Collision::map_collisions_at_position(this, currentX - tileSize, currentY, level)){
		toVisit.push_back(visitingPotential);
		goalDistance[visitingPotential]= Collision::distanceSquare(goalX, goalY, currentX - tileSize, currentY);
		startDistance[visitingPotential]= Collision::distanceSquare(startX, startY, currentX - tileSize, currentY);
		previous[visitingPotential] = currentI;
		seenAtLeastOnce[visitingPotential] = true;
		}

		// chose the unvisited one with lowest 'distGoal + distStart'
		minPlace = -1;
		min = -1;
		for (int j = 0; j < toVisit.size(); ++j) {
			//if(std::find(alreadyVisited.begin(), alreadyVisited.end(), toVisit[j]) == alreadyVisited.end()){ // not already visited
			if(!visited[toVisit[j]]){ // not already visited
				if (min ==-1||(startDistance[toVisit[j]] + goalDistance[toVisit[j]] < min)){
					min = startDistance[toVisit[j]] + goalDistance[toVisit[j]];
					minPlace = toVisit[j];
				}
			}
		}
		currentI = minPlace;
		currentX = level->positionInCoordinate(currentI).x;
		currentY = level->positionInCoordinate(currentI).y;
		alreadyVisited.push_back(currentI);
		visited[currentI] = true;
		seenAtLeastOnce[currentI] = true;
		toVisit.erase(std::remove(toVisit.begin(), toVisit.end(), currentI), toVisit.end());
		/*
		std::cout<<("--_")<<std::endl;
		std::cout<<(currentX)<<std::endl;
		std::cout<<(currentY)<<std::endl;
		std::cout<<("poids")<<std::endl;
		std::cout<<(startDistance[currentI])<<std::endl;
		std::cout<<(startDistance[currentI])<<std::endl;*/

	}

	//std::cout<<("nbloop :")<<std::endl;
	//std::cout<<(nbLoop)<<std::endl;

	if (nbLoop ==Npc::maxLoopForAStar){
		// if not to the end, go to the closest tile to the goal already visited
		minPlace = -1;
		min = -1;
		for (int j = 0; j < alreadyVisited.size(); ++j) {
			if (min ==-1||(goalDistance[alreadyVisited[j]] + startDistance[alreadyVisited[j]/2] < min)){
				min = goalDistance[alreadyVisited[j]] + startDistance[alreadyVisited[j]]/2;
				minPlace = alreadyVisited[j];
			}
		}
		currentI = minPlace;
		currentX = level->positionInCoordinate(currentI).x;
		currentY = level->positionInCoordinate(currentI).y;
	}


	//std::cout<<("after loop 1 in A* function")<<std::endl;

	while(currentI!=start){
		result.push_back(currentI);
		currentI = previous[currentI];
	}
	std::reverse(result.begin(), result.end());
	std::cout<<(result.size())<<std::endl;

	return result;
}

Npc::~Npc() {
	// Auto-generated destructor
}

