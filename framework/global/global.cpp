#include "../main.h"

namespace ctx {
	c_base_player*	ctx::local_player;
	c_user_cmd*		ctx::command;
	c_user_cmd		ctx::original_command;
	bool*			ctx::send_packet;

	int				ctx::screen_width;
	int				ctx::screen_height;

	imaterial*		ctx::visible_flattened;
	imaterial*		ctx::hidden_flattened;

	imaterial*		ctx::visible_textured;
	imaterial*		ctx::hidden_textured;

	imaterial*		ctx::visible_wireframe;
	imaterial*		ctx::hidden_wireframe;

	imaterial*		ctx::visible_shine;
	imaterial*		ctx::hidden_shine;

	imaterial*		ctx::visible_error;
	imaterial*		ctx::hidden_error;

	bool			ctx::is_scoped;
	bool			ctx::scopedwait;
	bool			ctx::smoothcanshoot;
	bool			ctx::in_thirdperson;

	vec3_t			ctx::antiaim_angle;
	vec3_t			ctx::fakelag_position;

	int				ctx::shots_fired;
	int				ctx::shots_missed;
	int				ctx::shots_hit_headshot;
	bool			ctx::stop_resolving;
	vec3_t			ctx::fake_angle;
	vec3_t			ctx::real_angle;
	matrix3x4_t*		ctx::fake_bone_matrix;
	bool			ctx::fake_bones_setup;

	vec3_t			ctx::enemy_angle_change;
	vec3_t			ctx::hit_angle_saved;
	bool			ctx::is_hit_saved;

	colour_t		ctx::crosshair_colour;
	bool			ctx::menu_open;

	int				ctx::local_deaths;
	int				ctx::local_kills;

	float			ctx::camera_x;
	float			ctx::camera_y;
	vec3_t			ctx::camera_origin;
	vec3_t			ctx::camera_angs;

	std::string		ctx::map_name;

	vec3_t			ctx::smooth_start;
	float			ctx::smooth_start_time = 0.0f;
	int				ctx::old_hit_scan = -1;
	bool			ctx::smooth_finished = false;
	c_base_player*	ctx::old_aimbot_target = nullptr;
	float			ctx::smooth_percentage_done;
	bool			ctx::has_flicked;
}
