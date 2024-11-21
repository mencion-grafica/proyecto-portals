#include "Map.h"

Map::Map()
{

}

Map::Map(std::string filename) {
	id++;
	leerArchivo(filename);
}

void Map::leerArchivo(std::string filename) {
	std::cout << "Leyendo desde fichero: " + filename + "\n";
	std::ifstream f(filename, std::ios_base::in);

	if (!f.is_open()) {
		std::cout << "ERROR Fichero " << filename << " no encontrado\n";
		return;
	}

	std::string line;
	Solid_t currentSolid;
	Side_t currentSide;
	Plane_t currentPlane;



	while (std::getline(f, line, '\n'))
	{
		if (key == "solid") {

			std::cout << "Estas en el solid" << std::endl;
			if (key == "\"id\"") {
				std::cout << "Leyendo ID..." << std::endl;
				str >> currentSolid.id;
				std::cout << "Estas en el solid con ID:" << currentSolid.id << std::endl;
			}
			else if (key == "side") {
				std::cout << "Estas en el side" << std::endl;
				if (key == "\"id\"") {
					std::cout << "Leyendo ID..." << std::endl;
					str >> currentSide.id;
					std::cout << "Estas en el side con ID:" << currentSide.id << std::endl;
				}
				else if (key == "\"plane\"") {

					printf("Estoy dentro del plano");

				}
			}
		}
	}
}

void Map::getPlane(int x1, int y1, int z1,int x2, int y2, int z2,int x3, int y3, int z3) {
	float v1x = x2 - x1;
	float v1y = y2 - y1;
	float v1z = z2 - z1;

	std::cout << "Vector 1: " << v1x << "x " << v1y << "y " << v1z << "z " << std::endl;


	float v2x = x3 - x1;
	float v2y = y3 - y1;
	float v2z = z3 - z1;
	std::cout << "Vector 2: " << v2x << "x " << v2y << "y " << v2z << "z " << std::endl;


	float a = v1y * v2z - v1z * v2y;
	float b = v1z * v2x - v1x * v2z;
	float c = v1x * v2y - v1y * v2x;
	float d = -(a * x1 + b * y1 + c * z1);

	std::cout << "Plano: " << a <<"x "<< b <<"y " << c << "z " << d << std::endl;

}

void Map::getPlane(int id) {
	float v1x = planeList[id].point2.x - planeList[id].point1.x;
	float v1y = planeList[id].point2.y - planeList[id].point1.y;
	float v1z = planeList[id].point2.z - planeList[id].point1.z;

	float v2x = planeList[id].point3.x - planeList[id].point1.x;
	float v2y = planeList[id].point3.y - planeList[id].point1.y;
	float v2z = planeList[id].point3.z - planeList[id].point1.z;

	planeList[id].a = v1y * v2z - v1z * v2y;
	planeList[id].b = v1z * v2x - v1x * v2z;
	planeList[id].c = v1x * v2y - v1y * v2x;
	planeList[id].d = -(planeList[id].a * planeList[id].point1.x + planeList[id].b * planeList[id].point1.y + planeList[id].c * planeList[id].point1.z);
}

void Map::leerSolid(std::ifstream f, std:: string line) {
	line.erase(0, line.find_first_not_of(" \t"));

	std::istringstream str(line);
	std::string key;
	str >> key;

	do{

	} while (std::getline(f, line, '\n'));

}

void Map::leerSide(std::ifstream f, std::string line) {
	line.erase(0, line.find_first_not_of(" \t"));

	std::istringstream str(line);
	std::string key;
	str >> key;

	do {
		if (key == "\"id\"") {

		}
		else if (key == "\"plane\"") {

		}
	} while (std::getline(f, line, '\n'));
}


void Map::printData() {

}