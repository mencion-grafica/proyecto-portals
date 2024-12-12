#include "Animator.h"

Animator::Animator()
{
}

Animator::Animator(Animation animation) {
	currentAnimation = animation;
}

void Animator::StartNewAnimation(Animation animation)
{
	currentTime = 0;
	currentAnimation = animation;
}
