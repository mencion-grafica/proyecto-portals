#pragma once
#include "Common.h"
#include "JointTransform.h"

class Keyframe
{
private:
	float timeStamp;
	//Map de id y JointTransform
	//Estoy pensando en quitar el int porque es el id que podría ser el index en un vector de JointTransforms
	std::map <int, JointTransform> pos;
public:
	Keyframe();
	Keyframe(float timeStamp);
	Keyframe(float timeStamp, std::map <int, JointTransform> jointKeyframes);
};

