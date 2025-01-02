#include "Map.h"

Map::Map()
{

}

Map::Map(std::string filename) {
	leerArchivo(filename);

	for (auto& s : worldMap.solidList)
		writeTRSFile(std::to_string(s.id));
}

void Map::leerArchivo(std::string filename) {
	//std::cout << "Leyendo desde fichero: " + filename + "\n";

	std::string line;
	std::string key;

	std::ifstream f(filename, std::ios_base::in);

	if (!f.is_open()) {
		std::cout << "ERROR Fichero " << filename << " no encontrado\n";
		return;
	}
	else {
		std::cout << "Fichero abierto" << std::endl;
	}

	while (std::getline(f, line, '\n')) {

		std::string lineIn = "";
		for (auto& c : line)
			if (c != '\"')
				lineIn += c;

		std::istringstream str(lineIn);
		std::string key;

		str >> key;

		if (key == "world")
			worldMap = leerWorld(f, line);
	}

	for (auto& s : worldMap.solidList) {
		getPlane(s);
		pushVertices(s);
	}
}

void Map::pushVertices(Solid_t& solidList) {
	if (solidList.sideList.size() > 5) {
		//Vert = vertice / Abj = abajo / Arr = arriba / Der = derecha / Izq = izquierda / Atras = atras / Alan = Alante
		glm::vec3 vertAbjIzqAtras = getVertices(solidList.sideList[4].plane, solidList.sideList[0].plane, solidList.sideList[2].plane);
		solidList.vertexList.push_back(vertAbjIzqAtras);
		glm::vec3 vertAbjIzqAlan = getVertices(solidList.sideList[4].plane, solidList.sideList[0].plane, solidList.sideList[3].plane);
		solidList.vertexList.push_back(vertAbjIzqAlan);
		glm::vec3 vertAbjDerAtras = getVertices(solidList.sideList[4].plane, solidList.sideList[1].plane, solidList.sideList[2].plane);
		solidList.vertexList.push_back(vertAbjDerAtras);
		glm::vec3 vertAbjDerAlan = getVertices(solidList.sideList[4].plane, solidList.sideList[1].plane, solidList.sideList[3].plane);
		solidList.vertexList.push_back(vertAbjDerAlan);


		glm::vec3 vertArrIzqAtras = getVertices(solidList.sideList[5].plane, solidList.sideList[0].plane, solidList.sideList[2].plane);
		solidList.vertexList.push_back(vertArrIzqAtras);
		glm::vec3 vertArrIzqAlan = getVertices(solidList.sideList[5].plane, solidList.sideList[0].plane, solidList.sideList[3].plane);
		solidList.vertexList.push_back(vertArrIzqAlan);
		glm::vec3 vertArrDerAtras = getVertices(solidList.sideList[5].plane, solidList.sideList[1].plane, solidList.sideList[2].plane);
		solidList.vertexList.push_back(vertArrDerAtras);
		glm::vec3 vertArrDerAlan = getVertices(solidList.sideList[5].plane, solidList.sideList[1].plane, solidList.sideList[3].plane);
		solidList.vertexList.push_back(vertArrDerAlan);

		solidList.center = getCenter(solidList);
	}
	else {
		std::cout << "No se puede calcular los vertices de un solid con menos de 6 lados" << std::endl;
	}
}

World_t Map::leerWorld(std::ifstream& f, std::string line) {
	World_t currentWorld;
	std::string key;

	do {
		std::getline(f, line, '\n');

		std::string lineIn = "";
		for (auto& c : line)
			if (c != '\"')
				lineIn += c;

		std::istringstream str(lineIn);
		str >> key;

		if (key == "id") {
			//std::cout << "Leyendo Id del Wordl" << std::endl;
			str >> currentWorld.id;
			//std::cout << currentWorld.id << std::endl;
		}
		else if (key == "solid") {
			currentWorld.solidList.push_back(leerSolid(f, lineIn));
		}
	} while (key != "}");

	std::cout << "Numero de solids: " << currentWorld.solidList.size() << std::endl;

	return currentWorld;
}

Solid_t Map::leerSolid(std::ifstream& f, std::string line) {

	Solid_t currentSolid;
	std::string key = "";
	do {
		std::getline(f, line, '\n');

		std::string lineIn = "";
		for (auto& c : line)
			if (c != '\"')
				lineIn += c;

		std::istringstream str(lineIn);
		str >> key;

		if (key == "id") {
			//std::cout << "Leyendo ID del solid..." << std::endl;
			str >> currentSolid.id;
			//std::cout << currentSolid.id << std::endl;
		}
		else if (key == "side") {
			currentSolid.sideList.push_back(leerSide(f, lineIn));
		}
	} while (key != "}");

	return currentSolid;
}

