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
	glm::vec3 center;
	std::vector<Side_t> sideList;
	std::vector<glm::vec3> vertexList;
} Solid_t;

typedef struct {
	int id;
	std::vector<Solid_t> solidList;
}World_t;

class Map
{
public:
	int id;
	Map();
	Map(std::string filename);
	World_t worldMap;

private:

	void leerArchivo(std::string filename);
	World_t leerWorld(std::ifstream &f, std::string line);
	Solid_t leerSolid(std::ifstream &f, std::string line);
	Side_t leerSide(std::ifstream &f, std::string line);
	Plane_t leerPlane(std::string line);
	void getPlane(Solid_t& solidList);
	void pushVertices(Solid_t& solidList);
	glm::vec3 getVertices(Plane_t p1, Plane_t p2, Plane_t p3);
	glm::vec3 getCenter(Solid_t& solid);


	void writeTRSFile(std::string filenam);

	std::vector<vertex_t> vertexList;
	std::vector<int> idList;
};

