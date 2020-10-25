#ifndef INTERFACES_H
#define INTERFACES_H
#ifdef _WIN32
#pragma once
#endif

//using iendscene = long(__stdcall*) (struct IDirect3DDevice9* pDevice);
//using resetfn = long(__stdcall*) (struct IDirect3DDevice9* pDevice, struct D3DPRESENT_PARAMETERS* pPresentParams);

namespace interfaces {
	extern ibaseclientdll*		client;
	extern icliententitylist*	entitylist;
	extern iclientmodeshared*	clientmode;
	extern imodel_info*			model_info;
	extern imodel_render*		model_render;
	extern imaterial_system*	material_system;
	extern irender_view*		render_view;
	extern igame_movement*		game_movement;
	extern iprediction*			prediction;

	extern ivengineclient*		engineclient;
	extern ienginetrace*		enginetrace;
	extern ivdebugoverlay*		debugoverlay;
	extern ienginevgui*			enginevgui;
	extern int32_t*				random_seed;
	extern ivefx*				efx;

	extern global_vars_base*	globals;
	extern DWORD*				d3ddevice;

	extern ipanel*				panel;
	extern isurface*			surface;

	extern iconvar*				convar;
	extern igame_event_manager* game_event;
	extern glow_manager_t*		glow_object;
	//extern iendscene*			end_scene;
	//extern resetfn*				reset;
}

#endif // !INTERFACES_H