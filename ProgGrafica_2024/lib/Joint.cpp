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

	localBindTransform = glm::mat4(floatElements[0], floatElements[1], floatElements[2], floatElements[3]
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

	this->localBindTransform = glm::mat4(floatElements[0], floatElements[1], floatElements[2], floatElements[3]
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

int Joint::GetIdCounter()
{
	return idCounter;
}

void Joint::CalcInverseBindTransform(glm::mat4 parentTransform) {
	// Rotación de ajuste para alinear sistemas de coordenadas
	glm::mat4 adjustCoordSystem = glm::rotate(glm::mat4(1.0f), glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0f));

	glm::mat4 globalTransform = parentTransform * adjustCoordSystem * this->localBindTransform;

	//std::cout << "Joint " << id << " Matrix:\n" << glm::to_string(globalTransform) << "\n";

	this->inverseBindTransform = glm::inverse(globalTransform);

	for (Joint& child : this->children) {
		child.CalcInverseBindTransform(globalTransform);
	}
}


void Joint::SetTransformMatrix(glm::mat4 matrix)
{
	this->transformationMatrix = matrix;
	//std::cout << "SetTransformMatrix de Joint " << id << " Matrix:\n" << glm::to_string(this->transformationMatrix) << "\n";
}

glm::mat4 Joint::GetTransformationMatrix()
{
	//std::cout << "GetTransformMatrix de Joint " << id << " Matrix:\n" << glm::to_string(this->transformationMatrix) << "\n";
	return this->transformationMatrix;
}

Joint Joint::GetJointById(int id)
{
	if (children.size() != 0) {
		for (int i = 0; i < children.size(); i++) {
			if (children[i].id == id) {
				return children[i];
			}
			else {
				children[i].GetJointById(id);
			}
		}
	}
	else return NULL;
}

int Joint::GetJointId()
{
	return this->id;
}



