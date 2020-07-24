#include "stdafx.h"
#include "main.h"

using namespace Memory::LinkedList;


void Element::insertBefore(Element * element)
{
	Element::bind(_Previous, element);
	Element::bind(element, this);
}

void Element::insertAfter(Element * element)
{
	Element::bind(element, _Next);
	Element::bind(this, element);
}

inline Head::Head() : _Size(0) { Element::bind(&_First, &_Last); }

inline bool Head::isEmpty() const { return _First._Next->is(); }

inline Element * Head::First() { return _First.Next(); }

inline Element * Head::Last() { return _Last.Previous(); }

inline Element const * Head::First() const { return _First.Next(); }

inline Element const *Head::Last() const { return _Last.Previous(); }

inline void Head::insertFirst(Element * element) { _First.insertAfter(element); }

inline void Head::insertLast(Element * element) { _Last.insertBefore(element); }

unsigned int Head::Size()
{
	if (_Size == 0)
	{
		unsigned int retValue = 0;
		Element const* element = First();
		while (element != NULL)
		{
			retValue++;
			element = element->Next();
		}
		return retValue;
	}
	return _Size;
}
