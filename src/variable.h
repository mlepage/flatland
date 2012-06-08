// Flatland.
// Copyright (C) 2003 Marc A. Lepage.


#ifndef GUARD__VARIABLE
#define GUARD__VARIABLE


#include <cstdio>
#include "directory.h"
#include "tstring.h"


/*******************************************************************************
	Variables shared by both the shell and the program code.
*******************************************************************************/
class Variable
{

public:

	static
	Variable*
	find(
		const tstring& ksName);

	static
	void
	init();


public:

#define DECL_VAR(name) static Variable name
	DECL_VAR(file_base_dir);
	DECL_VAR(first_level);
	DECL_VAR(render_brush_bounds);
	DECL_VAR(render_brush_outline);
	DECL_VAR(render_entity_bounds);
	DECL_VAR(render_entity_identifier);
	DECL_VAR(render_entity_image);
	DECL_VAR(render_entity_velocity);
	DECL_VAR(render_model_bounds);
	DECL_VAR(stylus_easy);
#undef DECL_VAR


public:

	float
	getFloatValue() const;

	const tstring&
	getName() const;

	const tstring&
	getValue() const;

	void
	setValue(
		const tstring& ksValue);


private:

	static
	void
	registerVariable(
		Variable& variable);

	static
	void
	unregisterVariable(
		Variable& variable);


private:

	static
	Directory<Variable> m_variableDirectory;


private:

	Variable(
		const tstring& ksName);

	Variable(
		const tstring& ksName,
		const tstring& ksValue);


private:

	tstring m_sName;

	tstring m_sValue;

	float m_fValue;


};


/*******************************************************************************
*******************************************************************************/
inline
Variable::Variable(
	const tstring& ksName) :
	m_sName(ksName),
	m_fValue(0)
{
}


/*******************************************************************************
*******************************************************************************/
inline
Variable::Variable(
	const tstring& ksName,
	const tstring& ksValue) :
	m_sName(ksName)
{
	setValue(ksValue);
}


/*******************************************************************************
	Returns null if no variable with that name is registered.
*******************************************************************************/
inline
Variable*
Variable::find(
	const tstring& ksName)
{
	return m_variableDirectory.find(ksName);
}


/*******************************************************************************
*******************************************************************************/
inline
float
Variable::getFloatValue() const
{
	return m_fValue;
}


/*******************************************************************************
*******************************************************************************/
inline
const tstring&
Variable::getName() const
{
	return m_sName;
}


/*******************************************************************************
*******************************************************************************/
inline
const tstring&
Variable::getValue() const
{
	return m_sValue;
}


/*******************************************************************************
	Note that this will effectively unregister a previously registered variable
	with the same name.
*******************************************************************************/
inline
void
Variable::registerVariable(
	Variable& variable)
{
	m_variableDirectory.insert(variable.getName(), variable);
}


/*******************************************************************************
*******************************************************************************/
inline
void
Variable::setValue(
	const tstring& ksValue)
{
	m_sValue = ksValue;

#if 0
	// This function doesn't appear to exist in my version.
	// It should be the same, but quicker, because it doesn't have to parse
	// the format specifier. But just use sscanf for now.
	m_fValue = _tstof(kpktValue);
#else
	_stscanf(ksValue.c_str(), _T("%f"), &m_fValue);
#endif
}


/*******************************************************************************
	Note that if you were to unregister one of the static variables, it would
	continue to be used by the code but not by the shell. Even if a new
	variable with the same name were created by the shell, it wouldn't be used
	by the code.
*******************************************************************************/
inline
void
Variable::unregisterVariable(
	Variable& variable)
{
	m_variableDirectory.erase(variable.getName());
}


#endif // GUARD
