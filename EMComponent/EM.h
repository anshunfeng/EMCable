#pragma once
#include <vector>
#include "EMBase.h"
#include "MyFWD.h"
#include "Custom.h"
#include "VectorR2.h"
#include "fieldStruct.h"
using namespace component;
namespace FEM {

	using component::ThreePhaseSource;
	using component::DCSource;
	using component::VectorR2;

	class EM : public EMBase {

	protected:
		Qstring					m_dir;
		Qstring					m_foldername;
		Qofstream				m_log;//文件输出流
		bool			  		m_isThreePhase;
		ThreePhaseSource		m_threephase;
		DCSource				m_dc;

	public:
		EM();
		~EM();

	public:
		virtual void    init(const component::Loader* loader, Qstring & _stateInfo);
		virtual void    solve();
		virtual void    output();
		virtual void    clear();
		virtual void    reportMeshInfo(Qostream& strm) const;


	};


}