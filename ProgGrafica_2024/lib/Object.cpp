#include "Object.h"

void printData(Object obj) 
{
	std::cout << "Object ID: " << obj.id << std::endl;

	for (int i = 0; i < obj.vertexList.size(); i++) 
	{
		std::cout << "Vertex " << i << ": " << obj.vertexList[i].vertexPos.x << " " << obj.vertexList[i].vertexPos.y << " " << obj.vertexList[i].vertexPos.z << std::endl;
	}

	for (int i = 0; i < obj.vertexList.size(); i++) 
	{
		std::cout << "Color " << i << ": " << obj.vertexList[i].vertexColor.r << " " << obj.vertexList[i].vertexColor.g << " " << obj.vertexList[i].vertexColor.b << std::endl;
	}

	for (int i = 0; i < obj.idList.size(); i++) 
	{
		std::cout << "ID " << i << ": " << obj.idList[i] << std::endl;
	}
}

Object::Object()
{
	this->id = this->idCounter++;

	this->position = glm::vec4(0.0f, 0.0f, 0.0f, 1.0f);
	this->rotation = glm::vec4(0.0f, 0.0f, 0.0f, 1.0f);
	this->scale = glm::vec4(1.0f);
	this->modelMatrix = glm::mat4(1.0f);
}

Object::Object(std::string fileName)
{
	this->position = glm::vec4(0.0f, 0.0f, 0.0f, 1.0f);
	this->rotation = glm::vec4(0.0f, 0.0f, 0.0f, 1.0f);
	this->scale = glm::vec4(1.0f);
	this->modelMatrix = glm::mat4(1.0f);
	this->activeGravity = true;

	std::cout << "Leyendo desde fichero\n";

	id = idCounter++;
	std::string line;

	std::ifstream f(fileName, std::ios_base::in);

	if (!f.is_open())
	{
		std::cout << "ERROR Fichero " << fileName << " no encontrado\n";
		return;
	}

	bool existsNormal = false;
	while (std::getline(f, line, '\n'))
	{
		std::istringstream str(line);
		std::string key;

		str >> key;

		if (key[0] != '#')
		{
			if (key == "vert")
			{
				glm::vec4 pos;
				str >> pos.x >> pos.y >> pos.z;
				pos.w = 1.0f;
				vertex_t vert;
				vert.vertexPos = pos;
				vertexList.push_back(vert);
			}
			else if (key == "color")
			{
				vertex_t& v = vertexList.back();
				str >> v.vertexColor.r >> v.vertexColor.g >> v.vertexColor.b >> v.vertexColor.a;
			}
			else if (key == "face")
			{
				for (int i = 0; i < 3; i++)
				{
					int f = 0;
					str >> f;
					idList.push_back(f);
				}
			}
			else if (key == "svert" || key == "sfrag")
			{
				std::string fileName;
				str >> fileName;
				prg->addShader(fileName);
			}
			else if (key == "normal")
			{
				vertex_t& v = vertexList.back();
				str >> v.vertexNormal.x >> v.vertexNormal.y >> v.vertexNormal.z >> v.vertexNormal.w;
				existsNormal = true;
			}
			else if (key == "uv")
			{
				vertex_t& v = vertexList.back();
				str >> v.vertexUv.x >> v.vertexUv.y;
			}
			/*
			else if (key == "texture")
			{
				std::string fileName;
				str >> fileName;
				std::cout << "Reading texture: " << fileName << std::endl;
				this->texture = new Texture(fileName);
			}
			*/
		}
	}

	prg->link();

	printData(*this);
}

