#ifndef IGLOWOBJECT_H
#define IGLOWOBJECT_H
#ifdef _WIN32
#pragma once
#endif

#define END_OF_FREE_LIST -1
#define ENTRY_IN_USE -2

struct glow_object_t {
	HANDLE entity_handle;
	vec3_t m_glow_colour;
	float m_glow_alpha;
	bool m_b_render_when_occluded;
	bool m_b_render_when_unoccluded;
	int m_n_split_screen_slot;
	int m_n_next_free_slot;
};

struct glow_manager_t {
	CUtlVector<glow_object_t> m_glow_object_definitions;
	int m_n_first_free_slot;
};


#endif