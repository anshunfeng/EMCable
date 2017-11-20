#pragma once
#include "fieldStruct.h"
#include <sstream>
#include <iomanip>
// 该类支持各种流操作
namespace component {

	class Loader {
	public:
		Loader();
		virtual ~Loader();	//虚析构函数，作为基类
		
		//调用了所有protected成员函数，虚函数
		virtual bool    load(const Qstring& filepath, Qstring& stateInfo);

		Qstring				   getFilePath() const;//内联
		ThreePhaseSource       getThreephase() const;//内联
		DCSource		       getDC() const;//内联
		Qstring				   getReport() const;//内联					
	protected:
		//void    parseHelper();
		bool    parseIsThreePhase();

		bool    parseThreephase();
		bool    parseDC();
		bool    parsePath();
		void    report(Qostream& strm) const;

	protected:
		Qfstream    m_config_file;//文件流
		Qsstream    m_log;//字符流

		ThreePhaseSource	 m_threephase;
		DCSource			 m_dc;
		Qstring				 m_path;
		bool			  	 m_isThreePhase;
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
		return m_log.str();//得到字符串
	}

} // namespace component