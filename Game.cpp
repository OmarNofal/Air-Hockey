#include "Game.h"
#include <math.h>
#include <utility>
#include <iostream>
#include <string>


#define PLAYER_MOVING_SPEED 300
#define PLAYER_RADIUS 20.0f
#define BALL_RADIUS 10.0f
#define BALL_DECELERATION 50
#define PLAYER_ACCELERATION 50
#define PLAYER_DECELERATION (BALL_DECELERATION + 20)

Game::Game(int screenWidth, int screenHeight): screenWidth(screenWidth), screenHeight(screenHeight), ball({screenWidth / 2, screenHeight / 2}) {
	player1 = Player({screenWidth / 2, 30});
	player2 = Player({screenWidth / 2, screenHeight - 30});
}


void Game::draw() {
	drawPlayer(1);
	drawPlayer(2);
	drawBall();
	drawScore();
}


void Game::drawPlayer(int playerNumber) {

	Player& drawnPlayer =  (playerNumber == 1) ? player1 : player2;
	
	auto playerPos = drawnPlayer.getPosition();
	
	drawCircle(playerPos.x, playerPos.y, PLAYER_RADIUS);

}

void Game::drawBall() {
	auto ballPosition = ball.getPosition();
	drawCircle(ballPosition.x, ballPosition.y, BALL_RADIUS);
}

void Game::drawCircle(int x, int y, float r) {
	glPushMatrix();
	glTranslatef(x, y, 0);
	GLUquadric* quadObj = gluNewQuadric();
	gluDisk(quadObj, 0, r, 100, 5);
	glPopMatrix();
}

void Game::drawScore() {
	std::string scoreText{ "Score: " };
	drawText(20, screenHeight - 30, (scoreText + std::to_string(player2Score) ).c_str());
	drawText(20, 20, (scoreText + std::to_string(player1Score)).c_str());
}

void Game::onKeyDown(char key) {
	keyPressedMap[key] = true;
}

void Game::onKeyUp(char key) {
	keyPressedMap[key] = false;
}

void Game::gameLoop(const float deltaTimeSeconds) {

	movePlayers(deltaTimeSeconds);
	moveBall(deltaTimeSeconds);
	resolveCollisions(deltaTimeSeconds);

}


void Game::movePlayers(const float deltaTimeSeconds) {

	// Moving player 1
	auto player1Velocity = glm::vec2(0.0f, 0.0f);
	
	if (keyPressedMap['a']) {
		player1Velocity.x -= PLAYER_MOVING_SPEED;
	}
	if (keyPressedMap['d']) {
		player1Velocity.x += PLAYER_MOVING_SPEED;
	}
	if (keyPressedMap['w']) {
		player1Velocity.y += PLAYER_MOVING_SPEED;
	}
	if (keyPressedMap['s']) {
		player1Velocity.y -= PLAYER_MOVING_SPEED;
	}

	// Moving player 2
	auto player2Velocity = glm::vec2(0.0f, 0.0f);
	
	if (keyPressedMap['j']) {
		player2Velocity.x -= PLAYER_MOVING_SPEED;
	}
	if (keyPressedMap['l']) {
		player2Velocity.x += PLAYER_MOVING_SPEED;
	}
	if (keyPressedMap['i']) {
		player2Velocity.y += PLAYER_MOVING_SPEED;
	}
	if (keyPressedMap['k']) {
		player2Velocity.y -= PLAYER_MOVING_SPEED;
	}

	player1.setVelocity(player1Velocity);
	player1.setPosition(player1.getPosition() + player1Velocity * deltaTimeSeconds);

	player2.setVelocity(player2Velocity);
	player2.setPosition(player2.getPosition() + player2Velocity * deltaTimeSeconds);

}


void Game::moveBall(const float deltaTimeSeconds) {

	auto speed = ball.getVelocity();
	auto position = ball.getPosition();

	glm::vec2 newPosition = (position + speed * deltaTimeSeconds);
	ball.setPosition(newPosition);
}


void Game::resolveCollisions(const float deltaTimeSeconds) {

	confinePlayerWithinBoundaries(1);
	confinePlayerWithinBoundaries(2);
	resolveBallCollisions();

}

