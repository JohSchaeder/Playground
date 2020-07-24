#pragma once
#ifndef _REFERENCE_H
#define _REFERENCE_H
using namespace Memory::LinkedList;
	template<class Target, class Source> 
	class Reference : public Element 
	{
	private:
		Target* refTarget;
		Source* refSource;
	protected:
		Reference* operator()(Target* dest, Source* source)
		{
			if (source == NULL)return NULL;
			if (refTarget != NULL)
				remove();
			if (dest != NULL)
			{
				refTarget = dest;
				refSource = source;
				return this;
			}
			return NULL;
		}
	public:
		Reference() : refTarget(NULL), refSource(NULL) {}
		virtual ~Reference() { }

		void remove() final override
		{
			Element::remove();
			refTarget = NULL;
			refSource = NULL;
		}
		void unlink()
		{
			Element::remove();
			refTarget = NULL;
		}

		Reference<Target, Source>* Next() { return ((Reference<Target, Source>*)Element::Next()); }
		Reference<Target, Source>* Previous() { return ((Reference<Target, Source>*)Element::Previous()); }
		Reference<Target, Source> const* Next() const { return ((Reference<Target, Source> const*)Element::Next()); }
		Reference<Target, Source> const* Previous() const { return ((Reference<Target, Source> const*)Element::Previous()); }

		Target* operator->() const { return refTarget; }

		Target* GetTarget() const { return refTarget; }
		Source* GetSource() const { return refSource; }

	private:
		Reference(Reference const&) { }
		Reference& operator=(Reference const&) { }

	};
	template<class Target, class Source>
	class ReferenceManager : public Head
	{
	public:
		ReferenceManager() { }
		~ReferenceManager() {}

		Reference<Target, Source>* Next() { return ((Reference<Target, Source>*)Head::Next()); }
		Reference<Target, Source>* Previous() { return ((Reference<Target, Source>*)Head::Previous()); }
		Reference<Target, Source> const* Next() const { return ((Reference<Target, Source> const*)Head::Next()); }
		Reference<Target, Source> const* Previous() const { return ((Reference<Target, Source> const*)Head::Previous()); }

		void clear()
		{
			Element* element;
			while ((element = First()) != NULL)
				element->remove();
		}
	};
#endif // !_REFERENCE_H
