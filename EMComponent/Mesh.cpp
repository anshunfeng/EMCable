#include "Mesh.h"
#include "Mathdef.h"
#include "MyDebugNew.h"
#include <iomanip>
#include <numeric>
#include <sstream>

using namespace component;
using std::setw;

Mesh::Mesh()
{
}
//  清空指针指向的内存
Mesh::~Mesh()
{
	if (!trianglePtrArray.empty())
		clear();
}


bool Mesh::loadMeshFile(const Qstring & filename, Qstring & info)
{
	std::string suffix = filename.substr(filename.find_last_of('.') + 1);// 判断文件格式
	if ("txt" == suffix)
	{
		if (!loadMatlabMeshFile(filename))
		{
			info.assign("fail loading Matlabtxt file: " + filename);
			return false;
		}
		if (!readMatlabTmpFile())
		{
			info.assign("fail reading Matlabtmp File!");
			return false;
		}
	}
	return true;
}

bool Mesh::loadMatlabMeshFile(const Qstring & filename)
{
	auto maxSize = std::numeric_limits<std::streamsize>::max();
	std::stringstream tmpNodeInfo, tmpTriInfo, tmpEdgeInfo;
	std::ifstream inputFile(filename, std::ios::in);
	if (inputFile.fail())
	{
		return false;
	}
	
	inputFile.ignore(maxSize, '\n');
	inputFile.ignore(maxSize, '\n');
	int     nodeID, nodeNum = 0;
	value_t coord1, coord2;
	while (true)
	{
		if (inputFile.peek() == '#')    break;//结束循环
		inputFile >> nodeID >> coord1 >> coord2;
		tmpNodeInfo << nodeID << '\t' << coord1 << '\t' << coord2 << '\n';
		if (nodeID != ++nodeNum)
		{
			return false;
		}

		inputFile.get();
	}
	int triID, triNum = 0;
	int node1, node2, node3, subId;
	inputFile.ignore(maxSize, '\n');
	while (true)
	{
		if (inputFile.peek() == '#')    break;//结束循环
		inputFile >> triID >> node1 >> node2 >> node3 >> subId;
		tmpTriInfo << triID << '\t' << node1 << '\t' << node2 << '\t' 
					<< node3 << '\t' << subId << '\n';
		if (triID != ++triNum)
		{
			std::cout << "false" << std::endl;
			return false;	
		}

		inputFile.get();
	}
	int edgeNum = 0;
	int edgePointId;
	inputFile.ignore(maxSize, '\n');
	while (inputFile >> edgePointId)
	{
		tmpEdgeInfo << edgePointId << '\n';
		++edgeNum;
	}
	if (!inputFile.eof())//
		return false;
	// 将读取的mesh信息以一定的格式存入temp.dat文件中
	Qofstream tmpFile("temp.dat", std::ios::out);
	if (tmpFile.fail())
	{
		return false;	
	}

	tmpFile << nodeNum << '\t' << triNum << '\t' << edgeNum << '\n';
	tmpFile << tmpNodeInfo.rdbuf() << tmpTriInfo.rdbuf() << tmpEdgeInfo.rdbuf();
	return true;
}

bool Mesh::readMatlabTmpFile()
{
	Qifstream input("temp.dat", std::ios::in);
	if (input.fail())
		return false;
	int  nodeNum, triNum, edgeNum;
	std::set<int> edgePointSets;
	input >> nodeNum >> triNum >> edgeNum;
	vertices.reserve(nodeNum);
	trianglePtrArray.reserve(triNum);

	VectorR2 node;
	int nodeID;
	for (int i = 0; i < nodeNum; ++i)
	{
		input >> nodeID >> node.x >> node.y;
		vertices.push_back(std::move(node));
	}

	int  triID, node1, node2, node3, subId;
	std::set<int>subSet;
	for (int i = 0; i < triNum; ++i)
	{
		input >> triID >> node1 >> node2 >> node3 >> subId;
		Triangle* pTri = new Triangle();
		pTri->setVertexID(node1 - 1, node2 - 1, node3 - 1);//索引
		pTri->setSunbdomainID(subId);
		subSet.insert(subId);
		trianglePtrArray.push_back(pTri);
	}
	subdomain_count = static_cast<int>(subSet.size());
	int edgePointId;
	for (int i = 0; i < triNum; ++i)
	{
		input >> edgePointId;
		edgePointSets.insert(edgePointId - 1);
	}
	m_boundary.setBoundary(edgePointSets);
	//input >> nodeID;
	if (!input.eof())
		return false;

	for (int t = 0; t < triNum; ++t)
	{
		trianglePtrArray[t]->getVertexID(node1, node2, node3);
		trianglePtrArray[t]->setVertexArea(vertices[node1], vertices[node2], vertices[node3]);
	}
	meshName = "MeshByMatlab";
	return true;
}

void Mesh::addTrianglePtr(Triangle * newTriangle)
{
	trianglePtrArray.push_back(newTriangle);
}

void Mesh::reportInfo(Qostream& strm) const
{
	auto oldState = strm.flags();
	strm << std::left << std::fixed;
	strm << "Mesh Information:\n";
	strm << setw(30) << "  -> MeshName: " << meshName << strm.widen('\n');
	strm << setw(30) << "  -> TotalVertices: " << vertices.size() << strm.widen('\n');
	strm << setw(30) << "  -> Triangles: " << trianglePtrArray.size() << strm.widen('\n');
	strm << setw(30) << "  -> EdgePoints:" << m_boundary.getBoundaryVerticesNum() << strm.widen('\n');
	strm << setw(30) << "  -> Subdomains:" << subdomain_count << strm.widen('\n');
	strm.flags(oldState); // 待注释
}

void Mesh::clear()
{
	auto tmpSize = trianglePtrArray.size();
	for (size_t i = 0; i < tmpSize; ++i)
	//删除指针，即删除指针指向的内存
		delete trianglePtrArray[i];
	//清空vector中的数据（这里指的是指针）
	trianglePtrArray.clear();
	vertices.clear();
}

//void Mesh::Debug()
//{
//	auto count = trianglePtrArray.size();
//	std::vector<int> sub_count(subdomain_count, 0);// subdomain_count每个子区域包含的三角单元数量，初始化为0
//	for (size_t i = 0; i < count; ++i)
//	{
//		++sub_count[trianglePtrArray[i]->getID()]; // 获取每个三角单元的ID
//	}
//	Qcout << "Number of subdomain : " << subdomain_count << '\n';
//	Qcout << "Number of triangles in each domain\n";
//	for (int i = 0; i<sub_count.size(); ++i)
//	{
//		Qcout << "  --subdomain " << i + 1 << " : " << std::setw(10) << sub_count[i] << '\n';
//	}
//	Qcout.flush();
//}
