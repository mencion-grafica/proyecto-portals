#pragma once

#include "Common.h"
#include "vertex.h"

typedef struct 
{
	float x, y, z;
} Point_t;

typedef struct  
{
	float a, b, c, d;
	
	Point_t point1;
	Point_t point2;
	Point_t point3;

} Plane_t;

typedef struct {
	int id;
	Plane_t plane;
} Side_t;

typedef struct {
	int id;
	std::vector<Side_t> sideList;
} Solid_t;

class Map
{
public:
	int id = 0;
	Map();
	Map(std::string filename);

	void getPlane(int id);
	void getPlane(int x1, int y1, int z1, int x2, int y2, int z2, int x3, int y3, int z3);
private:
	void leerArchivo(std::string filename);
	void leerSolid(std::ifstream f, std::string line);
	void leerSide(std::ifstream f, std::string lin);
	void printData();

	std::vector<vertex_t> vertexList;
	std::vector<int> idList;
	std::vector<Plane_t> planeList;
	std::vector<Solid_t> solidList;

};

