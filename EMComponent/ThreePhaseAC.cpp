#include <string>
#include "ThreePhaseAC.h"
#include "Loader.h"
#include "Mathdef.h"
#include "tools.h"
#include "Mesh.h"
#include "MatrixSolver.h"

using namespace component;
using namespace FEM;
using namespace Eigen;
using std::setw;

ThreePhaseAC::ThreePhaseAC()
{
}
ThreePhaseAC::~ThreePhaseAC()
{
	tool::totalTime(m_log);
}

void		ThreePhaseAC::init(const component::Loader* loader, Qstring & _stateInfo)
{
	EM::init(loader, _stateInfo);
	if (m_isThreePhase)
	{
		m_dir = tool::creatFolder(m_dir, m_foldername + "_ThreePhaseSolver");
	}
	else
	{
		std::cout << "fail start ThreePhaseSolve" << std::endl;
		return;
	}
	auto log_name = m_dir + "_" + "runtime_log.txt";
	m_log.open(log_name);
	if (!m_log)
	{
		Qcout << "fail creating runtime_log.txt" << std::endl;
	}
	// 配置信息
	m_log << loader->getReport();
	_m_currentDomains = loader->get_m_currentDomains();
	_m_dielectricDomains = loader->get_m_dielectricDomains();
	pMesh = std::make_shared<Mesh>();//实例化
	if (!pMesh->loadMeshFile(loader->getFilePath(), _stateInfo))
		return;
	m_unknownAScar = pMesh->getVertexUnkownNum();
	m_unknownCurrDensity = loader->getCurrDomainsNum();
	triangleNum = pMesh->getTriangleNum();
	J0_W_Delta = J0*PI2*m_threephase.m_freq*m_threephase.m_mentalSigma;
	//网格信息
	reportMeshInfo(m_log);
	//写入init耗时
	tool::elapsedTime(m_log, "end init");
}
void		ThreePhaseAC::fillV()
{
	V.zeros(m_unknownAScar + m_unknownCurrDensity);
	value_t currentA = m_threephase.m_amplitude*cos(m_threephase.m_phase*DegreesToRadians);
	value_t currentB = m_threephase.m_amplitude*cos(m_threephase.m_phase*DegreesToRadians - 120 * DegreesToRadians);
	value_t currentC = m_threephase.m_amplitude*cos(m_threephase.m_phase*DegreesToRadians - 240 * DegreesToRadians);
	V(m_unknownAScar) = currentA;
	V(m_unknownAScar + 1) = currentB;
	V(m_unknownAScar + 2) = currentC;
}
void		ThreePhaseAC::fillCoeffiMatArea()
{
	std::set<int>::iterator pos = _m_currentDomains.begin();
	for (int i = 0; i < m_unknownCurrDensity; ++i)
	{
		for (int j = 0; j < triangleNum; ++j)
		{
			if (pMesh->getTriangleRef(j).getSunbdomainID() == *pos)
			{
				CoeffiMat(m_unknownAScar + i, m_unknownAScar + i) += J0_W_Delta*pMesh->getTriangleRef(j).getTriArea();
			}
		}
		++pos;
	}

}
void		ThreePhaseAC::fillCoeffiMatQ()
{
	int count = 0;
	for (auto pos = _m_currentDomains.begin(); pos != _m_currentDomains.end(); ++pos)
	{
		for (int k = 0; k < triangleNum; ++k)
		{
			if (pMesh->getTriangleRef(k).getSunbdomainID() == *pos)
			{
				Triangle& tri = pMesh->getTriangleRef(k);
				int _nA, _nB, _nC;
				tri.getnAnBnC(_nA, _nB, _nC);
				bool nA_isBoundary = pMesh->isBoundaryPoint(_nA);
				bool nB_isBoundary = pMesh->isBoundaryPoint(_nB);
				bool nC_isBoundary = pMesh->isBoundaryPoint(_nC);

				Complex tempQ = -J0_W_Delta*(tri.getTriArea() / 3);
				if (!nA_isBoundary)
				{
					int _orderA = pMesh->getUnknownVertexOderbyIndex(_nA);
					CoeffiMat(_orderA, m_unknownAScar + count) += tempQ;
					CoeffiMat(m_unknownAScar + count, _orderA) += tempQ;
				}
				if (!nB_isBoundary)
				{
					int _orderB = pMesh->getUnknownVertexOderbyIndex(_nB);
					CoeffiMat(_orderB, m_unknownAScar + count) += tempQ;
					CoeffiMat(m_unknownAScar + count, _orderB) += tempQ;
				}
				if (!nC_isBoundary)
				{
					int _orderC = pMesh->getUnknownVertexOderbyIndex(_nC);
					CoeffiMat(_orderC, m_unknownAScar + count) += tempQ;
					CoeffiMat(m_unknownAScar + count, _orderC) += tempQ;
				}
			}
		}//for k
		++count;
	}//for pos
}
void		ThreePhaseAC::fillCoeffiMatSandT()
{
	for (int k = 0; k < triangleNum; k++)
	{
		Triangle tri = pMesh->getTriangleRef(k);
		int _nA, _nB, _nC;
		tri.getnAnBnC(_nA, _nB, _nC);
		value_t treArea = tri.getTriArea();
		bool nA_isBoundary = pMesh->isBoundaryPoint(_nA);
		bool nB_isBoundary = pMesh->isBoundaryPoint(_nB);
		bool nC_isBoundary = pMesh->isBoundaryPoint(_nC);

		bool isDielectric = _m_dielectricDomains.find(tri.getSunbdomainID()) != _m_dielectricDomains.end();
		bool isCondunctor = !(isDielectric);
		Complex tempResultT = J0_W_Delta*(treArea / 12);
		value_t tempSCoeffi = (1 / (mu0))*(0.25 / (treArea));

		value_t na1, na2, na3, nb1, nb2, nb3, nc1, nc2, nc3;
		int _orderA, _orderB, _orderC;

		if (!nA_isBoundary)
		{
			_orderA = pMesh->getUnknownVertexOderbyIndex(_nA);

			tri.getna123(na1, na2, na3);
			value_t tempResultS = tempSCoeffi*(na2*na2 + na3*na3);
			CoeffiMat(_orderA, _orderA) += tempResultS;
			if (isCondunctor)
			{
				CoeffiMat(_orderA, _orderA) += tempResultT;
			}
		}
		if (!nB_isBoundary)
		{
			_orderB = pMesh->getUnknownVertexOderbyIndex(_nB);
			tri.getnb123(nb1, nb2, nb3);
			value_t tempResultS = tempSCoeffi*(nb2*nb2 + nb3*nb3);
			CoeffiMat(_orderB, _orderB) += tempResultS;
			if (isCondunctor)
			{
				CoeffiMat(_orderB, _orderB) += tempResultT;
			}
		}
		if (!nC_isBoundary)
		{
			_orderC = pMesh->getUnknownVertexOderbyIndex(_nC);
			tri.getnc123(nc1, nc2, nc3);
			value_t tempResultS = tempSCoeffi*(nc2*nc2 + nc3*nc3);
			CoeffiMat(_orderC, _orderC) += tempResultS;
			if (isCondunctor)
			{
				CoeffiMat(_orderC, _orderC) += tempResultT;
			}
		}
		if ((!nA_isBoundary) && (!nB_isBoundary))
		{
			value_t tempResultS = tempSCoeffi*(na2*nb2 + na3*nb3);

			CoeffiMat(_orderA, _orderB) += tempResultS;
			CoeffiMat(_orderB, _orderA) += tempResultS;
			if (isCondunctor)
			{
				CoeffiMat(_orderA, _orderB) += tempResultT;
				CoeffiMat(_orderB, _orderA) += tempResultT;
			}
		}//AB

		if ((!nA_isBoundary) && (!nC_isBoundary))
		{
			value_t tempResultS = tempSCoeffi*(na2*nc2 + na3*nc3);

			CoeffiMat(_orderA, _orderC) += tempResultS;
			CoeffiMat(_orderC, _orderA) += tempResultS;
			if (isCondunctor)
			{
				CoeffiMat(_orderA, _orderC) += tempResultT;
				CoeffiMat(_orderC, _orderA) += tempResultT;
			}

		}//AC

		if ((!nB_isBoundary) && (!nC_isBoundary))
		{
			value_t tempResultS = tempSCoeffi*(nb2*nc2 + nb3*nc3);

			CoeffiMat(_orderB, _orderC) += tempResultS;
			CoeffiMat(_orderC, _orderB) += tempResultS;
			if (isCondunctor)
			{
				CoeffiMat(_orderB, _orderC) += tempResultT;
				CoeffiMat(_orderC, _orderB) += tempResultT;
			}
		}
	}//for k

}
void		ThreePhaseAC::fillEigenMatrix()
{
	Eigen_CoeffiMat.resize(m_unknownAScar + m_unknownCurrDensity, m_unknownAScar + m_unknownCurrDensity);
	Eigen_V.resize(m_unknownAScar + m_unknownCurrDensity);
	for (auto it = CoeffiMat.begin(); it != CoeffiMat.end(); it++)
	{
		Eigen_Triplet.emplace_back(static_cast<int>(it.col()), static_cast<int>(it.row()), *it);
	}
	//Qcout << " bug" << std::endl;
	Eigen_CoeffiMat.setFromTriplets(Eigen_Triplet.begin(), Eigen_Triplet.end());
	int count = 0;
	for (auto _it = V.begin(); _it != V.end(); _it++)
	{
		Eigen_V(count) = *_it;
		++count;
	}
}

