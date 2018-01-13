#ifndef SENSEFX_FXSIMULATION_H
#define SENSEFX_FXSIMULATION_H

#include "Simulation.h"
#include "DataContainer.h"

#include <string>
#include <vector>

class FXSimulation : public Simulation {

	private:
		std::string dirPath;

	public:
		FXSimulation(std::string dirPath);
		void simulate(DataContainer * globalData);

};


class FXSimulationCObject : public SimulationCObject  {

	public:
		FXSimulationCObject(Simulation * simulation);

		/* ??? Mozda treba da se prosledi DataContainer pokazivac */
		virtual void action()=0;

};


class File : public FXSimulationCObject {
	
	public:
		std::string path;
		std::string name;

		File(Simulation * simulation, std::string path);
		
		/* Default File action */
		virtual void action();
};


class Directory : public File {
	public:
		Directory(Simulation * simulation, std::string Path);

	/* Directory action, resetuje FXSimulaciju sa drugim direktorijumom */
	void action();

};


class RegularFile : public File {
	public:
		std::string extension;
		RegularFile(Simulation * simulation, std::string Path);

		void action();
};

#endif // SENSEFX_FXSIMULATION_H
