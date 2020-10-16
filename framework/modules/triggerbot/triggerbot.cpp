#include "../../main.h"
float safe_to_trigger(c_base_player* local_player, c_base_weapon* weapon)
{
	auto local_class = local_player->get_class_num();
	auto demo_melee = weapon->get_item_definition_index() == WPN_Sword || weapon->get_item_definition_index() == WPN_FestiveEyelander ||
		weapon->get_item_definition_index() == WPN_Golfclub || weapon->get_item_definition_index() == WPN_ScottsSkullctter || weapon->get_item_definition_index() == WPN_Headless || weapon->get_item_definition_index() == WPN_Claidheamhmor;

	if (weapon->get_slot() == 2 && !demo_melee && !(weapon->get_item_definition_index() == WPN_DisciplinaryAction)) { return 10.0f; }
	if (weapon->get_slot() == 2 && demo_melee) { return 11.5f; }
	if (weapon->get_item_definition_index() == WPN_DisciplinaryAction) { return 13.0f; }
	if (local_class == TF2_Medic && weapon->get_slot() == 1) { return 25.0f; }
	if (local_player->get_class_num() == TF2_Pyro && weapon->get_slot() == 0) { return 18.0f; }
	return 10000.0f;
}

auto get_distance = [&](vec3_t to, vec3_t from) -> float {
	vec3_t delta = to - from;
	float distance = ::sqrt(delta.length());
	if (distance < 1.0f) {
		distance = 1.0f;
	} return distance;
};

