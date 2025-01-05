#include "Keyframe.h"

Keyframe::Keyframe()
{
}

Keyframe::Keyframe(float timeStamp)
{
	this->timeStamp = timeStamp;
}

Keyframe::Keyframe(float timeStamp, std::vector<JointTransform> pos)
{
	this->timeStamp = timeStamp;
	this->pos = pos;
}

void Keyframe::SetTimeStamp(float timeStamp)
{
	this->timeStamp = timeStamp;
}

void Keyframe::AddJointKeyframe(JointTransform newPos)
{
	this->pos.push_back(newPos);
}

float Keyframe::GetTimeStamp()
{
	return timeStamp;
}

std::vector<JointTransform> Keyframe::GetPos()
{
	return pos;
}

void Keyframe::SetJointKeyframes(std::vector<JointTransform> pos)
{
	this->pos = pos;
}

