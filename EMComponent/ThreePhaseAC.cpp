#include <string>
#include "ThreePhaseAC.h"
#include "Loader.h"
#include "Mathdef.h"
#include "tools.h"
#include "Mesh.h"
#include "MatrixSolver.h"

using namespace component;
using namespace FEM;
using namespace iml;
using std::setw;

ThreePhaseAC::ThreePhaseAC()
{

}

ThreePhaseAC::~ThreePhaseAC()
{
	tool::totalTime(m_log);
}
void  ThreePhaseAC::init(const component::Loader* loader, Qstring & _stateInfo)
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
	//网格信息
	reportMeshInfo(m_log);
	//写入init耗时
	tool::elapsedTime(m_log, "end init");
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


	/*Qcout << setw(30) << "Filling S_Tdiag matrix:";
	fillCoeffiMatSandTdiag();
	Qcout << std::endl;
	tool::elapsedTime(m_log, "end fillSandTdiag");
*/
	Qcout << setw(30) << "Filling S_T matrix:";
	fillCoeffiMatSandT();
	Qcout << std::endl;
	tool::elapsedTime(m_log, "end fillSandT");

	Qcout << setw(30) << "Solving matrix equation:";

	/*Qcx_mat CoeffiMat;
	CoeffiMat.zeros(m_unknownAScar + m_unknownCurrDensity, m_unknownAScar + m_unknownCurrDensity);
	CoeffiMat(arma::span(0, m_unknownAScar - 1), arma::span(0, m_unknownAScar - 1)) =
	(1/(mu0))*Complex(1, 0)*S + J0*PI2*m_threephase.m_freq*m_threephase.m_mentalSigma*T;
	CoeffiMat(arma::span(0, m_unknownAScar - 1), arma::span(m_unknownAScar, m_unknownAScar + m_unknownCurrDensity - 1)) =
	-J0*PI2*m_threephase.m_freq*m_threephase.m_mentalSigma*Q;
	CoeffiMat(arma::span(m_unknownAScar, m_unknownAScar + m_unknownCurrDensity - 1), arma::span(0, m_unknownAScar - 1)) =
	-J0*PI2*m_threephase.m_freq*m_threephase.m_mentalSigma*trans(Q);
	CoeffiMat(arma::span(m_unknownAScar, m_unknownAScar + m_unknownCurrDensity - 1), arma::span(m_unknownAScar, m_unknownAScar + m_unknownCurrDensity - 1)) =
	J0*PI2*m_threephase.m_freq*m_threephase.m_mentalSigma*Area;*/
	/*int _iterNum;
	if (!CGNE(Asca_Jsk, CoeffiMat, V, 1e-6, 5, _iterNum))
	{
		Qcerr << "ERROR: fail solving CoeffiMat*Asca_Jsk = V" << std::endl;
		return;
	}*/

	if (!arma::solve(Asca_Jsk, CoeffiMat, V, arma::solve_opts::fast))
	{
	Qcerr << "ERROR: fail solving CoeffiMat*Asca_Jsk = V" << std::endl;
	return;
	}
	Qcout  << " finished" << std::endl;
	tool::elapsedTime(m_log, "end solve");

}
void        ThreePhaseAC::output()
{
	Qcout << setw(30) << "Output results:";

	//输出矩阵
	/*Qofstream outputArea(m_dir + "_matrix_Area.txt");
	if (outputArea.fail())
	{
		Qcout << "fail creating matrix_Area.txt" << std::endl;
	}
	Area.save(outputArea, arma::arma_ascii);

	Qofstream outputQ(m_dir + "_matrix_Q.txt");
	if (outputQ.fail())
	{
		Qcout << "fail creating matrix_Q.txt" << std::endl;
	}
	Q.save(outputQ, arma::arma_ascii);


	Qofstream outputS(m_dir + "_matrix_S.txt");
	if (outputS.fail())
	{
		Qcout << "fail creating matrix_S.txt" << std::endl;
	}
	S.save(outputS, arma::arma_ascii);


	Qofstream outputT(m_dir + "_matrix_T.txt");
	if (outputT.fail())
	{
		Qcout << "fail creating matrix_T.txt" << std::endl;
	}
	T.save(outputT, arma::arma_ascii);


	Qofstream outputV(m_dir + "_matrix_V.txt");
	if (outputV.fail())
	{
		Qcout << "fail creating matrix_V.txt" << std::endl;
	}
	V.save(outputV, arma::arma_ascii);*/

	auto result_Asca_name = m_dir + "_" + "results_Asca.txt";
	Qofstream result_Asca;
	result_Asca.open(result_Asca_name);
	if (!result_Asca)
	{
		Qcout << "fail creating results_Asca.txt" << std::endl;
	}
	
	for (int i = 0; i < Asca_Jsk.size() - 3; i++)
	{
			int _index = pMesh->getUnkownVertexIndex(i);
			result_Asca << _index << '\t' << pMesh->getVertex(_index).x << '\t' << pMesh->getVertex(_index).y << '\t' << abs(Asca_Jsk(i)) << std::endl;
	}
	for (int j = 0; j < pMesh->getVertexNum(); j++)
	{
		if (pMesh->isBoundaryPoint(j))
		{
			result_Asca << j << '\t' << pMesh->getVertex(j).x << '\t' << pMesh->getVertex(j).y << '\t' << 0 << std::endl;
		}
	}

	result_Asca.close();
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
	Asca_Jsk.clear();
	pMesh->clear();
}
void        ThreePhaseAC::reportMeshInfo(Qostream& strm)const
{
	pMesh->reportInfo(strm);
}
//void		ThreePhaseAC::fillCoeffiMatSandTdiag()
//{
//
//	//S.zeros(m_unknownAScar, m_unknownAScar);
//	//T.zeros(m_unknownAScar, m_unknownAScar);
//	for (int k = 0; k < triangleNum; k++)
//	{
//		Triangle tri = pMesh->getTriangleRef(k);
//		int _nA, _nB, _nC;
//		tri.getnAnBnC(_nA, _nB, _nC);
//		value_t treArea = tri.getTriArea();
//
//		bool isDielectric = _m_dielectricDomains.find(tri.getSunbdomainID()) != _m_dielectricDomains.end();
//		bool isCondunctor = !(isDielectric);
//		Complex tempResultT = J0*PI2*m_threephase.m_freq*m_threephase.m_mentalSigma*(treArea / 12);
//		
//		/*int _nA = pMesh->getTriangleRef(k).getnA();
//		int _nB = pMesh->getTriangleRef(k).getnB();
//		int _nC = pMesh->getTriangleRef(k).getnC();*/
//
//		if (!(pMesh->isBoundaryPoint(_nA)))
//		{
//			int _orderA = pMesh->getUnknownVertexOderbyIndex(_nA);
//			value_t na1, na2, na3;
//			tri.getna123(na1, na2, na3);
//			/*value_t na2 = tri.getna2();
//			value_t na3 = tri.getna3();*/
//			value_t tempResultS = static_cast<value_t>((1 / (mu0))*(0.25 / (treArea))*(na2*na2 + na3*na3));
//			CoeffiMat(_orderA, _orderA) += tempResultS;
//			if (isCondunctor)
//			{
//				CoeffiMat(_orderA, _orderA) += tempResultT;
//			}
//			
//		}
//		if (!(pMesh->isBoundaryPoint(_nB)))
//		{
//			int _orderB = pMesh->getUnknownVertexOderbyIndex(_nB);
//			value_t nb1, nb2, nb3;
//			tri.getnb123(nb1, nb2, nb3);
//			/*value_t nb2 = pMesh->getTriangleRef(k).getnb2();
//			value_t nb3 = pMesh->getTriangleRef(k).getnb3();*/
//			value_t tempResultS = static_cast<value_t>((1 / (mu0))*(0.25 / (treArea))*(nb2*nb2 + nb3*nb3));
//			CoeffiMat(_orderB, _orderB) += tempResultS;
//			if (isCondunctor)
//			{
//				CoeffiMat(_orderB, _orderB) += tempResultT;
//			}
//		}
//		if (!(pMesh->isBoundaryPoint(_nC)))
//		{
//			int _orderC = pMesh->getUnknownVertexOderbyIndex(_nC);
//			value_t nc1, nc2, nc3;
//			tri.getnc123(nc1, nc2, nc3);
//			/*value_t nc2 = pMesh->getTriangleRef(k).getnc2();
//			value_t nc3 = pMesh->getTriangleRef(k).getnc3();*/
//			value_t tempResultS = static_cast<value_t>((1 / (mu0))*(0.25 / (treArea))*(nc2*nc2 + nc3*nc3));
//			CoeffiMat(_orderC, _orderC) += tempResultS;
//			if (isCondunctor)
//			{
//				CoeffiMat(_orderC, _orderC) += tempResultT;
//			}
//		}
//
//	}
//
//}
void		ThreePhaseAC::fillCoeffiMatSandT()
{
	for (int k = 0; k < triangleNum; k++)
	{
		Triangle tri = pMesh->getTriangleRef(k);
		int _nA, _nB, _nC;
		tri.getnAnBnC(_nA, _nB, _nC);
		value_t treArea = tri.getTriArea();
		/*int _nA = pMesh->getTriangleRef(k).getnA();
		int _nB = pMesh->getTriangleRef(k).getnB();
		int _nC = pMesh->getTriangleRef(k).getnC();*/
		bool nA_isBoundary = pMesh->isBoundaryPoint(_nA);
		bool nB_isBoundary = pMesh->isBoundaryPoint(_nB);
		bool nC_isBoundary = pMesh->isBoundaryPoint(_nC);

		bool isDielectric = _m_dielectricDomains.find(tri.getSunbdomainID()) != _m_dielectricDomains.end();
		bool isCondunctor = !(isDielectric);
		Complex tempResultT = J0*PI2*m_threephase.m_freq*m_threephase.m_mentalSigma*(treArea / 12);

		value_t na1, na2, na3, nb1, nb2, nb3, nc1, nc2, nc3;
		int _orderA, _orderB, _orderC;
		if (!nA_isBoundary)
		{
			_orderA = pMesh->getUnknownVertexOderbyIndex(_nA);
			tri.getna123(na1, na2, na3);
			value_t tempResultS = static_cast<value_t>((1 / (mu0))*(0.25 / (treArea))*(na2*na2 + na3*na3));
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
			value_t tempResultS = static_cast<value_t>((1 / (mu0))*(0.25 / (treArea))*(nb2*nb2 + nb3*nb3));
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
			value_t tempResultS = static_cast<value_t>((1 / (mu0))*(0.25 / (treArea))*(nc2*nc2 + nc3*nc3));
			CoeffiMat(_orderC, _orderC) += tempResultS;
			if (isCondunctor)
			{
				CoeffiMat(_orderC, _orderC) += tempResultT;
			}
		}
		if ((!nA_isBoundary) && (!nB_isBoundary))
		{
			//int _orderA = pMesh->getUnknownVertexOderbyIndex(_nA);
			//int _orderB = pMesh->getUnknownVertexOderbyIndex(_nB);
			value_t tempResultS = static_cast<value_t>((1 / (mu0))*(0.25 / (treArea))*(na2*nb2 + na3*nb3));

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
			//int _orderA = pMesh->getUnknownVertexOderbyIndex(_nA);
			//int _orderC = pMesh->getUnknownVertexOderbyIndex(_nC);
			value_t tempResultS = static_cast<value_t>((1 / (mu0))*(0.25 / (treArea))*(na2*nc2 + na3*nc3));

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
			//int _orderB = pMesh->getUnknownVertexOderbyIndex(_nB);
			//int _orderC = pMesh->getUnknownVertexOderbyIndex(_nC);
			value_t tempResultS = static_cast<value_t>((1 / (mu0))*(0.25 / (treArea))*(nb2*nc2 + nb3*nc3));

			CoeffiMat(_orderB, _orderC) += tempResultS;
			CoeffiMat(_orderC, _orderB) += tempResultS;
			if (isCondunctor)
			{
				CoeffiMat(_orderB, _orderC) += tempResultT;
				CoeffiMat(_orderC, _orderB) += tempResultT;
			}
		}
	}
}
		
