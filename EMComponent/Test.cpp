#include <iostream>
#include "custom.h"
#include "VectorR2.h"
#include "VectorC2.h"
#include "Mathdef.h"
#include "Triangle.h"
#include "Loader.h"
#include "Mesh.h"
#include <string>
#include "Loader.h"
using namespace component;

int main()
{
	bool flag;
	std::string info;
	Loader test;
	std::string fileName("D:\\DC_Solver.txt");
	if (test.load(fileName, info))
		flag = 1;
	else
		flag = 0;


	std::cout << flag << std::endl;
	std::cout << info << std::endl;


	std::cout << test.getReport() << std::endl;
	

	system("pause");
	return 0;
}
