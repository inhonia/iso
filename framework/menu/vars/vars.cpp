#include "../../main.h"

namespace vars {
	namespace menu {
		bool menu_active	= false;
		int total_items		= 0;
		int current_pos		= 0;
		int font_size		= 15;
		int menu_x			= 250;
		int menu_y			= 250;
	}

	namespace aimbot {
		bool aimbot_tab = false;
		bool enabled = false;
		bool drawfov = true;
		int  aimbot_key = 0;
		int  field_of_view = 0;
		bool auto_fov = false;
		int  main_hitbox = 0;
		int pointscale = 75;
		bool hitscan = false;
		bool whilereloading = false;
		bool autoshoot = false;
		bool silent_aim = false;
		bool autoscope = false;
		//int smoothing = 0;
		int smoothingx = 0;
		int smoothingy = 0;
		bool smoothing = false;
		bool assist = false;
		bool bullet_time = false;
		bool lagfix = false;
		bool multipoint = false;
		bool prefer_medic = false;
		bool target_lock = false;
		bool projectile_aimbot = false;
		bool wait_for_charge = false;
		bool aimbot_sentry = false;
		bool aimbot_dispenser = false;
		bool aimbot_teleporters = false;
		bool ignore_cloaked = true;
		bool ignore_disguised = true;
		bool heavy_autorev = false;
	}

	namespace visuals {
		bool visuals_tab = false;
		bool box = false;
		bool health = false;
		bool name = false;
		bool weapon = false;
		bool steam_id = false;
		bool backtrack_visualization = false;
		bool custom_crosshair = true;
		bool spectators = false;
		int spectator_pos_x = 300;
		int spectator_pos_y = 800;
		int playerlist_pos_x = 600;
		int playerlist_pos_y = 1200;
		bool out_of_fov_arrows = false;
		bool nightmode = false;
		int nightmode_value = 100;
		bool projectile_prediction = false;
		bool thirdperson = false;
		int thirdperson_angle = 0;
		int thirdperson_dist = 150;
		int  forced_fov = 90;
		int real_fov = 0;
		bool no_zoom = false;
		bool no_recoil = false;
		bool no_scope = false;
		bool no_cloak = false;
		bool visible_only = true;
		bool aa_visual = false;

		int esp_type = 0;

		bool player_enemyonly = true;

		bool team_box = false;
		bool team_health = false;
		bool team_name = false;
		bool team_weapon = false;
		bool team_ammo = false;
		bool team_steamid = false;
		bool team_backtrack_visulization = false; //DUPLICATE OF OLD VARS DONT SHOW ON TEAMMATES LOL
		bool team_bones = false;
		bool team_chams = false;
		bool team_glow = false;
		bool team_d_lights = false;
		int team_d_lights_radius = 10.0f;
		bool team_hitboxes = false;

		bool enemy_box = false;
		bool enemy_health = false;
		bool enemy_name = false;
		bool enemy_weapon = false;
		bool enemy_ammo = false;
		bool enemy_steamid = false;
		//bool enemy_backtrack_visulization = false;
		int enemy_backtrack_visulization = 0;
		bool enemy_bones = false;
		bool enemy_chams = false;
		bool enemy_glow = false;
		bool enemy_d_lights = false;
		int enemy_d_lights_radius = 10.0f;
		bool enemy_hitboxes = false;

		int enemy_chams_visible_clr = 0;
		int enemy_chams_hidden_clr = 1;
		int team_chams_visible_clr = 5;
		int team_chams_hidden_clr = 6;

		int enemy_box_clr = 1;
		int enemy_box_clr_hidden = 0;
		int team_box_clr = 6;
		int team_box_clr_hidden = 5;

		int local_chams_clr = 4;

		bool building_enemyonly = false;
		bool building_box = false;
		bool building_health = false;
		bool building_name = false;
		bool building_builder = false;
		bool building_chams = false;
		bool building_level = false;
		bool building_glow = false;

		bool world_box = false;
		bool world_name = false;
		bool world_health_chams = false;
		bool world_ammo_chams = false;
		bool world_projectile_chams = false;
		bool world_money_chams = false;
		bool world_tank_chams = false;

		bool radar = false;
		int radar_drag_x = 300;
		int radar_drag_y = 300;
		int radar_width = 300;
		int radar_height = 300;

