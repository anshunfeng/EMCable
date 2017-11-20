#include <iostream>
#include "custom.h"
#include "VectorR2.h"
#include "VectorC2.h"
#include "Mathdef.h"
#include "Triangle.h"
#include "Loader.h"
#include "Mesh.h"
#include <string>
using namespace component;

int main()
{
	//value_t result = component::Area(a, b, c);

	//std::cout << a.x << a.y << a.norm()  << std::endl;
	//std::cout <<  << std::endl;
	//value_t total = 255;
	//value_t cur = 22;
	//size_t num1 = 100 * cur / total;
	//size_t num2 = 100 * 222 / total;
	//Qcout << (num1 < 10 ? " " : "") << num1 << '%' << std::flush << std::endl;
	//Qcout  << (num2 < 10 ? " " : "") << num2 << '%' << std::flush << std::endl;
	//
	//Qmat mat(2, 2);
	//mat(1, 1) = 100;
	//Qcout << mat;

	/*component::Triangle tri;
	component::VectorR2 a(1, 0);
	component::VectorR2 b(0, 1);
	component::VectorR2 c(0, 0);
	tri.setVertex(a, b, c);
	std::cout  << tri.getCenter().x << tri.getCenter().y << std::endl;
	*/	
	/*oader test;
	Qstring stateInfo;

	test.load("E:\\temp\\1.txt", stateInfo);
	Qcout << stateInfo << std::endl;
	Qcout << test.getReport() <<  std::endl;*/
	Mesh mesh;
	std::string info;
	std::string fileName("D:\\Important Documents\\cable_math\\FEM2D\\MeshFileForC.txt");

	mesh.loadMeshFile(fileName, info);
	std::cout << info << std::endl;
	mesh.reportInfo(std::cout);


	system("pause");
	return 0;
}