void		ThreePhaseAC::fillCoeffiMatQ()
{
//	Q.zeros(m_unknownAScar, m_unknownCurrDensity);

	int count = 0;
	for (auto pos = _m_currentDomains.begin(); pos != _m_currentDomains.end(); ++pos)
	{
		for (int k = 0; k < triangleNum; k++)
		{
			Triangle tri = pMesh->getTriangleRef(k);
			int _nA, _nB, _nC;
			tri.getnAnBnC(_nA, _nB, _nC);
			value_t treArea = tri.getTriArea();


			/*int _nA = pMesh->getTriangleRef(k).getnA();
			int _nB = pMesh->getTriangleRef(k).getnB();
			int _nC = pMesh->getTriangleRef(k).getnC();*/

			bool nA_isBoundary = pMesh->isBoundaryPoint(_nA);
			bool nB_isBoundary = pMesh->isBoundaryPoint(_nB);
			bool nC_isBoundary = pMesh->isBoundaryPoint(_nC);
			if (tri.getSunbdomainID() == *pos)
			{
				Complex tempQ = -J0*PI2*m_threephase.m_freq*m_threephase.m_mentalSigma*(treArea / 3);
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
		}
		++count;
	}	
}
void		ThreePhaseAC::fillCoeffiMatArea()
{
	//Area.zeros(m_unknownCurrDensity, m_unknownCurrDensity);
	std::set<int>::iterator pos = _m_currentDomains.begin();
	for (int i = 0; i < m_unknownCurrDensity; i++)
	{
		for (int j = 0; j < triangleNum; j++)
		{
			Triangle tri = pMesh->getTriangleRef(j);
			value_t treArea = tri.getTriArea();
			if (tri.getSunbdomainID() == *pos)
			{
				CoeffiMat(m_unknownAScar + i, m_unknownAScar + i) += J0*PI2*m_threephase.m_freq*m_threephase.m_mentalSigma*(treArea);
			}
		}
		++pos;
	}

}
void		ThreePhaseAC::fillV()
{
	V.zeros(m_unknownAScar + m_unknownCurrDensity);
	value_t currentA = m_threephase.m_amplitude*cos(m_threephase.m_phase*DegreesToRadians);
	value_t currentB = m_threephase.m_amplitude*cos(m_threephase.m_phase*DegreesToRadians - 120 * DegreesToRadians);;
	value_t currentC = m_threephase.m_amplitude*cos(m_threephase.m_phase*DegreesToRadians - 240 * DegreesToRadians);;
	V(m_unknownAScar) = currentA;
	V(m_unknownAScar + 1) = currentB;
	V(m_unknownAScar + 2) = currentC;
}

