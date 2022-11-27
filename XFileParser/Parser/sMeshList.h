#pragma once

#include <DirectXMath.h>
#include <windows.h>
#include <vector>
#include <string>
#include <utility>

#include "SkinInfo.h"
#include "sAnimationSet.h"
#include "sMaterialList.h"

namespace ns_HoLin
{
	struct sMeshFaces
	{
		DWORD number_of_indices;
		std::vector<std::vector<DWORD>> face_vertex_indices;

		sMeshFaces();
		sMeshFaces(sMeshFaces&&) = delete;
		sMeshFaces(const sMeshFaces&) = delete;
		~sMeshFaces();
		void Cleanup(HANDLE = NULL);
		sMeshFaces& operator=(sMeshFaces&&) = delete;
		sMeshFaces& operator=(const sMeshFaces&) = delete;
	};

	struct sMeshNormals
	{
		std::vector<DirectX::XMFLOAT3> normals;
		std::vector<std::vector<DWORD>> face_normals;

		sMeshNormals();
		sMeshNormals(sMeshNormals&&) = delete;
		sMeshNormals(const sMeshNormals&) = delete;
		~sMeshNormals();
		void Cleanup(HANDLE = NULL);
		sMeshNormals& operator=(sMeshNormals&&) = delete;
		sMeshNormals& operator=(const sMeshNormals&) = delete;
	};

	struct sIndexedColor
	{
		DWORD index;
		DirectX::XMFLOAT4 index_color; // x = r, y = g, z = b, w = a

		sIndexedColor() : index(0), index_color(DirectX::XMFLOAT4(1, 1, 1, 1)) {}
		sIndexedColor(sIndexedColor&&);
		sIndexedColor(const sIndexedColor&);
		sIndexedColor& operator=(sIndexedColor&&) = delete;
		sIndexedColor& operator=(const sIndexedColor&) = delete;
	};

	struct sMeshVertexColors
	{
		std::vector<sIndexedColor> vertex_colors;
		
		sMeshVertexColors() {}
		sMeshVertexColors(sMeshVertexColors&&) = delete;
		sMeshVertexColors(const sMeshVertexColors&) = delete;
		~sMeshVertexColors();
		void Cleanup(HANDLE = NULL);
		sMeshVertexColors& operator=(sMeshVertexColors&&) = delete;
		sMeshVertexColors& operator=(const sMeshVertexColors&) = delete;
	};

	struct sVertexDuplicationIndices
	{
		std::size_t number_of_indices; // Number of vertex indices. This is the number of vertices in the mesh.
		std::size_t nOriginalVertices; // Number of vertices in the mesh before any duplication occurs.
		std::vector<DWORD> Indices; /*The value indices[n] holds the vertex index that vertex[n]
										  in the vertex array for the mesh would have had if no duplication had occurred.
										  Indices in this array that are the same, therefore, indicate duplicate vertices.*/
		sVertexDuplicationIndices();
		sVertexDuplicationIndices(sVertexDuplicationIndices&&) = delete;
		sVertexDuplicationIndices(const sVertexDuplicationIndices&) = delete;
		~sVertexDuplicationIndices();
		void Cleanup(HANDLE = NULL);
		sVertexDuplicationIndices& operator=(sVertexDuplicationIndices&&) = delete;
		sVertexDuplicationIndices& operator=(const sVertexDuplicationIndices&) = delete;
	};

	struct sMeshExtraAttributes
	{
		sMeshNormals meshnormals;
		std::vector<DirectX::XMFLOAT2> texturecoord;
		std::vector<DWORD> material_indices;
		std::vector<sMaterial> smeshmateriallist;
		sMeshVertexColors scolors;
		ns_HoLin::sVertexDuplicationIndices sduplicates;
		ns_HoLin::sAnimation_Data sanimation;
		ns_HoLin::sSkinInfo *p_skininfo;
		std::vector<DirectX::XMFLOAT3> list_of_vectors;
		// if not empty, the first matrix applies to the mesh object
		// other matrices that follow are instances, if any, of that mesh object
		std::vector<DirectX::XMFLOAT4X4> list_of_matrices;

		sMeshExtraAttributes();
		sMeshExtraAttributes(sMeshExtraAttributes&&) = delete;
		sMeshExtraAttributes(const sMeshExtraAttributes&) = delete;
		~sMeshExtraAttributes();
		void Cleanup(HANDLE = NULL);
		sMeshExtraAttributes& operator=(sMeshExtraAttributes&&) = delete;
		sMeshExtraAttributes& operator=(const sMeshExtraAttributes&) = delete;
	};

	struct sMesh
	{
		std::string name;
		std::vector<DirectX::XMFLOAT3> vertices;
		sMeshFaces meshfaces;

		// Extra mesh attributes
		ns_HoLin::sMeshExtraAttributes *p_extra;
		// pointer to the next mesh
		ns_HoLin::sMesh *pnextmesh;

		sMesh();
		sMesh(sMesh&&) = delete;
		sMesh(const sMesh&) = delete;
		~sMesh();
		void Cleanup(HANDLE = NULL);
		sMesh& operator=(sMesh&&) = delete;
		sMesh& operator=(const sMesh&) = delete;
	};

	struct sMeshList
	{
		std::size_t number_of_meshes;
		sMesh *pfirstmesh, *plastmesh;

		sMeshList();
		sMeshList(sMeshList&&) = delete;
		sMeshList(const sMeshList&) = delete;
		~sMeshList();
		void Cleanup(HANDLE = NULL);
		sMeshList& operator=(sMeshList&&) = delete;
		sMeshList& operator=(const sMeshList&) = delete;
		sMesh* CreateNewMesh();
		void InsertMesh(sMesh* = nullptr);
		BOOL Find(std::string_view);
		sMesh* Search(std::string_view);
		sMesh* GetLastMesh() { return plastmesh; }
	};
}
