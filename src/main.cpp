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
#include "glm/glm.hpp"
#include "glm/vec3.hpp"
#include "glm/mat4x4.hpp"
#include "glm/gtc/type_ptr.hpp"

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

#include "Texture.h"
#include "Material.h"
#include "Light.h"

/* CAMERA */
#include "Camera.h"

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

/* Pomocne funkcije */
static void pauseTimers();
static void unpauseTimers();
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
	glutFullScreen(); 

	glutInitDisplayMode(GLUT_RGBA | GLUT_DEPTH | GLUT_DOUBLE);
	glEnable(GL_DEPTH_TEST);

	float clearR = stof(appConfig.getParameter("CLEAR_COLOR_R"));
	float clearG = stof(appConfig.getParameter("CLEAR_COLOR_G"));
	float clearB = stof(appConfig.getParameter("CLEAR_COLOR_B"));
	glClearColor(clearR, clearG, clearB, 1);

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

	/* Ukljucivanje providnosti objekata */
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glEnable(GL_BLEND);

	/* Importovanje textura/modela/materijala i svetla */
	globalData.textures = Texture2D::importAll("resources/textures");
	globalData.models = Model::importAll("resources/models");
	globalData.materials = Material::importAll("resources/materials");
	globalData.lights = Light::importAll("resources/lights");

	/* Ukljucivanje svetla */
	globalData.lights["light0"].enable();
	globalData.lights["light1"].enable();
	globalData.lights["light2"].enable();
	globalData.lights["light3"].enable();
	globalData.lights["light4"].enable();
	globalData.lights["light5"].enable();
	
	std::string dirPath = argv[1];

	/* Postavljamo inicijalni direktorijum i odavde
	 * fx_timer() preuzima dalji posao oko fajl explorera !!! */

	globalData.fxCurrentDir = dirPath;
	glutTimerFunc(globalData.fxTimerInterval, fx_timer, globalData.fxTimerId);

	glutMainLoop();

	return 0;
}


/* CALLBACK-ovi */
static void on_display()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glMatrixMode(GL_TEXTURE);
	glLoadIdentity();

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	/* Postavljanje tacke gledista */
	if (globalData.activeCamera != NULL)
		glLoadMatrixf(glm::value_ptr(globalData.activeCamera->viewMatrix()));

	/* Sortiranje po daljini od user-a  Posto se iz vektora cita unazad,
	 * sortiramo u obrnutom poretku, na pocetak idu najdalji - ovo je 
	 * bitno zbog ispravne obrade providnosti objekata!! */
	std::sort (globalData.objectsToDisplay.begin(), globalData.objectsToDisplay.end(), [] (Object * a, Object * b) -> bool {

		/* TODO: Ovo da bi bilo generalnije treba da bude daljina od kamere a ne od user-a 
		 * -> Od user-a je okej ako je kamera vezana za user-a, a ako se veze za neki drugi 
		 *  objekat ili neki drugi tip kamere bude (ne First Person) ovo nece valjati */
		float aDistanceFromUser = glm::length(globalData.activeUser->pointToObjectSys(a, glm::vec3(0,0,0)));
		float bDistanceFromUser = glm::length(globalData.activeUser->pointToObjectSys(b, glm::vec3(0,0,0)));

		if (aDistanceFromUser > bDistanceFromUser)
			return true;
		else
			return false;
	});

	/* Iscrtavanje objekata */
	for_each (globalData.objectsToDisplay.begin(), globalData.objectsToDisplay.end(), [] (Object * o) {

		if(DrawableObject * d_o = dynamic_cast<DrawableObject*>(o))
			d_o->draw();

	});

	/* Tekst za ime trenutnog (exploring) direktorijum */
	Text dirName(glm::vec2(15.0, globalData.screenSize.y - 25.0), glm::vec3(1,0,0), "Exploring: " + globalData.fxCurrentDir);
	globalData.textToScreenVec.push_back(dirName);

	/* Ovde se obradjuju prosledjeni tekstovi na ekran */
	for_each (globalData.textToScreenVec.begin(), globalData.textToScreenVec.end(), [] (Text t) {

		t.print(globalData.screenSize);

	});

	/* Ispisani tekst se brise */
	globalData.textToScreenVec.clear();

	glutSwapBuffers();
}

static void on_reshape(int width, int height) 
{
	glViewport(0, 0, width, height);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	Config & appConfig = globalData.configs["application"];
	float viewMin = stof(appConfig.getParameter("VIEW_DISTANCE_MIN"));
	float viewMax = stof(appConfig.getParameter("VIEW_DISTANCE_MAX"));

	gluPerspective(60, (float) width/(float) height, viewMin, viewMax);

	/* Cuvamo velicinu ekrana jer nam treba na drugim mestima */
	globalData.screenSize = glm::vec2(width,height);
}

