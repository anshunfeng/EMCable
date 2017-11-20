#ifndef _MESH_H_
#define _MESH_H_
#include <set>
#include <map>
#include "Triangle.h"
#include "Boundary.h"

namespace component {

	class Mesh {
//类成员
	private:
		Qstring                 meshName;
		int                     subdomain_count;
		std::vector<VectorR2>   vertices;
		std::vector<Triangle *> trianglePtrArray;
		Boundary                m_boundary;
//成员函数
	public:
		Mesh();
		~Mesh();
		Mesh(const Mesh&) = delete; 
		Mesh& operator=(const Mesh&) = delete; 
//内联
	public:
		Triangle&       getTriangleRef(size_t i); 
		VectorR2        getVertex(int i)const;
		int				getBoudaryPointsNum()const;
		int				getVertexNum()const;
		int             getTriangleNum() const;
		int             getSubdomainNum() const;
//非内联
	public:
		void            addTrianglePtr(Triangle *newTriangle);
		void            clear();
		void            reportInfo(Qostream& strm) const;//信息报告
	//	void            Debug();

	public:
		bool			loadMeshFile(const Qstring& filename, Qstring &info);
	private:
		bool			loadMatlabMeshFile(const Qstring& filename);
		bool			readMatlabTmpFile();
	};

	inline Triangle&  Mesh::getTriangleRef(size_t i)
	{
		return *trianglePtrArray[i];
	}
	inline VectorR2 Mesh::getVertex(int i) const
	{
		return vertices[i];
	}
	inline int Mesh::getBoudaryPointsNum() const
	{
		return m_boundary.getBoundaryVerticesNum();
	}
	inline int Mesh::getTriangleNum() const
	{
		return static_cast<int>(trianglePtrArray.size());
	}
	inline int Mesh::getVertexNum()const
	{
		return static_cast<int>(vertices.size());
	}
	inline int Mesh::getSubdomainNum() const
	{
		return subdomain_count;
	}
	
} 
#endif