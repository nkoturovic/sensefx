#ifndef SENSEFX_COLIDABLE_H
#define SENSEFX_COLIDABLE_H

#include "object.h"

class colidable : public virtual object {

	protected: 
		bool isOnGround = false;
		std::vector <object*> checkColisionList;

	public:
		bool isColiding(object * obj);
		void addToCheckColisionList(object * o);
		void removeFromCheckColisionList(object * o);
};

#endif
