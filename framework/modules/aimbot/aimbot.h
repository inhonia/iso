#pragma once

namespace aimbot {
	extern c_base_player* aimbot_target;

	void do_aimbot();
	void do_autorev();
	bool trace_hit_player();
	void reset_smooth();
	void fix_movement(c_user_cmd* pCmd, vec3_t old_angles, float old_fowardmove, float old_sidemove);

	c_base_player* get_best_target(c_base_player* local_player, c_user_cmd* command);
	c_base_player* get_closest_enemy(c_base_player* local_player, c_user_cmd* command, int fov);
	int get_best_hitbox(c_base_player* local_player, c_base_player* entity, bool is_a_building);

	vec3_t estimate_abs_velocity(c_base_player* entity);
	void get_projectile_fire_setup(c_base_player* local_player, const vec3_t& viewangles, vec3_t offset, vec3_t* source);
	bool calculate_proj_angle(const vec3_t& local_eyepos, const vec3_t& predicted_pos, const float& speed, const float& gravity, vec3_t& vec_hitbox);
	void projectile_prediction(c_base_player* local_player, c_base_player* entity, c_base_weapon* local_weapon, vec3_t& vec_hitbox, int quality);

	inline float get_speed(c_base_weapon* local_weapon)
	{
		int item_index = local_weapon->get_item_definition_index();
		float weapon_speed = 0.0f;
		switch (item_index) {
		case WPN_DirectHit:
			weapon_speed = 1980.0f; break;
		case WPN_BotRocketlauncherB:
		case WPN_BotRocketlauncherC:
		case WPN_BotRocketlauncherD:
		case WPN_BotRocketlauncherEG:
		case WPN_BotRocketlauncherES:
		case WPN_BotRocketlauncherG:
		case WPN_BotRocketlauncherR:
		case WPN_BotRocketlauncherS:
		case WPN_FestiveRocketLauncher:
		case WPN_NewRocketLauncher:
		case WPN_RocketLauncher:
		case WPN_Original:
		case WPN_Airstrike:
		case WPN_BlackBox:
		case WPN_CowMangler:
			weapon_speed = 1100.0f; break;
		case WPN_RighteousBison:
			weapon_speed = 1200.0f; break;
		case WPN_FestiveFlaregun:
		case WPN_Flaregun:
		case WPN_Detonator:
			weapon_speed = 2000.0f; break;
		case WPN_SyringeGun:
		case WPN_NewSyringeGun:
		case WPN_Blutsauger:
		case WPN_Overdose:
			weapon_speed = 1000.0f; break;
		case WPN_RescueRanger:
		case WPN_Crossbow:
		case WPN_FestiveCrossbow:
			weapon_speed = 2400.0f; break;
		case WPN_GrenadeLauncher:
		case WPN_NewGrenadeLauncher:
		case WPN_FestiveGrenadeLauncher:
			weapon_speed = 1217.0f; break;
		case WPN_LochNLoad:
			weapon_speed = 1513.3f; break;
		case WPN_LoooseCannon:
			weapon_speed = 1453.9f; break;
		case WPN_IronBomber:
			weapon_speed = 1216.6f; break;
		case WPN_ManMelter:
			weapon_speed = 3000.0f; break;
		case WPN_LibertyLauncher:
			weapon_speed = 1540.0f; break;
		case WPN_Pomson:
			weapon_speed = 1200.0f; break;
		case WPN_Huntsman:
		case WPN_CompoundBow:
		case WPN_FestiveHuntsman:
		{
			float begincharge = local_weapon->begin_charge_time();
			float charge = begincharge == 0.0f ? 0.0f : interfaces::globals->curtime - begincharge;
			if (charge > 1.0f)
				charge = 1.0f;
			float speed = (800 * charge + 1800);
			weapon_speed = speed; //wowie zowie
		} break;
		case WPN_Flamethrower:
		case WPN_FestiveFlamethrower:
		case WPN_NewFlamethrower: //idk what this one is
		case WPN_BotFlamethrowerB:
		case WPN_BotFlamethrowerC:
		case WPN_BotFlamethrowerD:
		case WPN_BotFlamethrowerEG:
		case WPN_BotFlamethrowerES:
		case WPN_BotFlamethrowerG:
		case WPN_BotFlamethrowerR:
		case WPN_BotFlamethrowerS:
		case WPN_Phlogistinator:
		case WPN_Rainblower:
		case WPN_Backburner:
		case WPN_Degreaser:
			weapon_speed = 1000.0f; break;
		case WPN_DragonsFury:
			weapon_speed = 3000.0f; break;
		case WPN_Jarate:
		case WPN_FestiveJarate:
		case WPN_Milk:
		case WPN_MutatedMilk:
		case 1105: //Self-Aware Beauty Mark
			weapon_speed = 1000.0f; break;
		case WPN_StickyLauncher:
		case WPN_NewStickyLauncher:
		case WPN_BotStickyD:
		case WPN_BotStickyB:
		case WPN_BotStickyC:
		case WPN_BotStickyEG:
		case WPN_BotStickyES:
		case WPN_BotStickyG:
		case WPN_BotStickyR:
		case WPN_BotStickyS:
		case WPN_StickyJumper:
		{
			float begincharge = local_weapon->begin_charge_time();
			float charge = begincharge == 0.0f ? 0.0f : interfaces::globals->curtime - begincharge;
			if (charge > 1.0f)
				charge = 1.0f;
			float speed = (800 * charge + 1600);
			weapon_speed = speed;
		} break;
		default: weapon_speed = 0.0f; break;
		}
		return weapon_speed;
	}

