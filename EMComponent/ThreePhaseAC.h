#pragma once
#include <memory>
#include "EM.h"

namespace FEM {

	class ThreePhaseAC :public EM {
		using MeshPointer = std::shared_ptr<component::Mesh>;

	protected:
		std::set<int>			_m_currentDomains;
		std::set<int>			_m_dielectricDomains;

		Complex		J0_W_Delta;
		int         m_unknownAScar;
		int			m_unknownCurrDensity;
		int			triangleNum;

		Qsp_cx_mat	CoeffiMat;
		Qcx_vec		Asca_Jsk;
		Qcx_vec		V;

		EigenSparse_cx_mat		Eigen_CoeffiMat;
		EigenVector_cx_vec		Eigen_Asca_Jsk;
		EigenVector_cx_vec		Eigen_V;
		EigenTriplet_cx			Eigen_Triplet;

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
		void		fillEigenMatrix();
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