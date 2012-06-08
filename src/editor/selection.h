// Flatland.
// Copyright (C) 2003 Marc A. Lepage.


#ifndef GUARD__SELECTION
#define GUARD__SELECTION


#include <algorithm>
#include <vector>


/*******************************************************************************
	Maintains an ordered selection by type.
*******************************************************************************/
template <typename ELEMENT>
class Selection
{


public:

	typedef std::vector<ELEMENT*>::iterator Iterator;


public:

	Selection::Iterator
	begin()
	{
		return m_cpElement.begin();
	}

	void
	clear()
	{
		m_cpElement.clear();
	}

	// Returns true if actually deselected.
	bool
	deselect(
		ELEMENT& element)
	{
		Iterator i =
			std::find(m_cpElement.begin(), m_cpElement.end(), &element);
		if (i == m_cpElement.end())
		{
			return false;
		}
		m_cpElement.erase(i);
		return true;
	}

	// Returns true if any element is actually deselected.
	bool
	deselect(
		const std::vector<ELEMENT*> cpElement)
	{
		bool bResult = false;
		for (int n = 0; n != cpElement.size(); ++n)
		{
			bResult |= deselect(*cpElement[n]);
		}
		return bResult;
	}

	Selection::Iterator
	end()
	{
		return m_cpElement.end();
	}

	bool
	isEmpty() const
	{
		return m_cpElement.empty();
	}

	bool
	isSelected(
		ELEMENT& element) const
	{
		return std::find(m_cpElement.begin(), m_cpElement.end(),
			&element) != m_cpElement.end();
	}

	int
	getSize() const
	{
		return m_cpElement.size();
	}

	// Returns true if actually selected.
	bool
	select(
		ELEMENT& element)
	{
		if (isSelected(element))
		{
			return false;
		}
		m_cpElement.push_back(&element);
		return true;
	}

	// Returns true if any element is actually selected.
	bool
	select(
		const std::vector<ELEMENT*> cpElement)
	{
		bool bResult = false;
		for (int n = 0; n != cpElement.size(); ++n)
		{
			bResult |= select(*cpElement[n]);
		}
		return bResult;
	}

	// Returns true if selected, false if unselected.
	bool
	toggle(
		ELEMENT& element)
	{
		Selection::Iterator i =
			std::find(m_cpElement.begin(), m_cpElement.end(), &element);
		if (i == m_cpElement.end())
		{
			// Select.
			m_cpElement.push_back(&element);
			return true;
		}
		else
		{
			// Deselect.
			m_cpElement.erase(i);
			return false;
		}
	}

	// Returns true if any element is actually select.
	bool
	toggle(
		const std::vector<ELEMENT*> cpElement)
	{
		bool bResult = false;
		for (int n = 0; n != cpElement.size(); ++n)
		{
			bResult |= toggle(*cpElement[n]);
		}
		return bResult;
	}


private:

	std::vector<ELEMENT*> m_cpElement;


};


#endif // GUARD
