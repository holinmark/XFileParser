// XFileParser.cpp : Defines the entry point for the console application.
//

#include "XFileParser.h"
#include "stdafx.h"

void PrintMatrix(DirectX::XMFLOAT4X4 matrix)
{
	for (std::size_t row = 0; row < 4; ++row) {
		for (std::size_t col = 0; col < 4; ++col) {
			std::clog << matrix.m[row][col] << ", ";
		}
		std::clog << '\n';
	}
	std::clog << '\n\n';
}

void PrintMaterials(std::vector<ns_HoLin::sMaterial> &materials, ns_HoLin::sMesh *p_mesh, const char *pstr)
{
	if (materials.size() > 0) {
		std::clog << pstr << '\n';
		for (std::size_t i = 0; i < materials.size(); ++i) {
			std::clog << "\tMaterial name: " << materials[i].name.c_str() << '\n';
			std::clog << "\tFace color " << materials[i].facecolor.x << ", ";
			std::clog << materials[i].facecolor.y << ", " << materials[i].facecolor.z << ", " << materials[i].facecolor.w << '\n';
			std::clog << "\tPower" << materials[i].power << '\n';
			std::clog << "\tSpecular " << materials[i].specularcolor.x << ", ";
			std::clog << materials[i].specularcolor.y << ", " << materials[i].specularcolor.z << '\n';
			std::clog << "\tEmmisive " << materials[i].emissivecolor.x << ", " << materials[i].emissivecolor.y;
			std::clog << materials[i].emissivecolor.z << '\n';
			std::clog << "\tFile name: " << materials[i].filename.c_str() << "\n\n";
		}
	}
	std::clog << std::endl;
	while (p_mesh) {
		std::clog << "Mesh name: " << p_mesh->name.c_str() << '\n';
		if (p_mesh->p_extra) {
			for (std::size_t i = 0; i < p_mesh->p_extra->smeshmateriallist.size(); ++i) {
				std::clog << "\tMaterial name: " << p_mesh->p_extra->smeshmateriallist[i].name.c_str() << '\n';
				std::clog << "\tFace color " << p_mesh->p_extra->smeshmateriallist[i].facecolor.x << ", ";
				std::clog << p_mesh->p_extra->smeshmateriallist[i].facecolor.y << ", " << p_mesh->p_extra->smeshmateriallist[i].facecolor.z << ", ";
				std::clog << p_mesh->p_extra->smeshmateriallist[i].facecolor.w << '\n';
				std::clog << "\tPower " << p_mesh->p_extra->smeshmateriallist[i].power << '\n';
				std::clog << "\tSpecular " << p_mesh->p_extra->smeshmateriallist[i].specularcolor.x << ", ";
				std::clog << p_mesh->p_extra->smeshmateriallist[i].specularcolor.y << ", " << p_mesh->p_extra->smeshmateriallist[i].specularcolor.z << '\n';
				std::clog << "\tEmmisive " << p_mesh->p_extra->smeshmateriallist[i].emissivecolor.x << ", ";
				std::clog << p_mesh->p_extra->smeshmateriallist[i].emissivecolor.y << ", " << p_mesh->p_extra->smeshmateriallist[i].emissivecolor.z << '\n';
				std::clog << "\tFile name: " << p_mesh->p_extra->smeshmateriallist[i].filename.c_str() << '\n';
			}
		}
		p_mesh = p_mesh->pnextmesh;
	}
	std::puts("");
}

void PrintMesh(ns_HoLin::sMeshList *p_list_of_meshes)
{
	ns_HoLin::sMesh *pmesh = p_list_of_meshes->pfirstmesh;
	
	if (pmesh) {
		std::clog << "Number of meshes " << p_list_of_meshes->number_of_meshes << '\n';
		while (pmesh) {
			std::clog << "\tMesh " << pmesh->name.c_str() << '\n';
			pmesh = pmesh->pnextmesh;
		}
	}
	else {
		std::clog << "\nNo mesh\n\n";
	}
}

void PrintFrames(ns_HoLin::sSequenceOfFrames *pframeseq)
{
	if (pframeseq != nullptr) {
		std::size_t index = 1;
		ns_HoLin::sFrame *pframe;
		
		std::clog << "\n\nFrames :\n";
		while (pframeseq) {
			std::clog << "Keys in frame sequences.\n";
			for (auto n : pframeseq->framenames) {
				std::clog << n.c_str() << ", ";
			}
			std::clog << std::endl;
			pframe = pframeseq->pfirstframe;
			while (pframe) {
				std::clog << "\tFrame : " << pframe->name.c_str() << '\n';
				if (pframe->parent_name.length() > 0)
					std::clog << "\t\tParent : " << pframe->parent_name.c_str() << '\n';
				else
					std::clog << "\t\tNo parent" << '\n';
				if (pframeseq->parent_children[pframe->name].empty() == false) {
					std::clog << "\t\tChildren " << pframeseq->parent_children[pframe->name].size() << "\n\t\t";
					for (auto c : pframeseq->parent_children[pframe->name]) {
						std::clog << c.c_str() << ", ";
					}
					std::clog << std::endl;
				}
				if (pframe->mesh.size() > 0) {
					std::clog << "Meshes\n";
					for (auto m : pframe->mesh) {
						std::clog << m.c_str() << ", ";
					}
					std::clog << '\n';
				}
				pframe = pframe->pnextframe;
			}
			std::clog << '\n';
			pframeseq = pframeseq->pnextseq;
		}
	}
	else {
		std::clog <<"\nNo frames\n\n";
	}
}

