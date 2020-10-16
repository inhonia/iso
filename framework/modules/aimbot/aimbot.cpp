#include "../../main.h"

inline float dist(vec3_t myPos, vec3_t enPos) { return sqrt((myPos.x - enPos.x) * (myPos.x - enPos.x) + (myPos.y - enPos.y) * (myPos.y - enPos.y) + (myPos.z - enPos.z) * (myPos.z - enPos.z)); }

auto is_headshot_weapon = [&](c_base_player* local_player, c_base_weapon* local_weapon) {
	if (local_weapon->get_slot() == 0 && local_player->get_class_num() == TF2_Sniper)
		if (local_weapon->get_item_definition_index() != WPN_Huntsman && local_weapon->get_item_definition_index() != WPN_CompoundBow)
			return true;

	if (local_weapon->get_item_definition_index() == WPN_Ambassador || local_weapon->get_item_definition_index() == WPN_FestiveAmbassador)
		return true;

	return false;
};

float safe_to_aim(c_base_player* local_player, c_base_weapon* weapon)
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

void clamp_angle(vec3_t &angles) {
	while (angles[0] > 89) { angles[0] -= 180; }
	while (angles[0] < -89) { angles[0] += 180; }
	while (angles[1] > 180) { angles[1] -= 360; }
	while (angles[1] < -180) { angles[1] += 360; }
	while (angles[2] != 0) { angles[2] = 0; }
}

vec3_t VelocityPrediction(c_base_player* ent, vec3_t position) {
	if (!ent) return vec3_t();
	float factor = 0;
	vec3_t offset;
	vec3_t accel = vec3_t(0, 0, 0);
	vec3_t vel = ent->get_velocity();

	factor = TIME_TO_TICKS(interfaces::engineclient->get_net_channel_info()->get_average_latency(FLOW_OUTGOING) * 2);
	for (int i = 0; i < factor; i++)
	{
		offset += (vel + accel * i) * interfaces::globals->interval_per_tick;
	}
	return position + offset;
}

float getlerptime()
{
	float updaterate = interfaces::convar->find_variable(xorstr("cl_interp_ratio"))->get_float();
	convar* minupdate = interfaces::convar->find_variable(xorstr("sv_minupdaterate"));
	convar* maxupdate = interfaces::convar->find_variable(xorstr("sv_maxupdaterate"));

	if (minupdate && maxupdate)
		updaterate = maxupdate->get_float();

	float ratio = interfaces::convar->find_variable(xorstr("cl_interp_ratio"))->get_float();

	if (ratio == 0)
		ratio = 1.0f;

	float lerp = interfaces::convar->find_variable(xorstr("cl_interp"))->get_float();
	convar* cmin = interfaces::convar->find_variable(xorstr("sv_client_min_interp_ratio"));
	convar* cmax = interfaces::convar->find_variable(xorstr("sv_client_max_interp_ratio"));

	if (cmin && cmax && cmin->get_float() != 1)
		ratio = clamp(ratio, cmin->get_float(), cmax->get_float());

	return max(lerp, ratio / updaterate);
}

bool valid_backtrack_tick(tick_record tick)
{
	float correct = 0;
	float max_unlag = interfaces::convar->find_variable(xorstr("sv_maxunlag"))->get_float();
	correct += interfaces::engineclient->get_net_channel_info()->get_latency(FLOW_OUTGOING);
	correct += interfaces::engineclient->get_net_channel_info()->get_latency(FLOW_INCOMING);
	correct += getlerptime();
	correct = clamp(correct, 0.f, max_unlag);
	float delta_time = correct - (interfaces::globals->curtime - tick.simulation_time);
	if (fabsf(delta_time) > 0.2)
		return true;

	return false;
}


namespace aimbot {
	c_base_player* aimbot::aimbot_target = nullptr;