static void on_keyboard(unsigned char c, int x, int y)
{
	/* U switch-u se obradjuju komande koje ne uticu na
	 * pojedinacne objekte vec na ceo program */
	switch (c) {

		/* TODO: Pause key iz config-a - mozda, za sada problem constexpr */
		case 'p': 
			if (globalData.timersActive)
				pauseTimers();
			else
				unpauseTimers();

			break;

		/* 27 -> ESC_KEY */
		case 27: 
			globalData.deallocFx();
			exit(EXIT_SUCCESS);

	}
	
	/* Belezimo key koji je pritisnut tj. belezimo 
	 * u nase globalne podatke da je pritisnut i
	 * na kojoj poziciji je pritisnut (x,y)*/
	globalData.pressedKeys[c] = true;

	/* TODO: Iskoristiti za poziciju glm::vec2 */
	globalData.keyPressedPositionX = x;
	globalData.keyPressedPositionY = y;
}

static void on_keyboard_release(unsigned char c, int x, int y)
{
	/* Belezimo key koji je pusten tj. belezimo
	 * u nase globalne podatke da je pusten i
	 * na kojoj poziciji je pusten (x,y)*/
	globalData.pressedKeys[c] = false;

	/* TODO: Iskoristiti za poziciju glm::vec2 */
	globalData.keyReleasedPositionX = x;
	globalData.keyReleasedPositionY = y;
}

static void on_mouse_move(int x, int y) 
{
	/* Belezimo poziciju misa u globalne podatke */
	globalData.mousePosition = glm::vec2(x,y);
}


/* TAJMERI */ 
static void redisplay_timer(int value)
{
	if (value != globalData.redisplayTimerId || !globalData.timersActive)
		return;

	glutPostRedisplay();
	glutTimerFunc(globalData.redisplayTimerInterval, redisplay_timer, globalData.redisplayTimerId);
}

static void keyboard_timer(int value) 
{
	if (value != globalData.keyboardTimerId || !globalData.timersActive)
		return;

	/* Obradjivanje zahteva za tastatru */
	for_each (globalData.objectsToKeyboard.begin(), globalData.objectsToKeyboard.end(), [] (Object * o) {

		if(MovableObject* m_o = dynamic_cast<MovableObject*>(o))
			m_o->processKeyboardInput(globalData.pressedKeys, globalData.keyPressedPositionX, globalData.keyPressedPositionY);

	});

	glutTimerFunc(globalData.keyboardTimerInterval, keyboard_timer, globalData.keyboardTimerId);
}

static void mouse_timer(int value) 
{
	if (value != globalData.mouseTimerId || !globalData.timersActive)
		return;

	/* TODO: Ovde lepse resenje !!!! */
	GLint m_viewport[4];
	glGetIntegerv( GL_VIEWPORT, m_viewport );

	glm::vec2 center(m_viewport[2]/2, m_viewport[3]/2);
	glm::vec2 mousePosition = globalData.mousePosition;

	glm::vec2 delta = center-mousePosition;

	for_each (globalData.objectsToMouseMove.begin(), globalData.objectsToMouseMove.end(), [delta] (Object * o) {

		if(MovableObject* m_o = dynamic_cast<MovableObject*>(o)) 
			m_o->processMouseMove(delta);

	});

	glutWarpPointer( center.x , center.y );

	glutTimerFunc(globalData.mouseTimerInterval, mouse_timer, globalData.mouseTimerId);
}

static void gravity_timer(int value)
{
	if (value != globalData.gravityTimerId || !globalData.timersActive)
		return;

	/* Pad objekata kojima je pridruzena gravitacija i nisu na podu */
	for_each (globalData.objectsToGravity.begin(), globalData.objectsToGravity.end(), [] (Object * o) {

		if(MovableObject* m_o = dynamic_cast<MovableObject*>(o)) {
			float gravity = -stof(globalData.configs["gravity"].getParameter("GRAVITY"));
			m_o->addToVelocity(glm::vec3(0.0, gravity, 0.0));
			m_o->move(m_o->getVelocity());
		}

	});

	glutTimerFunc(globalData.gravityTimerInterval, gravity_timer, globalData.gravityTimerId);
}


static void animation_timer(int value) 
{
	/* Ovaj tajmer moze da se shvati kao tajmer za sve poslove file explorer simulacije,
	 * ovde je ubacen za razresavanje kolizije sa objektima, za neku drugu simulaciju 
	 * ovaj tajmer postoji samo za file explorer */

	if (value != globalData.animationTimerId || !globalData.timersActive)
		return;
	
	for_each (globalData.objectsToAnimation.begin(), globalData.objectsToAnimation.end(), [] (Object * o) {

		if(AnimatedObject* a_o = dynamic_cast<AnimatedObject*>(o)) {
			if (a_o->isAnimationOngoing())
				a_o->animate();
		}

	});

	glutTimerFunc(globalData.animationTimerInterval, animation_timer, globalData.animationTimerId);

}