void PrintAnimationSet(ns_HoLin::sAnimationSetList *psetlist)
{
	std::clog << "\nAnimation data:\n";
	if (psetlist) {
		ns_HoLin::sAnimationSet *p_anim_set = psetlist->pfirst_set;
		ns_HoLin::sAnimation *p_anim = nullptr;
		ns_HoLin::sAnimation_Data *p = nullptr;
		std::size_t number_of_animations = 0;

		p_anim = psetlist->pfirst_animation;
		while (p_anim) {
			std::clog << "\tAnimation " << p_anim->name << '\n';
			p_anim = p_anim->pnextanimation;
		}
		p_anim_set = psetlist->pfirst_set;
		while (p_anim_set) {
			std::clog << "\tAnimation set " << p_anim_set->name << '\n';
			p_anim = p_anim_set->pfirstanimation;
			while (p_anim) {
				number_of_animations++;
				std::clog << "\t\tAnimation " << p_anim->name << '\n';
				p = p_anim->pfirst_data;
				/*
				while (p) {
					std::clog << "\t\t\tTransform type " << p->type_of_transform << '\n';
					for (DWORD i = 0; i < p->transformation_data.size(); ++i) {
						for (DWORD j=0; j < p->transformation_data[i].size(); ++j) {
							for (DWORD k = 0; k < p->transformation_data[i][j].tfkeys.size(); ++k) {
								std::clog << p->transformation_data[i][j].tfkeys[k] << ", ";
							}
							std::clog << '\n';
						}
						std::clog << '\n';
					}
					std::clog << '\n';
					p = p->pnextanimation_data;
				}
				*/
				p_anim = p_anim->pnextanimation;
			}
			p_anim_set = p_anim_set->pnext_set;
		}
		std::clog << "\nNumber of animations " << number_of_animations << '\n';
	}
}

void PrintDuplicates(ns_HoLin::sMesh *p_mesh)
{
	while (p_mesh) {
		std::cout << p_mesh->name.c_str() << '\n';
		if (p_mesh->p_extra) {
			for (auto i : p_mesh->p_extra->sduplicates.Indices) {
				std::cout << i << ", ";
			}
			std::cout << '\n' << p_mesh->p_extra->sduplicates.nIndices << " " << p_mesh->p_extra->sduplicates.nOriginalVertices << "\n\n";
		}
		p_mesh = p_mesh->pnextmesh;
	}
}

void PrintData(ns_HoLin::cTextXFileParser *p_xfile)
{
	//	ns_HoLin::sSequenceOfFrames *pframeseq = p_xfile->xfiledata.sframeslist.pfirstseq;
	ns_HoLin::sAnimationSetList *pset = &p_xfile->xfiledata.sanimationsetlist;

	PrintMesh(&p_xfile->xfiledata.smeshlist);
	//PrintFrames(p_xfile->xfiledata.sframeslist.pfirstseq);
	//PrintMaterials(p_xfile->xfiledata.smateriallist, pmesh, "Global materials");
	PrintAnimationSet(&p_xfile->xfiledata.sanimationsetlist);
	//PrintDuplicates(p_xfile->xfiledata.smeshlist.pfirstmesh);
}

ns_HoLin::cTextXFileParser* OpenFileWithMeshFileName()
{
	std::wifstream fin;
	wchar_t buff[MAX_PATH];
	ns_HoLin::cTextXFileParser *p_io = NULL;

	buff[0] = L'\0';
	fin.open(L"file.txt");
	if (fin.is_open()) {
		fin.getline(buff, MAX_PATH);
		fin.close();
		if (std::wcslen(buff) > 0) {
			p_io = new ns_HoLin::cTextXFileParser(buff);
			if (p_io == nullptr) {
				std::cerr << "Unable to allocate memory. " << __LINE__ << '\n';
				return (ns_HoLin::cTextXFileParser*)NULL;
			}
			std::wclog << L"Reading from mesh file \'" << buff << "\'\n";
		}
		else {
			std::cerr << "Error no mesh file name in \'file.txt\'.\n";
		}
	}
	else {
		std::cerr << "\nError opening \'file.txt\'.\n";
	}
	return p_io;
}