	void aimbot::do_aimbot() {
		c_base_player* local_player = ctx::local_player; c_user_cmd* command = ctx::command;

		if (!ctx::local_player || ctx::local_player->is_dead() || !ctx::command) { //sanity checks i forget about a lot
			return;
		}

		vec3_t vec_old_angles = command->viewangles; float fl_old_forwardmove = command->forward_move, fl_old_sidemove = command->side_move;

		if (!vars::aimbot::enabled || !utilities::is_variable_key_pressed(vars::aimbot::aimbot_key)) {
			return;
		}

		c_base_player* entity = get_best_target(local_player, command);
		if (!entity) { return; }

		c_base_weapon* weapon = local_player->get_active_weapon();
		if (!weapon) { return; }

		int item_index = weapon->get_item_definition_index();

		auto get_vec_offset = [local_player, weapon, entity, item_index]() -> vec3_t {
			auto offset = vec3_t(0, 0, 0);
			switch (item_index) {
			case WPN_DirectHit:
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
				offset = vec3_t(23.5, 12.0, -3.0); break;
			case WPN_RighteousBison:
				offset = vec3_t(23.5, 8.0, -3.0); break;
			case WPN_SyringeGun:
			case WPN_NewSyringeGun:
			case WPN_Blutsauger:
			case WPN_Overdose:
				offset = vec3_t(16.0, 6.0, -8.0); break;
			case WPN_RescueRanger:
			case WPN_Crossbow:
			case WPN_FestiveCrossbow:
			case WPN_Huntsman:
			case WPN_CompoundBow:
			case WPN_FestiveHuntsman:
				offset = vec3_t(23.5, 8.0, -3.0); break;
			case WPN_GrenadeLauncher:
			case WPN_NewGrenadeLauncher:
			case WPN_FestiveGrenadeLauncher:
			case WPN_LochNLoad:
			case WPN_LoooseCannon:
			case WPN_IronBomber:
				offset = vec3_t(16.0, 8.0, -6.0); break;
			case WPN_Jarate:
			case WPN_FestiveJarate:
			case WPN_Milk:
			case WPN_MutatedMilk:
			case 1105: //Self-Aware Beauty Mark
				offset = vec3_t(30.0, 7.0, -4.0); break;
			default: offset = vec3_t(0, 0, 0); break;
			}
			return offset;
		};

		int best_hitbox = aimbot::get_best_hitbox(local_player, entity, false);
		if (best_hitbox == -1) { return; }

		vec3_t  vec_hitbox = utilities::get_multipoint_position(entity, best_hitbox, vars::aimbot::pointscale),
				vec_eye_position = local_player->get_eye_position(), vec_eye_angles;

		auto size_of_position_adjustment = position_adjustment::ticks[entity->get_index()].size() - 1;

		

		if (entity->get_client_class()->class_id == 86 || entity->get_client_class()->class_id == 88 || entity->get_client_class()->class_id == 89) {
			if ((!vars::aimbot::aimbot_sentry && entity->get_client_class()->class_id == 88) ||
				(!vars::aimbot::aimbot_dispenser && entity->get_client_class()->class_id == 86) ||
				(!vars::aimbot::aimbot_teleporters && entity->get_client_class()->class_id == 89)) {
				return;
			}

			vec_hitbox = entity->get_world_space_center();

			aimbot::projectile_prediction(local_player, entity, weapon, vec_hitbox);
			if (!utilities::is_entity_visible(local_player, entity, local_player->get_eye_position(), vec_hitbox)) {
				return;
			}
		}
		else if (entity->get_client_class()->class_id == 246) {

			aimbot::projectile_prediction(local_player, entity, weapon, vec_hitbox);

			if (!utilities::is_entity_visible(local_player, entity, local_player->get_eye_position(), vec_hitbox)) {

				if (position_adjustment::ticks[entity->get_index()].empty()) { return; }

				if (utilities::is_entity_visible(local_player, entity, local_player->get_eye_position(), position_adjustment::ticks[entity->get_index()].at(size_of_position_adjustment).head_position) && !utilities::is_projectile(ctx::local_player, weapon)) {

					vec_hitbox = position_adjustment::ticks[entity->get_index()].at(size_of_position_adjustment).head_position;

				}
				else {
					return;
				}
			}
			else {
			}

			if (item_index == WPN_Ambassador || item_index == WPN_FestiveAmbassador) {
				if (!(interfaces::globals->curtime - weapon->last_fire_time() >= 1)) {
					return;
				}
			}
			if (vars::aimbot::autoscope && !utilities::has_condition(ctx::local_player, 2) && !ctx::is_scoped && local_player->get_class_num() == TF2_Sniper && weapon->get_slot() == 0) {
				command->buttons |= IN_ATTACK2;
				ctx::is_scoped = true;
			}
			
			if (local_player->get_class_num() == TF2_Sniper && weapon->get_slot() == 0 && weapon->get_charge() < 1.f && vars::aimbot::autoscope) {
				return;
			}
			else {
			}

			if (vars::aimbot::wait_for_charge && local_player->get_class_num() == TF2_Sniper && weapon->get_slot() == 0 && ctx::is_scoped)
			{
				float cdmg = weapon->get_charge() * 3;
				float maxhs = 450.0f;
				bool maxCharge = cdmg >= maxhs;

				if (entity->get_condnew() & TFCond_Cloaked) { //god new conds feel so much better
					cdmg = (cdmg * .80) - 1;
				}

				if (!(entity->get_health() <= 150 || entity->get_health() <= cdmg || (maxCharge && entity->get_health() > maxhs))) {
					return;
				}
			}

		}

		aimbot::aimbot_target = entity;

		auto bullet_time = [&](c_base_player* local_player) -> bool {
			if (!local_player) return false;

			c_base_weapon* local_weapon = local_player->get_active_weapon();
			int tick_base = local_player->get_tick_base();
			if (!local_weapon || !tick_base) return false;

			float next_attack = local_weapon->get_next_attack();
			bool can_tickbase = next_attack <= TICKS_TO_TIME(tick_base);

			return can_tickbase;
		};

		auto mathematics = [&](vec3_t hitbox, vec3_t eye_position, vec3_t eye_angles, c_user_cmd* command, c_base_weapon* weapon, bool bullet_time_enabled) -> void {
			if (bullet_time_enabled) {
				if (bullet_time(local_player) && weapon->get_slot() != 2) {
					vec3_t length = hitbox - vec_eye_position;
					vec_eye_angles += get_vec_offset();
					utilities::vector_angle(length, vec_eye_angles);
					clamp_angle(vec_eye_angles);

					if(is_headshot_weapon(ctx::local_player, weapon) && ctx::local_player->get_active_weapon()->get_next_attack() < interfaces::globals->curtime)
					{
					resolver::shots_fired[entity->get_index()]++;
					}

					command->viewangles = vec_eye_angles;
					if (!vars::aimbot::silent_aim) {
						interfaces::engineclient->set_view_angles(command->viewangles);
					} 
				}
				else if (weapon->get_slot() == 2) {
					vec3_t length = hitbox - vec_eye_position;
					vec_eye_angles += get_vec_offset();
					utilities::vector_angle(length, vec_eye_angles);
					clamp_angle(vec_eye_angles);
					command->viewangles = vec_eye_angles;
					if (!vars::aimbot::silent_aim) {
						interfaces::engineclient->set_view_angles(command->viewangles);
					}
				}
			}
			else {
				vec3_t local_pos = vec_eye_position;
				vec3_t enemy_pos = hitbox;
				vec3_t angle = utilities::vectorangles(local_pos, enemy_pos);

				if (vars::aimbot::smoothing) {
					float smoothingx = vars::aimbot::smoothingx;
					float smoothingy = vars::aimbot::smoothingy;

					if (smoothingx < 1) {
						smoothingx = 1;
					}

					if (smoothingy < 1) {
						smoothingy = 1;
					}

					vec3_t currentview, delta, aimdirection = angle;
					interfaces::engineclient->get_view_angles(currentview);
					delta = aimdirection - currentview;
					clamp_angle(delta);

					aimdirection.x = currentview.x + delta.x / smoothingx;
					aimdirection.y = currentview.y + delta.y / smoothingy;

					clamp_angle(aimdirection);
					interfaces::engineclient->set_view_angles(aimdirection);
					command->viewangles = aimdirection;

				}
				else {

					vec3_t length = hitbox - vec_eye_position;
					vec_eye_angles += get_vec_offset();
					utilities::vector_angle(length, vec_eye_angles);
					clamp_angle(vec_eye_angles);
					command->viewangles = vec_eye_angles;
					if (!vars::aimbot::silent_aim) {
						interfaces::engineclient->set_view_angles(command->viewangles);
					}
				}
			}
			
		}; 
		
		float server_time = ctx::local_player->get_tick_base() * interfaces::globals->interval_per_tick;
		bool bullet_time_hehe = !(weapon->get_next_attack() > server_time);

		mathematics(vec_hitbox, vec_eye_position, vec_eye_angles, command, weapon, vars::aimbot::bullet_time);
		aimbot::fix_movement(command, vec_old_angles, fl_old_forwardmove, fl_old_sidemove);

		bool is_knife = (ctx::local_player->get_class_num() == TF2_Spy && weapon->get_slot() == 2);

		if (vars::aimbot::autoshoot && !is_knife && !vars::aimbot::smoothing && !utilities::is_special_fire_type(local_player, weapon)) {
			command->buttons |= IN_ATTACK;
			if (command->buttons & IN_ATTACK && entity->get_client_class()->class_id == 246 && !utilities::is_projectile(ctx::local_player, weapon)) {
				if (vars::position_adjustment::enabled) {
					if (position_adjustment::ticks[entity->get_index()].empty()) { return; }
					if (vec_hitbox == position_adjustment::ticks[entity->get_index()].at(size_of_position_adjustment).head_position) {
						command->tick_count = TIME_TO_TICKS(position_adjustment::ticks[entity->get_index()].at(size_of_position_adjustment).simulation_time);
					}
				}
			} else if (command->buttons & IN_ATTACK && bullet_time_hehe) {
				*ctx::send_packet = false;
			} else {
				*ctx::send_packet = true;
			}
		}
		else if (vars::aimbot::autoshoot && is_knife) {
			if (weapon->ready_to_backstab()) {
				command->buttons |= IN_ATTACK;
			}

			if (command->buttons & IN_ATTACK && bullet_time_hehe && !utilities::is_special_fire_type(local_player, weapon)) {
				*ctx::send_packet = false;
			} else {
				*ctx::send_packet = true;
			}
		}
		else if (vars::aimbot::autoshoot && vars::aimbot::smoothing && local_player->get_class_num() == TF2_Medic) {
			command->buttons |= IN_ATTACK;
		}
		else if (vars::aimbot::autoshoot && vars::aimbot::smoothing && !utilities::is_projectile(ctx::local_player, weapon) && !(local_player->get_class_num() == TF2_Medic)) {
			vec3_t aim_angles, forward;
			interfaces::engineclient->get_view_angles(aim_angles);

			utilities::angle_vectors(aim_angles, &forward);
			forward = forward * 9999 + ctx::local_player->get_eye_position();

			ray_t ray; trace_t trace;
			trace_filter filt; filt.skip = ctx::local_player;

			ray.init(ctx::local_player->get_eye_position(), forward);
			interfaces::enginetrace->trace_ray(ray, MASK_AIM, &filt, &trace);

			if (!trace.ent || trace.ent->is_dead() || trace.ent->is_dormant() || trace.ent->get_team_num() == ctx::local_player->get_team_num()
				|| !(trace.ent->get_client_class()->class_id == CObjectDispenser||
					trace.ent->get_client_class()->class_id == CObjectSentrygun ||
					trace.ent->get_client_class()->class_id == CObjectTeleporter ||
					trace.ent->get_client_class()->class_id == CTFPlayer) ||
				trace.hitgroup < 0)
				return;

			if (trace.ent->get_condnew() & TFCond_Ubercharged || trace.ent->get_condnew() & TFCond_UberchargeFading || trace.ent->get_condnew() & TFCondEx_UberchargedHidden)
				return;

			if (trace.ent->get_client_class()->class_id == CTFPlayer) { //only do this check if you're actually tracing a player
				if (is_headshot_weapon(ctx::local_player, weapon) && trace.hitbox != 0) { //if so, only shoot when it traces a head hitbox
					return;
				}
			}

			ctx::command->buttons |= IN_ATTACK;
		}
		else if (vars::aimbot::autoshoot && vars::aimbot::smoothing >= 1 && utilities::is_projectile(ctx::local_player, weapon) && !utilities::is_special_fire_type(local_player, weapon)) {
			ctx::command->buttons |= IN_ATTACK;
			if (command->buttons & IN_ATTACK && entity->get_client_class()->class_id == 246 && !utilities::is_projectile(ctx::local_player, weapon)) {
				if (vars::position_adjustment::enabled) {
					if (position_adjustment::ticks[entity->get_index()].empty()) { return; }
					if (vec_hitbox == position_adjustment::ticks[entity->get_index()].at(size_of_position_adjustment).head_position) {
						command->tick_count = TIME_TO_TICKS(position_adjustment::ticks[entity->get_index()].at(size_of_position_adjustment).simulation_time);
					}
				}
			}
		}
		else if (vars::aimbot::autoshoot && utilities::is_projectile(ctx::local_player, weapon) && utilities::is_special_fire_type(local_player, weapon))
		{
			static bool begancharge;
			float begincharge = ctx::local_player->get_active_weapon()->begin_charge_time();
			float charge = begincharge == 0.0f ? 0.0f : interfaces::globals->curtime - begincharge;
			if (charge > 1.0f)
				charge = 1.0f;
			if (begancharge)
			{
				ctx::command->buttons &= ~IN_ATTACK;
				begancharge = false;
			}
			else
			{
				ctx::command->buttons |= IN_ATTACK;
				begancharge = true;
			}
		}
	}
	void aimbot::fix_movement(c_user_cmd* command, vec3_t old_angles, float old_fowardmove, float old_sidemove) {
		float delta = command->viewangles.y - old_angles.y, f1, f2;

		if (old_angles.y < 0.f) {
			f1 = 360.0f + old_angles.y;
		}
		else {
			f1 = old_angles.y;
		}

		if (command->viewangles.y < 0.0f) {
			f2 = 360.0f + command->viewangles.y;
		}
		else {
			f2 = command->viewangles.y;
		}

		if (f2 < f1) {
			delta = abs(f2 - f1);
		}
		else {
			delta = 360.0f - abs(f1 - f2);
		}
		delta = 360.0f - delta;

		command->forward_move = cos(DEG2RAD(delta)) * old_fowardmove + cos(DEG2RAD(delta + 90.f)) * old_sidemove;
		command->side_move = sin(DEG2RAD(delta)) * old_fowardmove + sin(DEG2RAD(delta + 90.f)) * old_sidemove;
	}

