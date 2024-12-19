#include "Player.h"

Player::Player()
{
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
	std::vector<float> floatPositions;
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

	const XMLAttribute* count = doc.FirstChildElement("COLLADA")->FirstChildElement("library_geometries")->FirstChildElement("geometry")->FirstChildElement("mesh")->FirstChildElement("source")->FirstChildElement("technique_common")->FirstChildElement("accessor")->FindAttribute("count");
	glm::vec4 pos;
	glm::vec4 color;
	int numVertex = count->IntValue();

	//Joints
	rootJoint = Joint(fileName);

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

	int counterNum = 0;

	//Por cada vértice en el .dae, que eso lo sacamos del count del accesor
	for (int i = 0; i < numVertex; i++) {
		pos.x = floatPositions.at(0);
		floatPositions.erase(floatPositions.begin());
		pos.y = floatPositions.at(0);
		floatPositions.erase(floatPositions.begin());
		pos.z = floatPositions.at(0);
		floatPositions.erase(floatPositions.begin());
		pos.w = 1.0f;

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
		glm::vec4 weights = glm::vec4{ -1.0f, -1.0f, -1.0f, -1.0f };
		vert.weightJoints = weights;

		int savedJoints = 0;
		if (intNumJoints.at(i) > 3) {
			savedJoints = 4;
		}
		else {
			savedJoints = intNumJoints.at(i);
		}

		//joints[0]
		for (int j = 0; j < savedJoints; j++) {
			vert.idJoints[j] = intlistIdJointsWeights[counterNum];
			counterNum++;
			vert.weightJoints[j] = intlistIdJointsWeights[counterNum];
			counterNum++;
		}
		//Por cada joint que afecte al vertice
			//vert.idJoints[j] = valorQueSea
			//vert.weightJoints[j] = buscar el peso en la lista de pesos

		vertexList.push_back(vert);
	}

	const char* trianglesData = doc.FirstChildElement("COLLADA")->FirstChildElement("library_geometries")->FirstChildElement("geometry")->FirstChildElement("mesh")->FirstChildElement("triangles")->FirstChildElement("p")->GetText();
	std::stringstream ss2(trianglesData);
	std::vector<std::string> stringVertex;
	while (getline(ss2, auxString, ' ')) {
		stringVertex.push_back(auxString);
	}

	//Vertices por cara
	const XMLAttribute* numTrianglesAtr = doc.FirstChildElement("COLLADA")->FirstChildElement("library_geometries")->FirstChildElement("geometry")->FirstChildElement("mesh")->FirstChildElement("triangles")->FindAttribute("count");
	int numTriangles = numTrianglesAtr->IntValue();

	for (int i = 0; i < numTriangles * 3; i++) {
		int id = stoi(stringVertex[i * 4]);
		//cout << id << endl;
		idList.push_back(id);
	}

	glm::mat4 empty(1.0f);

	rootJoint.CalcInverseBindTransform(empty);

	jointCount = rootJoint.GetIdCounter();
	//Textura
	//TODO: Faltan las UVs
	/*const char* textureName = doc.FirstChildElement("COLLADA")->FirstChildElement("library_images")->FirstChildElement("image")->FirstChildElement("init_from")->GetText();
	std::string stringTexture = textureName;
	std::string route = "data/" + stringTexture;
	this->texture = new Texture(route);*/

	//Aplicar shader
	prg->addShader("data/playerShader.vert");
	prg->addShader("data/shader.frag");

	prg->link();
	//printData(*this);
	//this->tex = nullptr;
}

void Player::move(double deltaTime)
{
	Object::move(deltaTime);
	animator.Update(deltaTime);
}

void Player::StartNewAnimation(Animation animation)
{
	animator.StartNewAnimation(animation);
}