#ifndef VARS_H
#define VARS_H
#ifdef _WIN32
#pragma once
#endif

namespace vars { 
	namespace menu {
		extern bool menu_active;
		extern int total_items;
		extern int current_pos;
		extern int font_size;
		extern int menu_x;
		extern int menu_y;
	}
	
	namespace aimbot {
		extern bool aimbot_tab;
		extern int aimbot_key;
		extern bool enabled;
		extern bool drawfov;
		extern int field_of_view;
		extern bool auto_fov;
		extern int main_hitbox;
		extern int pointscale;
		//extern int smoothing;
		extern int smoothingx;
		extern int smoothingy;
		extern bool smoothing;
		extern bool assist;
		extern bool hitscan;
		extern bool whilereloading;
		extern bool autoshoot;
		extern bool autoscope;
		extern bool silent_aim;
		extern bool bullet_time;
		extern bool lagfix;
		extern bool multipoint;
		extern bool prefer_medic;
		extern bool target_lock;
		extern bool projectile_aimbot;
		extern bool wait_for_charge;
		extern bool aimbot_sentry;
		extern bool aimbot_dispenser;
		extern bool aimbot_teleporters;
		extern bool ignore_cloaked;
		extern bool ignore_disguised;
		extern bool heavy_autorev;
	}

	namespace visuals {
		extern bool visuals_tab;
		extern bool box;
		extern bool health;
		extern bool name;
		extern bool weapon;
		extern bool steam_id;
		extern bool backtrack_visualization;
		extern bool custom_crosshair;
		extern bool spectators;
		extern int spectator_pos_x;
		extern int spectator_pos_y;
		extern bool out_of_fov_arrows;
		extern bool nightmode;
		extern int nightmode_value;
		extern bool projectile_prediction;
		extern bool thirdperson;
		extern int thirdperson_angle;
		extern int thirdperson_dist;
		extern int  forced_fov;
		extern int real_fov;
		extern bool no_zoom;
		extern bool no_recoil;
		extern bool no_scope;
		extern bool no_cloak;
		extern bool visible_only;
		extern bool aa_visual;

		extern int esp_type;

		extern bool player_enemyonly;

		extern bool team_box;
		extern bool team_health;
		extern bool team_name;
		extern bool team_weapon;
		extern bool team_ammo;
		extern bool team_steamid;
		extern bool team_backtrack_visulization;
		extern bool team_bones;
		extern bool team_chams;
		extern bool team_glow;
		extern bool team_d_lights;
		extern int team_d_lights_radius;
		extern bool team_hitboxes;
		extern bool team_glow;

		extern bool enemy_box;
		extern bool enemy_health;
		extern bool enemy_name;
		extern bool enemy_weapon;
		extern bool enemy_ammo;
		extern bool enemy_steamid;
		extern int enemy_backtrack_visulization;
		extern bool enemy_bones;
		extern bool enemy_chams;
		extern bool enemy_glow;
		extern bool enemy_d_lights;
		extern int enemy_d_lights_radius;
		extern bool enemy_hitboxes;
		extern bool enemy_glow;

		extern int enemy_chams_visible_clr;
		extern int enemy_chams_hidden_clr;
		extern int team_chams_visible_clr;
		extern int team_chams_hidden_clr;

		extern int enemy_box_clr;
		extern int enemy_box_clr_hidden;
		extern int team_box_clr;
		extern int team_box_clr_hidden;

		extern int local_chams_clr;

		extern bool building_enemyonly;
		extern bool building_box;
		extern bool building_health;
		extern bool building_name;
		extern bool building_builder;
		extern bool building_chams;
		extern bool building_level;
		extern bool building_glow;

		extern bool world_box;
		extern bool world_name;
		extern bool world_health_chams;
		extern bool world_ammo_chams;
		extern bool world_projectile_chams;
		extern bool world_money_chams;
		extern bool world_tank_chams;

		extern bool radar;
		extern int radar_drag_x;
		extern int radar_drag_y;
		extern int radar_width;
		extern int radar_height;

		extern bool player_list;
		extern int player_list_x;
		extern int player_list_y;
		//extern ColorV2 testclr;
	}