Object::Object(std::string fileName, bool activeGravity)
{
	this->position = glm::vec4(0.0f, 0.0f, 0.0f, 10.0f);
	this->rotation = glm::vec4(0.0f, 0.0f, 0.0f, 1.0f);
	this->scale = glm::vec4(1.0f);
	this->modelMatrix = glm::mat4(1.0f);
	this->activeGravity = activeGravity;

	std::cout << "Leyendo desde fichero\n";

	id = idCounter++;
	std::string line;

	std::ifstream f(fileName, std::ios_base::in);

	if (!f.is_open()) 
	{
		std::cout << "ERROR Fichero " << fileName << " no encontrado\n";
		return;
	}

	bool existsNormal = false;
	while (std::getline(f, line, '\n')) 
	{
		std::istringstream str(line);
		std::string key;

		str >> key;

		if (key[0] != '#') 
		{
			if (key == "vert") 
			{
				glm::vec4 pos;
				str >> pos.x >> pos.y >> pos.z;
				pos.w = 1.0f;
				vertex_t vert;
				vert.vertexPos = pos;
				vertexList.push_back(vert);
			}
			else if (key == "color") 
			{
				vertex_t& v = vertexList.back();
				str >> v.vertexColor.r >> v.vertexColor.g >> v.vertexColor.b >> v.vertexColor.a;
			}
			else if (key == "face") 
			{
				for (int i = 0; i < 3; i++) 
				{
					int f = 0;
					str >> f;
					idList.push_back(f);
				}
			}
			else if (key == "svert" || key == "sfrag")
			{
				std::string fileName;
				str >> fileName;
				prg->addShader(fileName);
			}
			else if (key == "normal") 
			{
				vertex_t& v = vertexList.back();
				str >> v.vertexNormal.x >> v.vertexNormal.y >> v.vertexNormal.z >> v.vertexNormal.w;
				existsNormal = true;
			}
			else if (key == "uv")
			{
				vertex_t& v = vertexList.back();
				str >> v.vertexUv.x >> v.vertexUv.y;
			}
			/*
			else if (key == "texture")
			{
				std::string fileName;
				str >> fileName;
				std::cout << "Reading texture: " << fileName << std::endl;
				this->texture = new Texture(fileName);
			}
			*/
		}
	}

	prg->link();

	printData(*this);
}

void Object::createTriangle()
{
	this->position = glm::vec4(0.0f, 0.0f, 0.0f, 1.0f);
	this->rotation = glm::vec4(0.0f, 0.0f, 0.0f, 1.0f);
	this->scale = glm::vec4(1.0f);

	this->vertexList = 
	{
		{
			{0, 0.5f, 0, 1}, // Position
			{0, 0, 1, 0}, // Normal
			{1, 0, 0, 1} // Color
		},
		{
			{-0.5f, -0.5f, 0, 1}, // Position
			{0, 0, 1, 0}, // Normal
			{0, 1, 0, 1} // Color
		},
		{
			{0.5f, -0.5f, 0, 1}, // Position
			{0, 0, 1, 0}, // Normal
			{0, 0, 1, 1} // Color
		}
	};

	this->idList = { 1, 0, 2 };
	this->modelMatrix = glm::mat4(1.0f);

	this->prg->link();
}

void Object::move(double deltaTime)
{

	if (activeGravity == true) {
		this->velocity.y += gravityForce * deltaTime;
		this->position += this->velocity * glm::vec4(deltaTime, deltaTime, deltaTime, deltaTime);

		if (this->position.y < 0) {
			this->position.y = 0;
			this->velocity.y = 0;
		}
	}

	updateModelMatrix();
}

void Object::updateModelMatrix()
{
	glm::mat4 translate = glm::translate(glm::mat4(1.0f), glm::vec3(position));
	glm::mat4 rotate = glm::rotate(glm::mat4(1.0f), rotation.y, glm::vec3(0.0f, 1.0f, 0.0f));
	glm::mat4 scale = glm::scale(glm::mat4(1.0f), glm::vec3(this->scale));

	this->modelMatrix = translate * rotate * scale;
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

	//Por cada v�rtice en el .dae, que eso lo sacamos del count del accesor
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

		//std::cout << pos.x << " " << pos.y << " " << pos.z << std::endl;
		//std::cout << color.r << " " << color.g << " " << color.b << std::endl;
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

	//Joints
	rootJoint = Joint(fileName);

	//Aplicar shader
	prg->addShader("data/shader.vert");
	prg->addShader("data/shader.frag");

	prg->link();
	//printData(*this);
	//this->tex = nullptr;
}
