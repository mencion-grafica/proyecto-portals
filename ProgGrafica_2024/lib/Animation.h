#pragma once
#include "Common.h"
#include "Keyframe.h"
#include "Joint.h"
#include "tinyxml2.h"

using namespace tinyxml2;

class Animation
{
private:
	float duration;
	std::vector<Keyframe> keyframes;

public:
	Animation();
	Animation(std::string fileName);
	Animation(const char* fileName, Joint rootJoint);
	float GetDuration();
	std::vector<Keyframe> GetKeyframes();
};

