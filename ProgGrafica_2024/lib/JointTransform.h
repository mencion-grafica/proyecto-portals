#pragma once
#include "Common.h"

class JointTransform
{
private:
	glm::vec3 position;
	glm::quat rotation;

public:
	JointTransform();
	JointTransform(glm::vec3 position, glm::quat rotation);
	static JointTransform Interpolate(JointTransform frame1, JointTransform frame2, float progress);
	glm::mat4 GetLocalTransform();
};

