#include "Player.h"

const int MAX_WEIGHTS = 4;

Player::Player()
{
}

Player::Player(std::string fileName)
{
	this->loadDaeFile(fileName.c_str());

	updateModelMatrix();
}

Player::Player(std::string fileName, glm::vec4 pos) {
	this->loadDaeFile(fileName.c_str());

	updateModelMatrix();
}

void Player::loadDaeFile(const char* fileName) {
	XMLDocument doc;

	doc.LoadFile(fileName);
	const char* title;
	const XMLAttribute* idText = doc.FirstChildElement("COLLADA")->FirstChildElement("library_geometries")->FirstChildElement("geometry")->FindAttribute("id");

	title = idText->Value();

	printf("Mesh: %s \n", title);

	title = doc.FirstChildElement("COLLADA")->FirstChildElement("library_geometries")->FirstChildElement("geometry")->FirstChildElement("mesh")->FirstChildElement("source")->FirstChildElement("float_array")->GetText();

	std::stringstream ss(title);
	std::vector<std::string> stringPositions;
	std::vector<std::string> stringColors;
	std::vector<std::string> stringNormals;
	std::vector<float> floatPositions;
	std::vector<float> floatNormals;
	std::vector<float> floatColors;

	std::string auxString;
	vertex_t vert;

	//Posiciones de los vertices
	while (getline(ss, auxString, ' ')) {
		stringPositions.push_back(auxString);
	}

	for (std::string str : stringPositions) {
		//cout << str << endl;
		floatPositions.push_back(stof(str));
	}

	const char* colors;
	colors = doc.FirstChildElement("COLLADA")->FirstChildElement("library_geometries")->FirstChildElement("geometry")->FirstChildElement("mesh")->FirstChildElement("source")->NextSibling()->NextSibling()->NextSibling()->FirstChildElement("float_array")->GetText();

	//Color de cada vertice
	std::stringstream ss3(colors);
	while (getline(ss3, auxString, ' ')) {
		stringColors.push_back(auxString);
	}

	for (std::string str : stringColors) {
		//cout << str << endl;
		floatColors.push_back(stof(str));
	}

	const char* normals;
	normals = doc.FirstChildElement("COLLADA")->FirstChildElement("library_geometries")->FirstChildElement("geometry")->FirstChildElement("mesh")->FirstChildElement("source")->NextSibling()->FirstChildElement("float_array")->GetText();

	//Color de cada vertice
	std::stringstream ss7(normals);
	while (getline(ss7, auxString, ' ')) {
		stringNormals.push_back(auxString);
	}

	for (std::string str : stringNormals) {
		//cout << str << endl;
		floatNormals.push_back(stof(str));
	}

	const XMLAttribute* count = doc.FirstChildElement("COLLADA")->FirstChildElement("library_geometries")->FirstChildElement("geometry")->FirstChildElement("mesh")->FirstChildElement("source")->FirstChildElement("technique_common")->FirstChildElement("accessor")->FindAttribute("count");
	glm::vec4 pos;
	glm::vec4 color;
	int numVertex = count->IntValue();

	//Joints
	this->rootJoint = new Joint(fileName);

	const char* numJointsList;
	std::vector<std::string> stringNumJoints;
	std::vector<float> intNumJoints;

	numJointsList = doc.FirstChildElement("COLLADA")->FirstChildElement("library_controllers")->FirstChildElement("controller")->FirstChildElement("skin")->FirstChildElement("vertex_weights")->FirstChildElement("vcount")->GetText();
	std::stringstream ss4(numJointsList);

	while (getline(ss4, auxString, ' ')) {
		stringNumJoints.push_back(auxString);
	}

	for (std::string str : stringNumJoints) {
		intNumJoints.push_back(stoi(str));
	}

	const char* listIdJointsWeights;
	std::vector<std::string> stringlistIdJointsWeights;
	std::vector<float> intlistIdJointsWeights;

	listIdJointsWeights = doc.FirstChildElement("COLLADA")->FirstChildElement("library_controllers")->FirstChildElement("controller")->FirstChildElement("skin")->FirstChildElement("vertex_weights")->FirstChildElement("v")->GetText();
	std::stringstream ss5(listIdJointsWeights);

	while (getline(ss5, auxString, ' ')) {
		stringlistIdJointsWeights.push_back(auxString);
	}

	for (std::string str : stringlistIdJointsWeights) {
		intlistIdJointsWeights.push_back(stoi(str));
	}

	const char* listWeights;
	std::vector<std::string> stringlistWeights;
	std::vector<float> floatlistWeights;

	listWeights = doc.FirstChildElement("COLLADA")->FirstChildElement("library_controllers")->FirstChildElement("controller")->FirstChildElement("skin")->FirstChildElement("source")->NextSiblingElement("source")->NextSiblingElement("source")->FirstChildElement("float_array")->GetText();
	std::stringstream ss6(listWeights);

	while (getline(ss6, auxString, ' ')) {
		stringlistWeights.push_back(auxString);
	}

	for (std::string str : stringlistWeights) {
		floatlistWeights.push_back(stof(str));
	}

	int counterNum = 0;

	glm::mat4 adjustCoordSystem = glm::rotate(glm::mat4(1.0f), glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0f));

	//Por cada vértice en el .dae, que eso lo sacamos del count del accesor
	for (int i = 0; i < numVertex; i++) {
		pos.x = floatPositions.at(0);
		floatPositions.erase(floatPositions.begin());
		pos.y = floatPositions.at(0);
		floatPositions.erase(floatPositions.begin());
		pos.z = floatPositions.at(0);
		floatPositions.erase(floatPositions.begin());
		pos.w = 1.0f;

		glm::vec4 adjustedPos = adjustCoordSystem * glm::vec4(pos.x, pos.y, pos.z, 1.0f);

		/*pos.x = adjustedPos.x;
		pos.y = adjustedPos.y;
		pos.z = adjustedPos.z;*/

		vert.vertexPos = pos;

		//TODO: Revisar para que coja los colores de las caras y repetir vertices que tengan varios colores
		color.r = floatColors.at(0);
		floatColors.erase(floatColors.begin());
		color.g = floatColors.at(0);
		floatColors.erase(floatColors.begin());
		color.b = floatColors.at(0);
		floatColors.erase(floatColors.begin());
		color.a = 1.0f;

		vert.vertexColor = color;

		//Asignar pesos y joints por vértice

		glm::ivec4 joints = glm::ivec4{ -1, -1, -1, -1 };
		vert.idJoints = joints;
		glm::vec4 weights = glm::vec4{ 0.0f, 0.0f, 0.0f, 0.0f };
		vert.weightJoints = weights;

		int savedJoints = 0;
		if (intNumJoints.at(i) > 3) {
			savedJoints = 4;
		}
		else {
			savedJoints = intNumJoints.at(i);
		}

		for (int j = 0; j < savedJoints; j++) {
			vert.idJoints[j] = intlistIdJointsWeights[counterNum];
			counterNum++;
			vert.weightJoints[j] = floatlistWeights[intlistIdJointsWeights[counterNum]];
			counterNum++;
		}

		//vert.weightJoints = glm::normalize(vert.weightJoints);

		vertexList.push_back(vert);
	}

	const char* trianglesData = doc.FirstChildElement("COLLADA")->FirstChildElement("library_geometries")->FirstChildElement("geometry")->FirstChildElement("mesh")->FirstChildElement("polylist")->FirstChildElement("p")->GetText();
	std::stringstream ss2(trianglesData);
	std::vector<std::string> stringVertex;
	while (getline(ss2, auxString, ' ')) {
		stringVertex.push_back(auxString);
	}

	//Vertices por cara
	const XMLAttribute* numTrianglesAtr = doc.FirstChildElement("COLLADA")->FirstChildElement("library_geometries")->FirstChildElement("geometry")->FirstChildElement("mesh")->FirstChildElement("polylist")->FindAttribute("count");
	int numTriangles = numTrianglesAtr->IntValue();

	for (int i = 0; i < numTriangles * 3; i++) {
		int id = stoi(stringVertex[i * 4]);
		int idNormal = stoi(stringVertex[(i * 4) + 1]);
		glm::vec4 normalVert{ floatNormals[idNormal * 3], floatNormals[(idNormal * 3) + 1], floatNormals[(idNormal * 3) + 2], 0.0f };
		vertexList[id].vertexNormal = normalVert;
		//cout << id << endl;
		idList.push_back(id);
	}

	glm::mat4 empty(1.0f);
	empty *= adjustCoordSystem;

	//this->rootJoint->CalcInverseBindTransform(empty);

	XMLElement* matrixList = doc.FirstChildElement("COLLADA")->FirstChildElement("library_controllers")->FirstChildElement("controller")->FirstChildElement("skin")->FirstChildElement("bind_shape_matrix")->NextSiblingElement("source")->NextSibling()->FirstChildElement("float_array");

	this->rootJoint->GetInverseBindTransform(empty, matrixList);

	jointCount = rootJoint->GetIdCounter();

	//Textura
	//TODO: Faltan las UVs
	/*const char* textureName = doc.FirstChildElement("COLLADA")->FirstChildElement("library_images")->FirstChildElement("image")->FirstChildElement("init_from")->GetText();
	std::string stringTexture = textureName;
	std::string route = "data/" + stringTexture;
	this->texture = new Texture(route);*/

	isAnimated = true;

	for (int i = 0; i < vertexList.size(); i++) {
		this->originalPositions.push_back(vertexList[i].vertexPos);
	}

	Animation* animation = new Animation(fileName, rootJoint);

	this->animator = Animator(animation);
	StartNewAnimation(animation);

	//Aplicar shader
	prg->addShader("data/playerShader.vert");
	prg->addShader("data/shader.frag");

	prg->link();
	//printData(*this);
	//this->tex = nullptr;
}

void Player::move(double deltaTime)
{
	updateModelMatrix();
	animator.Update(deltaTime, this->rootJoint);
	//UpdateVertex();
}

void Player::StartNewAnimation(Animation* animation)
{
	animator.StartNewAnimation(animation);
}

std::vector<glm::mat4> Player::GetJointTransforms()
{
	std::vector<glm::mat4> jointMatrices;

	AddJointsToList(*rootJoint, jointMatrices);

	return jointMatrices;
}

void Player::AddJointsToList(Joint joint, std::vector<glm::mat4>& list)
{
	list.push_back(joint.GetTransformationMatrix());

	//std::cout << "TransformationMatrix obtenida de Joint " << joint.id << " Matrix:\n" << glm::to_string(list.back()) << "\n";

	for (Joint child : joint.children) {
		AddJointsToList(child, list);
	}
}

int Player::GetJointCount()
{
	return jointCount;
}


