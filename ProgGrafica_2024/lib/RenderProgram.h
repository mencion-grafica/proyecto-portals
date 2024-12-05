#pragma once

#include "Common.h"

class Shader
{
public:
	std::string fileName;
	std::string src;
	int shaderID;
	unsigned int shaderType;

	Shader(std::string fileName);
	void readFile(std::string fileName);
	void compile();
	void getErrors();
};

class Program
{
public:
	GLuint programID = -1;
	std::map<std::string, Shader*> shaderList;
	std::map<std::string, unsigned int> varList;

	Program() {};
	void addShader(std::string fileName);
	void link();
	void getErrors();
	void readVarList();

	void setMVP(glm::mat4 mvp);
	void setVertexAttribute(std::string name, int ncomp, int type, size_t stride, void* offset);
	void use();
};