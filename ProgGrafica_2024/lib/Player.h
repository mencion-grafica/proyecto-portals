#pragma once

#include "Common.h"
#include "Object.h"
#include "Animator.h"
//Provisional
#include "Animation.h"

class Player : public Object
{
private:
	Joint rootJoint;
	int jointCount;
	Animator animator;

public:
	Player();

	Player(std::string fileName);

	Player(std::string fileName, glm::vec4 pos);

	void loadDaeFile(const char* fileName);

	void move(double deltaTime) override;

	void StartNewAnimation(Animation animation);

	std::vector<glm::mat4> GetJointTransforms();

	void AddJointsToList(Joint joint, std::vector<glm::mat4>& list);

	int GetJointCount();
};