#pragma once

#ifndef _LINKEDLIST_H
#define _LINKEDLIST_H

namespace Memory {
namespace LinkedList {
	class Head;
	class Element
	{
	private:
		friend class Head;
		Element* _Previous;
		Element* _Next;
	public:
		Element() : _Previous(NULL), _Next(NULL) {}
		virtual ~Element() { }
		
		bool is() const { return (_Next != NULL && _Previous != NULL); }

		bool hasNext() const { return (_Next != NULL && _Next->_Next != NULL); }
		bool hasPrevious() const { return (_Previous != NULL && _Previous->_Previous != NULL); }
		
		Element* Next() { return hasNext() ? _Next : NULL; }
		Element* Previous() { return hasPrevious() ? _Previous : NULL; }

		Element const* Next() const { return hasNext() ? _Next : NULL; }
		Element const* Previous() const { return hasPrevious() ? _Previous : NULL; }

		virtual void remove() 
		{
			if (is()) //Bind next & previous element of this element together and reset this node (Delete it from list)
			{
				Element::bind(_Previous, _Next);
				_Next = NULL;
				_Previous = NULL;
			}
		}

		void insertBefore(Element* element);
		void insertAfter(Element* element);

	private:
		inline static void bind(Element* from, Element* to)
		{
			from->_Next = to;
			to->_Previous = from;
		}

		Element(Element const&) { }
		Element& operator=(Element const&) { }
	};

	class Head
	{
	private:
		Element _First;
		Element _Last;
		unsigned int _Size;
	public:
		Head();

		bool isEmpty() const;

		Element* First();
		Element* Last();
		Element const* First() const;
		Element const* Last() const;

		void insertFirst(Element* element);
		void insertLast(Element* element);

		unsigned int Size();
	private:
		Head(Head const&){}
		Head& operator=(Head const&) {}
	};


} //END NAMESPACE LINKEDLIST
} //END NAMESPACE MEMORY

#endif // !_LINKEDLIST_H
