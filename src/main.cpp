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
	glClearColor(0.08, 0.08, 0.17, 1); // Klasa color potrebna (mozda i ne)

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	/* Postavljanje tacke gledista */
	glLoadMatrixf(glm::value_ptr(globalData.activeCamera->viewMatrix()));
	//gluLookAt(0.2, 0.2, 0.2, 0, 0.2, 0, 0, 1, 0);
	
	/* Iscrtavanje objekata */
	std::vector<object* > &objectsToDisplay = globalData.toDisplay;
	for_each (objectsToDisplay.begin(), objectsToDisplay.end(), [objectsToDisplay] (object * o) {
		o->draw();
	});

	glutSwapBuffers();
}

static void on_reshape(int width, int height) {
	glViewport(0, 0, width, height);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(60, (float) width/(float) height, 0.01f, 1500.0f);
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

static void gravity_timer(int value)
{
	if (value != globalData.gravityTimerId)
		return;

	std::vector<object* > &toGravity = globalData.toGravity;
	for_each (toGravity.begin(), toGravity.end(), [] (object * o) {
		o->velocity.y -= 0.005;
		//o->move(glm::vec3(0,o->velocity.y,0));
		o->move(glm::vec3(o->velocity.x,o->velocity.y,o->velocity.z));
	});

	glutTimerFunc(globalData.gravityTimerInterval, gravity_timer, globalData.gravityTimerId);
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
	glEnable(GL_DEPTH_TEST);

	/* Odsecanje zadnje strane poligona */
	//glFrontFace(GL_CW);
	// glEnable(GL_CULL_FACE);

	///* Normalize normals */
	//glEnable(GL_NORMALIZE);

	glutDisplayFunc(on_display);
	glutReshapeFunc(on_reshape);
	glutKeyboardFunc(on_keyboard);
	glutKeyboardUpFunc(on_keyboard_release);
	glutMotionFunc(on_mouse_move);
	glutPassiveMotionFunc(on_mouse_move);
	glutSetCursor(GLUT_CURSOR_NONE);
	glutTimerFunc(globalData.redisplayTimerInterval, redisplay_timer, globalData.redisplayTimerId);
	glutTimerFunc(globalData.mouseTimerInterval, mouse_timer, globalData.mouseTimerId);
	glutTimerFunc(globalData.keyboardTimerInterval, keyboard_timer, globalData.keyboardTimerId);
	glutTimerFunc(globalData.gravityTimerInterval, gravity_timer, globalData.gravityTimerId);

	/* Ispod je dat primer programa zbog testiranja-a */

	std::vector<object* > &objectsToDisplay = globalData.toDisplay;
	std::vector<object* > &objectsToKeyboard = globalData.toKeyboard;
	std::vector<object* > &objectsToMouseMove = globalData.toMouseMove;
	std::vector<object* > &objectsToGravity = globalData.toGravity;
	

	//axis cs(5);
	//cs.scale(glm::vec3(1,0.5f,1));
	//objectsToDisplay.push_back(&cs);
	//cs.translate(glm::vec3(2,1,2));

	wireCube floor1;
	grid floor1Grid(12*4);
	floor1Grid.translate(glm::vec3(0.0f,0.9f,0.0f));
	floor1Grid.fill = true;
	floor1.addChild(&floor1Grid);
	axis originCs;
	originCs.scale(glm::vec3(4.0f,4.0f,4.0f));
	objectsToDisplay.push_back(&originCs);
	
	floor1.rotate(12, glm::vec3(-.5,0,0.5));
	floor1.scale(glm::vec3(4.0f,0.5f,4.0f));
	objectsToDisplay.push_back(&floor1);

	user sampleUser;
	//sampleUser.addToCheckColisionList(&cs);
	sampleUser.addToCheckColisionList(&floor1);
	sampleUser.translate(glm::vec3(0,4,0));

	objectsToDisplay.push_back(&sampleUser);
	globalData.activeCamera = sampleUser.fpsViewCamera();

	axis cameracs(5);

	objectsToKeyboard.push_back(&sampleUser);
	objectsToMouseMove.push_back(&sampleUser);
	objectsToGravity.push_back(&sampleUser);

	wireCube cubes[65];
	grid topOfCube;
	topOfCube.fill = true;
	topOfCube.translate(glm::vec3(0,0.86f,0));
	int i;
	for (i=0; i<65; i++) {
		cubes[i].rotate((float)i/20.0f*180, glm::vec3(0,1,0));
		cubes[i].translate(glm::vec3(i*0.15+5.3, i*0.3f, i*0.15+5.3));
		cubes[i].scale(glm::vec3(1.0f, 0.21f, 1.0f));
		sampleUser.addToCheckColisionList(&cubes[i]);
		objectsToDisplay.push_back(&cubes[i]);
		cubes[i].addChild(&topOfCube);
	}

	grid floor2Grid(12*4);
	wireCube floor2;
	sampleUser.addToCheckColisionList(&floor2);
	floor2Grid.translate(glm::vec3(0.0f,0.9f,0.0f));
	floor2Grid.fill= true;
	floor2.addChild(&floor2Grid);

	axis origin2Cs;
	origin2Cs.scale(glm::vec3(1.0f,5.0f,1.0f));

	floor2.rotate((float)i/20.0f*180, glm::vec3(0,1,0));
	floor2.translate(glm::vec3(i*0.15+3, i*0.25f, i*0.15+5.3));

	floor2.scale(glm::vec3(4.0f,0.5f,4.0f));
	objectsToDisplay.push_back(&floor2);


	glutMainLoop();

	return 0;
}
