#pragma once

#include <map>

#include "Ball.h"
#include "Player.h"
#include "glut.h"


class Game {

private:
	Ball ball;
	Player player1;
	Player player2;

	std::map<unsigned char, bool> keyPressedMap;

	int player1Score = 0;
	int player2Score = 0;

	int screenWidth;
	int screenHeight;


public:
	
	Game(int screenWidth, int screenHeight);


	void draw();

	void onKeyDown(char key);
	void onKeyUp(char key);

	void onLeftMouseDown();
	void gameLoop(float timeDelta);

private:

	void movePlayers(const float deltaTimeSeconds);
	void moveBall(const float deltaTimeSeconds);
	void resolveCollisions(const float deltaTimeSeconds);

	void drawBall();
	void drawPlayer(int);
	void drawCircle(int x, int y, float r);
	void drawScore();

	void confinePlayerWithinBoundaries(int);
	void resolveBallCollisions();

	void checkBallPlayerCollision(int);
	void checkBallBordersCollision();
	void checkGoal();
	void reset();

	void drawText(int x, int y, const char* text); 
};