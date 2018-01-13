#ifndef SENSEFX_SIMULATION_H
#define SENSEFX_SIMULATION_H

#include "DataContainer.h"
#include "ColidableObject.h"
#include "TexturedObject.h"

#include <vector>


class SimulationCObject;

class Simulation {

	private:

	public:
		Simulation ();

		std::vector <SimulationCObject *> colisionList;

		/* Mora biti inicijalizovano */
		virtual void simulate(DataContainer * globalData)=0;

};

class SimulationCObject : public ColidableObject, public TexturedObject  {

	public:

		SimulationCObject(Simulation * simulation);

		Simulation * simulation;

		/* ??? Mozda treba da se prosledi DataContainer pokazivac */
		virtual void action()=0;

};

#endif // SENSEFX_SIMULATION_H
