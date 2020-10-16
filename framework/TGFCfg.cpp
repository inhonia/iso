#include "menu/vars/vars.h"
#include "TGFCfg.h"

// credits to team gamer food

void CConfig::Setup()
{
	// aim

	SetupValue(vars::aimbot::enabled, false, xorstr("aimbot_enabled")); //woo woo
	SetupValue(vars::aimbot::field_of_view, 0, xorstr("aimbot_fov"));
	SetupValue(vars::aimbot::main_hitbox, 0, xorstr("aimbot_bone"));
	SetupValue(vars::aimbot::pointscale, 75, xorstr("aimbot_pointscale"));
	SetupValue(vars::aimbot::aimbot_key, 0, xorstr("aimbot_key"));
	SetupValue(vars::aimbot::smoothing, false, xorstr("aimbot_smoothenabled"));
	SetupValue(vars::aimbot::smoothingx, 0, xorstr("aimbot_smoothingx"));
	SetupValue(vars::aimbot::smoothingy, 0, xorstr("aimbot_smoothingy"));
	SetupValue(vars::aimbot::autoshoot, false, xorstr("aimbot_autoshoot"));
	SetupValue(vars::aimbot::autoscope, false, xorstr("aimbot_autoscope"));
	SetupValue(vars::aimbot::drawfov, true, xorstr("aimbot_draw"));
	SetupValue(vars::aimbot::silent_aim, false, xorstr("aimbot_slient"));
	SetupValue(vars::aimbot::bullet_time, false, xorstr("aimbot_psilent"));
	SetupValue(vars::aimbot::wait_for_charge, false, xorstr("aimbot_waitforcharge"));
	SetupValue(vars::hvh::pitch_resolver, false, xorstr("aimbot_resolvex"));
	SetupValue(vars::hvh::yaw_resolver, false, xorstr("aimbot_resolvey"));

	SetupValue(vars::position_adjustment::enabled, false, xorstr("backtrack_enabled"));
	SetupValue(vars::aimbot::lagfix, false, xorstr("aimbot_lagfix"));

	SetupValue(vars::aimbot::aimbot_sentry, false, xorstr("aimbot_target_sentries"));
	SetupValue(vars::aimbot::aimbot_dispenser, false, xorstr("aimbot_target_dispensers"));
	SetupValue(vars::aimbot::aimbot_teleporters, false, xorstr("aimbot_target_teleporters"));
	SetupValue(vars::aimbot::prefer_medic, false, xorstr("aimbot_target_prefermedic"));
	SetupValue(vars::aimbot::ignore_cloaked, false, xorstr("aimbot_target_ignorecloaked"));
	SetupValue(vars::aimbot::ignore_disguised, false, xorstr("aimbot_target_ignoredisguised"));

	SetupValue(vars::triggerbot::triggerbot_enabled, false, xorstr("triggerbot_enabled"));
	SetupValue(vars::triggerbot::triggerbot_key, 0, xorstr("triggerbot_key"));
	SetupValue(vars::triggerbot::triggerbot_delay, 30, xorstr("triggerbot_delay"));
	SetupValue(vars::triggerbot::auto_detonate, false, xorstr("triggerbot_autodetonate"));
	SetupValue(vars::triggerbot::auto_airblast, false, xorstr("triggerbot_autoairblast"));

	//esp //fml

	SetupValue(vars::visuals::player_enemyonly, true, xorstr("visuals_enemysenemyonly"));
	SetupValue(vars::visuals::enemy_name, false, xorstr("visuals_enemyname"));
	SetupValue(vars::visuals::enemy_box, false, xorstr("visuals_enemybox"));
	SetupValue(vars::visuals::enemy_bones, false, xorstr("visuals_enemybones"));
	SetupValue(vars::visuals::enemy_health, false, xorstr("visuals_enemyhealth"));
	SetupValue(vars::visuals::enemy_chams, false, xorstr("visuals_enemychams"));
	SetupValue(vars::visuals::enemy_weapon, false, xorstr("visuals_enemyweapon"));
	SetupValue(vars::visuals::enemy_steamid, false, xorstr("visuals_enemysteamid"));
	SetupValue(vars::visuals::enemy_backtrack_visulization, 0, xorstr("visuals_backtrack"));
	//SetupValue(vars::visuals::enemy_d_lights, false, xorstr("visuals_enemy_dlights"));
	//SetupValue(vars::visuals::enemy_d_lights_radius, 30, xorstr("visuals_enemy_dlights_radius"));
	SetupValue(vars::visuals::enemy_hitboxes, false, xorstr("visuals_enemyhitboxes"));

	SetupValue(vars::visuals::team_name, false, xorstr("visuals_teamname"));
	SetupValue(vars::visuals::team_box, false, xorstr("visuals_teambox"));
	SetupValue(vars::visuals::team_bones, false, xorstr("visuals_teambones"));
	SetupValue(vars::visuals::team_health, false, xorstr("visuals_teamhealth"));
	SetupValue(vars::visuals::team_chams, false, xorstr("visuals_teamchams"));
	SetupValue(vars::visuals::team_weapon, false, xorstr("visuals_teamweapon"));
	SetupValue(vars::visuals::team_steamid, false, xorstr("visuals_teamsteamid"));
	//SetupValue(vars::visuals::team_backtrack_visulization, false, xorstr("visuals_backtrack"));
	//SetupValue(vars::visuals::team_d_lights, false, xorstr("visuals_team_dlights"));
	//SetupValue(vars::visuals::team_d_lights_radius, 30, xorstr("visuals_team_dlights_radius"));
	SetupValue(vars::visuals::team_hitboxes, false, xorstr("visuals_teamhitboxes"));

	SetupValue(vars::visuals::building_enemyonly, true, xorstr("visuals_buildingsenemyonly"));
	SetupValue(vars::visuals::building_name, false, xorstr("visuals_buildingname"));
	SetupValue(vars::visuals::building_box, false, xorstr("visuals_buildingbox"));
	SetupValue(vars::visuals::building_health, false, xorstr("visuals_buildinghealth"));
	SetupValue(vars::visuals::building_builder, false, xorstr("visuals_buildingbuilder"));
	SetupValue(vars::visuals::building_level, false, xorstr("visuals_buildinglevel"));
	SetupValue(vars::visuals::building_chams, false, xorstr("visuals_buildingchams"));

	SetupValue(vars::visuals::world_name, false, xorstr("visuals_worldname"));
	SetupValue(vars::visuals::world_box, false, xorstr("visuals_worldbox"));
	SetupValue(vars::visuals::world_health_chams, false, xorstr("visuals_worldhealthchams"));
	SetupValue(vars::visuals::world_ammo_chams, false, xorstr("visuals_worldammochams"));
	SetupValue(vars::visuals::world_projectile_chams, false, xorstr("visuals_worldprojectilechams"));
	SetupValue(vars::visuals::world_money_chams, false, xorstr("visuals_worldmoneychams"));
	SetupValue(vars::visuals::world_tank_chams, false, xorstr("visuals_worldtankchams"));

	SetupValue(vars::visuals::custom_crosshair, true, xorstr("visuals_crosshair"));
	SetupValue(vars::visuals::spectators, false, xorstr("visuals_spectators"));
	SetupValue(vars::visuals::spectator_pos_x, 300, xorstr("visuals_spectator_pos_x"));
	SetupValue(vars::visuals::spectator_pos_y, 800, xorstr("visuals_spectator_pos_y"));
	SetupValue(vars::visuals::nightmode_value, 100, xorstr("visuals_nightmodevalue"));
	SetupValue(vars::visuals::projectile_prediction, false, xorstr("visuals_projectilepred"));
	SetupValue(vars::visuals::out_of_fov_arrows, false, xorstr("visuals_oofovarrows"));

	SetupValue(vars::visuals::thirdperson, false, xorstr("visuals_thirdperson"));
	SetupValue(vars::visuals::thirdperson_angle, 0, xorstr("visuals_thirdpersonangle"));
	SetupValue(vars::visuals::aa_visual, false, xorstr("visuals_aavis"));
	SetupValue(vars::visuals::no_zoom, false, xorstr("visuals_nozoom"));
	SetupValue(vars::visuals::no_scope, false, xorstr("visuals_scopelines"));
	SetupValue(vars::visuals::forced_fov, 90, xorstr("visuals_forcedfov"));
	SetupValue(vars::visuals::radar, false, xorstr("visuals_radar"));
	SetupValue(vars::visuals::radar_drag_x, 300, xorstr("visuals_radar_drag_x"));
	SetupValue(vars::visuals::radar_drag_y, 300, xorstr("visuals_radar_drag_y"));
	SetupValue(vars::visuals::radar_height, 300, xorstr("visuals_radar_height"));
	SetupValue(vars::visuals::radar_width, 300, xorstr("visuals_radar_width"));
	SetupValue(vars::visuals::no_cloak, false, xorstr("visuals_nocloak"));
	SetupValue(vars::visuals::visible_only, true, xorstr("visuals_visonly"));

	SetupValue(vars::chams::chams_activation, 0, xorstr("chams_activation"));
	SetupValue(vars::chams::chams_model, 0, xorstr("chams_model"));
	SetupValue(vars::chams::local_player, false, xorstr("chams_localplayer"));
	SetupValue(vars::chams::wearables, true, xorstr("chams_wearables"));

	// ETC

	SetupValue(vars::miscellanous::autostrafe, 0, xorstr("etc_autostrafe"));
	SetupValue(vars::miscellanous::bunnyhop, false, xorstr("etc_bunnyhop"));
	SetupValue(vars::miscellanous::taunt_slide, true, xorstr("etc_tauntslide"));
	SetupValue(vars::miscellanous::edgejump, false, xorstr("etc_edgejump"));
	SetupValue(vars::miscellanous::nopunch, false, xorstr("etc_no_aimpunch"));
	SetupValue(vars::hvh::fakelag_value, 0, xorstr("hvh_fakelagvalue"));
	SetupValue(vars::hvh::fakelag_mode, 0, xorstr("hvh_fakelagmode"));
	SetupValue(vars::hvh::anti_aim_pitch, 0, xorstr("hvh_pitch"));
	SetupValue(vars::hvh::anti_aim_yaw, 0, xorstr("hvh_yaw"));

	SetupValue(vars::hvh::anti_aim_real_type, 0, xorstr("hvh_yaw_real_type"));
	SetupValue(vars::hvh::anti_aim_real_spinspeed, 0, xorstr("hvh_yaw_real_spinspeed"));
	SetupValue(vars::hvh::anti_aim_real_offset, 0, xorstr("hvh_real_offset"));
	SetupValue(vars::hvh::anti_aim_real_jitter, 0, xorstr("hvh_real_jitter"));
	SetupValue(vars::hvh::anti_aim_real_spin_range, 0, xorstr("hvh_real_spin_range"));
	SetupValue(vars::hvh::anti_aim_real_add_spin_speed, 0, xorstr("hvh_real_addspin_speed"));

	SetupValue(vars::hvh::anti_aim_fake_type, 0, xorstr("hvh_yaw_fake_type"));
	SetupValue(vars::hvh::anti_aim_fake_spinspeed, 0, xorstr("hvh_yaw_fake_spinspeed"));
	SetupValue(vars::hvh::anti_aim_fake_offset, 0, xorstr("hvh_fake_offset"));
	SetupValue(vars::hvh::anti_aim_fake_jitter, 0, xorstr("hvh_fake_jitter"));
	SetupValue(vars::hvh::anti_aim_fake_spin_range, 0, xorstr("hvh_fake_spin_range"));
	SetupValue(vars::hvh::anti_aim_fake_add_spin_speed, 0, xorstr("hvh_fake_addspin_speed"));

	SetupValue(vars::hvh::auto_pitch_resolver, false, xorstr("hvh_autopitchresolver"));
	SetupValue(vars::hvh::yaw_resolver, false, xorstr("hvh_autoyawresolver"));
	SetupValue(vars::chatspam::chatspam_enabled, false, xorstr("chatspam_enabled"));


	// CLR
	SetupValue(vars::colour::aimbot_fov, 0, xorstr("colour_aimbot_fov"));

	SetupValue(vars::colour::enemy_box_vis, 30, xorstr("colour_enemy_box_vis"));
	SetupValue(vars::colour::enemy_box_inv, 50, xorstr("colour_enemy_box_inv"));

	SetupValue(vars::colour::enemy_chams_vis, 30, xorstr("colour_enemy_chams_vis"));
	SetupValue(vars::colour::enemy_chams_inv, 50, xorstr("colour_enemy_chams_inv"));

	SetupValue(vars::colour::team_chams_vis, 70, xorstr("colour_team_chams_vis"));
	SetupValue(vars::colour::team_chams_invis, 90, xorstr("colour_team_chams_inv"));

	SetupValue(vars::colour::team_box_vis, 70, xorstr("colour_team_box_vis"));
	SetupValue(vars::colour::team_box_invis, 90, xorstr("colour_team_box_inv"));

	SetupValue(vars::colour::enemy_hitbox_vis, 30, xorstr("colour_enemy_hitbox_vis"));
	SetupValue(vars::colour::enemy_hitbox_inv, 50, xorstr("colour_enemy_hitbox_inv"));

	SetupValue(vars::colour::team_hitbox_vis, 70, xorstr("colour_team_hitbox_vis"));
	SetupValue(vars::colour::team_hitbox_inv, 90, xorstr("colour_team_hitbox_inv"));

	SetupValue(vars::colour::chams_self, 90, xorstr("colour_chams_self"));

	SetupValue(vars::colour::health_chams, 60, xorstr("colour_health_chams"));
	SetupValue(vars::colour::ammo_chams, 20, xorstr("colour_ammo_chams"));
	SetupValue(vars::colour::projectile_chams, 80, xorstr("colour_projectile_chams"));
	SetupValue(vars::colour::money_chams, 70, xorstr("colour_money_chams"));
	SetupValue(vars::colour::tank_chams, 50, xorstr("colour_tank_chams"));

	SetupValue(vars::colour::building_chams_vis, 30, xorstr("colour_building_chams_vis"));
	SetupValue(vars::colour::building_chams_inv, 50, xorstr("colour_building_chams_inv"));
	SetupValue(vars::colour::custom_ui_colour, 0, xorstr("colour_custom_ui"));

	// CHROMA
	SetupValue(vars::chroma::enabled, true, xorstr("chroma_enabled"));
	SetupValue(vars::chroma::in_game, true, xorstr("chroma_in_game"));
	SetupValue(vars::chroma::up_arrow, true, xorstr("chroma_up_arrow"));
	SetupValue(vars::chroma::killstreak, true, xorstr("chroma_killstreak"));
	//SetupValue(vars::chroma::health, false, xorstr("chroma_health"));

	// CHEAT SHIT
	SetupValue(vars::cheat::custom_ui_colour, false, xorstr("cheat_custom_ui_clr"));
}

