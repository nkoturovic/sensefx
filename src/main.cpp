#include <iostream>
#include <map>
#include <algorithm>

/* OPEN_GL */
#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>

/* GL_MATH */
#include <glm/glm.hpp>
#include <glm/vec3.hpp>
#include <glm/mat4x4.hpp>
#include <glm/gtc/type_ptr.hpp>

/* HELPERS */
#include "DataContainer.h"
#include "Config.h"

/* OBJECT */
#include "Object.h"
#include "DrawableObject.h"
#include "TexturedObject.h"
#include "MovableObject.h"
#include "User.h"
#include "math_objects.h"
#include "various_objects.h"
#include "objloader.h"
#include "Model.h"

/* CAMERA */
#include "Camera.h"

/* TEXTURE2D */
#include "Texture.h"
#include "Material.h"

#include "Light.h"

/* Simulacija File Explorer */
#include "FXSimulation.h"


using namespace std;

/* Ovde se nalaze svi podaci koji
 * se dele izmedju funkcija
 * (umesto globalnih promenljivih) */
DataContainer globalData;

/* CALLBACK-ovi */
static void on_display();
static void on_reshape(int width, int height);
static void on_keyboard(unsigned char c, int x, int y);
static void on_keyboard_release(unsigned char c, int x, int y);
static void on_mouse_move(int x, int y);

/* TAJMERI */
static void redisplay_timer(int value);
static void keyboard_timer(int value);
static void mouse_timer(int value);
static void gravity_timer(int value);
static void fx_simulation_timer(int value);


int main(int argc, char * argv[])
{
	globalData.configs = Config::importAll("configs", "DEVELOPMENT");

	Config appConfig = globalData.configs["application"];

	glutInit(&argc, argv);
	int dflWidth = std::stoi(appConfig.getParameter("WIN_WIDTH"));
	int dflHeight = std::stoi(appConfig.getParameter("WIN_HEIGHT"));

	glutInitWindowSize(dflWidth, dflHeight);
	glutInitWindowPosition(0, 0);

	const char * title = appConfig.getParameter("TITLE").c_str();
	glutCreateWindow(title);

	glutInitDisplayMode(GLUT_RGBA | GLUT_DEPTH | GLUT_DOUBLE);
	glEnable(GL_DEPTH_TEST);

	/* Inicijalizacija callback-ova i tajmera */
	glutDisplayFunc(on_display);
	glutReshapeFunc(on_reshape);
	glutKeyboardFunc(on_keyboard);
	glutKeyboardUpFunc(on_keyboard_release);
	glutMotionFunc(on_mouse_move);
	glutTimerFunc(globalData.fxSimulationTimerInterval, fx_simulation_timer, globalData.fxSimulationTimerId);
	glutPassiveMotionFunc(on_mouse_move);
	glutSetCursor(GLUT_CURSOR_NONE);
	glutTimerFunc(globalData.redisplayTimerInterval, redisplay_timer, globalData.redisplayTimerId);
	glutTimerFunc(globalData.mouseTimerInterval, mouse_timer, globalData.mouseTimerId);
	glutTimerFunc(globalData.keyboardTimerInterval, keyboard_timer, globalData.keyboardTimerId);
	glutTimerFunc(globalData.gravityTimerInterval, gravity_timer, globalData.gravityTimerId);

	/* Normalizacija normala */
	glEnable(GL_NORMALIZE);

	/* Uklucivanje osvetljenja */
	glEnable(GL_LIGHTING);

	/* Ukljucivanje tekstura */
	glEnable(GL_TEXTURE_2D);

	//glBlendFunc(GL_SRC_ALPHA, GL_SRC_COLOR);
	// THISONE glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	//glBlendFunc (GL_ONE, GL_ONE);
	glEnable(GL_BLEND);

       // glLightModeli(GL_LIGHT_MODEL_COLOR_CONTROL, GL_SEPARATE_SPECULAR_COLOR );
	/* Podesava se rezim iscrtavanja tekstura tako da boje na teksturi
	* potpuno odredjuju boju objekata. */
	// glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);

	/* Uticaj boje na materijal  */
	//glColorMaterial(GL_FRONT_AND_BACK, GL_EMISSION);

	/* Liste (vektori) objekata koji se prosledjuju callback funkcijama i tajmerima */
       // std::vector<Object*> &objectsToDisplay = globalData.objectsToDisplay;
       // std::vector<Object*> &objectsToKeyboard = globalData.objectsToKeyboard;
       // std::vector<Object*> &objectsToMouseMove = globalData.objectsToMouseMove;
       // std::vector<Object*> &objectsToGravity = globalData.objectsToGravity;
	
	/* IMPORT TEKSTURA MORA NAKON UKLJUCIVANJA TEKSTURA!! */
	globalData.textures = Texture2D::importAll("resources/textures");
	globalData.models = Model::importAll("resources/models");
	globalData.materials = Material::importAll("resources/materials");
	globalData.lights = Light::importAll("resources/lights");
	globalData.lights["light1"].enable();

	/*******************************************/
	/* Ispod je dat primer test (demo) program */
	/*******************************************/
	FXSimulation simulation("demodir");
	simulation.simulate(&globalData);
	
	glutMainLoop();

	return 0;
}