BOOL GetXFileHeader(ns_HoLin::cTextXFileParser *p_io, BOOL &textfile, BOOL &mode32bit)
{
	struct sXFileHeader
	{
		char magic_number[4];
		char major[2];
		char minor[2];
		char format_type[4];
		char float_size[4];
		char pad;
	};
	sXFileHeader sxheader;
	DWORD datasize = sizeof(sXFileHeader) - sizeof(char);
	DWORD bytesreceived = 0;

	memset((void*)&sxheader, 0, sizeof(sXFileHeader));
	if (p_io->sfile.hfile == NULL) {
		return FALSE;
	}
	std::cout << "Reading header.\n";
	if (ReadFile(p_io->sfile.hfile, (LPVOID)&sxheader, datasize, &bytesreceived, NULL)) {
		if (strstr((const char*)sxheader.magic_number, (const char*)"xof") == NULL) {
			std::cout << "Not an x file format file.\n";
			return FALSE;
		}
		if (strstr((const char*)sxheader.format_type, (const char*)"txt") != NULL) {
			textfile = TRUE;
			std::cout << "Text file.\n";
		}
		else if (strstr((const char*)sxheader.format_type, (const char*)"bin") != NULL) {
			textfile = FALSE;
			std::cout << "Binary file.\n";
		}
		if (strstr((const char*)sxheader.float_size, (const char*)"0032") != NULL) {
			mode32bit = TRUE;
			std::cout << "32 bit floating point.\n";
		}
		else if (strstr((const char*)sxheader.float_size, (const char*)"0064") != NULL) {
			mode32bit = FALSE;
			std::cout << "64 bit floating point.\n";
		}
	}
	return TRUE;
}

BOOL ReadMeshFile(ns_HoLin::cTextXFileParser *p_io)
{
	BOOL textfile = TRUE;
	BOOL mode32bit = TRUE;
	
	if (p_io) {
		if (*p_io) {
			if (GetXFileHeader(p_io, textfile, mode32bit)) {
				if (textfile) {
					if (p_io->ParseFile()) {
						std::clog << "File successfully parsed.\n";
						PrintData(p_io);
						return TRUE;
					}
					else {
						std::cerr << "Error parsing x file.\n";
					}
				}
				else {
					std::cout << "Binary mode not supported at this time.\n";
				}
			}
		}
	}
	else {
		std::cerr << "Unable to open \'file.txt\'.\n";
	}
	return FALSE;
}

int wmain(DWORD argv, const wchar_t **argc)
{
	std::array<std::wstring, 2> state_strings{ // Do not reposition strings
		std::wstring(L"-trackoutput"),
		std::wstring(L"-f"),
	};
	std::unordered_map<std::wstring, DWORD> map_options{
		std::make_pair(state_strings[0], 0),
		std::make_pair(state_strings[1], 1)};
	std::array<std::any, state_strings.size()> arguments_options;
	ns_HoLin::cTextXFileParser *p_xfile = nullptr;

	if (argv > 1) {
		for (DWORD i = 1; i < argv; ++i) {
			if (state_strings[0] == std::wstring(argc[i])) {
				arguments_options[map_options[state_strings[0]]] = TRUE;
			}
			else if (state_strings[1] == std::wstring(argc[i])) {
				if (argv <= (i + 1)) {
					std::cerr << "Error -f option, no file name entered, exiting.\n";
					return 0;
				}
				arguments_options[map_options[state_strings[1]]] = std::wstring(argc[++i]);
			}
			else {
				std::wcerr << L"Unknown command " << argc[i] << L'\n';
			}
		}
		if (arguments_options[map_options[state_strings[1]]].has_value()) {
			p_xfile = new ns_HoLin::cTextXFileParser(std::any_cast<std::wstring>(arguments_options[map_options[state_strings[1]]]).c_str());
		}
	}
	if (p_xfile == nullptr) {
		p_xfile = OpenFileWithMeshFileName();
	}
	if (p_xfile) {
		if (arguments_options[map_options[state_strings[0]]].has_value())
			p_xfile->trackoutput = std::any_cast<BOOL>(arguments_options[map_options[state_strings[0]]]);
		if (*p_xfile) {
			std::cout << "Mesh file opened.\n";
			try {
				if (ReadMeshFile(p_xfile)) {
					std::clog << "Success.\n";
				}
			}
			catch (ns_HoLin::sErrorMessageException serror) {
				std::cerr << serror.what();
			}
			catch (std::exception e) {
				std::cerr << e.what() << '\n';
			}
		}
		else {
			std::cerr << "Error opening mesh file.\n";
		}
		delete p_xfile;
		p_xfile = NULL;
	}
	else {
		std::cerr << "Error allocating memory.\n " << __LINE__ << '\n' << __FILE__ << '\n';
	}
	std::clog << "Closing program.\n";
	return 1;
}
