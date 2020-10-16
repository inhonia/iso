#pragma once

namespace resolver {
	float get_away_angle(c_base_player* entity);
	void do_resolve();
	

	extern float resolved_y[65];
	extern float resolved_x[65];
	extern bool is_faking_pitch[65];
	extern bool is_pitch_up[65];
	extern bool are_angs_saved[65];
	extern vec3_t old_eye_angs[65];
	extern vec3_t saved_eye_angs[65];
	extern vec3_t forced_angs[65];
	extern int shots_fired[65];
	extern int shots_missed[65];
	extern int shots_hit[65];
}