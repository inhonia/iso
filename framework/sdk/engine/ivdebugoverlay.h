#ifndef IVDEBUGOVERLAY_H
#define IVDEBUGOVERLAY_H
#ifdef _WIN32
#pragma once
#endif

class ivdebugoverlay
{
public:
	void add_box_overlay(const vec3_t& origin, const vec3_t& mins, const vec3_t& max, vec3_t const& orientation, int r, int g, int b, int a, float duration) {
		using original_fn = void(__thiscall*)(PVOID, const vec3_t&, const vec3_t&, const vec3_t&, vec3_t const&, int, int, int, int, float);
		return utilities::call_vfunc<original_fn>(this, 2)(this, origin, mins, max, orientation, r, g, b, a, duration);
	}

	int screen_position(const vec3_t& in, vec3_t& out)
	{
		using original_fn = int(__thiscall*)(PVOID, const vec3_t&, vec3_t&);
		return utilities::call_vfunc<original_fn>(this, 10)(this, in, out);
	}

};

#endif // !IVDEBUGOVERLAY_H