#pragma once
#include "Common.h"

class JointTransform
{
private:
	glm::vec3 position;
	glm::quat rotation;
	glm::mat4 localTransform;

public:
	JointTransform();
	JointTransform(glm::mat4 localTransform);
	JointTransform(glm::vec3 position, glm::quat rotation);
	JointTransform(glm::vec3 position, glm::quat rotation, glm::mat4 localTransform);
	static JointTransform Interpolate(JointTransform frame1, JointTransform frame2, float progress);
	glm::mat4 GetLocalTransform();
};

