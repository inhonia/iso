#include "../../main.h"

float get_move_angle(float speed)
{
	auto move_angle = RAD2DEG(asin(15.f / speed));

	if (!isfinite(move_angle) || move_angle > 90.f)
		move_angle = 90.f;
	else if (move_angle < 0.f)
		move_angle = 0.f;

	return move_angle;
}

float normalize_angle(float angle)
{
	if (angle > 180.f || angle < -180.f)
	{
		auto revolutions = round(abs(angle / 360.f));

		if (angle < 0.f)
			angle = angle + 360.f * revolutions;
		else
			angle = angle - 360.f * revolutions;
	}
	return angle;
}

void vector_angles(const vec3_t& forward, vec3_t &angles)
{
	if (forward.x == 0.f && forward.y == 0.f)
	{
		angles.x = forward.z > 0.f ? -90.f : 90.f;
		angles.y = 0.f;
	}
	else
	{
		angles.x = RAD2DEG(atan2(-forward.z, forward.length2d()));
		angles.y = RAD2DEG(atan2(forward.y, forward.x));
	}
	angles.z = 0.f;
}

int random_number(int min, int max)
{
	return rand() % (max - min + 1) + min;
}

namespace miscellanous {
	void miscellanous::do_miscellanous() {
		miscellanous::do_fakelag();
		miscellanous::do_movement();
		miscellanous::do_thirdperson();
		miscellanous::do_chatspam();
		miscellanous::do_nopush();
		miscellanous::do_edgejump();
		miscellanous::do_fakecrouch();
	}
	void miscellanous::do_nopush() { 
		static convar* tf_avoidteammates_pushaway = interfaces::convar->find_variable("tf_avoidteammates_pushaway");
		if (vars::miscellanous::nopush) {
			if (tf_avoidteammates_pushaway->get_int() == 1) {
				tf_avoidteammates_pushaway->set_value((int)0);
			}
			else if (tf_avoidteammates_pushaway->get_int() == 0) {
				tf_avoidteammates_pushaway->set_value((int)1);
			}
		}
	}

	void miscellanous::do_edgejump() {

	}

	void miscellanous::do_fakecrouch()
	{
		if (!vars::miscellanous::fake_crouch)
			return;

		if (!ctx::local_player || !ctx::local_player->get_life_state() == LIFE_ALIVE || !interfaces::engineclient->is_connected())
			return;

		static bool do_crouch = false;
		static int crouch_count = 0;

		if (crouch_count == 3)
		{
			crouch_count = 0;
			do_crouch = !do_crouch;
		}
		else
		{
			crouch_count++;
		}

		if (do_crouch)
		{
			ctx::command->buttons |= IN_DUCK;
			*ctx::send_packet = true;
		}
		else {
			ctx::command->buttons &= ~IN_DUCK;
			*ctx::send_packet = false;
		}

		if ((ctx::command->buttons & IN_ATTACK))
			*ctx::send_packet = true;
	}

	void miscellanous::do_anim_bug() {
		if (!ctx::local_player || !ctx::local_player->get_life_state() == LIFE_ALIVE || !interfaces::engineclient->is_connected())
			return;

		ctx::command->buttons ^= IN_FORWARD | IN_BACK | IN_MOVELEFT | IN_MOVERIGHT;
	}

