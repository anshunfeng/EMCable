#pragma once
#include"VectorR2.h"
#include <set>
namespace component {

	class Boundary {
	private:
		std::set<int>		   boundaryVerticesIndex;
		//int					   boundaryVerticesNum;
	public:
		Boundary();
		~Boundary();
//内联
	public:
		int		getBoundaryVerticesNum()const { return  static_cast<int>(boundaryVerticesIndex.size()); }
//非内联
	public:
		void	setBoundary(std::set<int>& _set);
		bool	isBoundary(const int& _id)const;
	};	

} 