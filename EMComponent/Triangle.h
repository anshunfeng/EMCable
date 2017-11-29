#pragma once
#include"VectorR2.h"

namespace component {

	class Triangle {
//类成员	
	private:
		int         nA;//索引
		int         nB;//索引
		int         nC;//索引
		VectorR2    VertexA;
		VectorR2    VertexB;
		VectorR2    VertexC;
		Qstring		triMaterial;
		int			subdomain_ID;
		bool		isMental;
		value_t		AreaTri;

		value_t		na1;
		value_t		na2;
		value_t		na3;

		value_t		nb1;
		value_t		nb2;
		value_t		nb3;

		value_t		nc1;
		value_t		nc2;
		value_t		nc3;
		
//内联
	public:
		void		getna123(value_t& _na1, value_t& _na2, value_t& _na3)const;
		void		getnb123(value_t& _nb1, value_t& _nb2, value_t& _nb3)const;
		void		getnc123(value_t& _nc1, value_t& _nc2, value_t& _nc3)const;
		value_t		getna1();
		value_t		getna2();
		value_t		getna3();
		value_t		getnb1();
		value_t		getnb2();
		value_t		getnb3();
		value_t		getnc1();
		value_t		getnc2();
		value_t		getnc3();
	
	public:
		void        setSunbdomainID(int _id);
		int         getSunbdomainID() const;
		value_t		getTriArea()const;
		//void		setTriMaterial(Qstring& _Material);
		//Qstring	getTriMaterial()const;
		void		setIsMental(bool _is);
		bool		getIsMental()const;
		bool		isVertexOfTri(int _index);
		int			getnA()const;
		int			getnB()const;
		int			getnC()const;
		void		getnAnBnC(int& _nA, int& _nB, int& _nC)const;
		
//非内联
	public:
		void        setVertexID(const int _A, const int _B, const int _C);
		void        setVertexArea(const VectorR2& _A, const VectorR2& _B, const VectorR2& _C);
		void        getVertexID(int& _A, int& _B, int& _C)const;
		void        getVertex(VectorR2& _VA, VectorR2& _VB, VectorR2& _VC)const;
	VectorR2		getTriCenter()const;
		int			getVertexIndex(int _idIndex)const;
	};
	inline void			Triangle::getna123(value_t& _na1, value_t& _na2, value_t& _na3)const
	{
		_na1 = na1;
		_na2 = na2;
		_na3 = na3;
	}
	inline void			Triangle::getnb123(value_t& _nb1, value_t& _nb2, value_t& _nb3)const
	{
		_nb1 = nb1;
		_nb2 = nb2;
		_nb3 = nb3;
	}
	inline void			Triangle::getnc123(value_t&_nc1, value_t& _nc2, value_t& _nc3)const
	{
		_nc1 = nc1;
		_nc2 = nc2;
		_nc3 = nc3;
	}
	inline value_t		Triangle::getna1() { return na1; }
	inline value_t		Triangle::getna2() { return na2; }
	inline value_t		Triangle::getna3() { return na3; }
	inline value_t		Triangle::getnb1() { return nb1; }
	inline value_t		Triangle::getnb2() { return nb2; }
	inline value_t		Triangle::getnb3() { return nb3; }
	inline value_t		Triangle::getnc1() { return nc1; }
	inline value_t		Triangle::getnc2() { return nc2; }
	inline value_t		Triangle::getnc3() { return nc3; }

	inline void         Triangle::setSunbdomainID(int _id) { subdomain_ID = _id; }
	inline int          Triangle::getSunbdomainID() const { return subdomain_ID; }
	inline value_t		Triangle::getTriArea()const { return AreaTri; }
	//inline void		Triangle::setTriMaterial(Qstring& _Material) { triMaterial = _Material; }
	//inline Qstring	Triangle::getTriMaterial()const { return triMaterial; }
	inline void			Triangle::setIsMental(bool _is) { isMental = _is; }
	inline bool			Triangle::getIsMental()const { return isMental; }
	inline bool			Triangle::isVertexOfTri(int _index)
	{
		return ((_index == nA) || (_index == nB) || (_index == nC));
	}
	inline int			Triangle::getnA()const { return nA; }
	inline int			Triangle::getnB()const { return nB; }
	inline int			Triangle::getnC()const { return nC; }
	inline void			Triangle::getnAnBnC(int& _nA, int& _nB, int& _nC)const
	{
		_nA = nA;
		_nB = nB;
		_nC = nC;
	}
}

