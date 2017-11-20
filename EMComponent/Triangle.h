#pragma once
#include"VectorR2.h"

namespace component {

	class Triangle {
//���Ա	
	private:
		int         nA;//����
		int         nB;//����
		int         nC;//����
		VectorR2    VertexA;
		VectorR2    VertexB;
		VectorR2    VertexC;
		Qstring		triMaterial;
		int			subdomain_ID;
		bool		isMental;
		value_t		AreaTri;
//����
	public:
		void        setSunbdomainID(int _id) { subdomain_ID = _id; }
		int         getSunbdomainID() const { return subdomain_ID; }
		value_t		getTriArea()const { return AreaTri; }
		void		setTriMaterial(Qstring& _Material) { triMaterial = _Material; }
		Qstring		getTriMaterial()const { return triMaterial; }
		void		setIsMental(bool _is) { isMental = _is; }
		bool		getIsMental()const { return isMental; }
//������
	public:
		void        setVertexID(const int _A, const int _B, const int _C);
		void        setVertexArea(const VectorR2& _A, const VectorR2& _B, const VectorR2& _C);
		void        getVertexID(int& _A, int& _B, int& _C)const;
		void        getVertex(VectorR2& _VA, VectorR2& _VB, VectorR2& _VC)const;
	VectorR2		getTriCenter()const;
	};

}

