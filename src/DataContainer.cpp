#include "DataContainer.h"
#include <algorithm>

DataContainer& DataContainer::operator=(DataContainer rhs) {

		std::vector <Object *> deleted;
		deleted.clear();
		deleted.resize(0);

		for_each(this->objectsToDisplay.begin(), this->objectsToDisplay.end(), [&deleted](Object * o) {
				deleted.push_back(o);
				delete o;
		});

		this->objectsToDisplay.clear();
		this->objectsToDisplay.resize(0);

		for_each(this->objectsToKeyboard.begin(), this->objectsToKeyboard.end(), [&deleted](Object * o) {

			if(std::find(deleted.begin(), deleted.end(), o) != deleted.end()) {
			    /* v sadrzi o */
			} else {
			    /* v ne sadrzi o x */
				deleted.push_back(o);
				delete o;
			}

		});

		this->objectsToKeyboard.clear();
		this->objectsToKeyboard.resize(0);

		for_each(this->objectsToMouseMove.begin(), this->objectsToMouseMove.end(), [&deleted](Object * o) {

			if(std::find(deleted.begin(), deleted.end(), o) != deleted.end()) {
			    /* v sadrzi o */
			} else {
			    /* v ne sadrzi o x */
				deleted.push_back(o);
				delete o;
			}
		});
	
		this->objectsToMouseMove.clear();
		this->objectsToMouseMove.resize(0);

		for_each(this->objectsToGravity.begin(), this->objectsToGravity.end(), [&deleted](Object * o) {

			if(std::find(deleted.begin(), deleted.end(), o) != deleted.end()) {
			    /* v sadrzi o */
			} else {
			    /* v ne sadrzi o x */
				deleted.push_back(o);
				delete o;
			}
		});

		this->objectsToGravity.clear();
		this->objectsToGravity.resize(0);


		this->objectsToDisplay = rhs.objectsToDisplay;
		this->objectsToKeyboard = rhs.objectsToKeyboard;
		this->objectsToMouseMove = rhs.objectsToMouseMove;
		this->objectsToGravity = rhs.objectsToGravity;
		this->activeCamera = rhs.activeCamera;

		return *this;
}
