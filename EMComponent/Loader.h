#pragma once
#include "fieldStruct.h"
#include <sstream>
#include <iomanip>
namespace component {

	class Loader {

	protected:
		Qfstream    m_config_file;//�ļ���
		Qsstream    m_log;//�ַ���

		ThreePhaseSource	 m_threephase;
		DCSource			 m_dc;
		Qstring				 m_path;
		bool			  	 m_isThreePhase;

	public:
		Loader();
		virtual ~Loader();
		virtual bool    load(const Qstring& filepath, Qstring& stateInfo);

		Qstring				   getFilePath() const;//����
		ThreePhaseSource       getThreephase() const;//����
		DCSource		       getDC() const;//����
		Qstring				   getReport() const;//����					
	protected:
		//void    parseHelper();
		bool    parseIsThreePhase();
		bool    parseThreePhase();
		bool    parseDC();
		bool    parsePath();
		void    report(Qostream& strm) const;

	
	};
	inline Qstring Loader::getFilePath() const
	{
		return m_path;
	}

	inline ThreePhaseSource Loader::getThreephase() const
	{
		return m_threephase;
	}

	inline DCSource Loader::getDC() const
	{
		return m_dc;
	}

	inline Qstring Loader::getReport() const
	{
		return m_log.str();
	}

} 