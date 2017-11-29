#include "Solver.h"
#include "Loader.h"
#include "ThreePhaseAC.h"
#include "MyDebugNew.h"

#define INVOKE_SOLVER(loader_name, solver_name)     \
        {   solver::invoke_solver<loader_name, solver_name>(conf_file, stateInfo); }

#define BEGIN_SOLVER(solver_string, loader_name, solver_name)   \
        if(solver_type == solver_string)                        \
        INVOKE_SOLVER(loader_name, solver_name)

#define ADD_SOLVER(solver_string, loader_name, solver_name)     \
        else if(solver_type == solver_string)                   \
        INVOKE_SOLVER(loader_name, solver_name)

#define END_SOLVER()                                                \
        else                                                        \
        {                                                           \
            stateInfo.assign("undefined EM solver " + solver_type); \
            return;                                                 \
        }

using namespace component;
using namespace FEM;

void solver::loadSolver(const Qstring& conf_file, Qstring& stateInfo)
{
	Qcout << std::left;
	Qifstream config(conf_file, std::ios::in);
	if (config.fail())
	{
		stateInfo.assign("load " + conf_file + " failed");
		return;
	}
	config.ignore(1024, ' ');
	Qstring solver_type;
	config >> solver_type;
	if (solver_type != "Solver_Type:")
	{
		stateInfo.assign("replace Solver_Type with " + solver_type);
		return;
	}
	config >> solver_type;
	Qcout << std::setw(30) << "Solver Information: " << solver_type << std::endl;

	BEGIN_SOLVER("ThreePhaseAC", Loader, ThreePhaseAC)
		END_SOLVER()

		config.close();
}
