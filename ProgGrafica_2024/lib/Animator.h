#pragma once
#include "Common.h"
#include "Animation.h"
#include "Object.h"

class Animator
{
private:
	Player model;
	Animation currentAnimation;
	float currentTime;
public:
	Animator();
	Animator(Player model);
	Animator(Animation animation);
	void StartNewAnimation(Animation animation);
	void Update(float deltaTime);
	void IncreaseAnimationTime(float deltaTime);
};

