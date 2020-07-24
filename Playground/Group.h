#pragma once
#ifndef _SYSTEM_GROUP_H
#define _SYSTEM_GROUP_H

#define MAX_SUBGROUP_SIZE 5
#define MAX_SUBGROUPS 5
#define MAX_GROUP_SIZE MAX_SUBGROUP_SIZE * MAX_SUBGROUPS
#include <list>
#include <set>

	enum GroupMemberStatus
	{
		MEMBER_STATUS_OFFLINE = 0x0000,
		MEMBER_STATUS_ONLINE  = 0x0001,
		MEMBER_STATUS_DEAD    = 0x0002,
		MEMBER_STATUS_AFK     = 0x0004,
		MEMBER_STATUS_DND     = 0x0008,
	};

	class Player;
	class Group
	{
	public:
		struct Member {
			ObjectUID id;
			unsigned short _class;
			unsigned short group;
			GroupMemberStatus status;
		};
		typedef std::list<Member> GroupMembers;
		typedef std::set<Player*> GroupInvites;
		typedef unsigned short SubGroups;

		unsigned int GetMembersCount() const { return _Members.size(); }
		
		void Create(Player* /* Leader */);

		bool AddMember(Player*);
		bool RemoveMember(ObjectUID&/* Kick */, ObjectUID&/* Who said */);

		void SetLeader(ObjectUID&);
		void ChangeLeader(ObjectUID&/* New */, ObjectUID&/* Who said */);

		void Disband();

		bool AddInvite(Player*);
		void RemoveInvite(Player*);
		void RemoveAllInvites();

		bool IsMember(ObjectUID &) const;
		bool IsLeader(ObjectUID &) const;

		bool IsFull() const;
		bool IsFullSub(unsigned char group) const;

		Player* GetInvited(ObjectUID&) const;

		void SendAll(Network::Packet& /* packet */) const;
		void SendAllBut(Network::Packet& /* packet */, ObjectUID& /* Skip */) const;

		void IncreaseSubCount(unsigned char group);

	private:
		GroupManager _MemberManager;
		GroupInvites _Invites;
		GroupMembers _Members;
		ObjectUID    _Leader;
		SubGroups    _subGroups[MAX_SUBGROUPS];
	};

#endif // !_SYSTEM_GROUP_H
