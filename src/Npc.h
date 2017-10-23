#ifndef NPC_H_
#define NPC_H_
#include <SFML/Graphics.hpp>
#include <iostream>
#include <vector>
#include "Collision.h"
#include "Character.h"

class Npc : public Character {
	public:
		Npc();
		Npc(std::vector<std::string>& sheetPaths_v, int speed_h_v, int x_v,
				int y_v, int sprx, int spry, int facing, int sizex, int sizey,
				bool posGlobal, std::vector<sf::Vector3f> positionsToGo, int toDoAfter);
        virtual ~Npc();
        void move(std::vector<sf::Vector2f> positions, std::vector<sf::Vector2f> sizes, QuadTree* root);
        void move(QuadTree* root, LevelMap* level);
        void moveAccordingly(QuadTree* root, LevelMap* level);
        void generateAStar(QuadTree* root, LevelMap* level);
        sf::Vector2f nextMove(int goalX, int goalY);
        std::vector<int> generateAStarSearch(QuadTree* root, LevelMap* level, int goalX, int goalY);
        static const int maxLoopForAStar = 500; // 800 is too long
        static const int maxIddleTurn = 60;
    private:
        int turnIddle;
        int finishedAStar; // 0 still doing, 1 finished, 2 not finished but no move
        bool positionsAreGlobal; //if true go to the map point, if not go according to last point
        bool isGoingToGoal; // not the case if a 'mood' is triggered
		std::vector<sf::Vector3f> positionsToGo; // z is not important right now
		std::vector<sf::Vector3f> positionsToGoLater;
		int currentPositionGoal;
		int currentPositionGoalLater;
		sf::Vector3f lastGoalPosition; // from where you started to go to the current goal
		bool readBackwardPositions;
		int toDoAfter; // 0 stop after all positions, 1 loop by going to 1rst, 2 loop by going backward
		std::vector<int> currentMoods; // ids of 'moods' (attack when sees something, follows character...)

};

#endif /* NPC_H_ */
