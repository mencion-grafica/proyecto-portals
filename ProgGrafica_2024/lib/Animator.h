#pragma once
#include "Common.h"
#include "Animation.h"

class Animator
{
private:
	Animation currentAnimation;
	float currentTime;
public:
	Animator();
	Animator(Animation animation);
	void StartNewAnimation(Animation animation);
	void Update(float deltaTime);
	void IncreaseAnimationTime(float deltaTime);
};

