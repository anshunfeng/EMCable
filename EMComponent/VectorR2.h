#pragma once
#include "VectorR3.h"
#include <cassert>

namespace component {

	class VectorR2 {
//类成员
	public:
		value_t x, y;
//内联
	public:
		VectorR2() :x(0.0f), y(0.0f) {}
		VectorR2(value_t _x, value_t _y) :x(_x), y(_y) {}
		VectorR2(const VectorR2&) = default;
		VectorR2(VectorR2&&) = default;
		~VectorR2() {}
//类外内联
	public:
		VectorR2& operator=(const VectorR2&);
//类内内联
		VectorR2& operator+=(const VectorR2& v)
		{
			x += v.x; y += v.y; 
			return *this;
		}

		VectorR2& operator-=(const VectorR2& v)
		{
			x -= v.x; y -= v.y; 
			return *this;
		}

		VectorR2& operator*=(const value_t d)
		{
			x *= d; y *= d; 
			return *this;
		}

		VectorR2& operator/=(const value_t d)
		{
			assert(d != 0);
			register value_t mInv = value_t(1.0) / d;
			x *= mInv; y *= mInv;
			return *this;
		}

		value_t Norm() const
		{
			return (std::sqrt(x*x + y*y));
		}

		VectorR2& Normalize()
		{
			*this /= Norm();
			return *this;
		}
	};
	// 内联成员函数定义
	inline VectorR2& VectorR2::operator=(const VectorR2& v1)
	{
		if (&v1 != this)
		{
			this->x = v1.x;
			this->y = v1.y;
		}
		return *this;
	}
	// 内联非成员函数声明
	inline VectorR2 operator+(const VectorR2& v1, const VectorR2& v2);
	inline VectorR2 operator-(const VectorR2& v1, const VectorR2& v2);
	inline value_t  operator^(const VectorR2& v1, const VectorR2& v2);
	inline VectorR2 operator*(const VectorR2& v1, const value_t d); //矢量x标量
	inline VectorR2 operator*(const value_t d, const VectorR2& v2); // 标量x矢量
	inline VectorR2 operator/(const VectorR2& v1, const value_t d);
	inline value_t Area(const VectorR2& v1, const VectorR2& v2, const VectorR2& v3);
	// 内联非成员函数定义
	inline VectorR2 operator+(const VectorR2& v1, const VectorR2& v2)
	{
		auto tmp = v1;
		return tmp += v2;
	}
	inline VectorR2 operator-(const VectorR2& v1, const VectorR2& v2)
	{
		auto tmp = v1;
		return tmp -= v2;
	}
	inline value_t operator^(const VectorR2& v1, const VectorR2& v2)
	{
		return (v1.x * v2.x + v1.y * v2.y);
	}
	inline VectorR2 operator*(const VectorR2& v1, const value_t d)
	{
		auto tmp = v1;
		return tmp *= d;
	}
	inline VectorR2 operator*(const value_t d, const VectorR2& v1)
	{
		return v1 * d;
	}
	inline VectorR2 operator/(const VectorR2& v1, const value_t d)
	{
		auto tmp = v1;
		return tmp /= d;
	}
	inline value_t Area(const VectorR2& v1, const VectorR2& v2, const VectorR2& v3)
	{
		value_t la = (v1 - v2).Norm();
		value_t lb = (v2 - v3).Norm();
		value_t lc = (v3 - v1).Norm();
		value_t ls = (la + lb + lc) / 2.0f;
		return sqrt(ls*(ls - la)*(ls - lb)*(ls - lc));
	}

} 