#pragma once

#ifndef SKININFO_H
#define SKININFO_H

#include <Windows.h>
#include <string>
#include <vector>
#include <DirectXMath.h>

namespace ns_HoLin
{
	struct sSkinHeader
	{
		DWORD nMaxSkinWeightsPerVertex;
		DWORD nMaxSkinWeightsPerFace;
		DWORD nBones;

		sSkinHeader();
		sSkinHeader(sSkinHeader&&) = delete;
		sSkinHeader(const sSkinHeader&) = delete;
		~sSkinHeader();
		sSkinHeader& operator=(sSkinHeader&&) = delete;
		sSkinHeader& operator=(const sSkinHeader&) = delete;
	};

	struct sSkinWeights
	{
		std::string transformNodeName;
		DWORD nWeights;
		std::vector<DWORD> p_vertexIndices;
		std::vector<float> p_weights;
		DirectX::XMFLOAT4X4 matrixOffset;

		sSkinWeights();
		sSkinWeights(sSkinWeights&&) noexcept;
		sSkinWeights(const sSkinWeights&) noexcept;
		~sSkinWeights();
		sSkinWeights& operator=(sSkinWeights&&) noexcept;
		sSkinWeights& operator=(const sSkinWeights&);
	};

	struct sSkinInfo
	{
		sSkinHeader skin_header;
		std::vector<sSkinWeights> p_skin_weights;

		sSkinInfo();
		sSkinInfo(sSkinInfo&&) = delete;
		sSkinInfo(const sSkinInfo&) = delete;
		~sSkinInfo();
		sSkinInfo& operator=(sSkinInfo&&) = delete;
		sSkinInfo& operator=(const sSkinInfo&) = delete;
		void CreateWeights(DWORD, DWORD, DWORD);
	};
}

#endif
