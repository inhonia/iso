#pragma once
#include "events.h"

auto is_headshot_weapon = [&](c_base_player* local_player, c_base_weapon* local_weapon) {
	if (local_weapon->get_slot() == 0 && local_player->get_class_num() == TF2_Sniper)
		if (local_weapon->get_item_definition_index() != WPN_Huntsman && local_weapon->get_item_definition_index() != WPN_CompoundBow)
			return true;

	if (local_weapon->get_item_definition_index() == WPN_Ambassador || local_weapon->get_item_definition_index() == WPN_FestiveAmbassador)
		return true;

	return false;
};

void events::initialize()
{
	interfaces::game_event->add_listener(this, xorstr("player_hurt"), false);
	interfaces::game_event->add_listener(this, xorstr("player_death"), false);
	interfaces::game_event->add_listener(this, xorstr("player_spawn"), false);
	//interfaces::convar->console_print(xorstr("[Isopropyl Debug] initialized event listeners!\n"));
}

void events::fire_game_event(igame_event* event) {
	colour_t custom_ui_colour = vars::cheat::custom_ui_colour ? colour_t::from_hsb(vars::colour::custom_ui_colour / 100, 1.f, 1.f, 255) : colour_t(218, 23, 91);

	if (!ctx::local_player) {
		return;
	}

	player_info_t local_player;
	interfaces::engineclient->get_player_info(interfaces::engineclient->get_local_player(), &local_player);

	int local_user_id = local_player.user_id;
	if (local_user_id == 0) return;

	if (strcmp(event->get_name(), xorstr("player_death")) == 0) {
		int attacker = event->get_int(xorstr("attacker"), 0);
		int user_id = event->get_int(xorstr("userid"), 0);
		int crittype = event->get_int(xorstr("crit_type"));
		int user_index = interfaces::engineclient->get_player_for_user_id(user_id);
		
		if (attacker == local_user_id && user_id != local_user_id)
		{
			player_info_t info;
			if (interfaces::engineclient->get_player_info(user_index, &info)) {
				ctx::local_kills++;
				std::string s = info.name;
				std::stringstream f;
				interfaces::convar->console_print_colour(custom_ui_colour, xorstr("[Isopropyl] "));
				if (crittype < 1) {
					f << xorstr("killed player ") << s.c_str() << ".\n";
					interfaces::convar->console_print_colour(colour_t(255, 255, 255), f.str().c_str());
					c_notification::get().add(true, colour_t(255, 255, 255), f.str());
				}
				else {
					f << xorstr("killed player ") << s.c_str() << xorstr(" with a crit.\n");
					interfaces::convar->console_print_colour(colour_t(255, 255, 255), f.str().c_str());
					c_notification::get().add(true, colour_t(255, 255, 255), f.str());
				}

			}
		}
		if (user_id == local_user_id)
		{
			ctx::local_deaths++;
		}
		return;
	}

	if (strcmp(event->get_name(), xorstr("player_hurt")) == 0)
	{
		int attacker = event->get_int(xorstr("attacker"), 0);
		int user_id = event->get_int(xorstr("userid"), 0);
		int hurt_damage = event->get_int(xorstr("damageamount"));
		int remaining_health = event->get_int(xorstr("health"));
		bool crit = event->get_bool(xorstr("crit"));
		int user_index = interfaces::engineclient->get_player_for_user_id(user_id);

		if (attacker == local_user_id && user_id != local_user_id)
		{


			player_info_t info;
			if (interfaces::engineclient->get_player_info(user_index, &info)) {
				std::string s = info.name;
				std::stringstream h;

				c_base_player* entity = reinterpret_cast<c_base_player*>(interfaces::entitylist->get_client_entity(user_index));
				if (!entity) { return; }

				c_base_weapon* local_weapon = ctx::local_player->get_active_weapon();
				if (!local_weapon) { return; }

				if (is_headshot_weapon(ctx::local_player, local_weapon) && hurt_damage >= 150 && vars::aimbot::enabled && (vars::hvh::yaw_resolver || vars::hvh::pitch_resolver)) {
					DWORD eye_angles = reinterpret_cast<DWORD>(aimbot::aimbot_target) + CNetVars::get().get_offset(xorstr("DT_TFPlayer"), xorstr("tfnonlocaldata"), xorstr("m_angEyeAngles[0]"));
					vec3_t &aimbot_entity = *reinterpret_cast<vec3_t*>(eye_angles);

					resolver::saved_eye_angs[entity->get_index()] = aimbot_entity;
					resolver::are_angs_saved[entity->get_index()] = true;
					resolver::shots_hit[entity->get_index()]++;
				}

				if (remaining_health > 0) {

					interfaces::convar->console_print_colour(custom_ui_colour, xorstr("[Isopropyl] "));
					if (!crit) {
						h << xorstr("hit ") << s.c_str() << xorstr(" for ") << hurt_damage << xorstr(" damage. ") << s.c_str() << xorstr(" has ") << remaining_health << xorstr(" health remaining.\n");
						interfaces::convar->console_print_colour(colour_t(255, 255, 255), h.str().c_str());
						c_notification::get().add(true, colour_t(255, 255, 255), h.str());
					}
					else {
						h << xorstr("critted ") << s.c_str() << xorstr(" for ") << hurt_damage << xorstr(" damage. ") << s.c_str() << xorstr(" has ") << remaining_health << xorstr(" health remaining.\n");
						interfaces::convar->console_print_colour(colour_t(255, 255, 255), h.str().c_str());
						c_notification::get().add(true, colour_t(255, 255, 255), h.str());
					}
				}

				else {
					interfaces::convar->console_print_colour(custom_ui_colour, xorstr("[Isopropyl] "));
					if (!crit) {
						h << xorstr("killing blow! hit ") << s.c_str() << xorstr(" for ") << hurt_damage << xorstr(" damage.\n");
						interfaces::convar->console_print_colour(colour_t(255, 255, 255), h.str().c_str());
						c_notification::get().add(true, colour_t(255, 255, 255), h.str());
					}
					else {
						h << xorstr("killing blow! critted ") << s.c_str() << xorstr(" for ") << hurt_damage << xorstr(" damage.\n");
						interfaces::convar->console_print_colour(colour_t(255, 255, 255), h.str().c_str());
						c_notification::get().add(true, colour_t(255, 255, 255), h.str());
					}
				}
			}
		}
		return;
	}
	
}