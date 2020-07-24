#pragma once
#ifndef _OBJECT_UID_H
#define _OBJECT_UID_H

enum HighUID
{
	TYPE_CONNECTION = 1,
	TYPE_PLAYER,
	TYPE_OBJECT,
	TYPE_NPC,
	TYPE_AI,
	NUM_TYPE,
};

class ObjectUID
{
	long _HighID, _LowID;
public:
	ObjectUID() : _HighID(0), _LowID(0) {}

	void clear() { _HighID = 0; _LowID = 0; }

	HighUID GetType() { return HighUID(_HighID); }
};
#endif // !_OBJECT_UID_H
