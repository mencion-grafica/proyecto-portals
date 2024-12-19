#include "Animator.h"

Animator::Animator()
{
}

Animator::Animator(Animation animation) {
	currentAnimation = animation;
}

void Animator::StartNewAnimation(Animation animation)
{
	currentTime = 0.0f;
	currentAnimation = animation;
}

void Animator::Update(float deltaTime)
{
	IncreaseAnimationTime(deltaTime);
	//std::cout << deltaTime << std::endl;
}

void Animator::IncreaseAnimationTime(float deltaTime)
{
	//Sumarle deltaTime
	currentTime += deltaTime;
	if (currentTime > currentAnimation.GetDuration()) {
		currentTime = currentTime - currentAnimation.GetDuration();
	}
}
