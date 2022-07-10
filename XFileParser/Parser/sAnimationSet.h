#pragma once

#ifndef SANIMATIONSET_H
#define SANIMATIONSET_H

#include <DirectXMath.h>
#include <Windows.h>

#include <string>
#include <utility>
#include <vector>

namespace ns_HoLin
{
	struct sTimedFloatKeys
	{
		DWORD time;
		std::vector<float> tfkeys;

		sTimedFloatKeys();
		sTimedFloatKeys(sTimedFloatKeys&&);
		sTimedFloatKeys(const sTimedFloatKeys&) = delete;
		~sTimedFloatKeys();
		void Cleanup(HANDLE = NULL);
		sTimedFloatKeys& operator=(sTimedFloatKeys&&) = delete;
		sTimedFloatKeys& operator=(const sTimedFloatKeys&) = delete;
	};

	struct sAnimation_Data
	{
		// 0 for rotation DirectX::XMFLOAT3
		// 1 for scale DirectX::XMFLOAT3
		// 2 for translate DirectX::XMFLOAT3
		DWORD type_of_transform;
		std::vector<ns_HoLin::sTimedFloatKeys> transformation_data;
		sAnimation_Data *pnextanimation_data;

		sAnimation_Data();
		sAnimation_Data(sAnimation_Data&&) = delete;
		sAnimation_Data(const sAnimation_Data&) = delete;
		~sAnimation_Data();
		void Cleanup(HANDLE = NULL);
		sAnimation_Data& operator=(sAnimation_Data&&) = delete;
		sAnimation_Data& operator=(const sAnimation_Data&) = delete;
	};

	struct sAnimation
	{
		std::string name;
		std::vector<std::string> list_of_objects;
		sAnimation_Data *pfirst_data, *plast_data;
		sAnimation *pnextanimation;

		sAnimation();
		sAnimation(sAnimation&&) = delete;
		sAnimation(const sAnimation&) = delete;
		~sAnimation();
		void Cleanup(HANDLE = NULL);
		sAnimation& operator=(sAnimation&&) = delete;
		sAnimation& operator=(const sAnimation&) = delete;
		sAnimation_Data* CreateAnimation_Data();
		void InsertAnimation_Data(sAnimation_Data*);
	};

	struct sAnimationSet
	{
		std::string name;
		sAnimation *pfirstanimation, *plastanimation;
		sAnimationSet *pnext_set;

		sAnimationSet();
		sAnimationSet(sAnimationSet&&) = delete;
		sAnimationSet(const sAnimationSet&) = delete;
		~sAnimationSet();
		void Cleanup(HANDLE = NULL);
		sAnimationSet& operator=(sAnimationSet&&) = delete;
		sAnimationSet& operator=(const sAnimationSet&) = delete;
		sAnimation* CreateAnimation(); // creates an animation object to be stored inside an AnimationSet
		void Insert(sAnimation*);
		sAnimation* SearchForAnimation(std::string_view);
	};

	struct sAnimationSetList
	{
		DWORD animtickspersecond;
		sAnimationSet *pfirst_set, *plast_set;
		sAnimation *pfirst_animation, *plast_animation; // Any declared animation outside of AnimationSet is stored

		sAnimationSetList();
		sAnimationSetList(sAnimationSetList&&) = delete;
		sAnimationSetList(const sAnimationSetList&) = delete;
		~sAnimationSetList();
		void Cleanup(HANDLE = NULL);
		sAnimationSetList& operator=(sAnimationSetList&&) = delete;
		sAnimationSetList& operator=(const sAnimationSetList&) = delete;
		sAnimationSet* CreateAnimationSet();
		sAnimationSet* GetLastAnimationSet() { return plast_set; } // check if returns nullptr
		sAnimationSet* SearchForAnimationSet(std::string_view);
		sAnimation* SearchForAnimation(std::string_view);
		sAnimation* CreateAnimation(); // creates an animation object outside of an AnimationSet
		void InsertAnimation(sAnimation*);
	};
}

#endif
