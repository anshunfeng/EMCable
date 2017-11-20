#include "Boundary.h"
using namespace component;

Boundary::Boundary()
{
}

Boundary::~Boundary()
{
	boundaryVerticesIndex.clear();
}

bool Boundary::isBoundary(const int& _id) const
{
	bool findResult = find(boundaryVerticesIndex.begin(), boundaryVerticesIndex.end(), _id) != boundaryVerticesIndex.end();
	return findResult;
}

void Boundary::setBoundary(std::set<int>& _set)
{
	boundaryVerticesIndex = _set;
}