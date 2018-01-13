#ifndef SENSEFX_COLIDABLE_OBJECT_H
#define SENSEFX_COLIDABLE_OBJECT_H

#include "Object.h"

class ColidableObject : public virtual Object {

	protected: 
		bool isOnGround = false;
		std::vector <Object*> checkColisionList;

	public:
		bool isColiding(Object * obj);
		void addToCheckColisionList(Object * o);
		void addToCheckColisionList(std::vector <Object *> objects);
		void removeFromCheckColisionList(Object * o);
};

#endif