		bool player_list = false;
		int player_list_x = 700;
		int player_list_y = 300;
	}

	namespace chams {
		int chams_activation = 0;
		int chams_model = 0;
		bool enemy_only = true;
		bool local_player = false;
		bool wearables = true;
		bool health_and_ammo = false;
		bool buildings = false;
	}

	namespace triggerbot {
		bool triggerbot_tab = false;
		bool triggerbot_enabled = false;
		bool triggerbot_on_key = false;
		int triggerbot_delay = 30;
		bool triggerbot_on_backtrack = false;
		int  triggerbot_key = 0;
		bool auto_airblast = false;
		bool auto_airblast_on_key = false;
		bool auto_detonate = false;
		bool auto_detonate_on_key = false;
	}

	namespace position_adjustment {
		bool position_adjustment_tab = false;
		bool enabled = false;
		int	 ticks = 0;
	}

	namespace miscellanous {
		bool miscellanous_tab = false;
		bool edgejump = false;
		bool bunnyhop = false;
		int autostrafe = 0;
		bool duck_in_air = false;
		bool taunt_slide = true;
		bool nopush = false;
		bool sv_pure_bypass = true;
		bool nopunch = false;
		bool fake_crouch = false;
	}

	namespace chatspam {
		bool chatspam_enabled = false;
		int  chatspam_new_lines = 0;
		int  chatspam_selection = 0;
	}

	namespace config {
		bool save;
		bool load;
		int selection;
	}

	namespace hvh {
		int fakelag_mode = 0;
		int fakelag_value = 0;
		int fakelag_jitter = 0;

		int anti_aim_pitch = 0;

		bool auto_pitch_resolver = true;
		bool resolver_key = false;
		int  max_missed_shots = 0;
		bool pitch_resolver = false;
		bool yaw_resolver = false;
		bool anti_aim_line = false;
		
		bool anti_aim_yaw = false;

		int anti_aim_menu_select = 0;

		int anti_aim_real_offset = 0; //edit sliders to take a min as well as a max for this. -180 to 180.
		int anti_aim_real_type = 0;
		int anti_aim_real_spinspeed = 10;
		int anti_aim_real_jitter = 0;
		int anti_aim_real_spin_range = 0;
		int anti_aim_real_add_spin_speed = 10;

		int anti_aim_fake_offset = 180;
		int anti_aim_fake_type = 0;
		int anti_aim_fake_spinspeed = 10;
		int anti_aim_fake_jitter = 0;
		int anti_aim_fake_spin_range = 0;
		int anti_aim_fake_add_spin_speed = 10;

		//bool anti_aim_desync = false;
		//int anti_aim_desync_type = 0;
	}

	namespace announcer {
		bool enabled = false;
	}

	namespace chroma {
		bool enabled = true;
		bool in_game = true;
		bool up_arrow = true;
		bool killstreak = true;
		bool health = false;
	}

	namespace example {
	    bool example_checkbox = false;
		int  example_slider = 0;
		int  example_combobox = 0;
		//float  example_colour = 70;
		//bool unhook = false;
	}

	namespace colour {
		float aimbot_fov = 0;
		float enemy_chams_vis = 30;
		float enemy_chams_inv = 50;
		float team_chams_vis = 70;
		float team_chams_invis = 90;
		float enemy_box_vis = 30;
		float enemy_box_inv = 50;
		float team_box_vis = 70;
		float team_box_invis = 90;
		float enemy_hitbox_vis = 80;
		float enemy_hitbox_inv = 60;
		float team_hitbox_vis = 10;
		float team_hitbox_inv = 40;
		float chams_self = 70;
		float health_chams = 60;
		float ammo_chams = 20;
		float money_chams = 40;
		float tank_chams = 50;
		float projectile_chams = 80;
		float building_chams_vis = 30;
		float building_chams_inv = 50;
		float custom_ui_colour = 0;
		float enemy_glow_vis = 30;
		float enemy_glow_inv = 50;
		float team_glow_vis = 70;
		float team_glow_inv = 90;
	}

	namespace cheat {
		bool custom_ui_colour = false;
		bool unhook = false;
		bool hud = false;
		bool monocromia = false;
	}

	namespace dummy {
		bool dummy1 = false;
		bool dummy2 = false;
		bool dummy3 = true;
		bool dummy4 = true;
		bool dummy5 = false;
	}
}