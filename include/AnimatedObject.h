#ifndef SENSEFX_ANIMATED_OBJECT_H
#define SENSEFX_ANIMATED_OBJECT_H

class AnimatedObject {

	protected:
		float animationParameter=0.0;
		bool animationOngoing = false;

	public:

		virtual void animate()=0;
		void startAnimation();
		void endAnimation();
		bool isAnimationOngoing();
};

#endif //ANIMATED_OBJECT