	void miscellanous::do_fakelag() {
		if (vars::hvh::fakelag_value > 0 && vars::hvh::fakelag_mode > 0) {
			if (!ctx::local_player || !ctx::local_player->get_life_state() == LIFE_ALIVE || !interfaces::engineclient->is_connected())
				return;

			c_net_channel* net_channel = interfaces::engineclient->get_net_channel_info();
			c_base_weapon* local_weapon = ctx::local_player->get_active_weapon();
			float server_time = ctx::local_player->get_tick_base() * interfaces::globals->interval_per_tick;
			bool bullet_time = !(local_weapon->get_next_attack() > server_time);

			float distance_per_tick = ctx::local_player->get_velocity().length2d() * interfaces::globals->interval_per_tick;
			int wish_ticks = 0;
			int adaptive_ticks = 2;
			int add_jitter = vars::hvh::fakelag_jitter;
			int wish_ticks_1 = vars::hvh::fakelag_value;

			int random_lag = random_number(wish_ticks, wish_ticks_1);
			int random_jitter = random_number(0, 100);

			if (ctx::command->buttons & IN_ATTACK && bullet_time) {
				return;
			}

			//if (vars::hvh::fakelag_mode == 1 ||
			//   (vars::hvh::fakelag_mode == 2 && !local_on_ground) ||
			//   (vars::hvh::fakelag_mode == 3 && local_on_ground)) {
			//	if (net_channel->m_nChokedPackets < vars::hvh::fakelag_value) {
			//		*ctx::send_packet = false;
			//	}
			//	else {
			//		*ctx::send_packet = true;
			//	}
			//}

			switch (vars::hvh::fakelag_mode)
			{

				case 0: 
					break;

				case 1:
					wish_ticks = wish_ticks_1;
					break;

				case 2: 
					if (interfaces::globals->tickcount % 30 < 15)
						wish_ticks = wish_ticks_1;
					break;

				case 3:
				{
					float adaptive_ticks = ceil(64.0 / distance_per_tick);
					wish_ticks = min(adaptive_ticks, wish_ticks_1);
				}
				break;

				case 4: 
					wish_ticks = random_lag;

				case 5: // p as fuck
				{
					if (interfaces::globals->tickcount % 15 < 8)
						wish_ticks = wish_ticks_1;
					else {
						float adaptive_ticks = ceil(64.0 / distance_per_tick);
						wish_ticks = min(adaptive_ticks, wish_ticks_1);
					}
				}
			}

			if (net_channel->m_nChokedPackets < wish_ticks) {
				if (random_jitter > (add_jitter / 2))
				{
					*ctx::send_packet = false;
				}
				else //jitter has won, ur not choking now.
				{
					*ctx::send_packet = true;
				}
			}
			else
			{
				*ctx::send_packet = true;
			}

		}
	}

