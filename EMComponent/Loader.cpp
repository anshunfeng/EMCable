#include "Loader.h"
#include <cctype>
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
	//  The first line is used to set solver
	if (!parseIsThreePhase())
	{
		stateInfo.assign("Judge is_ThreePhase failed");
		return false;
	}

	if (m_isThreePhase)
	{
		if (!parseThreePhase())
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

	auto maxSize = std::numeric_limits<std::streamsize>::max();
	Qstring iden;
	m_config_file.ignore(maxSize, ' ');
	m_config_file >> iden;
	m_config_file >> iden;
	if ((iden != "Three_Phase_Solver") && (iden != "DC_Solver"))
	{
		std::cout << "Change " << iden << " format to Three_Phase_Solver or DC_Solver" << std::endl;
		return false;
	}
	if (iden == "Three_Phase_Solver")
	{
		m_isThreePhase = true;
	}
	else
	{
		m_isThreePhase = false;
	}
	m_config_file.ignore(maxSize, '\n');
	return true;
}

//get()方法不会略过任何符号
//peek()方法预读取下一个字符（不管是何符号），不真正读取下一个字符

bool Loader::parseThreePhase()
{
	auto maxSize = std::numeric_limits<std::streamsize>::max();
	m_config_file.ignore(maxSize, ' ');
	Qstring iden;
	m_config_file >> iden;
	if (iden == "Frequency/Hz:")
	{
		m_config_file >> m_threephase.m_freq;
		m_config_file.ignore(maxSize, '\n');
	}
	else
	{
		std::cout << "Change " << iden << " format to Frequency/Hz: " << std::endl;
		return false;
	}
	m_config_file.ignore(maxSize, ' ');
	m_config_file >> iden;
	if (iden == "Amplitude/A:")
	{
		m_config_file >> m_threephase.m_amplitude;
		m_config_file.ignore(maxSize, '\n');
	}
	else
	{
		std::cout << "Change " << iden << " format to Amplitude/A: " << std::endl;
		return false;
	}
	m_config_file.ignore(maxSize, ' ');
	m_config_file >> iden;
	if (iden == "Phase/degree:")
	{
		m_config_file >> m_threephase.m_phase;
		m_config_file.ignore(maxSize, '\n');
	}
	else
	{
		std::cout << "Change " << iden << " format to Phase/degree: " << std::endl;
		return false;
	}
	m_config_file.ignore(maxSize, ' ');
	m_config_file >> iden;
	if (iden == "Dielectric_domains:")
	{
		while (m_config_file.peek() != '\n')
		{
			int _id;
			m_config_file >> _id;
			m_threephase.m_dielectricNum.insert(static_cast<int>(_id));
			
		}
		m_config_file.ignore(maxSize, '\n');
	}
	else
	{
		std::cout << "Change " << iden << " format to Dielectric_domains: " << std::endl;
		return false;
	}
	m_config_file.ignore(maxSize, ' ');
	m_config_file >> iden;
	if (iden == "Mental_sigma:")
	{
		m_config_file >> m_threephase.m_mentalSigma;
		m_config_file.ignore(maxSize, '\n');
	}
	else
	{
		std::cout << "Change " << iden << " format to Mental_sigma: " << std::endl;
		return false;
	}
	
	return true;
}
bool Loader::parseDC() 
{
	auto maxSize = std::numeric_limits<std::streamsize>::max();
	Qstring iden;
	m_config_file.ignore(maxSize, ' ');
	m_config_file >> iden;
	if (iden == "Amplitude/A:")
	{
		m_config_file >> m_dc.m_amplitude;
		m_config_file.ignore(maxSize, '\n');
	}
	else
	{
		std::cout << "Change " << iden << " format to Amplitude/A: " << std::endl;
		return false;
	}
	m_config_file.ignore(maxSize, ' ');
	m_config_file >> iden;
	if (iden == "Dielectric_domains:")
	{
		int  _id;
		while (m_config_file.peek() != '\n')
		{
			m_config_file >> _id;
			m_dc.m_dielectricNum.insert(_id);
		}
		m_config_file.ignore(maxSize, '\n');
	}
	else
	{
		std::cout << "Change " << iden << " format to Dielectric_domains: " << std::endl;
		return false;
	}
	m_config_file.ignore(maxSize, ' ');
	m_config_file >> iden;
	if (iden == "Mental_sigma:")
	{
		m_config_file >> m_dc.m_mentalSigma;
		m_config_file.ignore(maxSize, '\n');
	}
	else
	{
		std::cout << "Change " << iden << " format to Mental_sigma: " << std::endl;
		return false;
	}
	return true;
}
bool Loader::parsePath()
{
	auto maxSize = std::numeric_limits<std::streamsize>::max();
	Qstring iden;
	m_config_file.ignore(maxSize, ' ');
	m_config_file >> iden;
	if (iden == "MeshFile_path:")
	{
		getline(m_config_file, m_path);
	}
	else
	{
		std::cout << "Change " << iden << " format to MeshFile_path: " << std::endl;
		return false;
	}
	return true;
}

void component::Loader::report(Qostream & strm) const
{
	int old_flag = strm.flags();
	strm << std::left
		<< "Config Information:\n";
	if (m_isThreePhase)
	{
		strm << "  -> ThreePhase:\n"
			<< setw(30) << "    => Frequency: " << m_threephase.m_freq << "Hz" << '\n'
			<< setw(30) << "    => Amplitude: " << m_threephase.m_amplitude << "A" << '\n'
			<< setw(30) << "    => Phase: " << m_threephase.m_phase << "degree" << '\n'
			<< setw(30) << "    => Dielectric_domains: ";
		for (auto it = m_threephase.m_dielectricNum.begin(); it != m_threephase.m_dielectricNum.end(); ++it)
		{
			strm << *it << " ";
		}
		strm << "\n";
		strm << setw(30) << "    => Mental_sigma: " << m_threephase.m_mentalSigma << '\n';
		strm << setw(30) << "    => MeshFile_path: " << m_path << '\n';
	}

	else
	{
		strm << "  -> DC:\n"
			<< setw(30) << "    => Amplitude:" << m_dc.m_amplitude << "A" << '\n'
			<< setw(30) << "    => Dielectric_domains: ";
		for (auto it = m_dc.m_dielectricNum.begin(); it != m_dc.m_dielectricNum.end(); ++it)
		{
			strm << *it << " ";
		}
		strm << "\n";
		strm << setw(30) << "    => Mental_sigma: " << m_dc.m_mentalSigma << '\n';
		strm << setw(30) << "    => MeshFile_path: " << m_path << '\n';
	}
	strm.flags(old_flag);
}
