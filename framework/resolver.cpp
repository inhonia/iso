#include "main.h"

float distance(vec3_t a, vec3_t b)
{
	return sqrt(pow(a.x - b.x, 2) + pow(a.y - b.y, 2) + pow(a.z - b.z, 2));
}

float edge_distance(float edge_ray_yaw, c_base_player *entity) //DF pasta until do_anti_aim....
{
	trace_t trace;
	ray_t ray;
	vec3_t forward;
	trace_filter filter;
	filter.skip = entity;
	float sp, sy, cp, cy;
	sy = sinf(DEG2RAD(edge_ray_yaw)); // yaw
	cy = cosf(DEG2RAD(edge_ray_yaw));
	sp = sinf(DEG2RAD(0)); // pitch
	cp = cosf(DEG2RAD(0));
	forward.x = cp * cy;
	forward.y = cp * sy;
	forward.z = -sp;
	forward = forward * 300.0f + entity->get_eye_position();
	ray.init(entity->get_eye_position(), forward);
	interfaces::enginetrace->trace_ray(ray, MASK_PLAYERSOLID, &filter, &trace);
	float edgeDistance = (sqrt(pow(trace.startpos.x - trace.endpos.x, 2) + pow(trace.startpos.y - trace.endpos.y, 2)));
	return edgeDistance;
}

float f_edge_yaw = 0;
float edge_to_edge_on = 0;

