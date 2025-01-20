#include "Animator.h"

Animator::Animator()
{
}

Animator::Animator(Animation* animation) {
	currentAnimation = animation;
}

void Animator::StartNewAnimation(Animation* animation)
{
	currentTime = 0.0f;
	currentAnimation = animation;
	progress = 0;
}

void Animator::Update(float deltaTime, Joint* rootJoint)
{
	previousTime = currentTime;
	IncreaseAnimationTime(deltaTime);

	std::vector<glm::mat4> currentPose = CalculateCurrentPose();

	/*for (size_t i = 0; i < currentPose.size(); ++i) {
		std::cout << "Joint " << i << " Matrix:\n" << glm::to_string(currentPose[i]) << "\n";
	}*/

	ApplyPoseToJoints(currentPose, rootJoint, glm::mat4 {1.0f});
	//std::cout << deltaTime << std::endl;
}

void Animator::IncreaseAnimationTime(float deltaTime)
{
	//Sumarle deltaTime
	currentTime += deltaTime / 16.0f;
	if (currentTime > currentAnimation->GetDuration()) {
		currentTime = std::fmod(currentTime, currentAnimation->GetDuration());
	}
	//std::cout << currentTime << std::endl;
}

std::vector<Keyframe> Animator::GetPreviousAndNextFrame()
{
	std::vector<Keyframe> allFrames = this->currentAnimation->GetKeyframes();
	Keyframe previous = allFrames[0];
	Keyframe next = allFrames[0];

	if (currentTime <= allFrames[0].GetTimeStamp()) {
		next = allFrames[1];
		return { previous, next };
	}

	for (int i = 1; i < allFrames.size(); i++) {
		next = allFrames[i];
		if (next.GetTimeStamp() > currentTime) {
			break;
		}
		previous = allFrames[i];
	}

	if (currentTime >= allFrames.back().GetTimeStamp()) {
		previous = allFrames[allFrames.size() - 2];
		next = allFrames.back();
	}

	return { previous, next };
}


float Animator::CalculateKeyframeProgression(Keyframe previous, Keyframe next)
{
	float timeBetweenKeyframes = next.GetTimeStamp() - previous.GetTimeStamp();
	float currentTimeStamp = currentTime - previous.GetTimeStamp();

	//std::cout << currentTimeStamp / timeBetweenKeyframes << std::endl;

	if (timeBetweenKeyframes <= 0.0f) return 0.0f;

	return glm::clamp(currentTimeStamp / timeBetweenKeyframes, 0.0f, 1.0f);
}

std::vector<glm::mat4> Animator::InterpolatePoses(Keyframe previous, Keyframe next, float progression)
{
	std::vector<glm::mat4> currentPose;
	std::vector<JointTransform> previousJointTransforms = previous.GetPos();
	std::vector<JointTransform> nextJointTransforms = next.GetPos();

	for (int i = 0; i < previousJointTransforms.size(); i++) {
		JointTransform currentTransform = JointTransform::Interpolate(previousJointTransforms[i], nextJointTransforms[i], progression);
		glm::mat4 localTransform = currentTransform.GetLocalTransform();
		currentPose.push_back(localTransform);
	}

	return currentPose;
}

std::vector<glm::mat4> Animator::CalculateCurrentPose()
{
	std::vector<Keyframe> frames = GetPreviousAndNextFrame();
	progress = CalculateKeyframeProgression(frames[0], frames[1]);
	//if (progress > currentAnimation->GetDuration()) progress = 0;
	return InterpolatePoses(frames[0], frames[1], progress);
}

void Animator::ApplyPoseToJoints(std::vector<glm::mat4> currentPose, Joint* joint, glm::mat4 parentTransform)
{
	glm::mat4 currentLocalTransform = currentPose[joint->GetJointId()];
	glm::mat4 currentTransform = (parentTransform * currentLocalTransform);
	for (Joint& child : joint->children) {
		ApplyPoseToJoints(currentPose, &child, currentTransform);
	}
	currentTransform = currentTransform * joint->inverseBindTransform;
	joint->SetTransformMatrix(currentTransform);
}