static void pauseTimers() 
{
	globalData.timersActive = false;

	/* Prikazivanje kursor-a */
	glutSetCursor(GLUT_CURSOR_LEFT_ARROW);

	// TODO: Uncomment kada se pause key implementira
	//string pauseKey = globalData.configs["keyboard"].getParameter("PAUSE");
	// + Ovaj pause text da postoji neki config tipa messages, pa da se iz toga citaju poruke 
	
	Text pauseMsg(glm::vec2(globalData.screenSize.x/2.0 - 170.0, globalData.screenSize.y/2.0), glm::vec3(1,0,0), "Pause is set, to unpause press 'p' key");
	globalData.textToScreenVec.push_back(pauseMsg);


	glutPostRedisplay();
}

static void unpauseTimers() 
{
	globalData.timersActive = true;

	/* Sakrivanje kursor-a */
	glutSetCursor(GLUT_CURSOR_NONE);

	/* Tajmeri */
	glutTimerFunc(globalData.redisplayTimerInterval, redisplay_timer, globalData.redisplayTimerId);
	glutTimerFunc(globalData.mouseTimerInterval, mouse_timer, globalData.mouseTimerId);
	glutTimerFunc(globalData.keyboardTimerInterval, keyboard_timer, globalData.keyboardTimerId);
	glutTimerFunc(globalData.gravityTimerInterval, gravity_timer, globalData.gravityTimerId);
	glutTimerFunc(globalData.animationTimerInterval, animation_timer, globalData.animationTimerId);

	/* FX Simulation timer */
	glutTimerFunc(globalData.fxTimerInterval, fx_timer, globalData.fxTimerId);
}


static void fx_timer(int value)
{

	if (value != globalData.fxTimerId || !globalData.timersActive)
		return;

	/***************************************************
	*** Implementacija fajl explorera pocinje odavde ***
	***************************************************/
	/* Ovaj tajmer moze da se shvati kao tajmer za sve poslove file explorer simulacije,
	 * ovde je ubaceneno i razresavanje kolizije sa objektima, kada udjemo u zonu kolizije
	 * zapocinje se animacija objekta i ispisuje tekst (naziv fajla), kada se pritisne 
	 * taster 'e' metod action() na fajlu se poziva (npr. ulazak u sledeci direktorijum
	 * ovaj tajmer postoji samo za file explorer */

	/********************TODO*********************************
	*** O ovome treba jos dosta razmisliti i srediti ga!!! ***
	*** Da li ovako, sta i kako ...                        ***
	*********************************************************/

	/* Vazi pravilo - Obicno sto je veci komentar kod je losiji, a ovde je komentar veliki */

	/* Proverava da li je direktorijum promenjen i
	 * puni globalData (memoriju) potrebnim podacima 
	 * sto je razreseno u fx_changedir() funkciji */
	if (globalData.fxAlocatedDir != globalData.fxCurrentDir) {
		fx_changedir(globalData.fxCurrentDir);
	}

	/* Uzmemo listu svih fajlova */
	std::vector<Object* > colisionList = globalData.fxFiles;

	/* Sad gledamo da li se neki od fajlova sudara sa User-om, ako se sudara onda treba da 
	 * se rotira (tj. da se pokrene animacija i ostale potrebne stvari */
	for_each (colisionList.begin(), colisionList.end(), [] (Object * o) {

		/* TODO: Ovakve stvari (prolazak kroz sve fajlove verovatno jedu performanse 
		 * ovde treba mozda neki break - prestanak for_each na neki nacin 
		 * koji ce kod prvog isColiding=TRUE sa objektom - break-ovati petlju for_each
		 * da li je uopste moguce break-ovati for_each ?? */

		if(FileObject * f_o = dynamic_cast<FileObject*>(o)) {
			
			if (f_o->isColiding(globalData.activeUser)) {

				/* Ovde se razresavaju tzv. Akcije fajlova */
				Config & keyboardCfg = globalData.configs["keyboard"];
				char actionKey = keyboardCfg.getParameter("ACTION").c_str()[0]; 

				if (globalData.pressedKeys[ (int) actionKey ]) {
					f_o->action();
					globalData.pressedKeys[ (int) actionKey] = false;
				}

				/* TODO: Formatiranje teksta da bude iz config-a eksternog */
				/* Ispisujemo ime fajla na ekranu */

				/* Ovde uzimamo tip fajla, zatim i ime i ispisujemo tekst u donjem levom uglu ekrana */
				std::string fileType = "File";
				if (dynamic_cast<DirectoryObject*>(o)) {
					fileType = "Directory";	
				} else if(dynamic_cast<RegularFileObject*>(o)) {
					fileType = "File";
				}

				/* Stavljamo tekst u listu tekstova koji cekaju na ispis */
				globalData.textToScreenVec.push_back(Text(glm::vec2(15,15), glm::vec3(0,1,0), fileType + ": " + f_o->name));

				/* Animacija rotiranja i transliranja gore/dole */
				f_o->startAnimation();
				// f_o->startGlowing(); TODO: --> jos nije implementirano
			} else {
				/* Budemo sigurni da ni jedna vise animacija nije pustena sem
				 * ona kada se objekti sudaraju */
				f_o->endAnimation();
				// f_o->stopGlowing(); TODO: --> jos nije implementirano
			}
		}
	});

	
	glutTimerFunc(globalData.fxTimerInterval, fx_timer, globalData.fxTimerId);

}

