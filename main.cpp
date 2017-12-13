#include <iostream>
#include <map>

#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>

#include "vec.h"
#include "config.h"

using namespace std;


static void on_display() {

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glClearColor(1, 1, 1, 1); // Klasa color potrebna

	glutSwapBuffers();
}

static void on_reshape(int width, int height) {
	glViewport(0, 0, width, height);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(60, (float) width/(float) height, 0.1, 1500);
}

static void on_keyboard(unsigned char c, int x, int y) {
}

static void on_mouse(int button, int state, int x, int y) {
}

static void on_timer(int value) {
	if (value != 0)
		return;
	glutPostRedisplay();
	glutTimerFunc(10, on_timer, 0);
}

int main(int argc, char * argv[]) {

	std::map<string, config> confMap = config::importAll("configs", "DEVELOPMENT");
	config appConfig = confMap["application"];

	glutInit(&argc, argv);
	int dflWidth = std::stoi(appConfig.getParameter("WIN_WIDTH"));
	int dflHeight = std::stoi(appConfig.getParameter("WIN_HEIGHT"));

	glutInitWindowSize(dflWidth, dflHeight);
	glutInitWindowPosition(0, 0);

	const char * title = appConfig.getParameter("TITLE").c_str();
	glutCreateWindow(title);

	glutInitDisplayMode(GLUT_RGB | GLUT_DEPTH | GLUT_DOUBLE);
	glEnable(GL_DEPTH_TEST);
	glutDisplayFunc(on_display);
	glutReshapeFunc(on_reshape);
	glutKeyboardFunc(on_keyboard);
	glutTimerFunc(10, on_timer, 0);


	glutMainLoop();

	return 0;
}
