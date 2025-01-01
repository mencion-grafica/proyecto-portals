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

Animation::Animation(const char* fileName, Joint rootJoint)
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

	std::vector<Keyframe> listKeyframes;
	std::vector<glm::mat4> transformationMatrixs;
	std::vector<JointTransform> jointTransforms;
	XMLElement* currentJointAnimation = doc.FirstChildElement("COLLADA")->FirstChildElement("library_animations")->FirstChildElement("animation");
	int index = 0;

	const char* matrixElements;
	std::vector<std::string> stringMatrixElements;
	std::vector<float> floatMatrixElements;

	std::cout << rootJoint.idCounter << std::endl;

	for (int j = 0; j < rootJoint.idCounter; j++) {
		if (j != 0) {
			currentJointAnimation = currentJointAnimation->NextSiblingElement("animation");
		}

		matrixElements = currentJointAnimation->FirstChildElement("source")->NextSiblingElement("source")->FirstChildElement("float_array")->GetText();
		std::stringstream ss2(matrixElements);
		std::cout << j << std::endl;

		while (getline(ss2, auxString, ' ')) {
			stringMatrixElements.push_back(auxString);
		}

		for (std::string str : stringMatrixElements) {
			//cout << str << endl;
			floatMatrixElements.push_back(stof(str));
		}

		for (int i = 0; i < keyframesTime.size(); i++) {
			listKeyframes.push_back(Keyframe(keyframesTime[i]));

			transformationMatrixs.push_back(glm::mat4{ floatMatrixElements[0 + index], floatMatrixElements[1 + index], floatMatrixElements[2 + index], floatMatrixElements[3 + index], floatMatrixElements[4 + index], floatMatrixElements[5 + index], floatMatrixElements[6 + index], floatMatrixElements[7 + index], floatMatrixElements[8 + index], floatMatrixElements[9 + index], floatMatrixElements[10 + index], floatMatrixElements[11 + index], floatMatrixElements[12 + index], floatMatrixElements[13 + index], floatMatrixElements[14 + index], floatMatrixElements[15 + index] });

			glm::vec4 position = { floatMatrixElements[3 + index], floatMatrixElements[7 + index], floatMatrixElements[11 + index], floatMatrixElements[15 + index] };
			glm::mat3 rotation = glm::mat3(transformationMatrixs[i]);
			glm::quat quatRotation = glm::quat(rotation);

			jointTransforms.push_back(JointTransform(position, rotation));

			index += 16;
		}
	}
	//Por cada hijo del rootJoint pasar al siguiente hermano en el archivo y cargar los datos en el keyframe
}

float Animation::GetDuration()
{
	return duration;
}
