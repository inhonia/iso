#pragma once

struct backtracking_record {
	c_base_player* entity;
	vec3_t head_position;
};

struct position_adjustment_data {
	int tick_count;
	std::vector<backtracking_record> records;
};

struct tick_record {
	tick_record(float simulationtime, vec3_t headposition, studiohdr_t* studiomodel, vec3_t abs_origin, vec3_t wrld_spce_cntr, vec3_t angles) {
		simulation_time = simulationtime; 
		head_position = headposition;
		studio_model = studiomodel;
		origin = abs_origin;
		world_space_center = wrld_spce_cntr;
		eye_angles = angles;
	}

	float simulation_time = -1;
	vec3_t head_position;
	studiohdr_t* studio_model;
	vec3_t origin;
	vec3_t world_space_center;
	vec3_t eye_angles;
	matrix3x4_t bone_matrix[128];
	mstudiohitboxset_t* set;
};

namespace position_adjustment {
	extern std::vector<tick_record> ticks[64];
	extern void position_adjustment_start();
	extern void position_adjustment_calc();
	extern void do_position_adjustment();
};