#pragma once
#include "Common.h"
#include "Keyframe.h"

class Animation
{
private:
	float duration;
	std::vector<Keyframe> keyframes;

public:
	Animation();
	Animation(std::string fileName);
	float GetDuration();
};

