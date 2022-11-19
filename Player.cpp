#include "Player.h"



Player::Player() {

}

Player::Player(glm::vec2 startPos) : pos(startPos), velocity({0, 0})
{
}

void Player::setPosition(glm::vec2 newPos)
{
	pos = newPos;
}

void Player::setVelocity(glm::vec2 newVelocity)
{
	velocity = newVelocity;
}

glm::vec2 Player::getPosition()
{
	return pos;
}

glm::vec2 Player::getVelocity()
{
	return velocity;
}
