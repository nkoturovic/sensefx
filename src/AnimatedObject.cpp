#include "AnimatedObject.h"

void AnimatedObject::startAnimation() {
	this->animationOngoing = true;
}

void AnimatedObject::endAnimation() {
	this->animationOngoing = false;
}

bool AnimatedObject::isAnimationOngoing() {
	return this->animationOngoing;
}
