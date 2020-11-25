#ifndef GLOBAL_H
#define GLOBAL_H
#ifdef _WIN32
#pragma once
#endif

namespace ctx {
	extern c_base_player*	local_player;
	extern c_user_cmd*		command;
	extern c_user_cmd		original_command;
	extern bool*			send_packet;

	extern int				screen_width;
	extern int				screen_height;

	extern imaterial*		visible_flattened;
	extern imaterial*		hidden_flattened;

	extern imaterial*		visible_textured;
	extern imaterial*		hidden_textured;

	extern imaterial*		visible_wireframe;
	extern imaterial*		hidden_wireframe;

	extern imaterial*		visible_shine;
	extern imaterial*		hidden_shine;

	extern imaterial*		visible_error;
	extern imaterial*		hidden_error;

	extern bool				is_scoped;
	extern bool				scopedwait;
	extern bool				smoothcanshoot;
	extern bool				in_thirdperson;
	extern vec3_t           antiaim_angle;
	extern vec3_t			fakelag_position;
	extern int				shots_fired;
	extern int				shots_missed;
	extern int				shots_hit_headshot;
	extern bool				stop_resolving;
	extern vec3_t			fake_angle;
	extern vec3_t			real_angle;
	extern matrix3x4_t*		fake_bone_matrix;
	extern bool				fake_bones_setup;

	extern vec3_t			enemy_angle_change;
	extern vec3_t			hit_angle_saved;
	extern bool				is_hit_saved;

	extern colour_t			crosshair_colour;

	extern bool				menu_open;

	extern int				local_deaths;
	extern int				local_kills;

	extern float			camera_x;
	extern float			camera_y;
	extern vec3_t			camera_origin;
	extern vec3_t			camera_angs;

	extern std::string		map_name;

	extern vec3_t			smooth_start;
	extern float			smooth_start_time;
	extern int				old_hit_scan;
	extern bool				smooth_finished;
	extern float			smooth_percentage_done;
	extern bool				has_flicked;
		   
	extern c_base_player*	old_aimbot_target;
}

#endif // !GLOBAL_H
