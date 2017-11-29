#pragma once
#include "fieldStruct.h"
#include <sstream>
#include <iomanip>
namespace component {

	class Loader {

	protected:
		Qfstream    m_config_file;//文件流
		Qsstream    m_log;//字符流

		ThreePhaseSource		m_threephase;
		DCSource				m_dc;
		Qstring					m_path;
		bool			  		m_isThreePhase;

		std::set<int>			m_currentDomains;
		std::set<int>			m_dielectricDomains;
	public:
		Loader();
		virtual ~Loader();//虚析构函数
		virtual bool    load(const Qstring& filepath, Qstring& stateInfo);//虚函数

		Qstring				   getFilePath() const;//内联
		ThreePhaseSource       getThreephase() const;//内联
		DCSource		       getDC() const;//内联
		Qstring				   getReport() const;//内联	
		bool				   getIsThreePhase()const;//内联
		int					   getCurrDomainsNum()const;//内联
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