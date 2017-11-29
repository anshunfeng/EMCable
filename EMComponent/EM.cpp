#include <iomanip>
#include "EM.h"
#include "Loader.h"
#include "Mathdef.h"
using namespace component;
using namespace FEM;

EM::EM()
{
	m_log << std::left;
}
EM::~EM()
{
}

void EM::init(const component::Loader* loader, Qstring & _stateInfo)
{

	m_isThreePhase = loader->getIsThreePhase();
	
	if (m_isThreePhase)
	{
		m_threephase = loader->getThreephase();
	}
	else
	{
		m_dc = loader->getDC();
	}
	auto mesh_file = loader->getFilePath();
	auto pos_suffix = mesh_file.find_last_of('.');
	auto pos_dir = mesh_file.find_last_of('\\');
	
	if (pos_dir != Qstring::npos)
	{
	
		m_dir = mesh_file.substr(0, pos_dir+1);
		m_foldername = mesh_file.substr(pos_dir + 1, pos_suffix - pos_dir - 1);
	}
	/*Qcout  << std::endl;
	Qcout << m_dir << std::endl;
	Qcout << m_foldername << std::endl;
	*/
}

void EM::solve()
{
}

void EM::output()
{
}

void EM::clear()
{
}

void EM::reportMeshInfo(Qostream & strm) const
{
}


