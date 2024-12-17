#pragma once
#include "Common.h"
#include "JointTransform.h"

class Keyframe
{
private:
	float timeStamp;
	//Map de id y JointTransform
	std::map <int, JointTransform> pos;
public:
	Keyframe();
	Keyframe(float timeStamp, std::map <int, JointTransform> jointKeyframes);
};