	void miscellanous::do_movement() {

		if (!(ctx::local_player->get_flags() & FL_ONGROUND) && ctx::command->buttons & IN_JUMP) {

			if (vars::miscellanous::autostrafe == 1) { //"legit"
				if (ctx::command->moused_x > 5 || ctx::command->moused_x < -5)
					ctx::command->side_move = ctx::command->moused_x > 1 ? 450.f : -450.f;
			}

			if (vars::miscellanous::autostrafe == 2) { //rage strafer
				static auto switch_key = 1.f;
				static auto circle_yaw = 0.f;
				static auto old_yaw = 0.f;

				auto velocity = aimbot::estimate_abs_velocity(ctx::local_player);
				velocity.z = 0.f;

				auto speed = velocity.length2d();
				auto ideal_strafe = get_move_angle(speed);

				switch_key *= -1.f;

				if (GetAsyncKeyState(0x57)) {
					ctx::command->viewangles.y = -ctx::command->viewangles.y;
				}
				else if (GetAsyncKeyState(0x41) && !GetAsyncKeyState(0x44)) {
					ctx::command->viewangles.y += 45.0f;
				}
				else if (GetAsyncKeyState(0x44) && !GetAsyncKeyState(0x41)) {
					ctx::command->viewangles.y -= 45.0f;
				}
				else if (GetAsyncKeyState(0x44) && GetAsyncKeyState(0x41)) {
					ctx::command->viewangles.y += fmod(interfaces::globals->realtime * 3 / 10 * 360.0f, 360.0f);
				}
				auto yaw_delta = normalize_angle(ctx::command->viewangles.y - old_yaw);
				auto absolute_yaw_delta = abs(yaw_delta);

				circle_yaw = old_yaw = ctx::command->viewangles.y;
				if (yaw_delta > 0.f) ctx::command->side_move = -450.f;
				else if (yaw_delta < 0.f) ctx::command->side_move = 450.f;

				if (absolute_yaw_delta <= ideal_strafe || absolute_yaw_delta >= 30.f)
				{
					vec3_t velocity_angles;
					vector_angles(velocity, velocity_angles);

					auto velocity_delta = normalize_angle(ctx::command->viewangles.y - velocity_angles.y);
					auto retrack = 2.f * (ideal_strafe * 2.f);

					if (velocity_delta <= retrack || speed <= 15.f)
					{
						if (-retrack <= velocity_delta || speed <= 15.f)
						{
							ctx::command->viewangles.y += ideal_strafe * switch_key;
							ctx::command->side_move = switch_key * 450.f;
						}
						else
						{
							ctx::command->viewangles.y = velocity_angles.y - retrack;
							ctx::command->side_move = 450.f;
						}
					}
					else
					{
						ctx::command->viewangles.y = velocity_angles.y + retrack;
						ctx::command->side_move = -450.f;
					}
				}
			}
		}

		if (vars::miscellanous::bunnyhop) { //bhop (boolin)
			static bool first_jump = false, fake_jump;
			if (ctx::command->buttons & IN_JUMP) {
				if (!first_jump) {
					first_jump = fake_jump = true;
				}
				else if (!(ctx::local_player->get_flags() & FL_ONGROUND)) {
					if (fake_jump) {
						fake_jump = false;
					}
					else {
						ctx::command->buttons &= ~IN_JUMP;
					}
				}
				else {
					fake_jump = true;
				}
			}
			else {
				first_jump = false;
			}
		}

		if (vars::miscellanous::taunt_slide && ctx::local_player->get_condnew() & TFCond_Taunting) {
			float forward = 0.0f, side = 0.0f;

			if (ctx::command->buttons & IN_FORWARD) { forward += 450; }
			if (ctx::command->buttons & IN_BACK) { forward -= 450; }
			if (ctx::command->buttons & IN_MOVELEFT) { side -= 450; }
			if (ctx::command->buttons & IN_MOVERIGHT) { side += 450; }

			ctx::command->forward_move = forward;
			ctx::command->side_move = side;

			vec3_t view_angles;
			interfaces::engineclient->get_view_angles(view_angles);

			ctx::command->viewangles.y = view_angles[1];
			ctx::original_command.viewangles.y = view_angles[1];
		}

	}
	void miscellanous::do_thirdperson() {
		static bool thirdperson = false, check = false;
		static bool setting = vars::visuals::thirdperson;
		static bool last_thirdperson_check;

		if (GetAsyncKeyState(0x48) && !ctx::menu_open && interfaces::clientmode->is_chat_panel_out_of_focus()) {
			if (!check) setting = !setting;
			check = true;
		}
		else check = false;
		if (vars::visuals::thirdperson != last_thirdperson_check) {
			setting = vars::visuals::thirdperson;
			last_thirdperson_check = vars::visuals::thirdperson;
		}
		if (vars::visuals::thirdperson) {
			if (setting) {
				ctx::local_player->force_taunt_cam(true);
				thirdperson = true, ctx::in_thirdperson = true;
			}
		}
		if (!setting && thirdperson) {
			ctx::local_player->force_taunt_cam(false);
			thirdperson = false, ctx::in_thirdperson = false;
		}
	}

	void miscellanous::do_chatspam() { //fix up later, doesn't properly case thru the array
		std::string spam_array[11] = { xorstr("i bet you wish you had Isopropyl"), xorstr("no Isopropyl, no talk"), xorstr("step 1: get Isopropyl, step 2: ???, step 3: get schmoney"),
			xorstr("ratted by rud? can't relate"), xorstr("lmaobox tom is a pedophile"), xorstr("did you know tom lmaobox requested lewd photos of a minor? that's perfectly acceptable, right?"),
			xorstr("did you know rud logs IP addresses and other significant information from your PC?"), xorstr("fun fact: Isopropyl > you"), xorstr("HAHAHA! you're getting smoked by Isopropyl!"), xorstr("owned, freak"), ""};
		static int delay = -1;
		static int current_index = 1;
		static bool has_iterated;

		if (!vars::chatspam::chatspam_enabled) {
			return;
		}

		if (delay-- > 0) {
			return;
		}
		if (has_iterated)
			has_iterated = false;
		auto repeat = [&](int n, const char* str) -> std::string {
			std::ostringstream os;
			for (int i = 0; i < n; i++) {
				os << str;
			}
			return os.str();
		};

		std::string current_spam = spam_array[current_index];
		std::string our_message = "say ";
		//our_message.append(" ");
		//our_message.append(repeat(3, "\x0D"));
		our_message.append(current_spam);

		interfaces::engineclient->client_command(our_message.c_str(), true);

		if (!has_iterated) {
			current_index++;
		}

		if (current_index >= 11) {
			current_index = 1;
		}

		delay = 12;
		has_iterated = true;
	}
}
