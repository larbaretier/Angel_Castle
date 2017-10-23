/*
 * Character.h
 *
 *  Created on: 24 oct. 2016
 *      Author: Louis
 */

#ifndef CHARACTER_H_
#define CHARACTER_H_
#include <string>
#include <iostream>
#include "Thing.h"

class Character : public Thing {
	public:
			Character();
			Character(std::vector<std::string> sheetPaths_v);
			virtual ~Character();

	const std::string& getName() const {
		return name;
	}

	void setName(const std::string& name) {
		this->name = name;
	}

	int getSpeed() const {
		return speed;
	}

	void setSpeed(int speed) {
		this->speed = speed;
	}

	protected:
			int speed;
			std::string name;
};

#endif /* CHARACTER_H_ */
