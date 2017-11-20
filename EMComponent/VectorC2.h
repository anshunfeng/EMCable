#pragma once
#include "VectorR2.h"

namespace component {

	class VectorC2 {
//���Ա
	public:
		Complex x, y;
//����
	public:
		VectorC2() : x(0, 0), y(0, 0){ }
		VectorC2(value_t xVal, value_t yVal)
			:x(Complex(xVal, 0)), y(Complex(yVal, 0)) {}
		VectorC2(const Complex& xVal, const Complex& yVal)
			: x(xVal), y(yVal) {}
		VectorC2(const VectorC2&) = default;
		VectorC2(VectorC2&&) = default;
		~VectorC2() = default;
		VectorC2& operator=(const VectorC2& v) = default;
		VectorC2& operator=(VectorC2&&) = default;
//��������									
		VectorC2& operator+=(const VectorC2& v);
		VectorC2& operator-=(const VectorC2& v);
		VectorC2& operator*=(const value_t d);
		VectorC2& operator/=(const value_t d);
// ����ʸ����ģֵ��ƽ����
	public:
		value_t norm() const
		{
			return std::norm(x) + std::norm(y);
		}
	
	};
//��Ա�������ⶨ��
	inline VectorC2& VectorC2::operator+=(const VectorC2& v)
	{
		x += v.x, y += v.y;
		return *this;
	}
	inline VectorC2& VectorC2::operator-=(const VectorC2& v)
	{
		x -= v.x, y -= v.y;
		return *this;
	}
	inline VectorC2 & VectorC2::operator*=(const value_t d)
	{
		x *= d; y *= d;
		return *this;
	}
	inline VectorC2 & VectorC2::operator/=(const value_t d)
	{
		assert(d != 0);
		x /= d; y /= d;
		return *this;
	}

//�ǳ�Ա����
	inline VectorC2 operator+(const VectorC2 &v1, const VectorC2 &v2);
	inline VectorC2 operator-(const VectorC2 &v1, const VectorC2 &v2);
	inline VectorC2 operator*(const value_t d, const VectorC2 &v);
	inline VectorC2 operator/(const VectorC2 &v, const value_t d);
	inline Complex  operator^(const VectorC2 &v1, const VectorC2 &v2);
	inline Complex  operator^(const VectorC2 &vc, const VectorR2 &vr);
	inline Complex  operator^(const VectorR2 &vr, const VectorC2 &vc);
	inline VectorC2 operator*(const VectorR2& u, const Complex m);
	inline VectorC2 operator*(const Complex m, const VectorR2& u);
	inline VectorC2 operator*(const VectorC2& u, const Complex& c);
	inline VectorC2 operator*(const Complex& c, const VectorC2& u);
	inline VectorC2 conj(const VectorC2& vc);
	inline value_t  norm(const VectorC2& vc);
// ��ָ������ʸ������
	inline VectorC2 conj(const VectorC2& vc)
	{
		return VectorC2(std::conj(vc.x), std::conj(vc.y));
	}
// ��ָ���ĸ���ʸ����ģƽ����
	inline value_t norm(const VectorC2& vc)
	{
		return vc.norm();
	}
// �ǳ�Ա�����������������
	inline VectorC2 operator+(const VectorC2 &v1, const VectorC2 &v2)
	{
		auto tmp = v1;
		return tmp += v2;
	}
	inline VectorC2 operator-(const VectorC2 &v1, const VectorC2 &v2)
	{
		auto tmp = v1;
		return tmp -= v2;
	}
	inline VectorC2 operator*(const VectorC2 &v, const value_t d)
	{
		auto tmp = v;
		return tmp *= d;
	}
	inline VectorC2 operator*(const value_t d, const VectorC2 &v)
	{
		auto tmp = v;
		return tmp *= d;
	}
	inline VectorC2 operator/(const VectorC2 &v, const value_t d)
	{
		auto tmp = v;
		return tmp /= d;
	}
	inline Complex  operator^(const VectorC2 &v1, const VectorC2 &v2)
	{
		return (v1.x*v2.x + v1.y*v2.y);
	}
	inline Complex  operator^(const VectorC2 &vc, const VectorR2 &vr)
	{
		return (vc.x*vr.x + vc.y*vr.y);
	}
	inline Complex  operator^(const VectorR2 &vr, const VectorC2 &vc)
	{
		return (vc.x*vr.x + vc.y*vr.y);
	}
	inline VectorC2 operator*(const VectorR2& u, const Complex m)
	{
		return VectorC2(u.x*m, u.y*m);
	}
	inline VectorC2 operator*(const Complex m, const VectorR2& u)
	{
		return VectorC2(u.x*m, u.y*m);
	}
	inline VectorC2 operator*(const VectorC2& u, const Complex& c)
	{
		return VectorC2(u.x*c, u.y*c);
	}
	inline VectorC2 operator*(const Complex& c, const VectorC2& u)
	{
		return u * c;
	}


} // namespace component