	c_base_player* aimbot::get_best_target(c_base_player* local_player, c_user_cmd* command) {
		if (ctx::local_player->get_life_state() != LIFE_ALIVE)
			return nullptr;

		int fov;
		if (!vars::aimbot::auto_fov) {
			if (vars::aimbot::enabled) {
				fov = vars::aimbot::field_of_view;
			}
			else {
				fov = 85;
			}
		}
		float desired_fov = fov, max_distance = 8192.0f, max_melee_distance = 8192.0f;
		vec3_t local_viewangles = local_player->get_eye_position();
		int best_target_index = -1;

		auto convert_idx_to_player = [&](int idx) -> c_base_player* {
			c_base_player* return_entity = reinterpret_cast<c_base_player*>(interfaces::entitylist->get_client_entity(idx));
			if (!return_entity) return nullptr; return return_entity;
		};

		for (int idx = 1; idx < interfaces::entitylist->get_highest_entity_index(); idx++) {
			c_base_player* entity = reinterpret_cast<c_base_player*>(interfaces::entitylist->get_client_entity(idx));
			if (!entity || entity == local_player || !local_player) continue;
			if (entity->is_dormant() || entity->is_dead()) continue;

			if (entity->get_client_class()->class_id == CObjectDispenser ||
				entity->get_client_class()->class_id == CObjectSentrygun ||
				entity->get_client_class()->class_id == CObjectTeleporter ||
				entity->get_client_class()->class_id == CTFPlayer) {
				int mp_friendlyfire = interfaces::convar->find_variable("mp_friendlyfire")->get_int();

				c_base_weapon* local_weapon = local_player->get_active_weapon();
				if (!local_weapon) continue;
				auto local_class = local_player->get_class_num(), weapon_slot = local_weapon->get_slot();
				if (local_class == TF2_Medic) {
					if (weapon_slot == 1) { //medigun only should aim at teammates lol
						if (entity->get_team_num() != local_player->get_team_num()) {
							continue;
						}
					}
					else if (local_weapon->get_item_definition_index() == WPN_Crossbow || local_weapon->get_item_definition_index() == WPN_FestiveCrossbow) {
						if (entity->get_team_num() == local_player->get_team_num() && mp_friendlyfire == 0) {
							if (entity->get_health() == entity->get_max_health() || entity->get_condnew() & TFCond_Overhealed
								|| entity->get_condnew() & TFCond_Ubercharged) {
								continue;
							}
						}
					}

				}
				else {
					if (mp_friendlyfire == 0 && entity->get_team_num() == local_player->get_team_num()) {
						continue;
					}
				}

				auto size_of_position_adjustment = position_adjustment::ticks[entity->get_index()].size() - 1;
				int hitbox = get_best_hitbox(local_player, entity, false);
				if (hitbox == -1) { continue; }

				vec3_t aim_point = utilities::get_multipoint_position(entity, hitbox, vars::aimbot::pointscale);

				if (entity->get_client_class()->class_id == CObjectDispenser ||
					entity->get_client_class()->class_id == CObjectSentrygun ||
					entity->get_client_class()->class_id == CObjectTeleporter) {
					if ((!vars::aimbot::aimbot_sentry && entity->get_client_class()->class_id == CObjectSentrygun) ||
						(!vars::aimbot::aimbot_dispenser && entity->get_client_class()->class_id == CObjectDispenser) ||
						(!vars::aimbot::aimbot_teleporters && entity->get_client_class()->class_id == CObjectTeleporter)) {
						continue;
					}

					aim_point = entity->get_world_space_center();

					if (!utilities::is_entity_visible(local_player, entity, local_player->get_eye_position(), aim_point)) {
						continue;
					}
				}
				else if (entity->get_client_class()->class_id == 246) {
					if (!utilities::is_entity_visible(local_player, entity, local_player->get_eye_position(), aim_point)) {
						if (position_adjustment::ticks[entity->get_index()].empty()) { continue; }
						if (utilities::is_entity_visible(local_player, entity, local_player->get_eye_position(), position_adjustment::ticks[entity->get_index()].at(size_of_position_adjustment).head_position) && !utilities::is_projectile(ctx::local_player, local_weapon)) {
							aim_point = position_adjustment::ticks[entity->get_index()].at(size_of_position_adjustment).head_position;
						}
						else {
							continue;
						}
					}

					if (utilities::has_condition(entity, 32) || utilities::has_condition(entity, 16384)) {
						continue;
					}

					if (vars::aimbot::ignore_cloaked && (entity->get_condnew() & TFCond_Cloaked || entity->get_condnew() & TFCond_DeadRingered))
						continue;

					if (vars::aimbot::ignore_disguised && entity->get_condnew() & TFCond_Disguised)
						continue;

				}

				auto get_distance = [&](vec3_t to, vec3_t from) -> float {
					vec3_t delta = to - from;
					float distance = ::sqrt(delta.length());
					if (distance < 1.0f) {
						distance = 1.0f;
					} return distance;
				};

				float new_best_distance = safe_to_aim(local_player, local_weapon);
				float real_distance = get_distance(aim_point, local_player->get_eye_position());

				vec3_t angles, eye_angles, length = aim_point - local_viewangles;
				utilities::vector_angle(aim_point, angles);
				clamp_angle(angles);
				interfaces::engineclient->get_view_angles(eye_angles);

				float distance_to_target = utilities::get_field_of_view(eye_angles, local_viewangles, aim_point);
				if (distance_to_target > desired_fov) {
					continue;
				}
				if (vars::aimbot::prefer_medic && entity->get_class_num() == TF2_Medic) {
					return convert_idx_to_player(best_target_index);
				}
				else {
					if (distance_to_target < max_distance && real_distance < new_best_distance) {
						max_distance = distance_to_target;
						new_best_distance = real_distance;
						best_target_index = idx;
					}
				}

			}
		} 
		return convert_idx_to_player(best_target_index);
	}

