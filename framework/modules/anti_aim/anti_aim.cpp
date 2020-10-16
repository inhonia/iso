#include "../../main.h"

enum pitch_types {
	down = 1,
	up,
	fake_down,
	fake_up,
	fake_zero_up,
	fake_zero_down,
	lisp_up,
	lisp_down
};

enum yaw_types {
	static_yaw = 0,
	edge_yaw,
	spin_yaw
};

namespace anti_aim {
	float distance(vec3_t a, vec3_t b)
	{
		return sqrt(pow(a.x - b.x, 2) + pow(a.y - b.y, 2) + pow(a.z - b.z, 2));
	}

	vec3_t anti_aim::get_best_head_angle(vec3_t angle)
	{
		vec3_t pos = ctx::command->viewangles + ctx::local_player->get_world_space_center();
		float closest_distance = 150.0f;
		float radius = 360.0f;
		float step = M_PI * 2.0 / 16;

		for (float a = 0; a < (M_PI * 2.0); a += step)
		{
			vec3_t location(radius * cos(a) + pos.x, radius * sin(a) + pos.y, pos.z);

			ray_t ray;
			trace_t trace;
			ray.init(pos, location);
			trace_filter filter;
			filter.skip = ctx::local_player;
			interfaces::enginetrace->trace_ray(ray, MASK_PLAYERSOLID, &filter, &trace);
			float dist = distance(pos, trace.endpos);
			if (dist < closest_distance)
			{
				closest_distance = dist;
				angle.y = RAD2DEG(a);
				//interfaces::convar->console_print_colour(colour_t(255, 255, 0), "[isopropyl] traced %d at %f, resulting in %f \n", dist, a, -RAD2DEG(a));
				if (angle.x >= 0)
					return vec3_t(0, -RAD2DEG(a), 0);
				else if (angle.x < 0)
					return vec3_t(0, RAD2DEG(a), 0);
			}
		}
		return vec3_t(0, 0, 0); //obviously dont add anything if ur not tracing anything good lol
	}

	float edge_distance(float edge_ray_yaw) //DF pasta until do_anti_aim....
	{
		trace_t trace;
		ray_t ray;
		vec3_t forward;
		trace_filter filter;
		filter.skip = ctx::local_player;
		float sp, sy, cp, cy;
		sy = sinf(DEG2RAD(edge_ray_yaw)); // yaw
		cy = cosf(DEG2RAD(edge_ray_yaw));
		sp = sinf(DEG2RAD(0)); // pitch
		cp = cosf(DEG2RAD(0));
		forward.x = cp * cy;
		forward.y = cp * sy;
		forward.z = -sp;
		forward = forward * 300.0f + ctx::local_player->get_eye_position();
		ray.init(ctx::local_player->get_eye_position(), forward);
		interfaces::enginetrace->trace_ray(ray, MASK_PLAYERSOLID, &filter, &trace);
		float edgeDistance = (sqrt(pow(trace.startpos.x - trace.endpos.x, 2) + pow(trace.startpos.y - trace.endpos.y, 2)));
		return edgeDistance;
	}

	float f_edge_yaw = 0;
	float edge_to_edge_on = 0;

	bool find_edge(float edge_original_yaw)
	{
		float edge_left_dist = edge_distance(edge_original_yaw - 21);
		edge_left_dist = edge_left_dist + edge_distance(edge_original_yaw - 27); //are these numbers just fucking arbitrary? monkey code.

		float edge_right_dist = edge_distance(edge_original_yaw + 21);
		edge_right_dist = edge_right_dist + edge_distance(edge_original_yaw + 27);

		if (edge_left_dist >= 260) edge_left_dist = 999999999;
		if (edge_right_dist >= 260) edge_right_dist = 999999999;

		if (edge_left_dist == edge_right_dist) return false;

		if (edge_right_dist < edge_left_dist)
		{
			if (ctx::real_angle.x > 0)
				edge_to_edge_on = 1;
			else
				edge_to_edge_on = 2;

			return true;
		}
		else
		{
			if (ctx::real_angle.x > 0)
				edge_to_edge_on = 2;
			else
				edge_to_edge_on = 1;

			return true;
		}
	}

	float use_edge(float edge_view_angle)
	{
		bool edge_test = true;
		if (((edge_view_angle < -135) || (edge_view_angle > 135)) && edge_test == true)
		{
			if (edge_to_edge_on == 1) f_edge_yaw = (float)-90;
			if (edge_to_edge_on == 2) f_edge_yaw = (float)90;
			edge_test = false;
		}
		if ((edge_view_angle >= -135) && (edge_view_angle < -45) && edge_test == true)
		{
			if (edge_to_edge_on == 1) f_edge_yaw = (float)0;
			if (edge_to_edge_on == 2) f_edge_yaw = (float)179;
			edge_test = false;
		}
		if ((edge_view_angle >= -45) && (edge_view_angle < 45) && edge_test == true)
		{
			if (edge_to_edge_on == 1) f_edge_yaw = (float)90;
			if (edge_to_edge_on == 2) f_edge_yaw = (float)-90;
			edge_test = false;
		}
		if ((edge_view_angle <= 135) && (edge_view_angle >= 45) && edge_test == true)
		{
			if (edge_to_edge_on == 1) f_edge_yaw = (float)179;
			if (edge_to_edge_on == 2) f_edge_yaw = (float)0;
			edge_test = false;
		}
		return f_edge_yaw;
	}

