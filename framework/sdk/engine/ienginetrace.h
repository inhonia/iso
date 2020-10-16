#ifndef IENGINETRACE_H
#define IENGINETRACE_H
#ifdef _WIN32
#pragma once
#endif

struct ray_t
{

	vec3_t_aligned  start;			// starting point, centered within the extents
	vec3_t_aligned  delta;			// direction + length of the ray
	vec3_t_aligned  start_offset;	// Add this to m_Start to get the actual ray start
	vec3_t_aligned  extents;		// Describes an axis aligned box extruded along a ray

	bool	is_ray;		// are the extents zero?
	bool	is_swept;	// is delta != 0?

	void init(vec3_t const& start, vec3_t const& end)
	{

		assert( &end );
		vec3_t_subtract(end, start, delta);

		is_swept = (delta.length_sqr() != 0);

		vec3_t_clear(extents);
		is_ray = true;

		vec3_t_clear(start_offset);
		vec3_t_copy(start, this->start);

	}

	void init(vec3_t const& start, vec3_t const& end, vec3_t const& mins, vec3_t const& maxs)
	{

		assert( &end );
		vec3_t_subtract(end, start, delta);

		is_swept = (delta.length_sqr() != 0);

		vec3_t_subtract(maxs, mins, extents);
		extents *= 0.5f;
		is_ray = (extents.length_sqr() < 1e-6);

		vec3_t_add(mins, maxs, start_offset);
		start_offset *= 0.5f;

		vec3_t_add(start, start_offset, this->start);
		start_offset *= -1.0f;

	}

};

// standard trace filter
enum trace_type_t
{
	TRACE_EVERYTHING = 0,
	TRACE_WORLD_ONLY,
	TRACE_ENTITIES_ONLY,
	TRACE_EVERYTHING_FILTER_PROPS,
};

struct plane_t
{
	vec3_t	normal;
	float	dist;
	BYTE	type;
	BYTE	signbits;
	BYTE	pad[2];
};

struct surface_t
{
	const char*		name;
	short			surface_props;
	unsigned short	flags;
};

enum surface_flags_t
{
	DISPSURF_FLAG_SURFACE	= (1 << 0),
	DISPSURF_FLAG_WALKABLE	= (1 << 1),
	DISPSURF_FLAG_BUILDABLE = (1 << 2),
	DISPSURF_FLAG_SURFPROP1 = (1 << 3),
	DISPSURF_FLAG_SURFPROP2 = (1 << 4),
};

class itracefilter
{
public:
	virtual bool			should_hit_entity(PVOID entity, int contents_mask)	= 0;
	virtual trace_type_t	get_trace_type() const								= 0;
};

class trace_filter
{
public:

	virtual bool should_hit_entity(PVOID entity_handle, int contents_mask) {
		c_base_player* entity = (c_base_player*)entity_handle;
		switch (entity->get_client_class()->class_id) {
			case 55:
			case 64:
			case 117:
			case 230:
				return false; 
				break;
		}
		return !(entity_handle == skip);
	}

	virtual trace_type_t get_trace_type() const
	{
		return TRACE_EVERYTHING;
	}

public:
	PVOID skip;
};

class base_trace
{
public:

	// displacement flags tests
	bool is_disp_surface(void) { return ((disp_flags & DISPSURF_FLAG_SURFACE) != 0); }
	bool is_disp_surface_walkable(void) { return ((disp_flags & DISPSURF_FLAG_WALKABLE) != 0); }
	bool is_disp_surface_buildable(void) { return ((disp_flags & DISPSURF_FLAG_BUILDABLE) != 0); }
	bool is_dusp_surface_prop1(void) { return ((disp_flags & DISPSURF_FLAG_SURFPROP1) != 0); }
	bool is_disp_surface_prop2(void) { return ((disp_flags & DISPSURF_FLAG_SURFPROP2) != 0); }

public:

	vec3_t			startpos;		// start position
	vec3_t			endpos;			// final position
	plane_t			plane;			// surface normal at impact

	float			fraction;		// time completed, 1.0 = didn't hit anything

	int				contents;		// contents on other side of surface hit
	unsigned short	disp_flags;		// displacement flags for marking surfaces with data

	bool			all_solid;		// if true, plane is not valid
	bool			start_solid;	// if true, the initial point was in a solid area

	base_trace() {}

private:
	base_trace(const base_trace& other);
};

class game_trace : public base_trace
{
public:

	bool did_hit() const
	{
		return fraction < 1 || all_solid || start_solid;
	}

public:

	float			fractionleftsolid;	// time we left a solid, only valid if we started in solid
	surface_t		surface;			// surface hit (impact surface)

	int				hitgroup;			// 0 == generic, non-zero is specific body part
	short			physicsbone;		// physics bone hit by trace in studio

	c_base_player*	ent;

	int				hitbox;				// box hit by trace in studio

	game_trace() {}

private:
	game_trace(const game_trace& other);
};

class ienginetrace
{
public:

	void trace_ray(const ray_t& ray, unsigned int mask, trace_filter* trace_filt, trace_t* trace)
	{
		using original_fn = void(__thiscall*)(PVOID, const ray_t&, unsigned int, trace_filter*, trace_t*);
		return utilities::call_vfunc<original_fn>(this, 4)(this, ray, mask, trace_filt, trace);
	}

};

#endif // !IENGINETRACE_H