	c_base_player* aimbot::get_closest_enemy(c_base_player* local_player, c_user_cmd* command, int fov) {
		if (ctx::local_player->get_life_state() != LIFE_ALIVE)
			return nullptr;

		auto clamp_angle = [&](vec3_t& vector) {
			while (vector[0] > 89) { vector[0] -= 180; }
			while (vector[0] < -89) { vector[0] += 180; }
			while (vector[1] > 180) { vector[1] -= 360; }
			while (vector[1] < -180) { vector[1] += 360; }
			while (vector[2] != 0) { vector[2] = 0; }
		};

		float desired_fov = fov, max_distance = 8192.0f, max_melee_distance = 8192.0f;
		vec3_t local_viewangles = local_player->get_eye_position();
		int best_target_index = -1;

		auto convert_idx_to_player = [&](int idx) -> c_base_player* {
			c_base_player* return_entity = reinterpret_cast<c_base_player*>(interfaces::entitylist->get_client_entity(idx));
			if (!return_entity) return nullptr; return return_entity;
		};

		for (int idx = 1; idx < interfaces::entitylist->get_highest_entity_index(); idx++) {
			c_base_player* entity = reinterpret_cast<c_base_player*>(interfaces::entitylist->get_client_entity(idx));
			if (!entity || entity == local_player || !local_player) continue;
			if (entity->is_dormant() || entity->is_dead()) continue;

			if (entity->get_client_class()->class_id == CObjectDispenser ||
				entity->get_client_class()->class_id == CObjectSentrygun ||
				entity->get_client_class()->class_id == CObjectTeleporter ||
				entity->get_client_class()->class_id == CTFPlayer) {
				int mp_friendlyfire = interfaces::convar->find_variable("mp_friendlyfire")->get_int();

				c_base_weapon* local_weapon = local_player->get_active_weapon();
				if (!local_weapon) continue;
				auto local_class = local_player->get_class_num(), weapon_slot = local_weapon->get_slot();
				if (local_class == TF2_Medic) {
					if (weapon_slot == 1) { //medigun only should aim at teammates lol
						if (entity->get_team_num() != local_player->get_team_num()) {
							continue;
						}
					}
					else if (local_weapon->get_item_definition_index() == WPN_Crossbow || local_weapon->get_item_definition_index() == WPN_FestiveCrossbow) {
						if (entity->get_team_num() == local_player->get_team_num() && mp_friendlyfire == 0) {
							if (entity->get_health() == entity->get_max_health() || entity->get_condnew() & TFCond_Overhealed
								|| entity->get_condnew() & TFCond_Ubercharged) {
								continue;
							}
						}
					}

				}
				else {
					if (mp_friendlyfire == 0 && entity->get_team_num() == local_player->get_team_num()) {
						continue;
					}
				}

				auto size_of_position_adjustment = position_adjustment::ticks[entity->get_index()].size() - 1;
				int hitbox = get_best_hitbox(local_player, entity, false);
				if (hitbox == -1) { continue; }

				vec3_t aim_point = utilities::get_hitbox_position(entity, hitbox);

				if (entity->get_client_class()->class_id == CObjectDispenser ||
					entity->get_client_class()->class_id == CObjectSentrygun ||
					entity->get_client_class()->class_id == CObjectTeleporter) {
					if ((!vars::aimbot::aimbot_sentry && entity->get_client_class()->class_id == CObjectSentrygun) ||
						(!vars::aimbot::aimbot_dispenser && entity->get_client_class()->class_id == CObjectDispenser) ||
						(!vars::aimbot::aimbot_teleporters && entity->get_client_class()->class_id == CObjectTeleporter)) {
						continue;
					}

					aim_point = entity->get_world_space_center();

					if (!utilities::is_entity_visible(local_player, entity, local_player->get_eye_position(), aim_point)) {
						continue;
					}
				}
				else if (entity->get_client_class()->class_id == 246) {
					if (!utilities::is_entity_visible(local_player, entity, local_player->get_eye_position(), aim_point)) {
						if (position_adjustment::ticks[entity->get_index()].empty()) { continue; }
						if (utilities::is_entity_visible(local_player, entity, local_player->get_eye_position(), position_adjustment::ticks[entity->get_index()].at(size_of_position_adjustment).head_position) && !utilities::is_projectile(ctx::local_player, local_weapon)) {
							aim_point = position_adjustment::ticks[entity->get_index()].at(size_of_position_adjustment).head_position;
						}
						else {
							continue;
						}
					}

					if (utilities::has_condition(entity, 32) || utilities::has_condition(entity, 16384)) {
						continue;
					}

					if (vars::aimbot::ignore_cloaked && (entity->get_condnew() & TFCond_Cloaked || entity->get_condnew() & TFCond_DeadRingered))
						continue;

					if (vars::aimbot::ignore_disguised && entity->get_condnew() & TFCond_Disguised)
						continue;

				}

				auto get_distance = [&](vec3_t to, vec3_t from) -> float {
					vec3_t delta = to - from;
					float distance = ::sqrt(delta.length());
					if (distance < 1.0f) {
						distance = 1.0f;
					} return distance;
				};

				float new_best_distance = 9999999.f;
				float real_distance = get_distance(aim_point, local_player->get_eye_position());

				vec3_t angles, eye_angles, length = aim_point - local_viewangles;
				utilities::vector_angle(aim_point, angles);
				clamp_angle(angles);
				interfaces::engineclient->get_view_angles(eye_angles);

				float distance_to_target = utilities::get_field_of_view(eye_angles, local_viewangles, aim_point);
				if (distance_to_target > desired_fov) {
					continue;
				}
				if (vars::aimbot::prefer_medic && entity->get_class_num() == TF2_Medic) {
					return convert_idx_to_player(best_target_index);
				}
				else {
					if (distance_to_target < max_distance && real_distance < new_best_distance) {
						max_distance = distance_to_target;
						new_best_distance = real_distance;
						best_target_index = idx;
					}
				}

			}
		}
		return convert_idx_to_player(best_target_index);
	}


