#ifndef _MESH_H_
#define _MESH_H_
#include <set>
#include <map>
#include "Triangle.h"

namespace component {

	class Mesh {
//类成员
	private:
		Qstring                 meshName;
		int                     subdomain_count;
		std::map<int, VectorR2> indexVerticesAll;
		std::vector<Triangle *> trianglePtrArray;
		std::set<int>			boundaryIndexVertices;
		std::set<int>			unknowVertices;
//成员函数
	public:
		Mesh();
		~Mesh();
		Mesh(const Mesh&) = delete; 
		Mesh& operator=(const Mesh&) = delete; 
//内联
	public:
		Triangle&       getTriangleRef(size_t i); 
		VectorR2        getVertex( int i)const;
	//	VectorR2        getBoundayVertex( int i)const;
		int				getVertexUnkownNum()const;
		int				getVertexNum()const;
		int             getTriangleNum() const;
		int             getSubdomainNum() const;
		int				getBoundaryVerticesNum()const;
		
//非内联
	public:
		bool			isBoundaryPoint(int _index)const;
		int				getUnkownVertexIndex(int _order)const;
		int				getUnknownVertexOderbyIndex(int _index)const;
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
	
	inline Triangle&    Mesh::getTriangleRef(size_t i)
	{
		return *trianglePtrArray[i];
	}
	inline VectorR2	    Mesh::getVertex(int i) const
	{
		return indexVerticesAll.find(i)->second;
	}
	inline int			Mesh::getTriangleNum() const
	{
		return static_cast<int>(trianglePtrArray.size());
	}
	inline int			Mesh::getVertexNum()const
	{
		return static_cast<int>(indexVerticesAll.size());
	}
	inline int			Mesh::getSubdomainNum() const
	{
		return subdomain_count;
	}
	inline int			Mesh::getBoundaryVerticesNum()const
	{  return  static_cast<int>(boundaryIndexVertices.size()); }
	inline int			Mesh::getVertexUnkownNum()const
	{
		return static_cast<int>(unknowVertices.size());
	}
} 
#endif