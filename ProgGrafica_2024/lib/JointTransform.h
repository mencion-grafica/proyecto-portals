#pragma once
#include "Common.h"

//Realmente necesito esta clase? Luego lo miro con calma
class JointTransform
{
private:
	glm::vec4 position;
	glm::quat rotation;

public:
	JointTransform();
	JointTransform(glm::vec4 position, glm::quat rotation);
	JointTransform Interpolate(JointTransform frame1, JointTransform frame2, float progress);
};