/* CALLBACK-ovi */
static void on_display()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glClearColor(0.08, 0.08, 0.17, 1); // Klasa color potrebna (mozda i ne)

	
	glMatrixMode(GL_TEXTURE);
	glLoadIdentity();

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	/* Postavljanje tacke gledista */
	glLoadMatrixf(glm::value_ptr(globalData.activeCamera->viewMatrix()));
	//gluLookAt(0.2, 0.2, 0.2, 0, 0.2, 0, 0, 1, 0);

	std::vector<Object* > &objectsToDisplay = globalData.objectsToDisplay;
	/* Sortiranje po daljini od user-a  Posto se iz vektora cita unazad,
	 * sortiramo u obrnutom poretku, na pocetak idu najdalji */
	User * au = globalData.activeUser;
	std::sort (objectsToDisplay.begin(), objectsToDisplay.end(), [au] (Object * a, Object * b) -> bool {
		if (glm::length(au->pointToObjectSys(a, glm::vec3(0,0,0))) > glm::length(au->pointToObjectSys(b, glm::vec3(0,0,0))))
			return true;
		else
			return false;
	});

	/* Iscrtavanje objekata */
	for_each (objectsToDisplay.begin(), objectsToDisplay.end(), [objectsToDisplay] (Object * o) {
		if(DrawableObject * d_o = dynamic_cast<DrawableObject*>(o))
			d_o->draw();
	});

	/* Ovde se obradjuju prosledjeni tekstovi na ekran */

	for_each (globalData.textToScreenVec.begin(), globalData.textToScreenVec.end(), [] (Text t) {
		t.print(globalData.screenSize);
	});

	/* Ispisano se brise */
	globalData.textToScreenVec.clear();

	glutSwapBuffers();
}

static void on_reshape(int width, int height) 
{
	glViewport(0, 0, width, height);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(60, (float) width/(float) height, 0.01f, 2000.0f);

	globalData.screenSize = glm::vec2(width,height);
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

static void on_mouse_move(int x, int y) 
{
	globalData.mousePosition = glm::vec2(x,y);
}


/* TAJMERI */ static void redisplay_timer(int value)
{
	if (value != globalData.redisplayTimerId)
		return;

	glutPostRedisplay();
	glutTimerFunc(globalData.redisplayTimerInterval, redisplay_timer, globalData.redisplayTimerId);
}

static void keyboard_timer(int value) 
{
	if (value != globalData.keyboardTimerId)
		return;

	/* TODO: Obradjivanje zahteva tastature, ne bi trebalo ovde da stoje verovatno!!! */
	std::vector<Object* > &objectsToKeyboard = globalData.objectsToKeyboard;
	for_each (objectsToKeyboard.begin(), objectsToKeyboard.end(), [] (Object * o) {
		if(MovableObject* m_o = dynamic_cast<MovableObject*>(o))
			m_o->processKeyboardInput(globalData.pressedKeys, globalData.keyPressedPositionX, globalData.keyPressedPositionY);
	});

	glutTimerFunc(globalData.keyboardTimerInterval, keyboard_timer, globalData.keyboardTimerId);
}

static void mouse_timer(int value) 
{
	if (value != globalData.mouseTimerId)
		return;

	GLint m_viewport[4];
	glGetIntegerv( GL_VIEWPORT, m_viewport );

	glm::vec2 center(m_viewport[2]/2, m_viewport[3]/2);
	glm::vec2 mousePosition = globalData.mousePosition;

	glm::vec2 delta = center-mousePosition;

	std::vector<Object* > &objectsToMouseMove = globalData.objectsToMouseMove;
	for_each (objectsToMouseMove.begin(), objectsToMouseMove.end(), [delta] (Object * o) {
		if(MovableObject* m_o = dynamic_cast<MovableObject*>(o)) 
			m_o->processMouseMove(delta);
	});

	glutWarpPointer( center.x , center.y );

	glutTimerFunc(globalData.mouseTimerInterval, mouse_timer, globalData.mouseTimerId);
}

static void gravity_timer(int value)
{
	if (value != globalData.gravityTimerId)
		return;

	std::vector<Object* > &toGravity = globalData.objectsToGravity;
	for_each (toGravity.begin(), toGravity.end(), [] (Object * o) {
		if(MovableObject* m_o = dynamic_cast<MovableObject*>(o)) {
			m_o->addToVelocity(glm::vec3(0.0f, -0.005, 0.0f));
			m_o->move(m_o->getVelocity());
		}
	});

	glutTimerFunc(globalData.gravityTimerInterval, gravity_timer, globalData.gravityTimerId);
}

static void fx_simulation_timer(int value) {

	/* Ovaj tajmer moze da se shvati kao tajmer za sve poslove file explorer simulacije,
	 * ovde je ubacen za razresavanje kolizije sa objektima, za neku drugu simulaciju 
	 * ovaj tajmer treba da se menja */

	if (value != globalData.fxSimulationTimerId)
		return;

	User * activeUser = globalData.activeUser;
	std::vector<Object* > &simColisionList = globalData.simColisionList;

	for_each (simColisionList.begin(), simColisionList.end(), [activeUser] (Object * o) {
		if(File* f_o = dynamic_cast<File*>(o)) {
			if (f_o->isColiding(activeUser)) {
				
				/* TODO: Formatiranje teksta da bude iz config-a eksternog */
				globalData.textToScreenVec.push_back(Text(glm::vec2(15,15), glm::vec3(0.1,1,0.1), "File Name: " + f_o->name));
			}
		}
	});

	glutTimerFunc(globalData.fxSimulationTimerInterval, fx_simulation_timer, globalData.fxSimulationTimerId);

}
