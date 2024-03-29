#ifndef SDK_H
#define SDK_H
#ifdef _WIN32
#pragma once
#endif

// forward declarations
class client_networkable;
class client_renderable;
class client_unknown;
class client_entity;
class base_handle;
class game_trace;
class c_net_channel;
class client_class;
struct model_t;

// type definitions
typedef PVOID(*create_interface_fn)(const char* name, int* return_code);
typedef float matrix3x4_t[3][4];
typedef game_trace trace_t;

#pragma region includes

// client
#include "client/ibaseclientdll.h"
#include "client/icliententitylist.h"
#include "client/iclientmodeshared.h"
#include "client/user_cmd.h"
#include "client/c_base_player.h"
#include "client/model_info.h"

// engine
#include "engine/ivengineclient.h"
#include "engine/ivdebugoverlay.h"
#include "engine/ienginetrace.h"
#include "engine/ienginevgui.h"
#include "engine/iefx.h"
#include "engine/inetchannel.h"
#include "engine/ieventmanager.h"
#include "../iglowobject.h"
#include "../igamemovement.h"
#include "../imovehelper.h"
#include "../iprediction.h"

// public
#include "public/global_vars_base.h"

// vgui
#include "vgui/isurface.h"
#include "vgui/ipanel.h"

// convar
#include "convar/iconvar.h"

// misc defines
#include "../weapon_defs.h"
#include "../bsp_flags.h"
#include "../md5.h"

#pragma endregion

#endif // !SDK_H