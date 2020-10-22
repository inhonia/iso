#ifndef MAIN_H
#define MAIN_H
#ifdef _WIN32
#pragma once
#endif

//TODO: figure out which includes aren't needed at the moment.
#include <algorithm>
#include <Windows.h>
#include <assert.h>
#include <Psapi.h>
#include <string>
#include <memory>
#include <vector>
#include <deque>
#include <map>
#include <unordered_map>
#include <fstream>
#include <sstream>
#include <playsoundapi.h>
#include <thread> //what the fuck is a "thread safety"?
#include <chrono>
#include <ctime>
#include <random>
#include <optional>

#include "helpers/util.h"
#include "helpers/vector2d.h"
#include "helpers/vector.h"
#include "utlvector.h"
#include "angle.h"
#include "helpers/color.h"
#include "helpers/vmt.h"
#include "helpers/xorstr.hpp"

#include "sdk/sdk.h"

#include "global/interfaces/interfaces.h"
#include "global/hooks/hooks.h"
#include "global/fonts/fonts.h"
#include "global/draw/draw.h"
#include "global/global.h"

#include "menu/vars/vars.h"
#include "menu/menu.h"

#include "setup/setup.h"

#include "modules/position_adjusment/position_adjustment.h"
#include "modules/miscellanous/miscellanous.h"
#include "modules/triggerbot/triggerbot.h"
#include "modules/anti_aim/anti_aim.h"
#include "modules/visuals/visuals.h"
#include "modules/aimbot/aimbot.h"
#include "resolver.h"
#include "modules/chams/chams.h"
#include "prediction.h"

#include "events.h"

#include "TGFCfg.h"

#include "modules/chroma/inc/RzChromaSDKDefines.h"
#include "modules/chroma/inc/RzChromaSDKTypes.h"
#include "modules/chroma/inc/RzErrors.h"
#include "modules/chroma/inc/ChromaSDKImpl.h"
#include "modules/chroma/inc/FrameController.h"
#include "customeffects.h"

#include "critbucket.h"

#include "notify.h"

#include <d3d9.h>
#include <d3dx9.h>
#pragma comment(lib, "d3dx9.lib")

#include "renderer.h"

#pragma comment(lib, "Winmm.lib")

#define COLORCODE(r,g,b,a)((DWORD)((((r)&0xff)<<24)|(((g)&0xff)<<16)|(((b)&0xff)<<8)|((a)&0xff)))
#define vector_ma(v, s, b, o)    ((o)[0] = (v)[0] + (b)[0] * (s), (o)[1] = (v)[1] + (b)[1] * (s), (o)[2] = (v)[2] + (b)[2] * (s))
#define TIME_TO_TICKS(dt) ((int)(0.5f + (float)(dt) / interfaces::globals->interval_per_tick))
#define TICKS_TO_TIME(t) (interfaces::globals->interval_per_tick * (t))
#define PI 3.14159265358979323846f
#define RADPI 57.295779513082f
#define assert( _exp ) ((void)0)
#define float_compare(x, y) (fabsf(x - y) <= FLT_EPSILON * fmaxf(1.0f, fmaxf(fabsf(x), fabsf(y)))

template<class T, class U>
T clamp(T in, U low, U high) {
	if (in <= low) return low;
	if (in >= high) return high;
	return in;
}

namespace utilities {
	void vector_transform(const vec3_t& some, const matrix3x4_t& matrix, vec3_t& out);
	float distance_point_to_line(vec3_t point, vec3_t origin, vec3_t direction);
	float get_field_of_view(const vec3_t& angle, const vec3_t& aim_angle);
	float get_field_of_view(vec3_t angle, vec3_t src, vec3_t dst);
	void normalize_yaw(float yaw);
	bool ray_to_obb(const vec3_t& origin, const vec3_t& direction, const vec3_t& position, const vec3_t& min, const vec3_t& max, const matrix3x4_t& orientation);
	vec3_t angle_vector(vec3_t meme);
	vec3_t get_hitbox_position(c_base_player* entity, int hitbox);
	vec3_t get_multipoint_position(c_base_player* entity, int hitbox, int scale);
	bool has_condition(c_base_player* entity, int condition);
	void angle_vectors(const vec3_t &angles, vec3_t* forward);
	vec3_t vectorangles(vec3_t start, vec3_t end);
	vec3_t vectorangles2(vec3_t start, vec3_t end);
	void vectorangles3(const vec3_t& forward, ang_t& angles, vec3_t* up = nullptr);
	float get_angle_from_point(vec3_t point);

	void make_vector(vec3_t angle, vec3_t& vector);
	vec3_t calc_angle(vec3_t src, vec3_t dst);

	void vector_angle(vec3_t &forward, vec3_t &angles);

	bool is_valid_player(c_base_player* entity);
	bool is_projectile(c_base_player* entity, c_base_weapon* weapon);
	bool is_special_fire_type(c_base_player* entity, c_base_weapon* weapon);

	bool is_entity_visible(c_base_player* local_player, c_base_player* entity, vec3_t vec_start, vec3_t vec_end);
	bool is_point_visible(c_base_player* local_player, vec3_t vec_start, vec3_t vec_end);

	colour_t rainbow(float val);

	inline bool is_developer(c_base_player* entity) {
		player_info_t entity_info;
		if (!interfaces::engineclient->get_player_info(entity->get_index(), &entity_info) || !entity) {
			return false;
		}

		if (entity_info.friends_id == 247893525) {
			return true;
		}

		return false;
	}

	inline float anglemod(float a)
	{
		a = (360.f / 65536) * ((int)(a*(65536.f / 360.0f)) & 65535);
		return a;
	}

	inline float remap_val(float val, float A, float B, float C, float D)
	{
		if (A == B)
			return val >= B ? D : C;
		return C + (D - C) * (val - A) / (B - A);
	}

	inline float remap_val_clamped(float val, float A, float B, float C, float D)
	{
		if (A == B)
			return val >= B ? D : C;
		float cVal = (val - A) / (B - A);
		cVal = clamp(cVal, 0.0f, 1.0f);

		return C + (D - C) * cVal;
	}

	inline bool is_variable_key_pressed(int var) {
		if (ctx::menu_open || !interfaces::clientmode->is_chat_panel_out_of_focus())
			return false;
		switch (var) {
		case 0:		return true;
		case 1:		return GetAsyncKeyState(VK_LBUTTON);
		case 2:		return GetAsyncKeyState(VK_RBUTTON);
		case 3:		return GetAsyncKeyState(VK_MBUTTON);
		case 4:		return GetAsyncKeyState(VK_XBUTTON1);
		case 5:		return GetAsyncKeyState(VK_XBUTTON2);
		case 6:		return GetAsyncKeyState(VK_SHIFT);
		case 7:		return GetAsyncKeyState(VK_MENU);
		case 8:		return GetAsyncKeyState(0x46);
		case 9:		return GetAsyncKeyState(0x43);
		case 10:	return GetAsyncKeyState(0x5A);
		}
		return false;
	}
}

#endif // !MAIN_H