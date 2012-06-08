// Flatland.
// Copyright (C) 2003 Marc A. Lepage.


#pragma warning(disable: 4786)


#include "variable.h"


Directory<Variable>
Variable::m_variableDirectory;


#define DEF_VAR(name, value) Variable Variable::name(_T(#name), _T(value))
	DEF_VAR(file_base_dir, "");
	DEF_VAR(first_level, "demo1");
	DEF_VAR(physics_friction, "0.99");
	DEF_VAR(physics_speed_limit, "16");
	DEF_VAR(render_brush_bounds, "0");
	DEF_VAR(render_brush_outline, "0");
	DEF_VAR(render_console, "0");
	DEF_VAR(render_editor_image, "0.5");
	DEF_VAR(render_editor_model, "1");
	DEF_VAR(render_entity_bounds, "0");
	DEF_VAR(render_entity_identifier, "0");
	DEF_VAR(render_entity_image, "0");
	DEF_VAR(render_entity_velocity, "0");
	DEF_VAR(render_frame_info, "0");
	DEF_VAR(render_model_bounds, "0");
	DEF_VAR(stylus_easy, "0");
#undef DEF_VAR


/*******************************************************************************
*******************************************************************************/
void
Variable::init()
{
#define REG_VAR(name) registerVariable(name)
	REG_VAR(file_base_dir);
	REG_VAR(first_level);
	REG_VAR(physics_friction);
	REG_VAR(physics_speed_limit);
	REG_VAR(render_brush_bounds);
	REG_VAR(render_brush_outline);
	REG_VAR(render_console);
	REG_VAR(render_editor_image);
	REG_VAR(render_editor_model);
	REG_VAR(render_entity_bounds);
	REG_VAR(render_entity_identifier);
	REG_VAR(render_entity_image);
	REG_VAR(render_entity_velocity);
	REG_VAR(render_frame_info);
	REG_VAR(render_model_bounds);
	REG_VAR(stylus_easy);
#undef REG_VAR
}
