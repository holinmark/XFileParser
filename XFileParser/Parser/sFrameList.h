#pragma once

#include <DirectXMath.h>
#include <vector>
#include <string>
#include <unordered_map>
#include "sMeshList.h"

namespace ns_HoLin
{
	struct sFrame
	{
		std::string name;
		std::vector<std::string> mesh; // names of meshes associated with this frame
		std::vector<DirectX::XMFLOAT3> vectors;
		DirectX::XMFLOAT4X4 matrix;
		std::string parent_name;
		sFrame *pnextframe;
		static std::size_t number_of_frames_created;

		sFrame();
		sFrame(sFrame&&) = delete;
		sFrame(const sFrame&) = delete;
		~sFrame();
		void Cleanup(HANDLE = NULL);
		sFrame& operator=(sFrame&&) = delete;
		sFrame& operator=(const sFrame&) = delete;
	};

	struct sSequenceOfFrames
	{
		ns_HoLin::sFrame *pfirstframe, *plastframe;
		std::vector<std::string> framenames;
		std::unordered_map<std::string, std::vector<std::string>> parent_children;
		DWORD number_of_frames_in_sequence;
		sSequenceOfFrames *pnextseq;

		sSequenceOfFrames();
		sSequenceOfFrames(sSequenceOfFrames&&) = delete;
		sSequenceOfFrames(const sSequenceOfFrames&) = delete;
		~sSequenceOfFrames();
		void Cleanup(HANDLE = NULL);
		sSequenceOfFrames& operator=(sSequenceOfFrames&&) = delete;
		sSequenceOfFrames& operator=(const sSequenceOfFrames&) = delete;
		void InsertFrame(sFrame*);
		BOOL Find(std::string_view);
		sFrame* Search(std::string_view);
		sFrame* CreateNewFrame();
		sFrame* GetLastEntry();
	};

	struct sFrames
	{
		ns_HoLin::sSequenceOfFrames *pfirstseq, *plastseq;
		std::size_t number_of_sequences;

		sFrames();
		sFrames(sFrames&&) = delete;
		sFrames(const sFrames&) = delete;
		~sFrames();
		void Cleanup(HANDLE = NULL);
		sFrames& operator=(sFrames&&) = delete;
		sFrames& operator=(const sFrames&) = delete;
		ns_HoLin::sSequenceOfFrames* CreateNewSequence();
		ns_HoLin::sSequenceOfFrames* GetLastSequence() { return plastseq; }
		void InsertSequenceOfFrames(ns_HoLin::sSequenceOfFrames*);
		void InsertFrameIntoLastSequence(sFrame*);
		sFrame* SearchFrames(std::string_view);
		BOOL FindFrames(std::string_view);
		ns_HoLin::sSequenceOfFrames* SearchSequence(std::string_view);
		BOOL FindSequence(std::string_view);
	};
}
