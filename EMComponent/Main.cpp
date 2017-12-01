#include "Solver.h"
int main(int argc, char** argv)
{
	Qstring config, state("OK");
	Qcout << "Input Path of Config File:" << std::endl;
	std::getline(std::cin, config);
	//D:\\Important Documents\\cable_math\\EMCable\\InputFile\\Three_Phase_Config.txt
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
