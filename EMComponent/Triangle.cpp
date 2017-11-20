#include "Triangle.h"
#include <cassert>

using namespace component;

void Triangle::setVertexID(const int _A, const int _B, const int _C)
{
	nA = _A;
	nB = _B;
	nC = _C;
}

void Triangle::setVertexArea(const VectorR2& _VA, const VectorR2& _VB, const VectorR2& _VC)
{
	VertexA = _VA;
	VertexB = _VB;
	VertexC = _VC;
	AreaTri = Area(VertexA, VertexB, VertexC); //计算三角形的面积
}

void Triangle::getVertexID(int& _A, int& _B, int& _C) const
{
	_A = nA;
	_B = nB;
	_C = nC;
}

void Triangle::getVertex(VectorR2& _VA, VectorR2& _VB, VectorR2& _VC)const
{
	_VA = VertexA;
	_VB = VertexB;
	_VC = VertexC;
}

VectorR2 Triangle::getTriCenter() const
{
	return (VertexA + VertexB + VertexC) / 3;
}