	namespace chams {
		extern int chams_activation;
		extern int chams_model;
		extern bool enemy_only;
		extern bool wearables;
		extern bool local_player;
		extern bool health_and_ammo;
		extern bool buildings;
	}

	namespace triggerbot {
		extern bool triggerbot_tab;
		extern bool triggerbot_enabled;
		extern bool triggerbot_on_key;
		extern bool triggerbot_on_backtrack;
		extern int  triggerbot_delay;
		extern int  triggerbot_key;
		extern bool auto_airblast;
		extern bool auto_airblast_on_key;
		extern bool auto_detonate;
		extern bool auto_detonate_on_key;
	}

	namespace position_adjustment { 
		extern bool position_adjustment_tab;
		extern bool enabled;
		extern int	ticks;
	} 

	namespace miscellanous {
		extern bool miscellanous_tab;
		extern bool bunnyhop;
		extern int autostrafe;
		extern bool duck_in_air;
		extern bool taunt_slide;
		extern bool nopush;
		extern bool sv_pure_bypass;
		extern bool edgejump;
		extern bool nopunch;
		extern bool fake_crouch;
	}
	
	namespace chatspam {
		extern bool chatspam_enabled;
		extern int  chatspam_new_lines;
		extern int  chatspam_selection;
	}

	namespace config {
		extern bool save;
		extern bool load;
		extern int selection;
	}

	// fuck you
	namespace hvh {
		extern int fakelag_mode;
		extern int fakelag_value;
		extern int fakelag_jitter;

		extern int anti_aim_pitch;

		extern bool auto_pitch_resolver;
		extern bool resolver_key;
		extern int	max_missed_shots;
		extern bool pitch_resolver;
		extern bool yaw_resolver;
		extern bool anti_aim_line;

		extern bool anti_aim_yaw;

		extern int anti_aim_menu_select;

		extern int anti_aim_real_offset; //edit sliders to take a min as well as a max for this. -180 to 180.
		extern int anti_aim_real_type;
		extern int anti_aim_real_spinspeed;
		extern int anti_aim_real_jitter;
		extern int anti_aim_real_spin_range;
		extern int anti_aim_real_add_spin_speed;

		extern int anti_aim_fake_offset;
		extern int anti_aim_fake_type;
		extern int anti_aim_fake_spinspeed;
		extern int anti_aim_fake_jitter;
		extern int anti_aim_fake_spin_range;
		extern int anti_aim_fake_add_spin_speed;

		//extern bool anti_aim_desync;
		//extern int anti_aim_desync_type;
	}

	namespace announcer {
		extern bool enabled;
	}

	namespace chroma {
		extern bool enabled;
		extern bool in_game;
		extern bool up_arrow;
		extern bool killstreak;
		extern bool health;
	}

	namespace example {
		extern bool example_checkbox;
		extern int  example_slider;
		extern int  example_combobox;
		extern float example_colour;
	}

	namespace colour {
		extern float aimbot_fov;
		extern float enemy_chams_vis;
		extern float enemy_chams_inv;
		extern float team_chams_vis;
		extern float team_chams_invis;
		extern float enemy_box_vis;
		extern float enemy_box_inv;
		extern float team_box_vis;
		extern float team_box_invis;
		extern float enemy_hitbox_vis;
		extern float enemy_hitbox_inv;
		extern float team_hitbox_vis;
		extern float team_hitbox_inv;
		extern float chams_self;
		extern float health_chams;
		extern float ammo_chams;
		extern float money_chams;
		extern float tank_chams;
		extern float projectile_chams;
		extern float building_chams_vis;
		extern float building_chams_inv;
		extern float custom_ui_colour;
		extern float enemy_glow_vis;
		extern float enemy_glow_inv;
		extern float team_glow_vis;
		extern float team_glow_inv;
	}

	namespace cheat {
		extern bool custom_ui_colour;
		extern bool unhook;
		extern bool hud;
		extern bool monocromia;
	}

	namespace dummy {
		extern bool dummy1;
		extern bool dummy2;
		extern bool dummy3;
		extern bool dummy4;
		extern bool dummy5;
	}
}
#endif // !VARS_H