	int aimbot::get_best_hitbox(c_base_player* local_player, c_base_player* entity, bool is_a_building) {
		if (!local_player || !entity) return -1;
		if (local_player->get_life_state() != LIFE_ALIVE)
			return -1;

		auto local_weapon = local_player->get_active_weapon();
		if (!local_weapon) return -1;

		auto is_headshot_weapon = [&](c_base_player* local_player, c_base_weapon* local_weapon) {
			if (local_weapon->get_slot() == 0 && local_player->get_class_num() == TF2_Sniper)
				if (local_weapon->get_item_definition_index() != WPN_Huntsman && local_weapon->get_item_definition_index() != WPN_CompoundBow)
					return true;

			if (local_weapon->get_item_definition_index() == WPN_Ambassador || local_weapon->get_item_definition_index() == WPN_FestiveAmbassador)
				return true;

			return false;
		};

		int best_hitbox = 0;
		switch (vars::aimbot::main_hitbox) {
			case 0: best_hitbox = 0; break;
			case 1: best_hitbox = 1; break;
			case 2:
			{
				auto local_class = local_player->get_class_num();
				if (is_headshot_weapon(local_player, local_weapon))
					best_hitbox = 0;
				else if (local_class == TF2_Sniper && !is_headshot_weapon(local_player, local_weapon))
					best_hitbox = 1;
				else if (local_class == TF2_Scout)
					best_hitbox = 4;
				else if (local_class == TF2_Heavy)
					best_hitbox = 3;
				else best_hitbox = 1;

			} break;
			case 3: {
				for (best_hitbox = 0; best_hitbox < 17; best_hitbox++) {
					if (is_headshot_weapon(local_player, local_weapon) && utilities::is_entity_visible(local_player, entity, local_player->get_eye_position(), utilities::get_multipoint_position(entity, 0, vars::aimbot::pointscale))) {
						best_hitbox = 0;
						return best_hitbox;
					} else {
						return best_hitbox;
					}
				}
			} break;
			case 4: {
				for (best_hitbox = 1; best_hitbox < 17; best_hitbox++) {
					return best_hitbox;
				}
			} break;
		}

		/*if (!utilities::is_entity_visible(local_player, entity, local_player->get_eye_position(), utilities::get_hitbox_position(entity, best_hitbox))) {
			return -1;
		}*/

		if (utilities::get_multipoint_position(entity, best_hitbox, vars::aimbot::pointscale).is_zero()) {
			return -1;
		}

		return best_hitbox;
	}

