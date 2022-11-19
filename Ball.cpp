#include "Ball.h"





Ball::Ball(const glm::vec2 position, const glm::vec2 velocity) {
	this->position = position;
	this->velocity = velocity;
}

void Ball::setPosition(glm::vec2 newPos) {
	position = newPos;
}

glm::vec2 Ball::getVelocity()
{
	return velocity;
}

glm::vec2 Ball::getPosition()
{
	return position;
}


void Ball::setVelocity(glm::vec2 newVelocity) {
	velocity = newVelocity;
}