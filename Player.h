#pragma once
#include "glm/glm.hpp"

static const int radius = 50;

class Player {


private:
	glm::vec2 pos;
	glm::vec2 velocity;

public:

	Player();
	Player(glm::vec2 startPos);

	void setPosition(glm::vec2 newPos);
	void setVelocity(glm::vec2 velocity);

	glm::vec2 getPosition();
	glm::vec2 getVelocity();
};