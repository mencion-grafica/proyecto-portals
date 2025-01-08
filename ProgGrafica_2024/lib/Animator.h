#pragma once
#include "Common.h"
#include "Animation.h"
#include "Object.h"

class Animator
{
private:
	Animation* currentAnimation;
	float currentTime;
	float previousTime;
	float progress;
public:
	Animator();
	Animator(Animation* animation);
	void StartNewAnimation(Animation* animation);
	void Update(float deltaTime, Joint* rootJoint);
	void IncreaseAnimationTime(float deltaTime);
	std::vector<Keyframe> GetPreviousAndNextFrame();
	float CalculateKeyframeProgression(Keyframe previous, Keyframe next);
	std::vector<glm::mat4> InterpolatePoses(Keyframe previous, Keyframe next, float progression);
	std::vector<glm::mat4> CalculateCurrentPose();
	void ApplyPoseToJoints(std::vector<glm::mat4> currentPose, Joint* joint, glm::mat4 parentTransform);
};

