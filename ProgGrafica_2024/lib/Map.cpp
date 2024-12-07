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
	else {
		std::cout << "Fichero abierto" << std::endl;
	}

	std::string line;
	std::string key;

	Solid_t currentSolid;

	while (std::getline(f, line, '\n'))
	{
		std::istringstream str(line);
		std::string key;

		str >> key;

		if (key == "solid") {
			currentSolid = leerSolid(f, line);
		}
		std::cout << "Estas en el solid con ID:" << currentSolid.id << std::endl;
		solidList.push_back(currentSolid);
	}
	/*for(auto &s : solidList){
		getPlane(s.id);
	}*/
}

Solid_t Map::leerSolid(std::ifstream& f, std::string line) {

	Solid_t currentSolid = {0};
	Side_t currentSide = {0};

	std::string key;
	do {
		std::string lineIn="";
		for(auto &c:line)
			if(c!='\"')
				lineIn += c;

		std::istringstream str(lineIn);
		std::string key;

		str >> key;

		if (key == "id") {
			std::cout << "Leyendo ID del solid..." << std::endl;
			str >> currentSolid.id;
		}
		else if (key == "side") {
			currentSide = leerSide(f, lineIn); 
			currentSolid.sideList.push_back(currentSide);
		}
	} while (std::getline(f, line, '\n'));

	return currentSolid;

}

Side_t Map::leerSide(std::ifstream& f, std::string line) {
	
	Side_t currentSide = {0};
	char lastChar;


	do {
		std::string lineIn = "";
		for (auto& c : line)
			if (c != '\"')
				lineIn += c;

		std::istringstream str(lineIn);
		std::string key;

		str >> key;

		if (key == "id") {
			std::cout << "Leyendo ID del side..." << std::endl;
			str >> currentSide.id;
		}
		else if (key == "plane") {
			currentSide.plane = leerPlane(f, line);
		}
		
		lastChar = line[line.length()];
		std::getline(f, line, '\n');
	} while (lastChar != '}');
	//while (lastChar != '}');

	return currentSide;
}

Plane_t Map::leerPlane(std::ifstream& f, std::string line) {

	Plane_t res = {0};

	do {
		std::string lineIn = "";
		for (auto& c : line)
			if (c != '\"' && c != '(' && c != ')')
				lineIn += c;
				

		std::istringstream str(lineIn);
		std::string key;

		str >> key;

		if (key == "plane") {
			str >> res.point1.x;
			str >> res.point1.y;
			str >> res.point1.z;

			std::cout << "Punto 1: " << res.point1.x <<"x "<< res.point1.y << "y " << res.point1.z << "z " << std::endl;

			str >> res.point2.x;
			str >> res.point2.y;
			str >> res.point2.z;

			std::cout << "Punto 2: " << res.point2.x << "x " <<res.point2.y << "y " << res.point2.z << "z " << std::endl;

			str >> res.point3.x;
			str >> res.point3.y;
			str >> res.point3.z;

			std::cout << "Punto 3: " << res.point3.x << "x " << res.point3.y << "y " << res.point3.z << "z " << std::endl;
		}

	} while (std::getline(f, line, '\n'));

	return res;
}


void Map::getPlane(int x1, int y1, int z1, int x2, int y2, int z2, int x3, int y3, int z3) {
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

	std::cout << "Plano: " << a << "x " << b << "y " << c << "z " << d << std::endl;

}

void Map::getPlane(int id) {
	for (auto& s : solidList[id].sideList) {
		float v1x = s.plane.point2.x - s.plane.point1.x;
		float v1y = s.plane.point2.y - s.plane.point1.y;
		float v1z = s.plane.point2.z - s.plane.point1.z;

		float v2x = s.plane.point3.x - s.plane.point1.x;
		float v2y = s.plane.point3.y - s.plane.point1.y;
		float v2z = s.plane.point3.z - s.plane.point1.z;

		s.plane.a = v1y * v2z - v1z * v2y;
		s.plane.b = v1z * v2x - v1x * v2z;
		s.plane.c = v1x * v2y - v1y * v2x;
		s.plane.d = -(s.plane.a * s.plane.point1.x
			+ s.plane.b * s.plane.point1.y
			+ s.plane.c * s.plane.point1.z);

		std::cout << "Plano: " << s.plane.a << "X " << s.plane.b << "X " << s.plane.c << "X " << s.plane.d << std::endl;
	}

}