#include "JointTransform.h"

JointTransform::JointTransform()
{
}

JointTransform::JointTransform(glm::vec3 position, glm::quat rotation)
{
	this->position = position;
	this->rotation = rotation;
}

JointTransform JointTransform::Interpolate(JointTransform frame1, JointTransform frame2, float progress)
{
	glm::vec3 newPosition = glm::mix(frame1.position, frame2.position, progress);
	glm::quat newRotation = glm::slerp(frame1.rotation, frame2.rotation, progress);

	JointTransform newJointTransform = JointTransform(newPosition, newRotation);

	return newJointTransform;
}

glm::mat4 JointTransform::GetLocalTransform()
{
	glm::mat4 matrix = glm::mat4(1.0f);
	glm::translate(matrix, position);
	matrix *= glm::mat4_cast(rotation);

	return matrix;
}


