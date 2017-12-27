#include <iostream>
#include <map>
#include <algorithm>

#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>

#include "dataContainer.h"
#include "config.h"
#include "vec.h"
#include "mat.h"
#include "object.h"
#include "camera.h"
#include "axis.h"

using namespace std;

/* Ovde se nalaze svi podaci koji 
 * se dele izmedju funkcija 
 * (umesto globalnih promenljivih) */
dataContainer globalData;

static void on_display() 
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glClearColor(0.75, 0.75, 0.75, 1); // Klasa color potrebna (mozda i ne)

	/* TODO: Obradjivanje zahteva tastature, ne bi trebalo ovde da stoje verovatno!!! */
	std::vector<object* > &objectsToKeyboard = globalData.toKeyboard;
	for_each (objectsToKeyboard.begin(), objectsToKeyboard.end(), [] (object * o) {
		o->processKeyboardInput(globalData.pressedKeys, globalData.keyPressedPositionX, globalData.keyPressedPositionY);
	});

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	/* Postavljanje tacke gledista */
	mat4 tmp = globalData.activeCamera->matrix*globalData.cameraRelativeTo->matrix;
	mat4 view = tmp.inverse();

	glLoadMatrixf(view.elements);
	//gluLookAt(2, 3, 5, 0, 0, 0, 0, 1, 0);

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

static void on_mouse(int button, int state, int x, int y) {
}

static void on_timer(int value) 
{
	if (value != 0)
		return;
	glutPostRedisplay();
	glutTimerFunc(10, on_timer, 0);
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
	glutDisplayFunc(on_display);
	glutReshapeFunc(on_reshape);
	glutKeyboardFunc(on_keyboard);
	glutKeyboardUpFunc(on_keyboard_release);
	glutTimerFunc(10, on_timer, 0);

	/* Ispod je primer dat zbog testing-a */
	
	std::vector<object* > &objectsToDisplay = globalData.toDisplay;
	std::vector<object* > &objectsToKeyboard = globalData.toKeyboard;

	axis wcs(5);
	objectsToDisplay.push_back(&wcs);

	axis objectcs(5);

	camera cam3rdPerson;
	axis cameracs(5);
	cam3rdPerson.translate(vec3(0,2,2));
	cam3rdPerson.rotate(-30, vec3(1,0,0));
	cam3rdPerson.addChild(&cameracs);
	globalData.activeCamera = &cam3rdPerson;
	globalData.cameraRelativeTo = &objectcs;

	objectsToDisplay.push_back(&objectcs);
	objectsToDisplay.push_back(&cam3rdPerson);
	objectsToKeyboard.push_back(&objectcs);

	glutMainLoop();

	return 0;
}
