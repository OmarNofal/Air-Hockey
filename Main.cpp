
#include <chrono>
#include "Game.h"

#define WIDTH 700
#define HEIGHT 700

#define FRAME_TIME 6



Game game(WIDTH, HEIGHT);


long long previousFrameTimeMillis = -1;


void keyboardDown(unsigned char key, int mouseX, int mouseY) {
	game.onKeyDown(key);
}

void keyboardUp(unsigned char key, int mouseX, int mouseY) {
	game.onKeyUp(key);
}

void draw() {
	glClear(GL_COLOR_BUFFER_BIT);
	game.draw();
	glFlush();
}

long long getCurrentTimeMillis() {
	return std::chrono::duration_cast<std::chrono::milliseconds>(
		std::chrono::system_clock::now().time_since_epoch()
		).count();
}

void gameLoop(int _) {

	long long frameStartTimeMillis = getCurrentTimeMillis();
	
	long long deltaTimeMillis;
	if (previousFrameTimeMillis == -1) {
		deltaTimeMillis = 0;
	}
	else {
		deltaTimeMillis = frameStartTimeMillis - previousFrameTimeMillis;
	}
	previousFrameTimeMillis = frameStartTimeMillis;

	game.gameLoop(deltaTimeMillis / 1000.0f);
	glutPostRedisplay();
	
	// Run next frame as soon as possible
	glutTimerFunc(0, gameLoop, 0);
}



int main(int argc, char** argv) {
	glutInit(&argc, argv);

	glutInitWindowSize(WIDTH, HEIGHT);
	glutCreateWindow("Air Hockey");

	glutDisplayFunc(draw);
	glutKeyboardFunc(keyboardDown);
	glutKeyboardUpFunc(keyboardUp);
	glutTimerFunc(FRAME_TIME, gameLoop, 0);

	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB | GLUT_MULTISAMPLE);
	glClearColor(.2f, 0.0f, 0.0f, 0.0f);
	glEnable(GL_BLEND);
	glEnable(GL_POLYGON_SMOOTH);
	
	gluOrtho2D(0.0, WIDTH, 0.0, HEIGHT);


	glutMainLoop();
}