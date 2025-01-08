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
	float plus = 0.002;
	currentTime += deltaTime;
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

	/*for (int i = 0; i < allFrames.size(); i++) {
		if (allFrames[i].GetTimeStamp() > currentTime) {
			next = allFrames[i];
			break;
		}
	}*/

	for (int i = 1; i < allFrames.size(); i++) {
		next = allFrames[i];
		if (next.GetTimeStamp() > currentTime) {
			break;
		}
		previous = allFrames[i];
	}

	/*for (int i = 0; i < allFrames.size(); i++) {
		if (allFrames[i].GetTimeStamp() >= previousTime) {
			previous = allFrames[i];
			break;
		}
	}*/

	std::vector<Keyframe> adjacentKeyframes;
	adjacentKeyframes.push_back(previous);
	adjacentKeyframes.push_back(next);

	return adjacentKeyframes;
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
