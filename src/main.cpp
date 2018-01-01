#include <iostream>
#include <map>
#include <algorithm>

#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>

/* GL MATH Biblioteka */
#include <glm/glm.hpp>
#include <glm/vec3.hpp>
#include <glm/mat4x4.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "dataContainer.h"
#include "config.h"
#include "object.h"
#include "user.h"
#include "camera.h"
#include "math_objects.h"
#include "various_objects.h"

using namespace std;

/* Ovde se nalaze svi podaci koji 
 * se dele izmedju funkcija 
 * (umesto globalnih promenljivih) */
dataContainer globalData;

static void on_display() 
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glClearColor(0.5, 0.0, 0.0, 1); // Klasa color potrebna (mozda i ne)

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	/* Postavljanje tacke gledista */
	glLoadMatrixf(glm::value_ptr(globalData.activeCamera->viewMatrix()));
	//gluLookAt(3, 4, 5, 0, 0, 0, 0, 1, 0);

	/* Iscrtavanje objekata */
	std::vector<object* > &objectsToDisplay = globalData.toDisplay;
	for_each (objectsToDisplay.begin(), objectsToDisplay.end(), [] (object * o) {
		o->draw();
	});

	glutSwapBuffers();
}

static void on_reshape(int width, int height) {
	glViewport(0, 0, width, height);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(60, (float) width/(float) height, 0.1, 1500);
}

static void on_keyboard(unsigned char c, int x, int y) 
{
	/* U switch-u se obradjuju komande koje ne uticu na 
	 * pojedinacne objekte vec na ceo program */
	switch (c) {
		case 27: exit(EXIT_SUCCESS);
	}

	globalData.pressedKeys[c] = true; 
	globalData.keyPressedPositionX = x;
	globalData.keyPressedPositionY = y;
}

static void on_keyboard_release(unsigned char c, int x, int y)
{
	globalData.pressedKeys[c] = false; 
	globalData.keyReleasedPositionX = x;
	globalData.keyReleasedPositionY = y;
}

static void on_mouse_move(int x, int y) {
	globalData.mousePosition = glm::vec2(x,y);
}

static void mouse_timer(int value) {

	if (value != globalData.mouseTimerId)
		return;

	GLint m_viewport[4];
	glGetIntegerv( GL_VIEWPORT, m_viewport );

	glm::vec2 center(m_viewport[2]/2, m_viewport[3]/2);
	glm::vec2 mousePosition = globalData.mousePosition;

	glm::vec2 delta = center-mousePosition;

	std::vector<object* > &objectsToMouseMove = globalData.toMouseMove;
	for_each (objectsToMouseMove.begin(), objectsToMouseMove.end(), [delta] (object * o) {
		o->processMouseMove(delta);
	});

	glutWarpPointer( center.x , center.y );

	glutTimerFunc(globalData.mouseTimerInterval, mouse_timer, globalData.mouseTimerId);
}

static void keyboard_timer(int value) {

	if (value != globalData.keyboardTimerId)
		return;

	/* TODO: Obradjivanje zahteva tastature, ne bi trebalo ovde da stoje verovatno!!! */
	std::vector<object* > &objectsToKeyboard = globalData.toKeyboard;
	for_each (objectsToKeyboard.begin(), objectsToKeyboard.end(), [] (object * o) {
		o->processKeyboardInput(globalData.pressedKeys, globalData.keyPressedPositionX, globalData.keyPressedPositionY);
	});

	glutTimerFunc(globalData.keyboardTimerInterval, keyboard_timer, globalData.keyboardTimerId);

}

static void redisplay_timer(int value) 
{
	if (value != globalData.redisplayTimerId)
		return;

	glutPostRedisplay();
	glutTimerFunc(globalData.redisplayTimerInterval, redisplay_timer, globalData.redisplayTimerId);
}

int main(int argc, char * argv[]) 
{
	globalData.configs = config::importAll("configs", "DEVELOPMENT");
	config appConfig = globalData.configs["application"];

	glutInit(&argc, argv);
	int dflWidth = std::stoi(appConfig.getParameter("WIN_WIDTH"));
	int dflHeight = std::stoi(appConfig.getParameter("WIN_HEIGHT"));

	glutInitWindowSize(dflWidth, dflHeight);
	glutInitWindowPosition(0, 0);

	const char * title = appConfig.getParameter("TITLE").c_str();
	glutCreateWindow(title);

	glutInitDisplayMode(GLUT_RGB | GLUT_DEPTH | GLUT_DOUBLE);

	/* Anti-Aliasing */
	glEnable(GL_MULTISAMPLE);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_POLYGON_SMOOTH);
	glEnable(GL_LINE_SMOOTH);
	glHint(GL_LINE_SMOOTH_HINT, GL_NICEST);
	glHint(GL_POLYGON_SMOOTH_HINT, GL_NICEST);

	/* Normalize normals */
	glEnable(GL_NORMALIZE);

	glutDisplayFunc(on_display);
	glutReshapeFunc(on_reshape);
	glutKeyboardFunc(on_keyboard);
	glutKeyboardUpFunc(on_keyboard_release);
	glutPassiveMotionFunc(on_mouse_move);
	glutSetCursor(GLUT_CURSOR_NONE);
	glutTimerFunc(globalData.redisplayTimerInterval, redisplay_timer, globalData.redisplayTimerId);
	glutTimerFunc(globalData.mouseTimerInterval, mouse_timer, globalData.mouseTimerId);
	glutTimerFunc(globalData.keyboardTimerInterval, keyboard_timer, globalData.keyboardTimerId);

	/* Ispod je primer dat zbog testing-a */
	
	std::vector<object* > &objectsToDisplay = globalData.toDisplay;
	std::vector<object* > &objectsToKeyboard = globalData.toKeyboard;
	std::vector<object* > &objectsToMouseMove = globalData.toMouseMove;

	axis cs(5);
	objectsToDisplay.push_back(&cs);

	triangleFloor floor1(12*4);
	floor1.scale(glm::vec3(4.0f,1.0f,4.0f));
	floor1.translate(glm::vec3(0.0f,-0.13f,0.0f));
	objectsToDisplay.push_back(&floor1);

	axis random(5);
	random.translate(glm::vec3(1,1,1));

	user sampleUser;
	objectsToDisplay.push_back(&sampleUser);
	globalData.activeCamera = sampleUser.fpsViewCamera();

	axis cameracs(5);

	objectsToDisplay.push_back(&random);
	objectsToKeyboard.push_back(&sampleUser);
	objectsToMouseMove.push_back(&sampleUser);

	glutMainLoop();

	return 0;
}