void CConfig::SetupValue(int& value, int def, std::string name)
{
	value = def;
	ints.push_back(new ConfigValue< int >(name, &value));
}

void CConfig::SetupValue(float& value, float def, std::string name)
{
	value = def;
	floats.push_back(new ConfigValue< float >(name, &value));
}

void CConfig::SetupValue(bool& value, bool def, std::string name)
{
	value = def;
	bools.push_back(new ConfigValue< bool >(name, &value));
}

//void CConfig::SetupValue(colour_t& value, float def, std::string name)
//{
//	value.r = def;
//	value.g = def;
//	value.b = def;
//	value.a = def;
//
//	floats.push_back(new ConfigValue< float >(name + "red", &value.r));
//	floats.push_back(new ConfigValue< float >(name + "green", &value.g));
//	floats.push_back(new ConfigValue< float >(name + "blue", &value.b));
//	floats.push_back(new ConfigValue< float >(name + "alpha", &value.a));
//}

void CConfig::SetupValue(bool* value, bool def, int size, std::string name) // for multiboxes
{
	for (int c = 0; c < size; c++)
	{
		value[c] = def;

		name += std::to_string(c);

		bools.push_back(new ConfigValue< bool >(name, &value[c]));
	}
}

static char name[] = "isopropyl";

void CConfig::Save()
{
	static TCHAR path[MAX_PATH];
	std::string folder, file;

	if (SUCCEEDED(SHGetFolderPath(NULL, CSIDL_APPDATA, NULL, 0, path)))
	{
		folder = std::string(path) + xorstr("\\isopropyl\\");
		switch (vars::config::selection) {
		case 0: file = std::string(path) + xorstr("\\isopropyl\\cfg.ini"); break;
		case 1: file = std::string(path) + xorstr("\\isopropyl\\cfg2.ini"); break;
		case 2: file = std::string(path) + xorstr("\\isopropyl\\cfg3.ini"); break;
		case 3: file = std::string(path) + xorstr("\\isopropyl\\cfg4.ini"); break;
		default: break;
		}
		
	}

	CreateDirectory(folder.c_str(), NULL);

	for (auto value : ints)
		WritePrivateProfileString(name, value->name.c_str(), std::to_string(*value->value).c_str(), file.c_str());

	for (auto value : floats)
		WritePrivateProfileString(name, value->name.c_str(), std::to_string(*value->value).c_str(), file.c_str());

	for (auto value : bools)
		WritePrivateProfileString(name, value->name.c_str(), *value->value ? xorstr("yay") : xorstr("nay"), file.c_str());
}