namespace triggerbot {
	void triggerbot::do_triggerbot() {
		triggerbot::do_auto_shoot();
		triggerbot::do_auto_airblast();
		triggerbot::do_auto_detonate();
		triggerbot::do_auto_backstab();
	}
	void triggerbot::do_auto_shoot() {
		if (!vars::triggerbot::triggerbot_enabled || !utilities::is_variable_key_pressed(vars::triggerbot::triggerbot_key) || !ctx::local_player || ctx::local_player->get_life_state() == LIFE_DEAD) {
			return;
		}

		c_base_weapon* weapon = ctx::local_player->get_active_weapon();
		if (!weapon) {
			return;
		}

		bool item_index = weapon->get_item_definition_index();
		bool is_knife = (ctx::local_player->get_class_num() == TF2_Spy && weapon->get_slot() == 2);

		if (is_knife) {
			return;
		}

		vec3_t aim_angles, forward;
		interfaces::engineclient->get_view_angles(aim_angles);

		utilities::angle_vectors(aim_angles, &forward);
		forward = forward * 9999 + ctx::local_player->get_eye_position();

		ray_t ray; trace_t trace;
		trace_filter filt; filt.skip = ctx::local_player;

		ray.init(ctx::local_player->get_eye_position(), forward);
		interfaces::enginetrace->trace_ray(ray, MASK_SHOT_HULL | CONTENTS_HITBOX, &filt, &trace);

		//hit an entity, now check teams and etc.
		if (!trace.ent || trace.ent->is_dead() || trace.ent->is_dormant() || trace.ent->get_team_num() == ctx::local_player->get_team_num()
			|| !(trace.ent->get_client_class()->class_id == CObjectDispenser ||
				trace.ent->get_client_class()->class_id == CObjectSentrygun ||
				trace.ent->get_client_class()->class_id == CObjectTeleporter ||
				trace.ent->get_client_class()->class_id == CTFPlayer) ||
			trace.hitgroup < 0)
				return;

		//if the entity has some wack ass conditions don't shoot 'em
		if (trace.ent->get_condnew() & TFCond_Ubercharged || trace.ent->get_condnew() & TFCond_UberchargeFading || trace.ent->get_condnew() & TFCondEx_UberchargedHidden)
			return;
		if (vars::aimbot::ignore_cloaked && (trace.ent->get_condnew() & TFCond_Cloaked || trace.ent->get_condnew() & TFCond_DeadRingered))
			return;
		if (vars::aimbot::ignore_disguised && trace.ent->get_condnew() & TFCond_Disguised)
			return;
		//got a valid player!
	
		auto is_headshot_weapon = [&](c_base_player* local_player, c_base_weapon* local_weapon) { //is local weapon a headshot weapon?
			if (local_weapon->get_slot() == 0 && local_player->get_class_num() == TF2_Sniper)
				if (local_weapon->get_item_definition_index() != WPN_Huntsman && local_weapon->get_item_definition_index() != WPN_CompoundBow)
					return true;

			if (local_weapon->get_item_definition_index() == WPN_Ambassador || local_weapon->get_item_definition_index() == WPN_FestiveAmbassador)
				return true;

			return false;
		};
		if (trace.ent->get_client_class()->class_id == CTFPlayer) { //only do this check if you're actually tracing a player
			if (ctx::local_player->get_class_num() != TF2_Sniper && is_headshot_weapon(ctx::local_player, weapon) && trace.hitbox != 0) { //if so, only shoot when it traces a head hitbox
				return;
			}
			else if (is_headshot_weapon(ctx::local_player, weapon) && trace.hitbox != 0 && trace.ent->get_health() > 50 && ctx::local_player->get_condnew() & TFCond_Zoomed) {
				return;
			}
		}

		if (utilities::is_projectile(ctx::local_player, weapon)) { //return when you've got a projectile weapon. duh.
			return;
		}

		if (item_index == WPN_Ambassador || item_index == WPN_FestiveAmbassador) { // don't spam with amby! wait ur turn, bitch.
			if (!(interfaces::globals->curtime - weapon->last_fire_time() >= 1)) {
				return;
			}
		}

		if (vars::aimbot::wait_for_charge && ctx::local_player->get_class_num() == TF2_Sniper && weapon->get_slot() == 0 && ctx::is_scoped)
		{
			float cdmg = weapon->get_charge() * 3;
			float maxhs = 450.0f;
			bool maxCharge = cdmg >= maxhs;

			if (trace.ent->get_condnew() & TFCond_Cloaked) {
				cdmg = (cdmg * .80) - 1;
			}

			if (!(trace.ent->get_health() <= 150 || trace.ent->get_health() <= cdmg || (maxCharge && trace.ent->get_health() > maxhs))) {
				return;
			}
		}

		if (get_distance(trace.ent->get_world_space_center(), ctx::local_player->get_eye_position()) < safe_to_trigger(ctx::local_player, ctx::local_player->get_active_weapon()))
			ctx::command->buttons |= IN_ATTACK;
	}
	inline float dist(vec3_t myPos, vec3_t enPos) { return sqrt((myPos.x - enPos.x) * (myPos.x - enPos.x) + (myPos.y - enPos.y) * (myPos.y - enPos.y) + (myPos.z - enPos.z) * (myPos.z - enPos.z)); }
	void triggerbot::do_auto_airblast() {
		if (!vars::triggerbot::triggerbot_enabled || 
			!vars::triggerbot::auto_airblast || !utilities::is_variable_key_pressed(vars::triggerbot::triggerbot_key)) {
			return;
		}

		if (ctx::local_player->is_dead() || !ctx::local_player) {
			return;
		}

		if (ctx::local_player->get_class_num() != TF2_Pyro || 
			ctx::local_player->get_active_weapon()->get_slot() != 0) {
			return;
		}

		for (int i = 1; i <= interfaces::entitylist->get_highest_entity_index(); i++) {
			if (!ctx::local_player)
				continue;
			c_base_player* entity = reinterpret_cast<c_base_player*>(interfaces::entitylist->get_client_entity(i));
			c_base_weapon* weapon = ctx::local_player->get_active_weapon();
			if (!entity || !weapon) {
				continue;
			}

			if (entity->get_team_num() == ctx::local_player->get_team_num() || 
				ctx::local_player->is_dormant()) {
				continue;
			}

			int model_id = entity->get_client_class()->class_id;
			if (model_id == CBaseProjectile || model_id == CTFBaseProjectile || model_id == CTFBaseRocket ||
				model_id == CTFProjectile_Arrow || model_id == CTFProjectile_BallOfFire || model_id == CTFProjectile_Cleaver ||
				model_id == CTFProjectile_EnergyBall || model_id == CTFProjectile_EnergyRing || model_id == CTFProjectile_Flare ||
				model_id == CTFProjectile_GrapplingHook || model_id == CTFProjectile_HealingBolt || model_id == CTFProjectile_Jar ||
				model_id == CTFProjectile_JarGas || model_id == CTFProjectile_JarMilk || model_id == CTFProjectile_MechanicalArmOrb ||
				model_id == CTFProjectile_Rocket || model_id == CTFProjectile_SentryRocket || model_id == CTFProjectile_Throwable ||
				model_id == CTFProjectile_ThrowableBreadMonster || model_id == CTFProjectile_ThrowableBrick || model_id == CTFProjectile_ThrowableRepel ||
				model_id == CBaseGrenade || model_id == CTFGrenadePipebombProjectile || model_id == CTFWeaponBaseGrenadeProj) {

				c_net_channel* network = interfaces::engineclient->get_net_channel_info();
				if (!network) {
					continue;
				}

				convar* max_update_rate = interfaces::convar->find_variable(xorstr("sv_maxupdaterate"));
				if (!max_update_rate)
					continue;

				convar* min_interp_ratio = interfaces::convar->find_variable(xorstr("sv_client_min_interp_ratio"));
				if (!min_interp_ratio)
					continue;
				vec3_t center = entity->get_abs_origin();

				float interp = (min_interp_ratio->m_Value.m_nValue / max_update_rate->m_Value.m_nValue);

				float scale = interp + network->get_average_latency(FLOW_OUTGOING) + network->get_average_latency(FLOW_INCOMING);

				vec3_t speed;
				float speed2d;
				speed = aimbot::estimate_abs_velocity(entity);
				speed2d = aimbot::estimate_abs_velocity(entity).length2d();
				vec3_t screen;

				if (!visuals::world_to_screen(entity->get_abs_origin(), screen)) //ignore if offscreen
					continue;

				vector_ma(center, scale, speed, center);
				float distance = dist(ctx::local_player->get_eye_position(), entity->get_abs_origin());

				if (distance < 275.f) //find a good length
					ctx::command->buttons |= IN_ATTACK2;
			}
		}
	}
	void triggerbot::do_auto_detonate() {
		if (!vars::triggerbot::triggerbot_enabled ||
			!vars::triggerbot::auto_detonate ||
			(!utilities::is_variable_key_pressed(vars::triggerbot::triggerbot_key))) {
			return;
		}

		auto is_local_grenade = [&](c_base_player* entity) {
			if (entity->get_client_class()->class_id != 216) return false;
			if (entity->get_pipe_type() != 1) return false;

			auto get_thrower = entity->get_thrower();
			if (!get_thrower) return false;

			auto entity_handle = (c_base_player*)interfaces::entitylist->get_client_entity_from_handle(get_thrower);
			if (!entity_handle) return false;

			if (entity_handle != ctx::local_player) return false;
			return true;
		};

		for (int i = 0; i < interfaces::entitylist->get_highest_entity_index(); i++) {
			c_base_player* first_entity = reinterpret_cast<c_base_player*>(interfaces::entitylist->get_client_entity(i));
			if (!first_entity) {
				continue;
			}

			vec3_t sticky_position = first_entity->get_world_space_center();
			if (is_local_grenade(first_entity)) {
				for (int player = 0; player < interfaces::engineclient->get_max_clients(); player++) {
					if (i == ctx::local_player->get_index()) { continue; }
					c_base_player* second_entity = reinterpret_cast<c_base_player*>(interfaces::entitylist->get_client_entity(player));
					if (!second_entity || second_entity->is_dormant() || second_entity->is_dead()) {
						continue;
					}
					if (second_entity->get_team_num() == ctx::local_player->get_team_num()) {
						continue;
					}

					if (utilities::has_condition(second_entity, 32) || utilities::has_condition(second_entity, 16384) || 
						utilities::has_condition(second_entity, 16) || utilities::has_condition(second_entity, 8192)) {
						continue;
					}

					auto get_distance = [&](vec3_t to, vec3_t from) -> float {
						vec3_t delta = to - from;
						float distance = ::sqrt(delta.length());

						if (distance < 1.0f) {
							distance = 1.0f;
						} 
						
						return distance;
					};

					auto is_bomb_visible = [&](c_base_player* first, c_base_player* second) {
						trace_t tr; ray_t ray; trace_filter filter;

						auto first_origin = first->get_abs_origin(),
							 second_origin = second->get_abs_origin();

						filter.skip = first;
						ray.init(first_origin, second_origin);
						interfaces::enginetrace->trace_ray(ray, MASK_SHOT, &filter, &tr);

						return (tr.ent == second);
					};

					if (get_distance(sticky_position, second_entity->get_abs_origin()) < 12.0f) {
						if (is_bomb_visible(first_entity, second_entity)) {
							ctx::command->buttons |= IN_ATTACK2;
						}
					}
				}
			}
		}
	}

	void triggerbot::do_auto_backstab() {
		if (!vars::triggerbot::triggerbot_enabled || !utilities::is_variable_key_pressed(vars::triggerbot::triggerbot_key)) {
			return;
		}

		if (!ctx::local_player || ctx::local_player->is_dead()) {
			return;
		}

		c_base_weapon* local_weapon = ctx::local_player->get_active_weapon();
		if (!local_weapon) {
			return;
		}

		bool item_index = local_weapon->get_item_definition_index();
		bool is_knife = (ctx::local_player->get_class_num() == TF2_Spy && local_weapon->get_slot() == 2);

		if (is_knife) {
			if (local_weapon->ready_to_backstab()) {
				ctx::command->buttons |= IN_ATTACK;
			}
		}
	}
}
