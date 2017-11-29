#include "MatrixSolver.h"
#include <iomanip>

bool iml::BICGSTAB(const Qcx_mat & _A, const Qcx_vec & _b, Qcx_vec & _x,
	value_t _eps, int _max_iter, int& _iterNum)
{
	_iterNum = 0;
	Qcx_vec r0 = _b - _A * _x;
	Qcx_vec r1 = r0;
	Complex rou0(1, 0), alph(1, 0), omiga(1, 0);
	Qcx_vec v, p0, p1, s, t;
	v.zeros(arma::size(_x));
	p0.zeros(arma::size(_x));
	Complex rou1, beta;
	auto b_normal = arma::norm(_b);

	auto oldState = Qcout.flags();
	Qcout << std::fixed << std::setprecision(10) << std::right;
	for (int i = 0; i < _max_iter; ++i)
	{
		++_iterNum;
		rou1 = arma::cdot(r1, r0);
		beta = (rou1 / rou0) * (alph / omiga);
		p1 = r0 + beta * (p0 - omiga * v);
		v = _A * p1;
		alph = rou1 / (arma::cdot(r1, v));
		_x = _x + alph * p1;
		s = r0 - alph * v;
		auto resdual = arma::norm(s) / b_normal;
		Qcout << std::setw(7) << _iterNum << " of " << _max_iter
			<< std::setw(15) << resdual << std::endl;
		if (resdual < _eps)
			break;
		t = _A * s;
		omiga = arma::cdot(t, s) / arma::cdot(t, t);
		_x = _x + omiga * s;
		r0 = s - omiga * t;
		resdual = arma::norm(s) / b_normal;
		if (resdual < _eps)
			break;
		rou0 = rou1;
		p0 = p1;
	}
	Qcout.flags(oldState);
	if (_iterNum == _max_iter)
		return false;
	return true;
}

bool iml::CG(const Qmat & _A, const Qvec & _b, Qvec & _x, value_t _eps, int _max_iter, int & _iterNum)
{
	_iterNum = 0;
	_x.zeros(_A.n_rows);
	Qvec r = _b - _A * _x;
	auto p = r;
	auto rsold = arma::dot(r, r);//µã»ý
	while (_iterNum < _max_iter)
	{
		++_iterNum;
		Qvec Ap = _A * p;
		auto alpha = rsold / (arma::dot(p, Ap));
		_x = _x + alpha * p;
		r = r - alpha * Ap;
		auto rsnew = arma::dot(r, r);
		if (std::sqrt(rsnew) < _eps)
		{
			return true;
		}
		p = r + (rsnew / rsold) * p;
		rsold = rsnew;
	}
	return false;
}



bool iml::BICG(const Qcx_mat & _A, const Qcx_vec & _b, Qcx_vec & _x, value_t _eps, int _max_iter, int & _iterNum)
{
	return false;
}

bool iml::CGNE(const Qcx_mat & _A, const Qcx_vec & _b, Qcx_vec & _x, value_t _eps, int _max_iter, int & _iterNum)
{
	_iterNum = 0;
	_x.zeros(_A.n_cols);
	Qcx_vec r0 = _b - _A * _x;
	Qcx_vec p0 = (r0.t() * _A).t();
	Qcx_vec p1;
	while (_iterNum < _max_iter)
	{
		++_iterNum;
		auto alpha = (arma::cdot(r0, r0) / arma::cdot(p0, p0)).real();
		_x = _x + alpha * p0;
		r0 = r0 - alpha * (_A * p0);
		auto rs = arma::norm(r0);
		if (rs < _eps)
		{
			return true;
		}
		auto pn2 = arma::cdot(p0, p0);
		p1 = (r0.t() * _A).st();
		auto beta = -(arma::dot(p1, p0) / pn2);
		p0 = arma::conj(p1) + beta * p0;
	}
	return false;
}