void        ThreePhaseAC::solve()
{
	CoeffiMat.zeros(m_unknownAScar + m_unknownCurrDensity, m_unknownAScar + m_unknownCurrDensity);
	Qcout << setw(30) << "Filling V matrix:";
	fillV();
	tool::elapsedTime(m_log, "end fillV");
	Qcout << std::endl;

	Qcout << setw(30) << "Filling Area matrix:";
	fillCoeffiMatArea();
	tool::elapsedTime(m_log, "end fillArea");
	Qcout << std::endl;

	Qcout << setw(30) << "Filling Q matrix:";
	fillCoeffiMatQ();
	Qcout << std::endl;
	tool::elapsedTime(m_log, "end fillQ");

	Qcout << setw(30) << "Filling S_T matrix:";
	fillCoeffiMatSandT();
	Qcout << std::endl;
	tool::elapsedTime(m_log, "end fillSandT");

	Qcout << setw(30) << "Filling fillEigen matrix:";
	fillEigenMatrix();
	Qcout << std::endl;
	tool::elapsedTime(m_log, "end fillEigen matrix");

	Qcout << setw(30) << "Solving matrix equation:";

	//SuperLU Method
	SparseLU<SparseMatrix<Complex, ColMajor>, COLAMDOrdering<int> >   solver;
	// Compute the ordering permutation vector from the structural pattern of A
	solver.analyzePattern(Eigen_CoeffiMat);
	// Compute the numerical factorization 
	solver.factorize(Eigen_CoeffiMat);
	//Use the factors to solve the linear system 
	Eigen_Asca_Jsk.resize(m_unknownAScar + m_unknownCurrDensity);
	Eigen_Asca_Jsk = solver.solve(Eigen_V);



	//// QR Method
	//Eigen_Asca_Jsk.resize(m_unknownAScar + m_unknownCurrDensity);
	//SparseQR<SparseMatrix<Complex>, COLAMDOrdering<int>> QR;
	//QR.compute(Eigen_CoeffiMat);
	//
	//Eigen_Asca_Jsk = QR.solve(Eigen_V);

	Qcout  << " finished" << std::endl;
	tool::elapsedTime(m_log, "end solve");

}
void        ThreePhaseAC::output()
{
	Qcout << setw(30) << "Output results:";
	std::map<int, Complex> currDomainJs;
	int A_Jsize = static_cast<int>(Eigen_Asca_Jsk.size());
	int _count = 0;
	for (auto it = _m_currentDomains.begin(); it != _m_currentDomains.end(); it++)
	{
		currDomainJs.insert(std::make_pair(*it, Eigen_Asca_Jsk(A_Jsize + _count - 3)));
			++_count;
	}
	//use for matlab patching  total current including eddy current and source current
	auto patch_NoAir_total_J = m_dir + "_" + "Eigen_results_patch_NoAir_total_J.txt";
	auto patch_NoAir_eddy_J = m_dir + "_" + "Eigen_results_patch_NoAir_eddy_J.txt";
	Qofstream out1;
	Qofstream out2;
	out1.open(patch_NoAir_total_J);
	out2.open(patch_NoAir_eddy_J);
	if (!out1)
	{
		Qcout << "fail creating Eigen_results_patch_NoAir_total_J.txt" << std::endl;
	}
	if (!out2)
	{
		Qcout << "fail creating Eigen_results_patch_NoAir_eddy_J.txt" << std::endl;
	}
	std::stringstream nodeX1, nodeY1, nodeX2, nodeY2, value_total_J, value_eddy_J;
	for (int k = 0; k < triangleNum; k++)
	{
		Triangle& tri = pMesh->getTriangleRef(k);
		int triId = tri.getSunbdomainID();
		if (triId != 1)
		{
			int _nA, _nB, _nC;
			tri.getnAnBnC(_nA, _nB, _nC);
			if (_m_dielectricDomains.find(triId) != _m_dielectricDomains.end())
			{
				nodeX1 << pMesh->getVertex(_nA).x << '\t' << pMesh->getVertex(_nB).x << '\t' << pMesh->getVertex(_nC).x << '\n';
				nodeY1 << pMesh->getVertex(_nA).y << '\t' << pMesh->getVertex(_nB).y << '\t' << pMesh->getVertex(_nC).y << '\n';
				nodeX2 << pMesh->getVertex(_nA).x << '\t' << pMesh->getVertex(_nB).x << '\t' << pMesh->getVertex(_nC).x << '\n';
				nodeY2 << pMesh->getVertex(_nA).y << '\t' << pMesh->getVertex(_nB).y << '\t' << pMesh->getVertex(_nC).y << '\n';

				value_total_J << 0 << '\t' << 0 << '\t' << 0 << '\n';
				value_eddy_J << 0 << '\t' << 0 << '\t' << 0 << '\n';
			}
			else
			{
				if (_m_currentDomains.find(triId) != _m_currentDomains.end())
				{
					nodeX1 << pMesh->getVertex(_nA).x << '\t' << pMesh->getVertex(_nB).x << '\t' << pMesh->getVertex(_nC).x << '\n';
					nodeY1 << pMesh->getVertex(_nA).y << '\t' << pMesh->getVertex(_nB).y << '\t' << pMesh->getVertex(_nC).y << '\n';
					nodeX2 << pMesh->getVertex(_nA).x << '\t' << pMesh->getVertex(_nB).x << '\t' << pMesh->getVertex(_nC).x << '\n';
					nodeY2 << pMesh->getVertex(_nA).y << '\t' << pMesh->getVertex(_nB).y << '\t' << pMesh->getVertex(_nC).y << '\n';

					//total current
					value_t tmpValueA_total_J = abs(-J0_W_Delta*Eigen_Asca_Jsk(pMesh->getUnknownVertexOderbyIndex(_nA)) + J0_W_Delta*currDomainJs.find(triId)->second);
					value_t tmpValueB_total_J = abs(-J0_W_Delta*Eigen_Asca_Jsk(pMesh->getUnknownVertexOderbyIndex(_nB)) + J0_W_Delta*currDomainJs.find(triId)->second);
					value_t tmpValueC_total_J = abs(-J0_W_Delta*Eigen_Asca_Jsk(pMesh->getUnknownVertexOderbyIndex(_nC)) + J0_W_Delta*currDomainJs.find(triId)->second);
					//eddy current
					value_t tmpValueA_eddy_J = abs(-J0_W_Delta*Eigen_Asca_Jsk(pMesh->getUnknownVertexOderbyIndex(_nA)));
					value_t tmpValueB_eddy_J = abs(-J0_W_Delta*Eigen_Asca_Jsk(pMesh->getUnknownVertexOderbyIndex(_nB)));
					value_t tmpValueC_eddy_J = abs(-J0_W_Delta*Eigen_Asca_Jsk(pMesh->getUnknownVertexOderbyIndex(_nC)));
					value_total_J << tmpValueA_total_J << '\t' << tmpValueB_total_J << '\t' << tmpValueC_total_J << '\n';
					value_eddy_J << tmpValueA_eddy_J << '\t' << tmpValueB_eddy_J << '\t' << tmpValueC_eddy_J << '\n';
				}
				else
				{
					nodeX1 << pMesh->getVertex(_nA).x << '\t' << pMesh->getVertex(_nB).x << '\t' << pMesh->getVertex(_nC).x << '\n';
					nodeY1 << pMesh->getVertex(_nA).y << '\t' << pMesh->getVertex(_nB).y << '\t' << pMesh->getVertex(_nC).y << '\n';
					nodeX2 << pMesh->getVertex(_nA).x << '\t' << pMesh->getVertex(_nB).x << '\t' << pMesh->getVertex(_nC).x << '\n';
					nodeY2 << pMesh->getVertex(_nA).y << '\t' << pMesh->getVertex(_nB).y << '\t' << pMesh->getVertex(_nC).y << '\n';

					value_t tmpValueA = abs(-J0_W_Delta*Eigen_Asca_Jsk(pMesh->getUnknownVertexOderbyIndex(_nA)));
					value_t tmpValueB = abs(-J0_W_Delta*Eigen_Asca_Jsk(pMesh->getUnknownVertexOderbyIndex(_nB)));
					value_t tmpValueC = abs(-J0_W_Delta*Eigen_Asca_Jsk(pMesh->getUnknownVertexOderbyIndex(_nC)));
					value_total_J << tmpValueA << '\t' << tmpValueB << '\t' << tmpValueC << '\n';
					value_eddy_J << tmpValueA << '\t' << tmpValueB << '\t' << tmpValueC << '\n';
				}
			}
		}
	}
	out1 << nodeX1.rdbuf() << nodeY1.rdbuf() << value_total_J.rdbuf();
	//out2 <<  value_eddy_J.rdbuf();
	out2 << nodeX2.rdbuf() << nodeY2.rdbuf() << value_eddy_J.rdbuf();
	//Qcout << "flags" << '\n';
	out1.close();
	out2.close();

	//use for matlab patching No air domains' Asca
	auto patch_NoAir = m_dir + "_" + "Eigen_results_patch_NoAir_Asca.txt";
	Qofstream out3;
	out3.open(patch_NoAir);
	if (!out3)
	{
		Qcout << "fail creating Eigen_results_patch_NoAir_Asca.txt" << std::endl;
	}
	std::stringstream NodeX, NodeY, Value;
	for (int k = 0; k < triangleNum; k++)
	{
		Triangle& tri = pMesh->getTriangleRef(k);
		if (tri.getSunbdomainID() != 1)
		{
			int _nA, _nB, _nC;
			tri.getnAnBnC(_nA, _nB, _nC);
			bool nA_isBoundary = pMesh->isBoundaryPoint(_nA);
			bool nB_isBoundary = pMesh->isBoundaryPoint(_nB);
			bool nC_isBoundary = pMesh->isBoundaryPoint(_nC);
			NodeX << pMesh->getVertex(_nA).x << '\t' << pMesh->getVertex(_nB).x << '\t' << pMesh->getVertex(_nC).x << '\n';
			NodeY << pMesh->getVertex(_nA).y << '\t' << pMesh->getVertex(_nB).y << '\t' << pMesh->getVertex(_nC).y << '\n';
			if (nA_isBoundary)
			{
				Value << 0 << '\t';
			}
			else
			{
				Value << abs(Eigen_Asca_Jsk(pMesh->getUnknownVertexOderbyIndex(_nA))) << '\t';
			}
			if (nB_isBoundary)
			{
				Value << 0 << '\t';
			}
			else
			{
				Value << abs(Eigen_Asca_Jsk(pMesh->getUnknownVertexOderbyIndex(_nB))) << '\t';
			}
			if (nC_isBoundary)
			{
				Value << 0 << '\t';
			}
			else
			{
				Value << abs(Eigen_Asca_Jsk(pMesh->getUnknownVertexOderbyIndex(_nC))) << '\n';
			}
		}
	}
	out3 << NodeX.rdbuf() << NodeY.rdbuf() << Value.rdbuf();
	out3.close();

	//use for matlab patching all domains' Asca
	auto patch_name = m_dir + "_" + "Eigen_results_patch_all_Asca.txt";
	Qofstream out4;
	out4.open(patch_name);
	if (!out4)
	{
		Qcout << "fail creating Eigen_results_patch_all_Asca.txt" << std::endl;
	}
	std::stringstream tmpNodeX, tmpNodeY, tmpValue;
	for (int k = 0; k < triangleNum; k++) 
	{
		Triangle& tri = pMesh->getTriangleRef(k);
		int _nA, _nB, _nC;
		tri.getnAnBnC(_nA, _nB, _nC);
		bool nA_isBoundary = pMesh->isBoundaryPoint(_nA);
		bool nB_isBoundary = pMesh->isBoundaryPoint(_nB);
		bool nC_isBoundary = pMesh->isBoundaryPoint(_nC);
		tmpNodeX << pMesh->getVertex(_nA).x << '\t' << pMesh->getVertex(_nB).x << '\t' << pMesh->getVertex(_nC).x << '\n';
		tmpNodeY << pMesh->getVertex(_nA).y << '\t' << pMesh->getVertex(_nB).y << '\t' << pMesh->getVertex(_nC).y << '\n';
		if (nA_isBoundary)
		{
			tmpValue << 0 << '\t';
		}
		else
		{
			tmpValue << abs(Eigen_Asca_Jsk(pMesh->getUnknownVertexOderbyIndex(_nA))) << '\t';
		}
		if (nB_isBoundary)
		{
			tmpValue << 0 << '\t';
		}
		else
		{
			tmpValue << abs(Eigen_Asca_Jsk(pMesh->getUnknownVertexOderbyIndex(_nB))) << '\t';
		}
		if (nC_isBoundary)
		{
			tmpValue << 0 << '\n';
		}
		else
		{
			tmpValue << abs(Eigen_Asca_Jsk(pMesh->getUnknownVertexOderbyIndex(_nC))) << '\n';
		}
	}
	out4 << tmpNodeX.rdbuf() << tmpNodeY.rdbuf() << tmpValue.rdbuf();
	out4.close();
	//use for matlab surf/clabel all domains Asca picture
	auto Eigen_result_Asca_name = m_dir + "_" + "Eigen_results_Asca.txt";
	Qofstream out5;
	out5.open(Eigen_result_Asca_name);
	if (!out5)
	{
		Qcout << "fail creating Eigen_results_Asca.txt" << std::endl;
	}
	
	for (int i = 0; i < Eigen_Asca_Jsk.size() - 3; i++)
	{
			int _index = pMesh->getUnkownVertexIndex(i);
			out5 << _index << '\t' << pMesh->getVertex(_index).x << '\t' << pMesh->getVertex(_index).y << '\t' << abs(Eigen_Asca_Jsk(i)) << std::endl;
	}
	for (int j = 0; j < pMesh->getVertexNum(); j++)
	{
		if (pMesh->isBoundaryPoint(j))
		{
			out5 << j << '\t' << pMesh->getVertex(j).x << '\t' << pMesh->getVertex(j).y << '\t' << 0 << std::endl;
		}
	}
	out5.close();
	// finished flag
	Qcout << "finished" << std::endl;
	tool::elapsedTime(m_log, "end output");
}
void        ThreePhaseAC::clear()
{
	/*S.reset();
	T.reset();
	Q.reset();
	Area.reset();*/
	CoeffiMat.reset();
	V.reset();
	Asca_Jsk.reset();
	
	pMesh->clear();
}
void        ThreePhaseAC::reportMeshInfo(Qostream& strm)const
{
	pMesh->reportInfo(strm);
}
		

