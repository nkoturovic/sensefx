#include <iostream>
#include <map>
#include <algorithm>

/* OBRADA GRESAKA */
#include "ast.h"

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

/* RAD sa direktorijumom */
#include <experimental/filesystem>
namespace fs = std::experimental::filesystem;

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
static void animation_timer(int value);
static void fx_timer(int value);
static void fx_changedir(std::string newDir);


int main(int argc, char * argv[])
{
	/* Argumenti - inicijalna provera */
	string usage = "Usage: " + (string) argv[0] + " dirPath";
	ast(argc == 2, usage.c_str());

	globalData.configs = Config::importAll("configs", "DEFAULT");
	Config & appConfig = globalData.configs["application"];

	glutInit(&argc, argv);
	int dflWidth = std::stoi(appConfig.getParameter("WIN_WIDTH"));
	int dflHeight = std::stoi(appConfig.getParameter("WIN_HEIGHT"));

	glutInitWindowSize(dflWidth, dflHeight);
	glutInitWindowPosition(0, 0);

	const char * title = appConfig.getParameter("TITLE").c_str();
	glutCreateWindow(title);

	glutInitDisplayMode(GLUT_RGBA | GLUT_DEPTH | GLUT_DOUBLE);
	glEnable(GL_DEPTH_TEST);

	/* Callback-ovi */
	glutDisplayFunc(on_display);
	glutReshapeFunc(on_reshape);
	glutKeyboardFunc(on_keyboard);
	glutKeyboardUpFunc(on_keyboard_release);
	glutMotionFunc(on_mouse_move);
	glutPassiveMotionFunc(on_mouse_move);
	glutSetCursor(GLUT_CURSOR_NONE);

	/* Tajmeri */
	glutTimerFunc(globalData.redisplayTimerInterval, redisplay_timer, globalData.redisplayTimerId);
	glutTimerFunc(globalData.mouseTimerInterval, mouse_timer, globalData.mouseTimerId);
	glutTimerFunc(globalData.keyboardTimerInterval, keyboard_timer, globalData.keyboardTimerId);
	glutTimerFunc(globalData.gravityTimerInterval, gravity_timer, globalData.gravityTimerId);
	glutTimerFunc(globalData.animationTimerInterval, animation_timer, globalData.animationTimerId);

	/* Normalizacija normala */
	glEnable(GL_NORMALIZE);

	/* Uklucivanje osvetljenja */
	glEnable(GL_LIGHTING);

	/* Ukljucivanje tekstura */
	glEnable(GL_TEXTURE_2D);
	// glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);

	/* Ukljucivanje providnosti objekata */
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	//glBlendFunc (GL_ONE, GL_ONE_MINUS_SRC_ALPHA); 


	glEnable(GL_BLEND);

	/* Deljeni globalni podaci programa */
	DataContainer &gd = globalData;

	/* Importovanje textura/modela/materijala i svetla */
	gd.textures = Texture2D::importAll("resources/textures");
	gd.models = Model::importAll("resources/models");
	gd.materials = Material::importAll("resources/materials");
	gd.lights = Light::importAll("resources/lights");

	/* Ukljucivanje svetla */
	gd.lights["light0"].enable();
	gd.lights["light1"].enable();
	gd.lights["light2"].enable();
	gd.lights["light3"].enable();
	gd.lights["light4"].enable();
	gd.lights["light5"].enable();
	
	std::string dirPath = argv[1];
	/* Obezbedjujemo da se ne zavrsava sa '/' 
	 * jer je to invalid format */
	//if (dirPath[dirPath.size() - 1] == '/')
	//	dirPath[dirPath.size() - 1] = '\0';

	/* Postavljamo inicijalni direktorijum i odavde
	 * fx_timer() preuzima dalji posao oko fajlova !!! */
	globalData.fxCurrentDir = dirPath;
	glutTimerFunc(globalData.fxTimerInterval, fx_timer, globalData.fxTimerId);

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
	if (globalData.activeCamera != NULL)
		glLoadMatrixf(glm::value_ptr(globalData.activeCamera->viewMatrix()));

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
	for_each (objectsToDisplay.begin(), objectsToDisplay.end(), [] (Object * o) {
		if(DrawableObject * d_o = dynamic_cast<DrawableObject*>(o))
			d_o->draw();
	});

	/* Ovde se obradjuju prosledjeni tekstovi na ekran */
	/* Tekst za trenutni direktorijum */
	globalData.textToScreenVec.push_back(Text(glm::vec2(15.0f, globalData.screenSize.y - 25.0f), glm::vec3(1,0,0), "Exploring: " + globalData.fxCurrentDir));

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
		case 27: 
			globalData.deallocFx();
			exit(EXIT_SUCCESS);
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


static void animation_timer(int value) {

	/* Ovaj tajmer moze da se shvati kao tajmer za sve poslove file explorer simulacije,
	 * ovde je ubacen za razresavanje kolizije sa objektima, za neku drugu simulaciju 
	 * ovaj tajmer postoji samo za file explorer */

	if (value != globalData.animationTimerId)
		return;
	
	std::vector<Object* > &toAnimation = globalData.objectsToAnimation;
	for_each (toAnimation.begin(), toAnimation.end(), [] (Object * o) {
		if(AnimatedObject* a_o = dynamic_cast<AnimatedObject*>(o)) {
			if (a_o->isAnimationOngoing())
				a_o->animate();
		}
	});

	glutTimerFunc(globalData.animationTimerInterval, animation_timer, globalData.animationTimerId);

}

static void fx_timer(int value) {

	if (value != globalData.fxTimerId)
		return;

	/***************************************************
	*** Implementacija fajl explorera pocinje odavde ***
	***************************************************/
	/* Ovaj tajmer moze da se shvati kao tajmer za sve poslove file explorer simulacije,
	 * ovde je ubacen za razresavanje kolizije sa objektima, za neku drugu simulaciju 
	 * ovaj tajmer postoji samo za file explorer */

	DataContainer &gd = globalData;

	/* Proverava da li je direktorijum promenjen i
	 * puni globalData (memoriju) potrebnim podacima */
	if (gd.fxAlocatedDir != gd.fxCurrentDir) {
		fx_changedir(gd.fxCurrentDir);
	}


	User * activeUser = globalData.activeUser;
	std::vector<Object* > colisionList = globalData.fxFiles;

	for_each (colisionList.begin(), colisionList.end(), [activeUser] (Object * o) {


		if(FileObject * f_o = dynamic_cast<FileObject*>(o)) {
			
			if (f_o->isColiding(activeUser)) {

				/* Ovde se razresavaju tzv. Akcije fajlova */
				if (globalData.pressedKeys[(int)'e']) {
					f_o->action();
					globalData.pressedKeys[(int)'e'] = false;
				}

				/* TODO: Formatiranje teksta da bude iz config-a eksternog */
				/* Ispisujemo ime fajla na ekranu */

				std::string fileType = "File";
				if (dynamic_cast<DirectoryObject*>(o)) {
					fileType = "Directory";	
				} else if(dynamic_cast<RegularFileObject*>(o)) {
					fileType = "File";
				}

				globalData.textToScreenVec.push_back(Text(glm::vec2(15,15), glm::vec3(0,1,0), fileType + ": " + f_o->name));

				f_o->startAnimation();
				f_o->startGlowing();
			} else {
				/* Budemo sigurni da ni jedna vise animacija nije pustena sem
				 * ona kada se objekti sudaraju */
				f_o->endAnimation();
				f_o->stopGlowing();
			}
		}
	});

	
	glutTimerFunc(globalData.fxTimerInterval, fx_timer, globalData.fxTimerId);

}

/* Funkcija koja menja/ucitava direktorijum */
static void fx_changedir(std::string newDir) {

	DataContainer &gd = globalData;
	/* Ukoliko postoje fajlovi simulacije je vec postojala
	 * ukloni ih! prethodne promenljive simulacije (dealociraj) */
	if (gd.fxFiles.size()) 
		gd.deallocFx();

	/* Loading screen on directory change */
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	Text loading(glm::vec2(globalData.screenSize.x/2.0f - 100, globalData.screenSize.y/2.0f), glm::vec3(1,1,1), "Loading, please wait . . . ");
	loading.print(globalData.screenSize);	
	glutSwapBuffers();

	/* Config za sobu je */
	Config & roomCfg = gd.configs["room"];

	std::vector <FileObject *> filesVec = FileObject::importAll(newDir, stoi(roomCfg.getParameter("MAX_OBJECTS_IN_ROOM")), &globalData);
	int filesVecSize = filesVec.size();


	/* Kreiranje sobe (objekta) */
	Room & room = * new Room;
	gd.alocatedFxObjects.push_back(&room);
	gd.objectsToDisplay.push_back(&room);

	/* Postavljanmo velicinu matrice (poda) */
	int n = stoi(roomCfg.getParameter("FLOOR_MATRIX_MIN_SIZE"));
	if ((filesVecSize) > n*n)
		n = ceil(sqrt(filesVecSize));

	/* Floor scale coef - odredjuje razmaknutost izmedju fajlova
	 * samim tim i konacnu velicinu soba jer oni moraju tacno da stanu
	 * u sobu, i time je moguce skalirati sobu a zajedno i sve u njoj */
	glm::vec3 fscaleCoef(1.0,1.0,1.0);
	fscaleCoef.x = stof(roomCfg.getParameter("FLOOR_SCALE_X"));
	fscaleCoef.z = stof(roomCfg.getParameter("FLOOR_SCALE_Z"));
	float h = stof(roomCfg.getParameter("ROOM_HEIGHT"));
	float files_h = stof(roomCfg.getParameter("FILES_HEIGHT"));

	/* Postavljanje velicine sobe u odnosu na broj fajlova i floor scaleCoef */
	room.setDimensions(glm::vec3(fscaleCoef.x*n,h,fscaleCoef.z*n));

	/* Pravljenje User objekta */
	User& user = * new User;
	gd.alocatedFxObjects.push_back(&user);
	/* User-u dajemo i lampu :) */
	user.addChild(&globalData.lights["user_flashlight"]);

	/* Dodavanje pokazivaca na user-a u razne vektore
	 * koji se prosledjuju call-backovima i timer-ima */
	gd.objectsToKeyboard.push_back(&user);
	gd.objectsToMouseMove.push_back(&user);
	gd.objectsToGravity.push_back(&user);
	
	/* Dodavanje sobe u user-ov colision list */
	user.addToCheckColisionList(room.getColisionList());

	/* Aktivni user je nas napravljeni user */
	gd.activeUser = &user;

	/* Aktivna kamera je kamera naseg user-a */
	gd.activeCamera = user.fpsViewCamera();

	/* Transliramo user-a na njegovu pocetnu poziciju */
	user.translate(user.pointToObjectSys(glm::vec3(0,h/2.0,-0.5)*fscaleCoef));

	/* Inicijalna pozicija (pocetak sobe) - Mesto od kog se zapocinje translacija fajlova po podu */
	glm::vec3 initialPosition = glm::vec3(-(float)n/2.0f - 0.5, files_h, -0.5) * fscaleCoef;

	/* Parent Directory je na kraju vektora, prvo njega dodajemo */
	FileObject * parentDir = filesVec.back();
	parentDir->translate(parentDir->pointToObjectSys(glm::vec3(0,files_h,-0.5)*fscaleCoef));

	/* Dodavanje u potrebne liste */
	gd.objectsToDisplay.push_back(parentDir);
	gd.objectsToAnimation.push_back(parentDir);
	gd.fxFiles.push_back(parentDir);
	gd.alocatedFxObjects.push_back(parentDir);

	filesVec.pop_back();

	/* Prolazimio kroz "matricu" poda i dodajemo fajlove na odg. poziciju */
	for (int i=1; i<=n; i++) {
		for (int j=1; j<=n; j++) {
			/* Ako nismo stigli do kraja vektora fajlova */
			if (filesVec.size()) {

				/* Uzmi fajl/direktorijum iz vektora fajlova */
				TransformableObject * o = filesVec.back();

				/* Transliraj na inicijalnu poziciju */
				o->translate(o->vecToObjectSys(initialPosition));

				/* Transliraj na sledecu slobodnu poziciju (dokle smo stigli u matrici poda) */
				o->translate(o->vecToObjectSys(glm::vec3( (float)j, 0, -(float)i)*fscaleCoef));

				/* Ubaci u listu za iscrtavanje */
				gd.objectsToDisplay.push_back(o);

				/* Ubaci u listu za animacije */
				gd.objectsToAnimation.push_back(o);

				/* Ubaci u listu fajlova */
				gd.fxFiles.push_back(o);

				/* Ubacujemo u listu alociranih objekata */
				/* Alocirani su funkcijom File::ImportAll() */
				gd.alocatedFxObjects.push_back(o);

				/* Izbacujemo element iz vektora fajlova */
				filesVec.pop_back();

			} else {
				/* Break ako nema vise elemenata */
				break;
			}
		}
	}

	/* Alocirani fajlovi odgovaraju trenutnom direktorijumu */
	gd.fxAlocatedDir = gd.fxCurrentDir;
}