	inline float get_gravity(c_base_weapon* local_weapon)
	{
		int item_index = local_weapon->get_item_definition_index();
		float weapon_gravity = 0.0f;
		switch (item_index) {
		case WPN_RescueRanger:
		case WPN_Crossbow:
		case WPN_FestiveCrossbow:
		case WPN_Flaregun:
		case WPN_FestiveFlaregun:
		case WPN_Detonator:
			weapon_gravity = 0.3f; break;
		case WPN_ManMelter:
			weapon_gravity = 0.2f; break;
		case WPN_GrenadeLauncher:
		case WPN_NewGrenadeLauncher:
		case WPN_FestiveGrenadeLauncher:
		case WPN_LoooseCannon:
		case WPN_LochNLoad:
		case WPN_IronBomber:
			//0.82f
			weapon_gravity = 0.4f; break;
		case WPN_SyringeGun:
			weapon_gravity = 0.4f; break;
		case WPN_Huntsman:
		case WPN_CompoundBow:
		case WPN_FestiveHuntsman: //wow this is fucking sauce :weary:
		{
			float begincharge = local_weapon->begin_charge_time();
			float charge = begincharge == 0.0f ? 0.0f : interfaces::globals->curtime - begincharge;
			if (charge > 1.0f)
				charge = 1.0f;
			float gravity = 0.4f - charge / 2;
			weapon_gravity = gravity;
		} break;
		case WPN_Jarate:
		case WPN_FestiveJarate:
		case WPN_Milk:
		case WPN_MutatedMilk:
		case 1105: //Self-Aware Beauty Mark
			weapon_gravity = 0.50f; break;
		case WPN_StickyLauncher:
		case WPN_NewStickyLauncher:
		case WPN_BotStickyD:
		case WPN_BotStickyB:
		case WPN_BotStickyC:
		case WPN_BotStickyEG:
		case WPN_BotStickyES:
		case WPN_BotStickyG:
		case WPN_BotStickyR:
		case WPN_BotStickyS:
		case WPN_StickyJumper:
		{
			float begincharge = local_weapon->begin_charge_time();
			float charge = begincharge == 0.0f ? 0.0f : interfaces::globals->curtime - begincharge;
			if (charge > 1.0f)
				charge = 1.0f;
			float gravity = 0.4f - charge / 2;
			weapon_gravity = gravity;
		} break;
		case WPN_Flamethrower:
		case WPN_FestiveFlamethrower:
		case WPN_NewFlamethrower: //idk what this one is
		case WPN_BotFlamethrowerB:
		case WPN_BotFlamethrowerC:
		case WPN_BotFlamethrowerD:
		case WPN_BotFlamethrowerEG:
		case WPN_BotFlamethrowerES:
		case WPN_BotFlamethrowerG:
		case WPN_BotFlamethrowerR:
		case WPN_BotFlamethrowerS:
		case WPN_Phlogistinator:
		case WPN_Rainblower:
		case WPN_Backburner:
		case WPN_Degreaser:
			weapon_gravity = 0.4f; break;
		default: weapon_gravity = 0.0f; break;
		}
		return weapon_gravity;
	}

}