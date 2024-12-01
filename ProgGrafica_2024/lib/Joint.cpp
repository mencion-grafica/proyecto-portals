#include "Joint.h"

Joint::Joint() {

}

Joint::Joint(const char* fileName) {
	id = 0;
	idCounter++;

	XMLDocument doc;

	doc.LoadFile(fileName);

	XMLElement* rootJoint = doc.FirstChildElement("COLLADA")->FirstChildElement("library_visual_scenes")->FirstChildElement("visual_scene")->FirstChildElement("node")->NextSiblingElement("node")->FirstChildElement("node");

	std::vector<std::string> strMatrixElements;
	std::vector<float> floatElements;

	const char* matrixElements = rootJoint->FirstChildElement("matrix")->GetText();
	std::stringstream ss(matrixElements);
	std::string auxString;

	while (getline(ss, auxString, ' ')) {
		strMatrixElements.push_back(auxString);
	}

	for (std::string str : strMatrixElements) {
		//cout << str << endl;
		floatElements.push_back(stof(str));
	}

	transformationMatrix = glm::mat4(floatElements[0], floatElements[1], floatElements[2], floatElements[3]
									, floatElements[4], floatElements[5], floatElements[6], floatElements[7]
									, floatElements[8], floatElements[9], floatElements[10], floatElements[11]
									, floatElements[12], floatElements[13], floatElements[14], floatElements[15]);

	if (rootJoint->FirstChildElement("node") != NULL) {
		XMLElement* currentJoint = rootJoint->FirstChildElement("node");

		children.push_back(Joint(idCounter, currentJoint));

		//std::cout << children.size() << std::endl;

		while (currentJoint->NextSiblingElement("node") != NULL) {
			currentJoint = currentJoint->NextSiblingElement("node");

			children.push_back(Joint(idCounter, currentJoint));

			//std::cout << children.size() << std::endl;
		}
	}
}

Joint::Joint(int id, XMLElement* joint) {
	this->id = id;
	idCounter++;

	std::vector<std::string> strMatrixElements;
	std::vector<float> floatElements;

	const char* matrixElements = joint->FirstChildElement("matrix")->GetText();
	std::stringstream ss(matrixElements);
	std::string auxString;

	while (getline(ss, auxString, ' ')) {
		strMatrixElements.push_back(auxString);
	}

	for (std::string str : strMatrixElements) {
		//cout << str << endl;
		floatElements.push_back(stof(str));
	}

	transformationMatrix = glm::mat4(floatElements[0], floatElements[1], floatElements[2], floatElements[3]
									, floatElements[4], floatElements[5], floatElements[6], floatElements[7]
									, floatElements[8], floatElements[9], floatElements[10], floatElements[11]
									, floatElements[12], floatElements[13], floatElements[14], floatElements[15]);

	if (joint->FirstChildElement("node") != NULL) {
		XMLElement* currentJoint = joint->FirstChildElement("node");

		children.push_back(Joint(idCounter, currentJoint));

		//std::cout << children.size() << std::endl;

		while (currentJoint->NextSiblingElement("node") != NULL) {
			currentJoint = currentJoint->NextSiblingElement("node");

			this->children.push_back(Joint(idCounter, currentJoint));

			//std::cout << this->children.size() << std::endl;
		}
	}
}

//Buscar por id en arbol (por comodidad desde el rootJoint)
Joint* Joint::SearchById(int id)
{
	return NULL;
}


