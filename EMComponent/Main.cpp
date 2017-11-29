#include "Solver.h"

/*********************** Main ***************************/
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
	//Qmat test(4,5);
	//test(0, 0) = 1;
	//test(0, 1) = 1;
	//test(1, 1) = 1;
	//test(1, 2) = 1;
	//test(2, 2) = 1;
	//test(2, 3) = 1;
	//Qcout << test.n_rows << std::endl;



#ifdef _DEBUG
	_CrtDumpMemoryLeaks();
#endif

	system("pause");
	return 0;
}
