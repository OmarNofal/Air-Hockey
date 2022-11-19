#pragma once

#include "glm/vec2.hpp"
#include "Color.h"


static const Color ballColor = {0.3f, 0.2f, 0.6f};

class Ball {

private:
	glm::vec2 velocity;
	glm::vec2 position;


public:
	void setVelocity(glm::vec2);
	void setPosition(glm::vec2);
	glm::vec2 getVelocity();
	glm::vec2 getPosition();

	Ball() = delete;
	Ball(const glm::vec2 position, const glm::vec2 velocity = { 0, 0 });

};