	vec3_t aimbot::estimate_abs_velocity(c_base_player* entity) {
		typedef void(__thiscall * estimate_abs_velocity_fn)(c_base_player*, vec3_t &);
		static uintptr_t function = utilities::find_signature(xorstr("client.dll"), xorstr("E8 ? ? ? ? F3 0F 10 4D ? 8D 85 ? ? ? ? F3 0F 10 45 ? F3 0F 59 C9 56 F3 0F 59 C0 F3 0F 58 C8 0F 2F 0D ? ? ? ? 76 07")) + 0x1;
		static uintptr_t estimate = ((*(PDWORD)(function)) + function + 4);

		estimate_abs_velocity_fn vel = (estimate_abs_velocity_fn)estimate;
		vec3_t v; vel(entity, v); return v;
	}

	void aimbot::projectile_prediction(c_base_player* local_player, c_base_player* entity, c_base_weapon* local_weapon, vec3_t& vec_hitbox) {
		if (local_player->get_life_state() != LIFE_ALIVE) return;
		auto item_index = local_weapon->get_item_definition_index();
		auto get_speed = [&local_player, &local_weapon, &entity, &item_index]() -> float {
			auto weapon_speed = 0.0f;
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
				//weapon_speed = 1200.0f; break;
				weapon_speed = 1065.0f; break; //lets test this...
			case WPN_LochNLoad:
				weapon_speed = 1500.0f; break;
			case WPN_LoooseCannon:
				weapon_speed = 1440.0f; break;
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
		};
		auto get_gravity = [&local_player, &local_weapon, &entity, &item_index]() -> float {
			auto weapon_gravity = 0.0f;
			switch (item_index) {
			case WPN_RescueRanger:
			case WPN_Crossbow:
			case WPN_FestiveCrossbow:
			case WPN_Flaregun:
			case WPN_FestiveFlaregun:
			case WPN_ManMelter:
			case WPN_Detonator:
				weapon_gravity = 0.2f; break;
			case WPN_GrenadeLauncher:
			case WPN_NewGrenadeLauncher:
			case WPN_FestiveGrenadeLauncher:
			case WPN_LoooseCannon:
			case WPN_LochNLoad:
			case WPN_IronBomber:
				weapon_gravity = 0.82f; break;
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
			default: weapon_gravity = 0.0f; break;
			}
			return weapon_gravity;
		};
		auto is_projectile_weapon = [&local_player, &local_weapon, &entity, &item_index]() -> bool {
			auto local_class = local_player->get_class_num(), weapon_slot = local_weapon->get_slot();
			if (local_player->get_life_state() != LIFE_ALIVE) return false;
			if (local_class == TF2_Demoman || local_class == TF2_Soldier || local_class == TF2_Medic || local_class == TF2_Pyro /*technically...*/) {
				if (weapon_slot == 0) {
					return true;
				}
			}

			if (local_class == TF2_Soldier) {
				if (item_index == WPN_RighteousBison)
					return true;
			}

			if (local_class == TF2_Engineer) {
				if (weapon_slot == 0) {
					if (item_index == WPN_RescueRanger || item_index == WPN_Pomson) {
						return true;
					}
				}
			}
			if (local_class == TF2_Sniper) {
				if (weapon_slot == 0) {
					if (item_index == WPN_Huntsman || item_index == WPN_CompoundBow || item_index == WPN_FestiveHuntsman) {
						return true;
					}
				}
				if (weapon_slot == 1) {
					if (item_index == WPN_Jarate || item_index == WPN_FestiveJarate) {
						return true;
					}
				}
			}

			if (local_class == TF2_Pyro) {
				if (item_index == WPN_CowMangler || item_index == WPN_FestiveFlaregun || item_index == WPN_Flaregun || item_index == WPN_ManMelter || item_index == WPN_Detonator) {
					return true;
				}
			}

			if (local_class == TF2_Scout) {
				if (item_index == WPN_Milk || item_index == WPN_MutatedMilk) {
					return true;
				}
			}

			if (local_class == TF2_Demoman) {
				if (item_index == WPN_StickyLauncher ||
					item_index == WPN_NewStickyLauncher ||
					item_index == WPN_BotStickyD ||
					item_index == WPN_BotStickyB ||
					item_index == WPN_BotStickyC ||
					item_index == WPN_BotStickyEG ||
					item_index == WPN_BotStickyES ||
					item_index == WPN_BotStickyG ||
					item_index == WPN_BotStickyR ||
					item_index == WPN_BotStickyS ||
					item_index == WPN_StickyJumper) {
					return true;
				}
			}

			return false;
		};
		auto distance_to_ground = [&entity]() -> float {
			if (entity->get_flags() & FL_ONGROUND) return 0.0f;
			auto distance_to_ground = [&entity](vec3_t origin) -> float
			{
				trace_t ground_trace; ray_t ray;
				trace_filter filter; filter.skip = entity;
				vec3_t endpos = origin;

				endpos.z -= 8192;
				ray.init(origin, endpos);
				interfaces::enginetrace->trace_ray(ray, MASK_PLAYERSOLID, &filter, &ground_trace);
				return 8192.0f * ground_trace.fraction;
			};

			vec3_t origin = entity->get_abs_origin();
			float v1 = distance_to_ground(origin + vec3_t(10.0f, 10.0f, 0.0f));
			float v2 = distance_to_ground(origin + vec3_t(-10.0f, 10.0f, 0.0f));
			float v3 = distance_to_ground(origin + vec3_t(10.0f, -10.0f, 0.0f));
			float v4 = distance_to_ground(origin + vec3_t(-10.0f, -10.0f, 0.0f));
			return min(v1, min(v2, min(v3, v4)));
		};
		if (is_projectile_weapon()) {
			bool on_ground = entity->get_flags() & FL_ONGROUND;
			if (local_player->get_class_num() == TF2_Medic || local_player->get_class_num() == TF2_Engineer || local_player->get_class_num() == TF2_Pyro)
				vec_hitbox = utilities::get_hitbox_position(entity, 2);
			else if (local_player->get_class_num() == TF2_Sniper && local_weapon->get_slot() == 0) {
				vec_hitbox = utilities::get_hitbox_position(entity, 0);
				vec_hitbox[2] += 4.f;
			}
			else if (local_player->get_class_num() == TF2_Soldier) {
				if (local_weapon->get_slot() == 0) {
					vec_hitbox = entity->get_abs_origin();
					vec_hitbox[2] += 8.0f; //gamer girl toeaim
				}
				else if (local_weapon->get_slot() == 1) {
					vec_hitbox = entity->get_world_space_center();
				}
			}
			else if (local_player->get_class_num() == TF2_Demoman || (local_player->get_class_num() == TF2_Sniper && local_weapon->get_slot() == 1) || local_player->get_class_num() == TF2_Scout) {
				//vec_hitbox = entity->get_abs_origin();

				vec_hitbox = utilities::get_hitbox_position(entity, 1); //should be pelvis..
				if (on_ground) {
					vec_hitbox[2] -= 55.0f; //fuck it lets get wild
				}
				else {
					vec_hitbox[2] -= 75.0f; //lol.. i mean it works doesn't it
				}
			}
			auto hitbox_pred = [&local_player, &entity, &on_ground](vec3_t hitbox, float speed, float gravity, float distance_to_ground) -> vec3_t {
				vec3_t result = hitbox, bestpos = result;
				
				float server_gravity = interfaces::convar->find_variable(xorstr("sv_gravity"))->get_float() / 2;
				auto vec_velocity = aimbot::estimate_abs_velocity(entity);
				auto medianTime = (local_player->get_eye_position().dist_to(result) / speed), range = 1.5f,
					currenttime = medianTime - range;

				if (currenttime <= 0.0f) currenttime = 0.01f;

				auto besttime = currenttime, mindelta = 65536.0f; auto maxsteps = 300;
				for (int steps = 0; steps < maxsteps; steps++, currenttime += ((float)(2 * range) / (float)maxsteps)) {
					vec3_t curpos = result; curpos += vec_velocity * currenttime;

					if (distance_to_ground > 0.0f) {
						curpos.z -= currenttime * currenttime * server_gravity * (entity->get_condnew() & TFCondEx2_Parachute ? 0.448f : 1.0f);
						if (curpos.z < result.z - distance_to_ground) {
							curpos.z = result.z - distance_to_ground;
						}
					}

					auto rockettime = (local_player->get_eye_position().dist_to(curpos) / speed);
					if (fabs(rockettime - currenttime) < mindelta) {
						besttime = currenttime; bestpos = curpos;
						mindelta = fabs(rockettime - currenttime);
					}
				}
				bestpos.z += (server_gravity * besttime * besttime * gravity);
				return bestpos;
			};

			vec_hitbox = hitbox_pred(vec_hitbox, get_speed(), get_gravity(), distance_to_ground());
		}
	}

	void aimbot::do_autorev() {
		if (!vars::aimbot::enabled || !utilities::is_variable_key_pressed(vars::aimbot::aimbot_key)) {
			return;
		}
		if (ctx::local_player->get_life_state() != LIFE_ALIVE) return;

		if (ctx::local_player->get_class_num() == TF2_Heavy && ctx::local_player->get_active_weapon()->get_slot() == 0 && vars::aimbot::heavy_autorev) {
			ctx::command->buttons |= IN_ATTACK2;
		}
	}
}	