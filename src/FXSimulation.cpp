#include "FXSimulation.h"
#include "math_objects.h"
#include "various_objects.h"

#include <experimental/filesystem>
#include <fstream>
#include <iostream>

namespace fs = std::experimental::filesystem;

#include <map>
#include <regex>


FXSimulationCObject::FXSimulationCObject(Simulation * simulation) : SimulationCObject(simulation) { }

FXSimulation::FXSimulation(std::string dirPath) : dirPath(dirPath) {}

void FXSimulation::simulate(DataContainer * globalData) {

	DataContainer * gd = globalData;
	/* Istrazuje direktorijum i kreira dve liste
	 * regularni fajlovi i direktorijumi */
	fs::path dirPathp = fs::path(this->dirPath);
	std::vector <TransformableObject *> directories;
	std::vector <TransformableObject *> regularFiles;

	for (auto &p : fs::directory_iterator(dirPathp)) {
		fs::path filePathp = fs::u8path(p);
		std::string filePath = filePathp.u8string();

		if (fs::is_directory(p)) {
			Directory * dir = new Directory(this, filePath);
			dir->material = gd->materials["directory"];
			directories.push_back(dir);
		} else {
			RegularFile * file = new RegularFile(this, filePath);
			file->material = gd->materials["regular_file"];
			/* Ako postoji tekstura za ekstenziju formata file_ext, postavi je */
			std::string tmpTexPath = (std::string)"resources/textures/" + "file_" + file->extension + ".bmp";

		    	if (fs::exists(tmpTexPath) && file->extension.length() > 0) {
				file->texture = Texture2D(tmpTexPath);
			}
			regularFiles.push_back(file);
		}
	   }

	DataContainer newGd;

	//Axis &wcs = *new Axis; 
	//newGd.objectsToDisplay.push_back(&wcs);

	Room & room = *new Room;
	newGd.objectsToDisplay.push_back(&room);

	/* Visina sobe TODO: IZMESTITI NA FINIJE MESTO */
	float h = 3.55;

	/* N je max od fajlova i direktorijuma */
	/* Mesta za direktorijume ima: xxx za n>=3!!! 
	 * Formula za direktorijume: N = ceil(O/3)+2)
	 * Mesta za fajlove (unutra) ima U = (N-2)^2
	 * Formula za fajlove: N = ceil(sqrt(Unutra)) + 2 */

	int rfVecSize = regularFiles.size();
	int dirVecSize = directories.size();

	/* 3 je minimum za br fajlova, direktorijuma */
	/* Za njega stavi 4x4 matricu (u nju staje), a dalje
	 * racunaj po formuli */
	int n = 4;

	/* TODO: Ovaj IF srediti malo */
	if (rfVecSize > 3 || dirVecSize > 3) {

		if (rfVecSize > dirVecSize) {
			n = ceil(sqrt(rfVecSize) + 2.0);
		} else {
			n = ceil((dirVecSize/3.0)+2.0);
		}
	}

	/* Final scale coef - odredjuje razmaknutost izmedju fajlova
	 * samim tim i konacnu velicinu soba jer oni moraju tacno da stanu
	 * u sobu, i time je moguce skalirati sobu a zajedno i sve u njoj */
	glm::vec3 fscaleCoef(3.0f, 1.0f, 3.0f);
	room.setDimensions(glm::vec3(n,h,n));

	
	User & user = * new User;
	/* vec.x = Pola od room dimensions -0.5*/
	/* vec.z = -0.5*/
	user.translate(user.vecToObjectSys(glm::vec3( -((float) n/2.0f - 0.5), h/2.0f, -0.5)*fscaleCoef));
	//Axis * wcsArr = new Axis[32];

	std::vector <TransformableObject *> * tmpVec;

	for (int i=1; i<=n; i++) {
		for (int j=1; j<=n; j++) {

			/* Ako je pozicija za fajl gledaj listu sa fajlovima */
			if (i != 1 && i != n && j != 1 && j != n) {
				tmpVec = &regularFiles;
			/* Ako je pozicija za dir gledaj listu sa dir */
			}  else if (!((i == 1 && (j == 1 || j == n)) || (j == 1 && i == n) || (i == n && j == n) || i == 1)) {
				tmpVec = &directories;
			} else {
				continue;
				/* Neki treci slucaj? -- nastavi */
			}

			/* Ako lista sa f/d nije prazna */
			if (tmpVec->size()) {
					/* Uzmi fajl/direktorijum */
					TransformableObject * o = tmpVec->back();

					/* Transliraj na inicijalnu poziciju */
					o->translate(o->vecToObjectSys(glm::vec3( -((float) n/2.0f + 0.5), 0.65, +0.5))*fscaleCoef);

					/* Transliraj na sledecu slobodnu poziciju */
					o->translate(o->vecToObjectSys(glm::vec3( (float)j, 0, -(float)i)*fscaleCoef));

					/* Ubaci u listu za iscrtavanje */
					newGd.objectsToDisplay.push_back(o);
					/* Izbaci iz liste f/d */
					tmpVec->pop_back();
			}
		}

		/* Ako nema vise f/d izadji, kad prodje
		 * testing vrati ga!!*/
		//if (!(regularFiles.size() || directories.size())) 
		//	break;
	}


	room.setDimensions(glm::vec3(fscaleCoef.x*n,h,fscaleCoef.x*n));

	user.addToCheckColisionList(room.getColisionList());
	newGd.activeCamera = user.fpsViewCamera();

	newGd.objectsToKeyboard.push_back(&user);
	newGd.objectsToMouseMove.push_back(&user);
	newGd.objectsToGravity.push_back(&user);

	*globalData = newGd;
}


File::File(Simulation * simulation, std::string path) : FXSimulationCObject(simulation), path(path)  {

	const std::regex regName(".*?([^\\/]+)?(\\.*)$", std::regex_constants::icase);

	std::smatch match;

	if (std::regex_search(this->path, match, regName)) {
		this->name = match[1];
	}
}

void File::action() {}

RegularFile::RegularFile(Simulation * simulation, std::string filePath) : File(simulation, filePath) {
	/* Odredjivanje extenzije na osnovu imena */
	const std::regex extInName(".*?\\.(.+)", std::regex_constants::icase);
	std::smatch matchExt;
	if (std::regex_search(this->name, matchExt, extInName)) {
		this->extension = matchExt[1];
	}
}
Directory::Directory(Simulation * simulation, std::string filePath) : File(simulation, filePath) {}

void RegularFile::action() {}
void Directory::action() {}
