#include "Animation.h"

Animation::Animation()
{
}

Animation::Animation(std::string fileName)
{
	//Aqui iria toda la lectura del archivo y guardar los datos
	const char* fileNameChar = fileName.c_str();
	XMLDocument doc;

	doc.LoadFile(fileNameChar);
}

Animation::Animation(const char* fileName, Joint* rootJoint)
{
	//Aqui iria toda la lectura del archivo y guardar los datos
	XMLDocument doc;

	doc.LoadFile(fileName);

	std::vector<float> keyframesTime;
	const char* times;
	std::string auxString;

	times = doc.FirstChildElement("COLLADA")->FirstChildElement("library_animations")->FirstChildElement("animation")->FirstChildElement("source")->FirstChildElement("float_array")->GetText();

	std::stringstream ss(times);
	std::vector<std::string> stringKeyframesTime;

	//Crear cada keyframe
	while (getline(ss, auxString, ' ')) {
		stringKeyframesTime.push_back(auxString);
	}

	for (std::string str : stringKeyframesTime) {
		//cout << str << endl;
		keyframesTime.push_back(stof(str));
	}

	this->duration = keyframesTime[keyframesTime.size() - 1];

	std::vector<Keyframe> listKeyframes;
	glm::mat4 transformationMatrix;
	std::vector<JointTransform> jointTransforms;
	XMLElement* currentJointAnimation = doc.FirstChildElement("COLLADA")->FirstChildElement("library_animations")->FirstChildElement("animation");
	int index = 0;

	const char* matrixElements;
	std::vector<std::string> stringMatrixElements;
	std::vector<float> floatMatrixElements;

	std::cout << rootJoint->idCounter << std::endl;

	for (int i = 0; i < keyframesTime.size(); i++) {
		listKeyframes.push_back(Keyframe(keyframesTime[i]));
	}

	this->duration = keyframesTime[keyframesTime.size() - 1];

	for (int j = 0; j < rootJoint->idCounter; j++) {
		if (j != 0) {
			currentJointAnimation = currentJointAnimation->NextSiblingElement("animation");
		}

		matrixElements = currentJointAnimation->FirstChildElement("source")->NextSiblingElement("source")->FirstChildElement("float_array")->GetText();
		std::stringstream ss2(matrixElements);

		while (getline(ss2, auxString, ' ')) {
			stringMatrixElements.push_back(auxString);
		}

		for (std::string str : stringMatrixElements) {
			//cout << str << endl;
			floatMatrixElements.push_back(stof(str));
		}

		for (int i = 0; i < keyframesTime.size(); i++) {
			transformationMatrix = glm::mat4{ floatMatrixElements[0 + index], floatMatrixElements[1 + index], floatMatrixElements[2 + index], floatMatrixElements[3 + index], floatMatrixElements[4 + index], floatMatrixElements[5 + index], floatMatrixElements[6 + index], floatMatrixElements[7 + index], floatMatrixElements[8 + index], floatMatrixElements[9 + index], floatMatrixElements[10 + index], floatMatrixElements[11 + index], floatMatrixElements[12 + index], floatMatrixElements[13 + index], floatMatrixElements[14 + index], floatMatrixElements[15 + index] };

			glm::vec3 position = glm::vec3(transformationMatrix[0][0], transformationMatrix[0][1], transformationMatrix[0][2]);

			glm::mat3 rotationMatrix = glm::mat3(
				glm::normalize(glm::vec3(transformationMatrix[0][0], transformationMatrix[0][1], transformationMatrix[0][2])),
				glm::normalize(glm::vec3(transformationMatrix[1][0], transformationMatrix[1][1], transformationMatrix[1][2])),
				glm::normalize(glm::vec3(transformationMatrix[2][0], transformationMatrix[2][1], transformationMatrix[2][2]))
			);

			glm::quat quatRotation = glm::quat_cast(rotationMatrix);

			quatRotation = glm::normalize(quatRotation);

			//jointTransforms.push_back(JointTransform(position, rotation));

			listKeyframes[i].AddJointKeyframe(JointTransform(position, quatRotation));

			index += 16;
		}

		index = 0;

		stringMatrixElements.clear();
		floatMatrixElements.clear();
	}

	this->keyframes = listKeyframes;
	//Por cada hijo del rootJoint pasar al siguiente hermano en el archivo y cargar los datos en el keyframe
}

float Animation::GetDuration()
{
	return duration;
}

std::vector<Keyframe> Animation::GetKeyframes()
{
	return this->keyframes;
}
