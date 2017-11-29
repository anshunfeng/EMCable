#ifndef _SOLVER_H_
#define _SOLVER_H_

#include "Custom.h"
#include <memory>

namespace solver {

	template <typename QLoader, typename QSolver>
	void invoke_solver(const Qstring& conf_file, Qstring& state)
	{
		auto loader_sptr = std::make_unique<QLoader>();
		if (loader_sptr->load(conf_file, state))
		{
			auto solver_sptr = std::make_unique<QSolver>();
			solver_sptr->init(loader_sptr.get(), state);
			if (state == "OK")//
			{
				solver_sptr->reportMeshInfo(Qcout);
				solver_sptr->solve();
				solver_sptr->output();
			}
			solver_sptr->clear();
		}
	}

	void loadSolver(const Qstring& conf_file, Qstring& stateInfo);

}

#endif
