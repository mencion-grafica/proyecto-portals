#pragma once

#include "Common.h"
#include "vertex.h"

class Map
{
public:
	Map();
	Map(std::string filename);

private:
	void leerArchivo(std::string filename);
	void printData();

	std::vector<vertex_t> vertexList;
	std::vector<int> idList;
};