/* Funkcija koja menja/ucitava direktorijum */
static void fx_changedir(std::string newDir) {

	/* Ukoliko postoje fajlovi simulacije je vec postojala
	 * ukloni ih! prethodne promenljive fx simulacije (dealociraj) */
	if (globalData.fxFiles.size()) 
		globalData.deallocFx();

	/* Loading screen on directory change */
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	Text loading(glm::vec2(globalData.screenSize.x/2.0 - 100.0, globalData.screenSize.y/2.0), glm::vec3(1,1,1), "Loading, please wait . . . ");
	loading.print(globalData.screenSize);	
	glutSwapBuffers();

	/* Config za sobu je */
	Config & roomCfg = globalData.configs["room"];

	/* Ucitavamo fajlove - maksimalno do MAX_OBJECTS_IN_ROOM iz room.cfg */
	std::vector <FileObject *> filesVec = FileObject::importAll(newDir, stoi(roomCfg.getParameter("MAX_OBJECTS_IN_ROOM")), &globalData);
	int filesVecSize = filesVec.size();


	/* Kreiranje sobe (objekta) */
	Room & room = * new Room;
	globalData.alocatedFxObjects.push_back(&room);
	globalData.objectsToDisplay.push_back(&room);

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
	globalData.alocatedFxObjects.push_back(&user);
	/* User-u dajemo i lampu :) */
	user.addChild(&globalData.lights["user_flashlight"]);

	/* Dodavanje pokazivaca na user-a u razne vektore
	 * koji se prosledjuju call-backovima i timer-ima */
	globalData.objectsToKeyboard.push_back(&user);
	globalData.objectsToMouseMove.push_back(&user);
	globalData.objectsToGravity.push_back(&user);
	
	/* Dodavanje sobe u user-ov colision list */
	user.addToCheckColisionList(room.getColisionList());

	/* Aktivni user je nas napravljeni user */
	globalData.activeUser = &user;

	/* Aktivna kamera je kamera naseg user-a */
	globalData.activeCamera = user.fpsViewCamera();

	/* Transliramo user-a na njegovu pocetnu poziciju */
	user.translate(user.pointToObjectSys(glm::vec3(0,h/2.0,-0.5)*fscaleCoef));

	/* Inicijalna pozicija (pocetak sobe) - Mesto od kog se zapocinje translacija fajlova po podu */
	glm::vec3 initialPosition = glm::vec3(-(float)n/2.0 - 0.5, files_h, -0.5) * fscaleCoef;

	/* Parent Directory je na kraju vektora, prvo njega dodajemo */
	FileObject * parentDir = filesVec.back();
	parentDir->translate(parentDir->pointToObjectSys(glm::vec3(0,files_h,-0.5)*fscaleCoef));

	/* Dodavanje u potrebne liste */
	globalData.objectsToDisplay.push_back(parentDir);
	globalData.objectsToAnimation.push_back(parentDir);
	globalData.fxFiles.push_back(parentDir);
	globalData.alocatedFxObjects.push_back(parentDir);

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
				globalData.objectsToDisplay.push_back(o);

				/* Ubaci u listu za animacije */
				globalData.objectsToAnimation.push_back(o);

				/* Ubaci u listu fajlova */
				globalData.fxFiles.push_back(o);

				/* Ubacujemo u listu alociranih objekata */
				/* Alocirani su funkcijom File::ImportAll() */
				globalData.alocatedFxObjects.push_back(o);

				/* Izbacujemo element iz vektora fajlova */
				filesVec.pop_back();

			} else {
				/* Break ako nema vise elemenata */
				break;
			}
		}
	}

	/* Alocirani fajlovi odgovaraju trenutnom direktorijumu */
	globalData.fxAlocatedDir = globalData.fxCurrentDir;
}
