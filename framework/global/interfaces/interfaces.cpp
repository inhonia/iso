#include "../../main.h"

namespace interfaces {
	ibaseclientdll*			interfaces::client;
	icliententitylist*		interfaces::entitylist;
	iclientmodeshared*		interfaces::clientmode;
	imodel_info*			interfaces::model_info;
	imodel_render*			interfaces::model_render;
	imaterial_system*		interfaces::material_system;
	irender_view*			interfaces::render_view;
	igame_movement*			interfaces::game_movement;
	C_Prediction*			interfaces::prediction;
	CMoveHelper*			interfaces::move_helper;

	ivengineclient*			interfaces::engineclient;
	ienginetrace*			interfaces::enginetrace;
	ivdebugoverlay*			interfaces::debugoverlay;
	ienginevgui*			interfaces::enginevgui;
	int32_t*				interfaces::random_seed;
	ivefx*					interfaces::efx;

	global_vars_base*		interfaces::globals;
	DWORD*					interfaces::d3ddevice;

	ipanel*					interfaces::panel;
	isurface*				interfaces::surface;

	iconvar*				interfaces::convar;
	igame_event_manager*	interfaces::game_event;

	glow_manager_t*			interfaces::glow_object;

	//iendscene*				interfaces::end_scene;
	//resetfn*				interfaces::reset;
}