void Game::confinePlayerWithinBoundaries(int playerNumber) {

	auto& player = (playerNumber == 1) ? player1 : player2;
	auto playerPos = player.getPosition();

	int maxY = (playerNumber == 1) ? screenHeight / 2 : screenHeight;
	int minY = (playerNumber == 1) ? 0 : screenHeight / 2;

	if (playerPos.x >= screenWidth - PLAYER_RADIUS) {
		playerPos.x = screenWidth - PLAYER_RADIUS;
	} 

	if (playerPos.x < 0 + PLAYER_RADIUS) {
		playerPos.x = PLAYER_RADIUS;
	}

	if (playerPos.y > maxY - PLAYER_RADIUS) {
		playerPos.y = maxY - PLAYER_RADIUS;
	}
	
	if (playerPos.y < minY + PLAYER_RADIUS) {
		playerPos.y = minY + PLAYER_RADIUS;
	}

	player.setPosition(playerPos);
}


void Game::resolveBallCollisions() {
	
	checkBallPlayerCollision(1);
	checkBallPlayerCollision(2);
	checkBallBordersCollision();
	checkGoal();

}

void Game::checkBallPlayerCollision(int playerNumber)
{

	auto player = (playerNumber == 1) ? player1 : player2;
	auto playerPosition = player.getPosition();
	auto playerVelocity = player.getVelocity();

	auto ballPosition = ball.getPosition();
	auto ballVelocity = ball.getVelocity();

	const int distance = glm::distance(playerPosition, ballPosition);

	if (distance <= BALL_RADIUS + PLAYER_RADIUS) {

		printf("Ball position: <%f, %f>\n", ballPosition.x, ballPosition.y);
		printf("Player position: <%f, %f>\n", playerPosition.x, playerPosition.y);

		printf("Player velocity: <%f, %f>\n", playerVelocity.x, playerVelocity.y);

		auto diff = ballPosition - playerPosition;

		printf("Diff: <%f, %f>\n", diff.x, diff.y);

		auto newVelocityDirection = glm::normalize(diff);

		printf("New velocity direction: <%f, %f>\n", newVelocityDirection.x, newVelocityDirection.y);

		glm::vec2 velocity;
		if (playerVelocity == glm::vec2{ 0, 0 }) {
			velocity = -ballVelocity;
		}
		else {
			velocity = playerVelocity * glm::dot( newVelocityDirection, glm::normalize(playerVelocity) );
		}

		auto newVelocity = newVelocityDirection * glm::length(velocity);

		float velocityBias;
		if (ballVelocity == glm::vec2{0, 0}) 
			velocityBias = 1;
		else 
			velocityBias = glm::dot(newVelocityDirection, glm::normalize(ballVelocity));
		
		
		auto finalVelocity = newVelocity ;

		std::cout << "Ball new velocity: " << finalVelocity.x << " " << finalVelocity.y << std::endl << std::endl;

		ball.setVelocity(finalVelocity);
	}

}

void Game::checkBallBordersCollision() {


	auto ballPosition = ball.getPosition();
	auto ballVelocity = ball.getVelocity();


	if (ballPosition.x > screenWidth - BALL_RADIUS) {
		ballPosition.x = screenWidth - BALL_RADIUS;
		ballVelocity.x = -ballVelocity.x;
	}
	else if (ballPosition.x < BALL_RADIUS) {
		ballPosition.x = BALL_RADIUS;
		ballVelocity.x = -ballVelocity.x;
	}

	ball.setPosition(ballPosition);
	ball.setVelocity(ballVelocity);
}


void Game::checkGoal() {

	auto ballPosition = ball.getPosition();

	// GOAL for player 2
	if (ballPosition.y < BALL_RADIUS) {
		player2Score++;
		reset();
	}
	// goal for player 1
	else if (ballPosition.y > screenHeight - BALL_RADIUS) {
		player1Score++;
		reset();
	}

}

void Game::reset() {
	ball.setPosition({ screenWidth / 2, screenHeight / 2 });
	ball.setVelocity({ 0, 0 });
}


void Game::drawText(int x, int y, const char* text) {
	int len, i;

	//set the position of the text in the window using the x and y coordinates
	glRasterPos2f(x, y);

	//get the length of the string to display
	len = (int)strlen(text);

	//loop to display character by character
	for (i = 0; i < len; i++)
	{
		glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, text[i]);
	}
}