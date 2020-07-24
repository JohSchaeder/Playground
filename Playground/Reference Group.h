#pragma once

#ifndef _GROUP_REFERENCE_H
#define _GROUP_REFERENCE_H

using namespace Memory::LinkedList;
	class Group;
	class Player;
	enum GroupTypes
	{
		USE_SUB_GROUP,
		NO_SUB_GROUP,
	};
	template<GroupTypes type=NO_SUB_GROUP>
	class OptionalGroupReference : public Reference<Group, Player>
	{
	public:
		OptionalGroupReference() : Reference<Group, Player>() {}
		~OptionalGroupReference() { remove(); }

		OptionalGroupReference* operator()(Group* dest, Player* source) { return (OptionalGroupReference*)Reference<Group, Player>::operator()(dest, source); }
		OptionalGroupReference* Next() { return ((OptionalGroupReference*)Reference<Group, Player>::Next()); }
		OptionalGroupReference const* Next() const { return ((OptionalGroupReference const*)Reference<Group, Player>::Next()); }
	};
	//Player has object GroupReference,
	template<>
	class OptionalGroupReference<USE_SUB_GROUP> : public Reference<Group, Player>
	{
	protected:
		unsigned short _SubGroup;
	public:
		OptionalGroupReference() : Reference<Group, Player>(), _SubGroup(0) {}
		~OptionalGroupReference() { remove(); }

		OptionalGroupReference* operator()(Group* dest, Player* source, unsigned short subgroup) { return (OptionalGroupReference*)Reference<Group, Player>::operator()(dest, source); }

		OptionalGroupReference* Next() { return ((OptionalGroupReference*)Reference<Group, Player>::Next()); }
		OptionalGroupReference const* Next() const { return ((OptionalGroupReference const*)Reference<Group, Player>::Next()); }

		unsigned short GetSubGroup() const { return _SubGroup; }
		void SetSubGroup(unsigned short num) { _SubGroup = num; }
	};
	typedef OptionalGroupReference<USE_SUB_GROUP> ReferenceGroupElement, GroupElement, GroupLink;
	typedef ReferenceManager<Group, Player> GroupManager;
	/*
	//Group class has object GroupReference, remember to keep a seperate list for actual access of data for members.
	class GroupReferenceManager : public ReferenceManager<Group, Player>
	{
	public:
		GroupReference* First() { return ((GroupReference*)ReferenceManager<Group, Player>::First()); }
		GroupReference const* First() const { return ((GroupReference const*)ReferenceManager<Group, Player>::First()); }
	};*/
	class Player { public: GroupLink group; };

#endif // !_GROUP_REFERENCE_H