bool find_edge(float edge_original_yaw, c_base_player* entity)
{
	float edge_left_dist = edge_distance(edge_original_yaw - 21, entity);
	edge_left_dist = edge_left_dist + edge_distance(edge_original_yaw - 27, entity); //are these numbers just fucking arbitrary? monkey code.

	float edge_right_dist = edge_distance(edge_original_yaw + 21, entity);
	edge_right_dist = edge_right_dist + edge_distance(edge_original_yaw + 27, entity);

	if (edge_left_dist >= 260) edge_left_dist = 999999999;
	if (edge_right_dist >= 260) edge_right_dist = 999999999;

	if (edge_left_dist == edge_right_dist) return false;

	if (edge_right_dist < edge_left_dist)
	{
		if (entity->get_eye_position().x > 0)
			edge_to_edge_on = 1;
		else
			edge_to_edge_on = 2;

		return true;
	}
	else
	{
		if (entity->get_eye_position().x > 0)
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

auto is_headshot_wep = [&](c_base_player* local_player, c_base_weapon* local_weapon) {
	if (local_weapon->get_slot() == 0 && local_player->get_class_num() == TF2_Sniper)
		if (local_weapon->get_item_definition_index() != WPN_Huntsman && local_weapon->get_item_definition_index() != WPN_CompoundBow)
			return true;

	if (local_weapon->get_item_definition_index() == WPN_Ambassador || local_weapon->get_item_definition_index() == WPN_FestiveAmbassador)
		return true;

	return false;
};

bool analyze_player_pitch(float pitch, bool &up) //thanks again vlad, basic system for detecting fake pitches...
{
	if (pitch < -utilities::anglemod(89.3f))
	{
		up = true;
		return true;
	}
	else if (pitch > utilities::anglemod(89.3f))
	{
		up = false;
		return true;
	}
	return false;
}


namespace resolver {
	void do_resolve();

	vec3_t forced_angs[65];
	bool is_faking_pitch[65];
	bool is_pitch_up[65];
	bool are_angs_saved[65];
	vec3_t old_eye_angs[65];
	vec3_t saved_eye_angs[65];
	int shots_fired[65];
	int shots_missed[65];
	int shots_hit[65];
	
}

float resolver::get_away_angle(c_base_player* entity)
{
	ang_t away;
	utilities::vectorangles3((ctx::local_player->get_world_space_center() - entity->get_eye_position()), away);
	return away.y;
}

void resolver::do_resolve() {

	if (ctx::local_player->is_dead())
		return;

	for (int i = 0; i <= interfaces::engineclient->get_max_clients(); i++) {
		c_base_player* entity = reinterpret_cast<c_base_player*>(interfaces::entitylist->get_client_entity(i));
		if (!entity)
			continue;

		auto index = entity->get_index();

		if (entity->is_dead())
		{
			shots_missed[index] = 0;
			shots_fired[index] = 0;
			shots_hit[index] = 0;
		}

		if (entity->is_dead() || entity->is_dormant()
			|| entity->get_team_num() == ctx::local_player->get_team_num()
			|| entity == ctx::local_player) {
			continue;
		}

		//only run below code if not debugging...
#if NDEBUG
		if (entity->is_player_bot())
			continue;
#endif

		if (vars::hvh::pitch_resolver || vars::hvh::yaw_resolver) {
			if (shots_missed[index] > 7) {
				shots_missed[index] = 0;
				shots_fired[index] = 0;
				shots_hit[index] = 0;
			}

			shots_missed[index] = shots_fired[index] - shots_hit[index];
		}

		auto normalize_yaw = [&](float yaw) {
			if (yaw > 180) {
				yaw -= (round(yaw / 360) * 360.f);
			}
			else if (yaw < -180) {
				yaw += (round(yaw / 360) * -360.f);
			} return yaw;
		};


		if (aimbot::aimbot_target) {
			DWORD eye_angles = reinterpret_cast<DWORD>(aimbot::aimbot_target) + CNetVars::get().get_offset(xorstr("DT_TFPlayer"), xorstr("tfnonlocaldata"), xorstr("m_angEyeAngles[0]"));
			vec3_t &aimbot_entity = *reinterpret_cast<vec3_t*>(eye_angles);
			if (!is_headshot_wep(ctx::local_player, ctx::local_player->get_active_weapon())) //obviously, don't.
				return;

			if (vars::hvh::pitch_resolver) {

				//is_faking_pitch[index] = analyze_player_pitch(aimbot_entity.x, is_pitch_up[index]);

				switch (shots_missed[index] % 4) {
				case 0: aimbot_entity.x = are_angs_saved[index] ? saved_eye_angs[index].x : aimbot_entity.x; break;

				case 1: {
					aimbot_entity.x = 90;
				} break;

				case 2: {
					aimbot_entity.x = -90;
				} break;

				case 3: {
					aimbot_entity.x = 0;
				} break;
					
				default: break;
				}

			}

			if (vars::hvh::yaw_resolver) {

				int shotsfiredold = ctx::shots_missed;
				float previous = 0.0f;

				if (GetAsyncKeyState(VK_LMENU)) { //le manual resolver...
					vec3_t screen;
					vec3_t currentview;
					interfaces::engineclient->get_view_angles(currentview);
					if (visuals::world_to_screen(currentview, screen)) {
						float angle = normalize_yaw(utilities::get_angle_from_point(screen));
						aimbot_entity.y = angle;
					}
				}

				else {
					float away = get_away_angle(aimbot::aimbot_target);
					switch (shots_missed[index] % 9) {
					case 0: aimbot_entity.y = are_angs_saved[index] ? saved_eye_angs[index].y : aimbot_entity.y; break;
					case 1: {
						if (find_edge(away, entity))
							aimbot_entity.y = use_edge(away);
					}
					case 2: aimbot_entity.y = normalize_yaw(away + 180.f); break;
					case 3: aimbot_entity.y = normalize_yaw(away + 90.f);  break;
					case 4: aimbot_entity.y = normalize_yaw(away - 90.f);  break;
					case 5: aimbot_entity.y = normalize_yaw(away + 45.f);  break;
					case 6: aimbot_entity.y = normalize_yaw(away - 45.f);  break;
					case 7: aimbot_entity.y = normalize_yaw(away + 135.f); break;
					case 8: aimbot_entity.y = normalize_yaw(away - 135.f); break;
					default: break;
					}

				}

			}
			if (vars::aimbot::lagfix) {
				/* do things */
			}
		}
	}
}