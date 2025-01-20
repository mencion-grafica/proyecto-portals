#include "JointTransform.h"

JointTransform::JointTransform()
{
}

JointTransform::JointTransform(glm::mat4 localTransform)
{
}

JointTransform::JointTransform(glm::vec3 position, glm::quat rotation)
{
	this->position = position;
	this->rotation = rotation;
}

JointTransform::JointTransform(glm::vec3 position, glm::quat rotation, glm::mat4 localTransform)
{
	this->position = position;
	this->rotation = rotation;
	this->localTransform = localTransform;
}

JointTransform JointTransform::Interpolate(JointTransform frame1, JointTransform frame2, float progress)
{
	glm::vec3 newPosition = glm::mix(frame1.position, frame2.position, progress);
	glm::quat newRotation = glm::slerp(frame1.rotation, frame2.rotation, progress);
	newRotation = glm::normalize(newRotation);

	//glm::mat4 newLocalTransform = glm::interpolate(frame1.localTransform, frame2.localTransform, progress);

	//std::cout << progress << std::endl;

	JointTransform newJointTransform = JointTransform(newPosition, newRotation);

	return newJointTransform;
}

glm::mat4 JointTransform::GetLocalTransform()
{
	glm::mat4 adjustCoordSystem = glm::rotate(glm::mat4(1.0f), glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
	glm::vec3 newPosition = position;
	newPosition.z = -newPosition.z;
	glm::mat4 matrix = glm::mat4(1.0f);

	glm::mat4 t = glm::translate(glm::mat4(1.0), position);
	glm::mat4 r = glm::mat4_cast(rotation);
	glm::mat4 s = glm::scale(glm::mat4(1.0), glm::vec3(1.0f));

	matrix = glm::translate(matrix, position);
	matrix *= glm::mat4_cast(rotation);
	matrix = glm::scale(matrix, glm::vec3(1.0f));
	//matrix = adjustCoordSystem * matrix;


	return t * r * s;
}


