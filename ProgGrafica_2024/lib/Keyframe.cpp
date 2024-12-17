#include "Keyframe.h"

Keyframe::Keyframe()
{
}

Keyframe::Keyframe(float timeStamp, std::map<int, JointTransform> jointKeyframes)
{
	this->timeStamp = timeStamp;
	this->pos = jointKeyframes;
}