void CConfig::Load()
{
	static TCHAR path[MAX_PATH];
	std::string folder, file;

	if (SUCCEEDED(SHGetFolderPath(NULL, CSIDL_APPDATA, NULL, 0, path)))
	{
		folder = std::string(path) + xorstr("\\isopropyl\\");
		switch (vars::config::selection) {
		case 0: file = std::string(path) + xorstr("\\isopropyl\\cfg.ini"); break;
		case 1: file = std::string(path) + xorstr("\\isopropyl\\cfg2.ini"); break;
		case 2: file = std::string(path) + xorstr("\\isopropyl\\cfg3.ini"); break;
		case 3: file = std::string(path) + xorstr("\\isopropyl\\cfg4.ini"); break;
		default: break;
		}

	}

	CreateDirectory(folder.c_str(), NULL);

	char value_l[32] = { '\0' };

	for (auto value : ints)
	{
		GetPrivateProfileString(name, value->name.c_str(), "", value_l, 32, file.c_str());
		*value->value = atoi(value_l);
	}

	for (auto value : floats)
	{
		GetPrivateProfileString(name, value->name.c_str(), "", value_l, 32, file.c_str());
		*value->value = atof(value_l);
	}

	for (auto value : bools)
	{
		GetPrivateProfileString(name, value->name.c_str(), "", value_l, 32, file.c_str());
		*value->value = !strcmp(value_l, xorstr("yay"));
	}
}

CConfig* g_Config = new CConfig();