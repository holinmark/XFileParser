#include "stdafx.h"

namespace ns_HoLin
{
	sTimedFloatKeys::sTimedFloatKeys()
	{
		time = 0;
	}

	sTimedFloatKeys::sTimedFloatKeys(sTimedFloatKeys &&other)
	{
		this->time = other.time;
		other.time = 0;
		this->tfkeys = std::move(other.tfkeys);
	}

	sTimedFloatKeys::~sTimedFloatKeys()
	{
		Cleanup();
	}

	void sTimedFloatKeys::Cleanup(HANDLE hfile)
	{
		time = 0;
		tfkeys.clear();
	}

	sAnimation_Data::sAnimation_Data()
	{
		pnext_animation_data = nullptr;
	}

	sAnimation_Data::~sAnimation_Data()
	{
		sAnimation_Data::Cleanup();
	}

	void sAnimation_Data::Cleanup(HANDLE hfile)
	{
		if (pnext_animation_data) {
			delete pnext_animation_data;
			pnext_animation_data = nullptr;
		}
	}
	
	sAnimation::sAnimation()
	{
		pfirst_data = nullptr;
		plast_data = nullptr;
		pnext_animation = nullptr;
	}

	sAnimation::~sAnimation()
	{
		Cleanup();
	}
	
	void sAnimation::Cleanup(HANDLE hfile)
	{
		name.clear();
		list_of_objects.clear();
		if (pfirst_data) {
			delete pfirst_data;
			pfirst_data = nullptr;
			plast_data = nullptr;
		}
		if (pnext_animation) {
			delete pnext_animation;
			pnext_animation = nullptr;
		}
	}

	sAnimation_Data* sAnimation::CreateAnimation_Data()
	{
		sAnimation_Data *p = new sAnimation_Data;

		if (p) {
			if (plast_data) {
				plast_data->pnext_animation_data = p;
				plast_data = p;
			}
			else {
				pfirst_data = p;
				plast_data = p;
			}
		}
		else
			p = nullptr;
		return (sAnimation_Data*)p;
	}

	void sAnimation::InsertAnimation_Data(sAnimation_Data *p)
	{
		if (plast_data) {
			plast_data->pnext_animation_data = p;
			plast_data = p;
		}
		else {
			pfirst_data = p;
			plast_data = p;
		}
	}
	
	BOOL sAnimation::SearchOwnerList(std::string_view name)
	{
		for (std::size_t i = 0; i < this->list_of_objects.size(); i++) {
			if (name == this->list_of_objects[i]) {
				return TRUE;
			}
		}
		return FALSE;
	}
	
	sAnimationSet::sAnimationSet()
	{
		pfirst_animation = nullptr;
		plast_animation = nullptr;
		pnext_set = nullptr;
	}

	sAnimationSet::~sAnimationSet()
	{
		Cleanup();
	}

	void sAnimationSet::Cleanup(HANDLE hfile)
	{
		name.clear();
		if (pfirst_animation) {
			delete pfirst_animation;
		}
		if (pnext_set) {
			delete pnext_set;
		}
		pfirst_animation = NULL;
		plast_animation = NULL;
		pnext_set = NULL;
	}

	sAnimation* sAnimationSet::CreateAnimation()
	{
		ns_HoLin::sAnimation *p_animation = (ns_HoLin::sAnimation*)new ns_HoLin::sAnimation;

		if (p_animation) {
			if (plast_animation) {
				plast_animation->pnext_animation = p_animation;
				plast_animation = p_animation;
			}
			else {
				pfirst_animation = p_animation;
				plast_animation = p_animation;
			}
		}
		else
			p_animation = nullptr;
		return (ns_HoLin::sAnimation*)p_animation;
	}

	void sAnimationSet::Insert(sAnimation *p)
	{
		if (plast_animation) {
			plast_animation->pnext_animation = p;
			plast_animation = p;
		}
		else {
			pfirst_animation = p;
			plast_animation = p;
		}
	}

	sAnimation* sAnimationSet::SearchForAnimation(std::string_view find_name)
	{
		if (pfirst_animation) {
			sAnimation *p = pfirst_animation;
			while (p) {
				if (p->name == find_name)
					return p;
				p = p->pnext_animation;
			}
		}
		return (sAnimation*)nullptr;
	}

	sAnimationSetList::sAnimationSetList()
	{
		anim_ticks_per_second = 0;
		pfirst_set = nullptr;
		plast_set = nullptr;
		pfirst_animation = nullptr;
		plast_animation = nullptr;
	}

	sAnimationSetList::~sAnimationSetList()
	{
		Cleanup();
	}

	void sAnimationSetList::Cleanup(HANDLE hfile)
	{
		anim_ticks_per_second = 0;
		if (pfirst_set) {
			delete pfirst_set;
			pfirst_set = nullptr;
			plast_set = nullptr;
		}
		if (pfirst_animation) {
			delete pfirst_animation;
			pfirst_animation = nullptr;
			plast_animation = nullptr;
		}
	}

	sAnimationSet* sAnimationSetList::CreateAnimationSet()
	{
		sAnimationSet *pset = (sAnimationSet*)new sAnimationSet;

		if (pset) {
			if (plast_set) {
				plast_set->pnext_set = pset;
				plast_set = pset;
			}
			else {
				pfirst_set = pset;
				plast_set = pset;
			}
		}
		else
			pset = nullptr;
		return (sAnimationSet*)pset;
	}

	sAnimationSet* sAnimationSetList::SearchForAnimationSet(std::string_view name)
	{
		if (pfirst_set) {
			sAnimationSet *p = pfirst_set;
			while (p) {
				if (p->name == name)
					return p;
				p = p->pnext_set;
			}
		}
		return (sAnimationSet*)nullptr;
	}

	sAnimation* sAnimationSetList::SearchForAnimation(std::string_view name)
	{
		if (pfirst_set) {
			sAnimationSet *pset = pfirst_set;
			sAnimation *panimation = pset->pfirst_animation;

			while (pset) {
				while (panimation) {
					if (panimation->name == name)
						return panimation;
					panimation = panimation->pnext_animation;
				}
				pset = pset->pnext_set;
			}
		}
		return (sAnimation*)nullptr;
	}
	
	
	sAnimation* sAnimationSetList::SearchAnimationsForString(std::string_view name)
	{
		sAnimationSet *p_set = pfirst_set;
		sAnimation *p_anim = nullptr;
		
		while (p_set) {
			p_anim = p_set->pfirst_animation;
			while (p_anim) {
				for (auto n : p_anim->list_of_objects) {
					if (n == name) {
						return p_anim;
					}
				}
				p_anim = p_anim->pnext_animation;
			}
			p_set = p_set->pnext_set;
		}
		return (sAnimation*)nullptr;
	}

	sAnimation* sAnimationSetList::CreateAnimation()
	{
		sAnimation *p_animation = new sAnimation;
		
		if (p_animation) {
			if (plast_animation) {
				plast_animation->pnext_animation = p_animation;
				plast_animation = p_animation;
			}
			else {
				pfirst_animation = p_animation;
				plast_animation = p_animation;
			}
		}
		else {
			p_animation = nullptr;
		}
		return (sAnimation*)p_animation;
	}
	
	void sAnimationSetList::InsertAnimation(ns_HoLin::sAnimation *p)
	{
		if (plast_animation) {
			plast_animation->pnext_animation = p;
			plast_animation = p;
		}
		else {
			pfirst_animation = p;
			plast_animation = p;
		}
	}
}
