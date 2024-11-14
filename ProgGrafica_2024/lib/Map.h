#pragma once
#include "Common.h"

class Map
{
public:
	Map map();
	Map map(string filename);



private:
	void leerArchivo(string filename);
	void printData();

	std::vector<vertex_t> vertexList;
	std::vector<int> idList;


};

