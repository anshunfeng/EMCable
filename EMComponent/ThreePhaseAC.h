#pragma once
#include <memory>
#include "EM.h"

namespace FEM {

	class ThreePhaseAC :public EM {
		using MeshPointer = std::shared_ptr<component::Mesh>;

	protected:
		std::set<int>			_m_currentDomains;
		std::set<int>			_m_dielectricDomains;

		int         m_unknownAScar;
		int			m_unknownCurrDensity;
		int			triangleNum;

		Qcx_mat		CoeffiMat;
		/*Qmat		S;
		Qmat		T;
		Qmat		Q;
		Qmat		Area;*/
		Qcx_vec		Asca_Jsk;
		Qcx_vec		V;

		MeshPointer pMesh;
		//Qofstream   rad_data_, rcs_data_;

	public:
		ThreePhaseAC();
		~ThreePhaseAC();
	public:
		int			getUnknownAScarNum()const;
		int			getUnkownCurrNum()const;
	public:
		void        init(const component::Loader* loader, Qstring & _stateInfo) override;
		void        solve() override;
		void        output() override;
		void        clear() override;
		void        reportMeshInfo(Qostream& strm) const override;
	protected:
		void		fillCoeffiMatSandT();
		//void		fillCoeffiMatSandTdiag();
		void		fillCoeffiMatQ();
		void		fillCoeffiMatArea();
		void		fillV();
		//bool        writeZIVData();
	};
	inline int ThreePhaseAC::getUnknownAScarNum()const
	{
		return m_unknownAScar;
	}
	inline int ThreePhaseAC::getUnkownCurrNum()const
	{
		return m_unknownCurrDensity;
	}

}