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
Mesh::~Mesh()
{
	if (!trianglePtrArray.empty())
		clear();
}


bool Mesh::loadMeshFile(const Qstring & filename, Qstring & info)
{
	std::string suffix = filename.substr(filename.find_last_of('.') + 1);// 判断文件格式
	//Matlab mesh file, txt
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
	else {
		//Comsol mesh file
		if ("mphtxt" == suffix)
		{
			if (!loadMphtxtMeshFile(filename))
			{
				info.assign("fail loading mphtxt file: " + filename);
				return false;
			}
			if (!readMphtxtTmpFile())
			{
				info.assign("fail reading mphtxttmp File!");
				return false;
			}
		}
	}
	return true;
}
bool Mesh::loadMphtxtMeshFile(const Qstring& filename)
{
	auto maxSize = std::numeric_limits<std::streamsize>::max();
	std::stringstream tmpNodeInfo, tmpTriInfo, tmpEdgeInfo, tmpTriMaterialInfo;
	std::ifstream inputFile(filename, std::ios::in);
	if (inputFile.fail())
	{
		return false;
	}
	Qstring strFlags;
	//读取节点信息
	while (true)
	{
		getline(inputFile, strFlags);
		auto posStr = strFlags.find("# sdim");
		if (posStr != Qstring::npos)
		{
			break;
		}
	}
	
	int nodeNum;
	inputFile >> nodeNum;
	inputFile.ignore(maxSize, '\n');
	while (true)
	{
		getline(inputFile, strFlags);
		if (strFlags == "# Mesh point coordinates")
		{
			break;
		}
	}
	value_t coord1, coord2;
	for(int i = 0; i < nodeNum; i++)
	{	
		inputFile >> coord1 >> coord2;
		coord1 = 1e-3*coord1;
		coord2 = 1e-3*coord2;
		tmpNodeInfo << i << '\t' << coord1 << '\t' << coord2 << '\n';
		inputFile.ignore(maxSize, '\n');
	}
	//读取边界信息
	while (true)
	{
		getline(inputFile, strFlags);
		if (strFlags == "2 # number of nodes per element")
		{
			break;
		}
	}
	int boundaryAllNum;
	inputFile >> boundaryAllNum;
	inputFile.ignore(maxSize, '\n');
	while (true)
	{
		getline(inputFile, strFlags);
		if (strFlags == "# Elements")
		{
			break;
		}
	}
	int boundaryNodesIndex;
	for (int i = 1; i <= boundaryAllNum; i++)
	{
		inputFile >> boundaryNodesIndex;
		tmpEdgeInfo << boundaryNodesIndex << '\n';
		inputFile.ignore(maxSize, '\n');
	}
	//读取三角单元信息

	while (true)
	{
		getline(inputFile, strFlags);
		if (strFlags == "3 # number of nodes per element")
		{
			break;
		}
	}
	int triNum;
	inputFile >> triNum;
	inputFile.ignore(maxSize, '\n');
	while (true)
	{
		getline(inputFile, strFlags);
		if (strFlags == "# Elements")
		{
			break;
		}
	}
	//int triIndex;
	int node1, node2, node3;
	for (int i = 0; i < triNum; i++)
	{
		inputFile >> node1 >> node2 >> node3;
		tmpTriInfo << i << '\t' << node1 << '\t' << node2 << '\t'
			<< node3 << '\n';
		inputFile.ignore(maxSize, '\n');
	}
	//读取三角单元区域属性信息
	inputFile.ignore(maxSize, '\n');
	int count;
	inputFile >> count;
	inputFile.ignore(maxSize, '\n');
	inputFile.ignore(maxSize, '\n');
	if (count != triNum)
	{
		Qcout << "Num of triNum not match";
		return 0;
	}
	int domainId;
	for (int i = 0; i < count; i++)
	{
		inputFile >> domainId;
		tmpTriMaterialInfo << domainId << '\n';
	}
	inputFile >> domainId;
	if (!inputFile.eof())//
		return false;

	// 将读取的mesh信息以一定的格式存入temp.dat文件中
	Qofstream tmpFile("temp.dat", std::ios::out);
	if (tmpFile.fail())
	{
		return false;
	}
	tmpFile << nodeNum << '\t' << boundaryAllNum << '\t' << triNum << '\n';
	tmpFile << tmpNodeInfo.rdbuf() << tmpEdgeInfo.rdbuf() << tmpTriInfo.rdbuf() 
		<< tmpTriMaterialInfo.rdbuf();
	return true;
}
bool Mesh::readMphtxtTmpFile()
{
	Qifstream input("temp.dat", std::ios::in);
	if (input.fail())
		return false;
	
	int  nodeNum, triNum, boundaryAllNum;
	//std::set<int> edgePointSets;
	input >> nodeNum >> boundaryAllNum >> triNum;
	//vertices.reserve(nodeNum);
	trianglePtrArray.reserve(triNum);
	VectorR2 node;
	int nodeIndex;
	for (int i = 0; i < nodeNum; ++i)
	{
		input >> nodeIndex >> node.x >> node.y;
		indexVerticesAll.insert(std::make_pair(nodeIndex, node));
	}
	int edgePointIndex;
	std::map<int, std::set<int> > boundaryAllIndexVertices;
	std::set<int> tmp;
	//tmp.insert(0);
	for (int i = 0; i < boundaryAllNum; ++i)
	{
		input >> edgePointIndex;
		boundaryAllIndexVertices.insert(std::make_pair(edgePointIndex, tmp));
	}
	
	int  triIndex, node1, node2, node3;
	for (int i = 0; i < triNum; ++i)
	{
		input >> triIndex >> node1 >> node2 >> node3;
		//trianglePtrArray[i]->setVertexID(node1, node2, node3);
		Triangle* pTri = new Triangle();
		pTri->setVertexID(node1, node2, node3);//索引
		//pTri->setSunbdomainID(subId);
		//subSet.insert(subId);
		trianglePtrArray.push_back(pTri);
	}
	

	std::set<int> subSet;
	int domainId;
	for (int i = 0; i < triNum; ++i)
	{
		input >> domainId;
		trianglePtrArray[i]->setSunbdomainID(domainId);
		subSet.insert(domainId);
	}
	subdomain_count = static_cast<int>(subSet.size());//统计区域总数
	input >> domainId;
	if (!input.eof())
		return false;

	for (int t = 0; t < triNum; ++t)
	{
		trianglePtrArray[t]->getVertexID(node1, node2, node3);
		int subId = trianglePtrArray[t]->getSunbdomainID();
		auto iterNode1 = boundaryAllIndexVertices.find(node1);
		auto iterNode2 = boundaryAllIndexVertices.find(node2);
		auto iterNode3 = boundaryAllIndexVertices.find(node3);
		if (iterNode1 != boundaryAllIndexVertices.end())
		{	
			iterNode1->second.insert(subId);
		}
		if (iterNode2 != boundaryAllIndexVertices.end())
		{
			iterNode2->second.insert(subId);
		}
		if (iterNode3 != boundaryAllIndexVertices.end())
		{
			iterNode3->second.insert(subId);
		}
	}

	for (int t = 0; t < triNum; ++t)
	{
		trianglePtrArray[t]->getVertexID(node1, node2, node3);
		trianglePtrArray[t]->setVertexArea(indexVerticesAll.find(node1)->second,
			indexVerticesAll.find(node2)->second, indexVerticesAll.find(node3)->second);
	}

	//Qcout << "flags" << std::endl;

	for (auto it = boundaryAllIndexVertices.begin(); it != boundaryAllIndexVertices.end(); ++it)
	{
		if (it->second.size() == 1)
		{
			boundaryIndexVertices.insert(it->first);
		}
	}

	int _i = 0;
	for (auto it = indexVerticesAll.begin(); it != indexVerticesAll.end(); ++it)
	{
		if (!(boundaryIndexVertices.find(it->first) != boundaryIndexVertices.end()))
		{
			unknowVerticesIndexOrder.insert(std::make_pair(it->first, _i));
			unknowVerticesOrderIndex.insert(std::make_pair(_i, it->first));
			++_i;
		}

	}
	meshName = "MeshByComsol";
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
	//std::set<int> edgePointSets;
	input >> nodeNum >> triNum >> edgeNum;
	//vertices.reserve(nodeNum);
	trianglePtrArray.reserve(triNum);

	VectorR2 node;
	int nodeID;
	for (int i = 0; i < nodeNum; ++i)
	{
		input >> nodeID >> node.x >> node.y;
		indexVerticesAll.insert(std::make_pair(nodeID - 1, node));
	//	vertices.push_back(std::move(node));
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
	subdomain_count = static_cast<int>(subSet.size());//统计区域总数
	int edgePointId;
	for (int i = 0; i < edgeNum; ++i)
	{
		input >> edgePointId;
		boundaryIndexVertices.insert(edgePointId - 1);
	}
	
	input >> nodeID;
	if (!input.eof())
		return false;


	for (int t = 0; t < triNum; ++t)
	{
		trianglePtrArray[t]->getVertexID(node1, node2, node3);
		trianglePtrArray[t]->setVertexArea(indexVerticesAll.find(node1)->second,
			indexVerticesAll.find(node2)->second, indexVerticesAll.find(node3)->second);
	}

	int _i = 0;
	for (auto it = indexVerticesAll.begin(); it != indexVerticesAll.end(); ++it)
	{
		if (!(boundaryIndexVertices.find(it->first) != boundaryIndexVertices.end()))
		{
			unknowVerticesIndexOrder.insert(std::make_pair(it->first, _i));
			unknowVerticesOrderIndex.insert(std::make_pair(_i, it->first));
			++_i;
		}
	
	}
	meshName = "MeshByMatlab";
	return true;
}

void Mesh::reportInfo(Qostream& strm) const
{
	auto oldState = strm.flags();
	strm << std::left << std::fixed;
	strm << "Mesh Information:\n";
	strm << setw(30) << "  -> MeshName: " << meshName << strm.widen('\n');
	strm << setw(30) << "  -> TotalVertices: " << indexVerticesAll.size() << strm.widen('\n');
	strm << setw(30) << "  -> Triangles: " << trianglePtrArray.size() << strm.widen('\n');
	strm << setw(30) << "  -> EdgePoints:" << boundaryIndexVertices.size() << strm.widen('\n');
	strm << setw(30) << "  -> Subdomains:" << subdomain_count << strm.widen('\n');
	strm.flags(oldState);

}

void Mesh::clear()
{
	auto tmpSize = trianglePtrArray.size();
	for (size_t i = 0; i < tmpSize; ++i)
	//删除指针，即删除指针指向的内存
		delete trianglePtrArray[i];
	//清空vector中的数据（这里指的是指针）
	trianglePtrArray.clear();
	//vertices.clear();
}
void Mesh::addTrianglePtr(Triangle * newTriangle)
{
	trianglePtrArray.push_back(newTriangle);
}


//bool Mesh::isBoundaryPoint(int _index)const
//{
//
//	return boundaryIndexVertices.find(_index) != boundaryIndexVertices.end();
//}
//int	Mesh::getUnknownVertexOderbyIndex(int _index)const
//{
//	return unknowVerticesIndexOrder.find(_index)->second;
//}
//
