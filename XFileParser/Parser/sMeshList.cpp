#include "stdafx.h"

namespace ns_HoLin
{
	sMeshFaces::sMeshFaces()
	{
		this->number_of_indices = 0;
	}

	sMeshFaces::~sMeshFaces()
	{
		Cleanup();
	}

	void sMeshFaces::Cleanup(HANDLE hfile)
	{
		number_of_indices = 0;
		face_vertex_indices.clear();
	}

	sMeshNormals::sMeshNormals()
	{
	}

	sMeshNormals::~sMeshNormals()
	{
		Cleanup();
	}

	void sMeshNormals::Cleanup(HANDLE hfile)
	{
		normals.clear();
		face_normals.clear();
	}

	sIndexedColor::sIndexedColor(sIndexedColor &&other)
	{
		this->index = other.index;
		this->index_color = other.index_color;
	}

	sIndexedColor::sIndexedColor(const sIndexedColor &other)
	{
		this->index = other.index;
		this->index_color = other.index_color;
	}

	/*sIndexedColor& sIndexedColor::operator=(const sIndexedColor &other)
	{
		this->index = other.index;
		this->index_color = other.index_color;
		return *this;
	}*/

	sMeshVertexColors::~sMeshVertexColors()
	{
		Cleanup();
	}
	
	void sMeshVertexColors::Cleanup(HANDLE hfile)
	{
		vertex_colors.clear();
	}
	
	sVertexDuplicationIndices::sVertexDuplicationIndices()
	{
		number_of_indices = 0;
		nOriginalVertices = 0;
	}
	
	sVertexDuplicationIndices::~sVertexDuplicationIndices()
	{
		Cleanup();
	}

	void sVertexDuplicationIndices::Cleanup(HANDLE hfile)
	{
		number_of_indices = 0;
		nOriginalVertices = 0;
		Indices.clear();
	}

	sMeshExtraAttributes::sMeshExtraAttributes()
	{
		p_skininfo = NULL;
	}

	sMeshExtraAttributes::~sMeshExtraAttributes()
	{
		Cleanup();
	}

	void sMeshExtraAttributes::Cleanup(HANDLE hfile)
	{
		meshnormals.Cleanup(hfile);
		texturecoord.clear();
		material_indices.clear();
		smeshmateriallist.clear();
		scolors.Cleanup();
		sanimation.Cleanup();
		if (p_skininfo) {
			delete p_skininfo;
			p_skininfo = NULL;
		}
		list_of_vectors.clear();
		list_of_matrices.clear();
	}

	sMesh::sMesh()
	{
		p_extra = nullptr;
		pnextmesh = nullptr;
	}

	sMesh::~sMesh()
	{
		Cleanup();
	}

	void sMesh::Cleanup(HANDLE hfile)
	{
		name.clear();
		vertices.clear();
		meshfaces.Cleanup();
		if (p_extra) {
			delete p_extra;
			p_extra = nullptr;
		}
		if (pnextmesh) {
			delete pnextmesh;
			pnextmesh = nullptr;
		}
	}

	sMeshList::sMeshList()
	{
		number_of_meshes = 0;
		pfirstmesh = nullptr;
		plastmesh = nullptr;
	}

	sMeshList::~sMeshList()
	{
		Cleanup();
	}

	void sMeshList::Cleanup(HANDLE hfile)
	{
		number_of_meshes = 0;
		if (pfirstmesh) {
			delete pfirstmesh;
			pfirstmesh = nullptr;
			plastmesh = nullptr;
		}
	}

	sMesh* sMeshList::CreateNewMesh()
	{
		sMesh *p_tmp = (sMesh*)new sMesh;

		if (p_tmp) {
			if (plastmesh) {
				plastmesh->pnextmesh = p_tmp;
				plastmesh = p_tmp;
			}
			else {
				pfirstmesh = p_tmp;
				plastmesh = p_tmp;
			}
			number_of_meshes++;
		}
		return (sMesh*)p_tmp;
	}

	void sMeshList::InsertMesh(sMesh *pmesh)
	{
		if (pmesh) {
			if (pfirstmesh) {
				plastmesh->pnextmesh = pmesh;
				plastmesh = pmesh;
			}
			else {
				pfirstmesh = pmesh;
				plastmesh = pmesh;
			}
			number_of_meshes++;
		}
	}

	BOOL sMeshList::Find(std::string_view name)
	{
		if (pfirstmesh) {
			sMesh *ptmp = pfirstmesh;

			while (ptmp) {
				if (ptmp->name.size() > 0) {
					if (ptmp->name == name) {
						return TRUE;
					}
				}
				ptmp = ptmp->pnextmesh;
			}
		}
		return FALSE;
	}
	
	sMesh* sMeshList::Search(std::string_view name)
	{
		if (pfirstmesh) {
			sMesh *pmesh = pfirstmesh;
			while (pmesh) {
				if (pmesh->name == name)
					return pmesh;
				pmesh = pmesh->pnextmesh;
			}
		}
		return (sMesh*)nullptr;
	}
}
