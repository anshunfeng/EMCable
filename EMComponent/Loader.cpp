#include "Loader.h"
#include <cctype>//字符处理库
#include <numeric>
#include <iomanip>
#include <sstream>

using namespace component;
using std::setw;
Loader::Loader()
{
}
Loader::~Loader()
{
	m_config_file.close();
}

bool Loader::load(const Qstring & filepath, Qstring & stateInfo)
{
	m_config_file.open(filepath, std::ios::in);
	if (m_config_file.fail())
	{
		stateInfo.assign("open config file failed");
		return false;
	}
	auto maxSize = std::numeric_limits<std::streamsize>::max();
	m_config_file.ignore(maxSize, '\n');    //  The first line is used to set solver
	if (!parseIsThreePhase())
	{
		stateInfo.assign("Judge is_ThreePhase failed");
		return false;
	}

	if (m_isThreePhase)
	{
		if (!parseThreephase())
		{
			stateInfo.assign("parse ThreePhase faild");
			return false;
		}
	}
	else
	{
		if (!parseDC())
		{
			stateInfo.assign("parse DC failed");
			return false;
		}
	}

	if (!parsePath())
	{
		stateInfo.assign("parse mesh path failed");
		return false;
	}

	report(Qcout);
	report(m_log);
	return true;
}
bool Loader::parseIsThreePhase()
{
	Qstring iden;
	m_config_file >> iden;
	if (iden != "isThreePhase:")
		return false;
	m_config_file >> iden;
	if (iden == "Yes")
		m_isThreePhase = true;
	else
	{
		if (iden == "No")
			m_isThreePhase = false;
		else
			return false;
	}
	return true;
}

//get()方法不会略过任何符号
//peek()方法预读取下一个字符（不管是何符号），不真正读取下一个字符


bool Loader::parseThreephase()
{
	auto maxSize = std::numeric_limits<std::streamsize>::max();
	Qstring iden;
	m_config_file >> iden;
	if (iden == "freq:")
	{
		m_config_file >> m_threephase.m_freq;
		m_config_file.ignore(maxSize, '\n');
	}
	else
		return false;

	m_config_file >> iden;
	if (iden == "amplitude:")
	{
		m_config_file >> m_threephase.m_amplitude;
		m_config_file.ignore(maxSize, '\n');
	}
	else
		return false;

	m_config_file >> iden;
	if (iden == "phase:")
	{
		m_config_file >> m_threephase.m_phase;
		m_config_file.ignore(maxSize, '\n');
	}
	else
		return false;
	return true;
}
bool Loader::parseDC() 
{
	auto maxSize = std::numeric_limits<std::streamsize>::max();
	Qstring iden;
	m_config_file >> iden;
	if (iden == "amplitude:")
	{
		m_config_file >> m_dc.m_amplitude;
		m_config_file.ignore(maxSize, '\n');
	}
	else
		return false;
	return true;
}
bool Loader::parsePath()
{
	
	Qstring iden;
	m_config_file >> iden;
	if (iden == "path:")
		m_config_file >> m_path;
	else
		return false;
	return true;
}

void component::Loader::report(Qostream & strm) const
{
	int old_flag = strm.flags();
	strm << std::left
		<< "Config Information:\n";
	if (m_isThreePhase)
		strm << "  -> ThreePhase:\n"
		<< setw(30) << "    => frequency:" << m_threephase.m_freq << "Hz" << '\n'
		<< setw(30) << "    => Amplitude:" << m_threephase.m_amplitude << "A" << '\n'
		<< setw(30) << "    => phase:" << m_threephase.m_phase << "degree" << '\n';
	else
		strm << "  -> DC:\n"
		<< setw(30) << "    => Amplitude:" << m_dc.m_amplitude << "A" << '\n';
	
	strm << setw(30) << "  -> File path: " << m_path << '\n';
	strm.flags(old_flag);
}