	void do_anti_aim() {
		if (!ctx::local_player || ctx::local_player->is_dead()) {
			return;
		}

		if (!ctx::command) {
			return;
		}

		if (utilities::has_condition(ctx::local_player, 128)) {
			return;
		}

		c_base_weapon* local_weapon = ctx::local_player->get_active_weapon();
		if (!local_weapon) {
			return;
		}

		float server_time = ctx::local_player->get_tick_base() * interfaces::globals->interval_per_tick;
		bool bullet_time = !(local_weapon->get_next_attack() > server_time);

		if (ctx::command->buttons & IN_ATTACK && bullet_time) {
			return;
		}

		float old_side_move = ctx::command->side_move, old_forward_move = ctx::command->forward_move;
		vec3_t old_viewangles = ctx::command->viewangles, temp_angles = ctx::command->viewangles;

		switch (vars::hvh::anti_aim_pitch) {

			case down:		temp_angles.x = 89.0f;	break;

			case up:		temp_angles.x = -89.0f; break;

			case fake_down: temp_angles.x = 271.0f; break;

			case fake_up:	temp_angles.x = -271.0f; break;

			case fake_zero_up: temp_angles.x = -1080.0f; break;

			case fake_zero_down: temp_angles.x = 1080.0f; break;

			case lisp_up:	temp_angles.x += -87480; break; //actually fake viewangles, real up (shows your x axis the same as camera angs)

			case lisp_down: temp_angles.x += 87480; break; //same as above but down.

			default: break;

		}

		if (vars::hvh::anti_aim_yaw) {
			if (vars::hvh::fakelag_mode == 0 || vars::hvh::fakelag_value < 2)
				*ctx::send_packet = ctx::command->command_number % 4;

			if (!*ctx::send_packet) { //do reals here

				switch (vars::hvh::anti_aim_real_type) {

					case static_yaw: temp_angles.y = old_viewangles.y + vars::hvh::anti_aim_real_offset; break;
				
					case edge_yaw: {
						if (find_edge(temp_angles.y))
							temp_angles.y = use_edge(temp_angles.y) + vars::hvh::anti_aim_real_offset;
					} break;

					case spin_yaw: temp_angles.y = fmod(interfaces::globals->realtime * vars::hvh::anti_aim_real_spinspeed * 90.f, 360.f);
				}

				if (vars::hvh::anti_aim_real_jitter != 0)
				{
					temp_angles.y += rand() % vars::hvh::anti_aim_real_jitter;
				}

				if (vars::hvh::anti_aim_real_spin_range != 0)
				{
					temp_angles.y += 0 - (vars::hvh::anti_aim_real_spin_range / 2) + fmodf(interfaces::globals->curtime * vars::hvh::anti_aim_real_add_spin_speed * 3, vars::hvh::anti_aim_real_spin_range);
				}

			}
			else if (*ctx::send_packet) { //do fakes here
				switch (vars::hvh::anti_aim_fake_type) {
					case static_yaw: temp_angles.y = old_viewangles.y + vars::hvh::anti_aim_fake_offset; break;

					case edge_yaw: {
						if (find_edge(temp_angles.y))
							temp_angles.y = use_edge(temp_angles.y) + vars::hvh::anti_aim_fake_offset;
					} break;

					case spin_yaw: temp_angles.y = fmod(interfaces::globals->realtime * vars::hvh::anti_aim_fake_spinspeed * 90.f, 360.f);
				}

				if (vars::hvh::anti_aim_fake_jitter != 0)
				{
					temp_angles.y += rand() % vars::hvh::anti_aim_fake_jitter;
				}

				if (vars::hvh::anti_aim_fake_spin_range != 0)
				{
					temp_angles.y += 0 - (vars::hvh::anti_aim_fake_spin_range / 2) + fmodf(interfaces::globals->curtime * vars::hvh::anti_aim_fake_add_spin_speed * 3, vars::hvh::anti_aim_fake_spin_range);
				}
			}

			utilities::normalize_yaw(temp_angles.y);

			/* [thanks vlad]
			Vladyslav Tsymbulov, [09.04.20 17:28]
			the catch is that animstate class in source games has so called 'max angle per tick', meaning you cant just report +180/-180 changes each frame

			Vladyslav Tsymbulov, [09.04.20 17:28]
			it basically has a restriction of 90 degrees per tick

			Vladyslav Tsymbulov, [09.04.20 17:28]
			and this is why fake yaw is possible

			Vladyslav Tsymbulov, [09.04.20 17:29]
			you report 0/360/0/360/... and all the clients see it as 0/90/0/90

			Vladyslav Tsymbulov, [09.04.20 17:29]
			because animstate doesnt allow you to report rotations more than 90 degrees per tick
			*/

			//simple angle stepping to get around this troubling issue
			//now that i think about it, this might not even actually run every tick
			//fuck it bro i can just say i did it anyways. suck me JOHNNY BOY.
			static float current, last = 0;

			if (last == 0)
				last = ctx::command->viewangles.y;

			current = temp_angles.y;

			if (last < (current - 89.f)) {
				current = last + 89.f;
			}
			if (last > (current + 89)) {
				current = last - 89.f;
			}

			last = ctx::command->viewangles.y;
		}

		utilities::normalize_yaw(temp_angles.y); //second normalization just in case the stepping fucks shit up.

		if (!*ctx::send_packet) {
			ctx::real_angle = temp_angles;
		}
		else if (*ctx::send_packet) {
			ctx::fake_angle = temp_angles;
		}
		ctx::command->viewangles = temp_angles;

		aimbot::fix_movement(ctx::command, old_viewangles, old_forward_move, old_side_move);
	}
}