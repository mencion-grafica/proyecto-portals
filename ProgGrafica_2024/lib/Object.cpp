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

Object::Object(std::string fileName, bool activeGravity)
{
	this->position = glm::vec4(0.0f, 0.0f, 0.0f, 1.0f);
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
	const float gravity = -9.8f;
	if (this->activeGravity) {
		this->velocity.y += gravity * deltaTime;
		if (this->position.y <= 0.5)
			//if (this->activeGravity) 
			this->position.y += 0 * deltaTime;
		else
			this->position.y += this->velocity.y * deltaTime;
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

void Object::createCamera()
{
	this->camera = new Camera();

}
