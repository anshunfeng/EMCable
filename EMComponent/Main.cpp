#include "Solver.h"
#include "Mesh.h"
using namespace component;
using namespace std;
int main(int argc, char** argv)
{
	Qstring config, state("OK");
	Qcout << "Input Path of Config File:" << std::endl;
	std::getline(std::cin, config);
	//D:\\Important Documents\\cable_math\\EMCable\\InputFile\\Three_Phase_Config.txt
//D:\\Important Documents\\cable_math\\EMCable\\InputFile\\Three_Phase_Config_JE85SC.txt
	if (config.empty())
	{
		Qcout << "Please examine the config path!" << std::endl;
	}
	solver::loadSolver(config, state);
	Qcout << "Exit State: " << state << std::endl;
#ifdef _DEBUG
	_CrtDumpMemoryLeaks();
#endif

	system("pause");
	return 0;
}