Side_t Map::leerSide(std::ifstream& f, std::string line) {

	Side_t currentSide{0};
	std::string key;

	do {
		std::getline(f, line, '\n');

		std::string lineIn = "";
		for (auto& c : line)
			if (c != '\"')
				lineIn += c;

		std::istringstream str(lineIn);
		str >> key;

		if (key == "id") {
			//std::cout << "Leyendo ID del side..." << std::endl;
			str >> currentSide.id;
		}
		else if (key == "plane") {
			currentSide.plane = leerPlane(line);
		}
	} while (key != "}");

	return currentSide;
}

Plane_t Map::leerPlane(std::string line) {

	Plane_t res = { 0 };

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

		//std::cout << "Punto 1: " << res.point1.x << "x " << res.point1.y << "y " << res.point1.z << "z " << std::endl;

		str >> res.point2.x;
		str >> res.point2.y;
		str >> res.point2.z;

		//std::cout << "Punto 2: " << res.point2.x << "x " << res.point2.y << "y " << res.point2.z << "z " << std::endl;

		str >> res.point3.x;
		str >> res.point3.y;
		str >> res.point3.z;

		//std::cout << "Punto 3: " << res.point3.x << "x " << res.point3.y << "y " << res.point3.z << "z " << std::endl;
	}

	return res;
}

void Map::getPlane(Solid_t& solidList) {
	for (auto& s : solidList.sideList) {
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

		//std::cout << "Plano" << s.id << ": " << s.plane.a << "X " << s.plane.b << "Y " << s.plane.c << "Z " << s.plane.d << std::endl;
	}
}

glm::vec3 Map::getVertices(Plane_t p1, Plane_t p2, Plane_t p3) {
	glm::mat3 matrix = glm::mat3(glm::vec3(p1.a, p1.b, p1.c), glm::vec3(p2.a, p2.b, p2.c), glm::vec3(p3.a, p3.b, p3.c));

	glm::mat3 matrixX = glm::mat3(glm::vec3(p1.d, p1.b, p1.c), glm::vec3(p2.d, p2.b, p2.c), glm::vec3(p3.d, p3.b, p3.c));

	glm::mat3 matrixY = glm::mat3(glm::vec3(p1.a, p1.d, p1.c), glm::vec3(p2.a, p2.d, p2.c), glm::vec3(p3.a, p3.d, p3.c));

	glm::mat3 matrixZ = glm::mat3(glm::vec3(p1.a, p1.b, p1.d), glm::vec3(p2.a, p2.b, p2.d), glm::vec3(p3.a, p3.b, p3.d));

	float determinantOriginal = glm::determinant(matrix);
	float determinantX = glm::determinant(matrixX);
	float determinantY = glm::determinant(matrixY);
	float determinantZ = glm::determinant(matrixZ);

	float x = determinantX / determinantOriginal;
	float y = determinantY / determinantOriginal;
	float z = determinantZ / determinantOriginal;

	glm::vec3 point = glm::vec3(x, y, z);
	//std::cout << "Vertice: " << x << "X " << y << "Y " << z << "Z " << std::endl;
	return point;
}

glm::vec3 Map::getCenter(Solid_t& solid) {
	float pX = 0;
	float pY = 0;
	float pZ = 0;
	for (auto& s : solid.vertexList) {
		pX += s.x;
		pY += s.y;
		pZ += s.z;
	}
	//std::cout << "Suma: " << pX << "X " << pY << "Y " << pZ << "Z " << std::endl;
	pX = pX / 8;
	pY = pY / 8;
	pZ = pZ / 8;
	//std::cout << "Centro: " << pX << "X " << pY << "Y " << pZ << "Z " << std::endl;

	return glm::vec3(pX, pY, pZ);
}

void Map::writeTRSFile(std::string filename) {
	filename += ".trs";
	//Hay que crear el directorio con anterioridad o modificar la ruta a donde cada usuario quiera
	std::string path = "data/Map/";
	//std::cout << "Escribiendo en fichero: "+ path + filename + "\n";
	std::fstream f(path+filename, std::ios::in);

	if (!f.is_open()) {
		f.open(path+filename, std::ios::out);
	}
	else {
		f.open(path+filename, std::ios::app);
	}

	for (auto& s : worldMap.solidList[id].vertexList) {
		f << "vert " << s.x << " " << s.y << " " << s.z << std::endl;
		f << "color 0 0 0 0" << std::endl;
	}
	f << "face 0 1 3" << std::endl;
	f << "face 2 0 3" << std::endl;

	f << "face 4 7 5" << std::endl;
	f << "face 6 7 4" << std::endl;

	f << "face 4 0 2" << std::endl;
	f << "face 6 4 2" << std::endl;

	f << "face 1 5 7" << std::endl;
	f << "face 3 1 7" << std::endl;

	f << "face 2 3 7" << std::endl;
	f << "face 6 2 7" << std::endl;

	f << "face 4 5 1" << std::endl;
	f << "face 0 4 1" << std::endl;

	f << "svert data/shader.vert" << std::endl;
	f << "sfrag data/shader.frag" << std::endl;

	f.close();
}