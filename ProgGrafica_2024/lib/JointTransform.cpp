#include "JointTransform.h"

JointTransform::JointTransform()
{
}

JointTransform::JointTransform(glm::vec4 position, glm::quat rotation)
{
	this->position = position;
	this->rotation = rotation;
}

JointTransform JointTransform::Interpolate(JointTransform frame1, JointTransform frame2, float progress)
{
	glm::vec4 newPosition = glm::mix(frame1.position, frame2.position, progress);
	glm::quat newRotation = glm::mix(frame1.rotation, frame2.rotation, progress);

	JointTransform newJointTransform = JointTransform(newPosition, newRotation);

	return newJointTransform;
}
