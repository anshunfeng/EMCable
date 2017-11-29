#pragma once
#include "Custom.h"

namespace iml {

	bool    BICGSTAB(const Qcx_mat& _A, const Qcx_vec& _b, Qcx_vec& _x,
		value_t _eps, int _max_iter, int& _iterNum);
	bool    CG(const Qmat& _A, const Qvec& _b, Qvec& _x,
		value_t _eps, int _max_iter, int& _iterNum);
	bool    BICG(const Qcx_mat& _A, const Qcx_vec& _b, Qcx_vec& _x,
		value_t _eps, int _max_iter, int& _iterNum);
	bool    CGNE(const Qcx_mat& _A, const Qcx_vec& _b, Qcx_vec& _x,
		value_t _eps, int _max_iter, int& _iterNum);

} // namespace iml