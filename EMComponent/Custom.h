#pragma once
#define ARMA_USE_LAPACK
#define ARMA_USE_BLAS
#define ARMA_USE_CXX11

#ifndef _DEBUG
#define ARMA_NO_DEBUG 
#endif

#include <string>
#include <complex>
#include <fstream>
#include <sstream>
#include <iostream>
#include <iomanip> // 有关数据输出格式
#include <assert.h>
#include <armadillo> // 矩阵运算

#define     Qcout               std::cout
#define     Qcin                std::cin
#define     Qcerr               std::cerr

typedef std::fstream            Qfstream;
typedef std::ifstream           Qifstream;
typedef std::ofstream           Qofstream;
typedef std::stringstream       Qsstream;
typedef std::ostream            Qostream;
typedef std::string             Qstring;
typedef float                   value_t;

typedef std::complex<value_t>   Complex;
typedef arma::Mat<value_t>      Qmat;
typedef arma::Mat<Complex>      Qcx_mat;
typedef arma::Col<value_t>      Qvec;
typedef arma::Col<Complex>      Qcx_vec;
typedef arma::SpMat<Complex>    Qsp_cx_mat;
typedef arma::SpMat<value_t>    Qsp_mat;
typedef arma::Mat<size_t>       Qumat;

//  desktop: wt
namespace wt {

	//  sparse matrix template
	template <typename T>
	class QSpMat {
	public:
		// 构造函数
		QSpMat() { }

		// 矩阵维度设置
		void set_size(size_t r, size_t c)
		{
			const size_t num = r * c;
			loc_.set_size(2, num);
			elems_.set_size(num);
		}

		// 矩阵元素设值
		void set_value(const size_t r, const size_t c, const size_t i, const T& val)
		{
			assert(i < elems_.n_elem);
			loc_(0, i) = r;
			loc_(1, i) = c;
			elems_(i) = val;
		}

		// 类模板调用，返回列矢量elems_的第i+1个元素值
		T& operator()(const size_t i)
		{
			assert(i < elems_.n_elem);
			return elems_(i);
		}

		// 类模板调用，返回列矢量elems_的第i + 1个元素值（常量）
		const T& operator()(const size_t i) const
		{
			assert(i < elems_.n_elem);
			return elems_(i);
		}

		// 返回矩阵loc_第0+1行，第i+1列元素(size_t类型）
		size_t row(const size_t i) const
		{
			assert(i < elems_.n_elem);
			return loc_(0, i);
		}
		// 返回矩阵loc_第1+1行，第i+1列元素(size_t类型）
		size_t col(const size_t i) const
		{
			assert(i < elems_.n_elem);
			return loc_(1, i);
		}

		// 返回elems_列向量的大小
		size_t size() const
		{
			return elems_.n_elem;
		}
	private:
		Qumat           loc_;
		arma::Col<T>    elems_;
	};

	//  column-major ordering matrix template
	template <typename T>
	class QMat {
	public:
		// 构造函数
		QMat() : row_(0), col_(0), vect_() { }
		void set_size(const size_t r, const size_t c)
		{
			row_ = r;
			col_ = c;
			vect_.resize(r * c);
		}
		void set_size(const size_t r, const size_t c, const T& val)
		{
			row_ = r;
			col_ = c;
			vect_.resize(r * c, val);
		}
		void reset(const T& val)
		{
			for (auto& elem : vect_)
				elem = val;
		}

		T& operator()(const size_t ir, const size_t ic)
		{
			assert(ic * row_ + ir < vect_.size());
			return vect_[ic * row_ + ir];
		}
		const T& operator()(const size_t ir, const size_t ic) const
		{
			assert(ic * row_ + ir < vect_.size());
			return vect_[ic * row_ + ir];
		}
		T& operator()(const size_t ith)
		{
			assert(ith < vect_.size());
			return vect_[ith];
		}
		const T& operator() (const size_t ith) const
		{
			assert(ith < vect_.size());
			return vect_[ith];
		}

		size_t row() const { return row_; }
		size_t col() const { return col_; }

		size_t size() const { return vect_.size(); };
		void clear() { vect_.clear(); }
	private:
		size_t          row_, col_;
		std::vector<T>  vect_;
	};

} // namespace wt