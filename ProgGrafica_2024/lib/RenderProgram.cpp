#include "RenderProgram.h"

Shader::Shader(std::string fileName)
{
	this->fileName = fileName;

	if (fileName.ends_with(".vert"))
	{
		shaderType = GL_VERTEX_SHADER;
	}
	else if (fileName.ends_with(".frag"))
	{
		shaderType = GL_FRAGMENT_SHADER;
	}

	readFile(fileName);
	compile();
	getErrors();
}

void Shader::readFile(std::string fileName)
{
	std::ifstream t(fileName);

	if (t.is_open())
	{
		std::string str((std::istreambuf_iterator<char>(t)), std::istreambuf_iterator<char>());
		src = str;
	}
	else
	{
		std::cout << "Unable to open file " << fileName << std::endl;
	}
}

void Shader::compile()
{
	shaderID = glCreateShader(shaderType);
	const char* source = src.c_str();
	glShaderSource(shaderID, 1, &source, nullptr);
	glCompileShader(shaderID);
}

void Shader::getErrors()
{
	GLint retCode;
	char errorLog[1024];
	GLint fragment_compiled;
	glGetShaderiv(this->shaderID, GL_COMPILE_STATUS, &fragment_compiled);

	if (fragment_compiled != GL_TRUE)
	{
		GLsizei log_length = 0;
		GLchar message[1024];
		glGetShaderInfoLog(this->shaderID, 1024, &log_length, message);
		std::cout << "ERROR " << fileName << "\n" << message << "\n\n";
	}
}

void Program::addShader(std::string fileName) 
{
	shaderList[fileName] = new Shader(fileName);
}

void Program::link() 
{
	programID = glCreateProgram();

	for (auto& s : shaderList) 
	{
		glAttachShader(programID, s.second->shaderID);
	}

	glLinkProgram(programID);
	getErrors();
	readVarList();
}

void Program::getErrors() 
{
	GLint program_linked;
	glGetProgramiv(this->programID, GL_LINK_STATUS, &program_linked);

	if (program_linked != GL_TRUE) 
	{
		GLsizei log_length = 0;
		GLchar message[1024];
		glGetProgramInfoLog(this->programID, 1024, &log_length, message);
		std::cout << "ERROR: " << message << "\n\n";
	}
}

void Program::readVarList() 
{
	int count = 0;

	glGetProgramiv(programID, GL_ACTIVE_ATTRIBUTES, &count);
	std::cout << "Active Attributes: " << count << std::endl;
	
	for (int i = 0; i < count; i++) 
	{
		GLuint idx = i;
		GLsizei buffSize, length;
		GLint size;
		GLenum type;
		char name[30];
		buffSize = 30;
		unsigned int location = 0;

		glGetActiveAttrib(programID, (GLuint)idx, buffSize, &length, &size, &type, name);
		location = glGetAttribLocation(programID, name);

		std::cout << "Attribute: " << name << " Location: " << location << std::endl;
		varList[name] = location;
	}

	glGetProgramiv(programID, GL_ACTIVE_UNIFORMS, &count);
	for (int i = 0; i < count; i++) 
	{
		GLuint idx = i;
		GLsizei buffSize, length;
		GLint size;
		GLenum type;
		char name[30];
		buffSize = 30;
		unsigned int location = 0;

		glGetActiveUniform(programID, (GLuint) idx, buffSize, &length, &size, &type, name);
		location = glGetUniformLocation(programID, name);

		std::cout << "Uniform: " << name << " Location: " << location << std::endl;
		varList[name] = location;
	}
}

void Program::setMVP(glm::mat4 mvp) 
{
	if (varList.find("MVP") != varList.end()) 
	{
		glUniformMatrix4fv(varList["MVP"], 1, GL_FALSE, &mvp[0][0]);
	}
	else 
	{
		std::cout << "ERROR: " << __FILE__ << ":" << __LINE__ << " " << __FUNCTION__ << std::endl;
	}
}

void Program::setTransformsArray(glm::mat4* transforms)
{
	if (varList.find("jointTransforms[0]") != varList.end()) {
		//glUniformMatrix4fv(varList["jointTransforms"], sizeof(transforms), GL_FALSE, transforms);
	}
}

void Program::setMatrix(std::string name, glm::mat4 m)
{
	if (varList.find(name) != varList.end()) 
	{
		glUniformMatrix4fv(varList[name], 1, GL_FALSE, &m[0][0]);
	}
	else 
	{
		//std::cout << "ERROR: " << __FILE__ << ":" << __LINE__ << " " << __FUNCTION__ << ". Trying to set " << name << std::endl;
	}
}

void Program::setInteger(std::string name, int data) 
{
	if (varList.find(name) != varList.end()) glUniform1i(varList[name], data);
}

void Program::setFloat(std::string name, float data) 
{
	if (varList.find(name) != varList.end()) glUniform1f(varList[name], data);
}

void Program::setVec4(std::string name, glm::vec4 data)
{
	if (varList.find(name) != varList.end()) glUniform4f(varList[name], data.x, data.y, data.z, data.w);
}

void Program::setVertexAttribute(std::string name, int ncomp, int type, size_t stride, void* offset) 
{
	if (varList.find(name) != varList.end()) 
	{
		unsigned int location = varList[name];
		glEnableVertexAttribArray(location);
		glVertexAttribPointer(location, ncomp, type, GL_FALSE, stride, offset);
	}
	else 
	{
		//std::cout << "ERROR: " << __FILE__ << ":" << __LINE__ << " " << __FUNCTION__ << std::endl;
	}
}

void Program::use() 
{
	glUseProgram(programID);
}