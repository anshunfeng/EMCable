#pragma once
#include "fieldStruct.h"
#include <sstream>
#include <iomanip>
namespace component {

	class Loader {

	protected:
		Qfstream    m_config_file;//�ļ���
		Qsstream    m_log;//�ַ���

		ThreePhaseSource		m_threephase;
		DCSource				m_dc;
		Qstring					m_path;
		bool			  		m_isThreePhase;

		std::set<int>			m_currentDomains;
		std::set<int>			m_dielectricDomains;
	public:
		Loader();
		virtual ~Loader();//����������
		virtual bool    load(const Qstring& filepath, Qstring& stateInfo);//�麯��

		Qstring				   getFilePath() const;//����
		ThreePhaseSource       getThreephase() const;//����
		DCSource		       getDC() const;//����
		Qstring				   getReport() const;//����	
		bool				   getIsThreePhase()const;//����
		int					   getCurrDomainsNum()const;//����
		std::set<int>		   get_m_currentDomains()const;
		std::set<int>		   get_m_dielectricDomains()const;
	protected:
		//void    parseHelper();
		bool    parseIsThreePhase();
		bool    parseThreePhase();
		bool    parseDC();
		bool    parsePath();
		void    report(Qostream& strm) const;

	
	};
	inline std::set<int> Loader::get_m_currentDomains()const
	{
		return m_currentDomains;
	}
	inline std::set<int> Loader::get_m_dielectricDomains()const
	{
		return m_dielectricDomains;
	}
	
	inline int  Loader::getCurrDomainsNum()const
	{
		return static_cast<int>(m_currentDomains.size());
	}
	inline bool Loader::getIsThreePhase()const
	{ return m_isThreePhase; }
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