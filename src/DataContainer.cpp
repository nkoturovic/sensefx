#include "DataContainer.h"
#include "various_objects.h"
#include <algorithm>

void DataContainer::deallocFx() {

	for_each( fxFiles.begin(), fxFiles.end(), [](Object * o) { 

		if (RegularFileObject * rf_o = dynamic_cast<RegularFileObject*>(o))
			delete rf_o;

		else if (DirectoryObject * d_o = dynamic_cast<DirectoryObject*>(o))
			delete d_o;	

		else if (User * u_o = dynamic_cast<User *>(o))
			delete u_o;

		else if (Room * r_o = dynamic_cast<Room *>(o))
			delete r_o;

	});

	fxFiles.clear();
	objectsToDisplay.clear();
	objectsToGravity.clear();
	objectsToMouseMove.clear();
	objectsToKeyboard.clear();
	objectsToAnimation.clear();
}
