#include "../../main.h"

float player_alpha[64];
float building_alpha[64];
//int spectator_count = 0;
inline float dist(vec3_t myPos, vec3_t enPos) { return sqrt((myPos.x - enPos.x) * (myPos.x - enPos.x) + (myPos.y - enPos.y) * (myPos.y - enPos.y) + (myPos.z - enPos.z) * (myPos.z - enPos.z)); }
colour_t rainbow() {
	static float rainbow_value = 0.0f;
	rainbow_value += .001;

	if (rainbow_value > 1.0f) {
		rainbow_value = 0.0f;
	}

	return colour_t::from_hsb(rainbow_value, 1.0f, 1.0f);
}

colour_t fire() {
	static float fire_value = 0.0f;
	fire_value += fmodf(interfaces::globals->curtime / 12, 10);
	return colour_t(255, fire_value, 0);
}

RECT get_text_size_t(DWORD font, const char* text, ...) {
	size_t origsize = strlen(text) + 1;
	const size_t newsize = 100;
	size_t convertedChars = 0;
	wchar_t wcstring[newsize];
	mbstowcs_s(&convertedChars, wcstring, origsize, text, _TRUNCATE);

	RECT rect; int x, y;
	interfaces::surface->get_text_size(font, wcstring, x, y);
	rect.left = x; rect.bottom = y;
	rect.right = x;
	return rect;
}

int get_reserve_ammo() {
	int ammotype;
	if (ctx::local_player->get_active_weapon()->get_slot() == 0)
		ammotype = ctx::local_player->get_active_weapon()->get_prim_ammo_type();

	if (ctx::local_player->get_active_weapon()->get_slot() != 0)
		ammotype = ctx::local_player->get_active_weapon()->get_sec_ammo_type();

	if (ammotype == -1)
		return -1;

	c_base_player* owner = (c_base_player*)interfaces::entitylist->get_client_entity_from_handle(ctx::local_player->get_active_weapon()->get_weapon_owner());

	if (!owner)
		return -1;

	DYNVAR_OFF_RETURN(int, owner, (4 * ammotype), "DT_BasePlayer", "localdata", "m_iAmmo");
}

colour_t coloursarray[10] = { colour_t(0, 195, 255), colour_t(250, 0, 255), colour_t(174, 255, 0), colour_t(141, 83, 173), colour_t(209, 42, 224), colour_t(255, 191, 0), colour_t(255, 0, 97), colour_t(0, 55, 255), colour_t(246, 255, 0), colour_t(0, 195, 255) };

namespace visuals {
	void hitbox_esp(matrix3x4_t* matrix, int bone, vec3_t min, vec3_t max, colour_t colour)
	{
		vec3_t vec_pointlist[] = {
			vec3_t(min.x, min.y, min.z),
			vec3_t(min.x, max.y, min.z),
			vec3_t(max.x, max.y, min.z),
			vec3_t(max.x, min.y, min.z),

			vec3_t(max.x, max.y, max.z),
			vec3_t(min.x, max.y, max.z),
			vec3_t(min.x, min.y, max.z),
			vec3_t(max.x, min.y, max.z)
		}, vec_transformed[8];

		for (int i = 0; i < 8; i++)
		{
			//vec_pointlist[i].x *= 0.9;
			//vec_pointlist[i].y *= 0.9;
			//vec_pointlist[i].z *= 0.9;
			utilities::vector_transform(vec_pointlist[i], matrix[bone], vec_transformed[i]);
		}
		draw::draw_3d_box(vec_transformed, colour);
	}

	void drawline(float angle, float range, int x, int y, int len, colour_t col) {

		auto normalize_yaw = [&](float yaw) {
			if (yaw > 180) {
				yaw -= (round(yaw / 360) * 360.f);
			}
			else if (yaw < -180) {
				yaw += (round(yaw / 360) * -360.f);
			} return yaw;
		};

		float tempint = angle;
		tempint += 45.f;

		float yawang = tempint * (PI / 180.0f);

		float sinyaw = sin(yawang);
		float cosyaw = cos(yawang);

		float x2 = range * (-cosyaw) + range * sinyaw;
		float y2 = range * (-cosyaw) - range * sinyaw;

		int x_cord = x + int(x2 / range * (len));
		int y_cord = y + int(y2 / range * (len));
		normalize_yaw(angle);
		draw::draw_line(x, y, x_cord, y_cord, col);
		draw::draw_string_centered(x_cord, y_cord, col, font::smallfont, xorstr("%.1f"), angle);
	}

	bool is_on_screen(vec3_t origin, vec3_t &screen)
	{
		if (!world_to_screen(origin, screen))
			return false;
		int scr_w = ctx::screen_width,
			scr_h = ctx::screen_height;

		bool x_check = scr_w > screen.x > 0, y_check = scr_h > screen.y > 0;
		return x_check && y_check;
	}

	void visuals::render_fake_hitboxes()
	{
		colour_t custom_ui_colour = vars::cheat::custom_ui_colour ? colour_t::from_hsb(vars::colour::custom_ui_colour / 100, 1.f, 1.f, 255) : colour_t(218, 23, 91);

		matrix3x4_t bones[128];
		studiohdr_t* hdr = interfaces::model_info->get_studio_model(ctx::local_player->get_model());
		mstudiohitboxset_t* set = hdr->get_hitbox_set(0);
		if (ctx::local_player->setup_bones(bones, 128, 256, interfaces::globals->curtime)) {
			for (int i = 0; i < set->numhitboxes; i++)
			{
				mstudiobbox_t* hitbox = set->get_new_hitbox(i);

				if (!hitbox)
					continue;

				int bone = hitbox->bone;
				vec3_t v_min = hitbox->bbmin;
				vec3_t v_max = hitbox->bbmax;

				vec3_t center = (v_min + v_max) * 0.5f;
				vec3_t vhitbox;

				utilities::vector_transform(center, bones[hitbox->bone], vhitbox);

				hitbox_esp(bones, bone, v_min, v_max, custom_ui_colour);
			}
		}
	}

	void visuals::render_watermark() {
		colour_t custom_ui_colour = vars::cheat::custom_ui_colour ? colour_t::from_hsb(vars::colour::custom_ui_colour / 100, 1.f, 1.f, 255) : colour_t(218, 23, 91);

		int scr_w = ctx::screen_width,
			scr_h = ctx::screen_height;

		static int secretmark_x, secretmark_y;

		static float fps, last_time;

		static float framerate = 0;

		if ((interfaces::globals->tickcount - last_time) > 80)
			last_time = interfaces::globals->tickcount;

		framerate = 0.9 * framerate + (1.0 - 0.9) * interfaces::globals->absoluteframetime;
		fps = int(1.f / framerate);

		std::stringstream ss;

		auto net_channel = interfaces::engineclient->get_net_channel_info();
		int average_latency;
		if (net_channel)
			average_latency = ctx::local_player ? ((net_channel->get_latency(FLOW_INCOMING) * 1000) + (net_channel->get_latency(FLOW_OUTGOING) * 1000)) : 0;
		else
			average_latency = 0;
		
		std::string latency = std::to_string((int)average_latency);
		
		if (interfaces::engineclient->is_in_game())
			ss << "Isopropyl | alpha v1.1" << " | fps: " << fps << " | latency: " << latency.c_str() << "ms";
		else 
			ss << "Isopropyl | alpha v1.1";

		int x, y, w, h;
		size_t origsize = strlen(ss.str().c_str()) + 1;
		const size_t newsize = 100;
		size_t convertedChars = 0;
		wchar_t wcstring[newsize];
		mbstowcs_s(&convertedChars, wcstring, origsize, ss.str().c_str(), _TRUNCATE);

		interfaces::surface->get_text_size(font::menu_font, wcstring, x, y);

		int textsize = x;

		draw::draw_filled_rect(scr_w - (textsize + 20) - 5, 6, textsize + 20, 20, colour_t(23, 20, 27, 144));
		draw::draw_outlined_rect(scr_w - (textsize + 20) - 5, 6, textsize + 20, 1, custom_ui_colour);
		draw::draw_string(scr_w - (textsize + 10) - 5, 9, colour_t(186, 183, 194), font::menu_font, ss.str().c_str());
	}

	void visuals::render_hud() {
		if (!vars::cheat::hud)
			return;
		if (ctx::local_player->get_life_state() != LIFE_ALIVE)
			return;

		int scr_w = ctx::screen_width,
			scr_h = ctx::screen_height;

		if (!interfaces::engineclient->is_in_game() || !ctx::local_player)
			return;

		int local_clip1, local_clip2, reserve;
		int local_health = ctx::local_player->get_health();
		if (ctx::local_player->get_active_weapon()) {
			local_clip1 = ctx::local_player->get_active_weapon()->get_clip_1();
			local_clip2 = ctx::local_player->get_active_weapon()->get_clip_2();
			reserve = get_reserve_ammo();
		}

		//health
		draw::draw_string(scr_w * 0.30 + 5, scr_h - 386, colour_t(0, 0, 0, 144), font::hud, "%i", local_health);
		draw::draw_string(scr_w * 0.30, scr_h - 390, colour_t(255, 255, 255), font::hud, "%i", local_health);

		//if the weapon has both a 1st clip and a 2nd clip...
		if (local_clip1 >= 0 and reserve >= 0)
		{
			draw::draw_string(scr_w * 0.65 + 5, scr_h - 386, colour_t(0, 0, 0, 144), font::hud, "%i / %i", local_clip1, reserve);
			draw::draw_string(scr_w * 0.65, scr_h - 390, colour_t(255, 255, 255), font::hud, "%i / %i", local_clip1, reserve);
		}
		//if the weapon only has a 1st clip...
		if (local_clip1 >= 0 and reserve < 0)
		{
			draw::draw_string(scr_w * 0.65 + 5, scr_h - 386, colour_t(0, 0, 0, 144), font::hud, "%i", local_clip1);
			draw::draw_string(scr_w * 0.65, scr_h - 390, colour_t(255, 255, 255), font::hud, "%i", local_clip1);
		}
		//if the weapon only has a 2nd clip...
		if (local_clip1 < 0 and reserve >= 0)
		{
			draw::draw_string(scr_w * 0.65 + 5, scr_h - 386, colour_t(0, 0, 0, 144), font::hud, "%i", reserve);
			draw::draw_string(scr_w * 0.65, scr_h - 390, colour_t(255, 255, 255), font::hud, "%i", reserve);
		}

		draw::draw_string(scr_w * 0.30, 300, colour_t(255, 255, 255), font::smallfont, "CAM X: %.1f", ctx::camera_x);
		draw::draw_string(scr_w * 0.30, 307, colour_t(255, 255, 255), font::smallfont, "CAM Y: %.1f", ctx::camera_y);
		draw::draw_string(scr_w * 0.30, 314, colour_t(255, 255, 255), font::smallfont, "CAM OGN[0]: %.1f", ctx::camera_origin[0]);
		draw::draw_string(scr_w * 0.30, 321, colour_t(255, 255, 255), font::smallfont, "CAM OGN[1]: %.1f", ctx::camera_origin[1]);
		draw::draw_string(scr_w * 0.30, 328, colour_t(255, 255, 255), font::smallfont, "CAM OGN[2]: %.1f", ctx::camera_origin[2]);
		draw::draw_string(scr_w * 0.30, 335, colour_t(255, 255, 255), font::smallfont, "CAM ANG[0]: %.1f", ctx::camera_angs[0]);
		draw::draw_string(scr_w * 0.30, 342, colour_t(255, 255, 255), font::smallfont, "CAM ANG[1]: %.1f", ctx::camera_angs[1]);
		draw::draw_string(scr_w * 0.30, 349, colour_t(255, 255, 255), font::smallfont, "CAM ANG[2]: %.1f", ctx::camera_angs[2]);

	}

	void visuals::render_esp() {
		if (!ctx::local_player) {
			return;
		}

		if (vars::visuals::no_scope) {
			if (ctx::is_scoped && !ctx::local_player->is_dead()) {
				draw::draw_line(ctx::screen_width / 2, 0, ctx::screen_width / 2, ctx::screen_height, colour_t(0, 0, 0, 255));
				draw::draw_line(0, ctx::screen_height / 2, ctx::screen_width, ctx::screen_height / 2, colour_t(0, 0, 0, 255));

				float chargeamt = ctx::local_player->get_active_weapon()->get_charge();

				int mathedcharge = (chargeamt / 150) * 100;

				int c_w = ctx::screen_width / 2,
					c_h = ctx::screen_height / 2;

				draw::draw_string(c_w + 7, c_h + 7, colour_t(255, 255, 255), font::smallfont, xorstr("CHARGE: %d%%"), mathedcharge);
			}
		}

		if (vars::visuals::custom_crosshair) {
			colour_t custom_ui_colour = vars::cheat::custom_ui_colour ? colour_t::from_hsb(vars::colour::custom_ui_colour / 100, 1.f, 1.f, 255) : colour_t(218, 23, 91);
			int c_w = ctx::screen_width / 2,
				c_h = ctx::screen_height / 2;

			colour_t white(255, 255, 255);
			colour_t accent = custom_ui_colour;
			colour_t crosshairclr;

			vec3_t aim_angles, forward;
			interfaces::engineclient->get_view_angles(aim_angles);

			utilities::angle_vectors(aim_angles, &forward);
			forward = forward * 9999 + ctx::local_player->get_eye_position();

			ray_t ray; trace_t trace;
			trace_filter filt; filt.skip = ctx::local_player;

			ray.init(ctx::local_player->get_eye_position(), forward);
			interfaces::enginetrace->trace_ray(ray, MASK_SHOT_HULL | CONTENTS_HITBOX, &filt, &trace);
			if (!trace.ent || trace.ent->is_dead() || trace.ent->is_dormant() || trace.ent->get_team_num() == ctx::local_player->get_team_num()
				|| !(trace.ent->get_client_class()->class_id == CObjectDispenser ||
					trace.ent->get_client_class()->class_id == CObjectSentrygun ||
					trace.ent->get_client_class()->class_id == CObjectTeleporter ||
					trace.ent->get_client_class()->class_id == CTFPlayer) ||
				trace.hitgroup < 0) {
				crosshairclr = white;
			}
			else {
				crosshairclr = accent;
			}
			ctx::crosshair_colour = crosshairclr;
			interfaces::surface->draw_set_colour_t(colour_t(0, 0, 0));
			interfaces::surface->draw_filled_rect(c_w - 4, c_h - 2, c_w - 4 + 8, c_h - 2 + 4);
			interfaces::surface->draw_filled_rect(c_w - 2, c_h - 4, c_w - 2 + 4, c_h - 4 + 8);

			interfaces::surface->draw_set_colour_t(crosshairclr);
			interfaces::surface->draw_filled_rect(c_w - 3, c_h - 1, c_w - 3 + 6, c_h - 1 + 2);
			interfaces::surface->draw_filled_rect(c_w - 1, c_h - 3, c_w - 1 + 2, c_h - 3 + 6);


			if (vars::aimbot::smoothing && vars::aimbot::enabled) {
				//smoothing info lol
			}
		}

		if (vars::visuals::aa_visual && vars::hvh::anti_aim_yaw) {
			auto normalize_yaw = [&](float yaw) {
				if (yaw > 180) {
					yaw -= (round(yaw / 360) * 360.f);
				}
				else if (yaw < -180) {
					yaw += (round(yaw / 360) * -360.f);
				} return yaw;
			};

			int x = ctx::screen_width / 5, y = ctx::screen_height * 0.20;
			colour_t custom_ui_colour = vars::cheat::custom_ui_colour ? colour_t::from_hsb(vars::colour::custom_ui_colour / 100, 1.f, 1.f, 255) : colour_t(218, 23, 91);

			int radius = 80;
			float range = 5;
			int linelength = 50;
			vec3_t viewangles, realview, fakeview; 
			interfaces::engineclient->get_view_angles(viewangles);

			realview.y = viewangles.y - ctx::real_angle.y;
			fakeview.y = viewangles.y - ctx::fake_angle.y;
			
			normalize_yaw(realview.y);
			normalize_yaw(fakeview.y);

			drawline(realview.y, range, x, y, linelength, colour_t(255, 255, 255));
			
			drawline(fakeview.y, range, x, y, linelength, custom_ui_colour);
		}

		if (vars::aimbot::drawfov && vars::aimbot::enabled && !ctx::local_player->is_dead()) {
			int w, h;
			colour_t circle_colour_outside = colour_t::from_hsb(vars::colour::aimbot_fov / 100, 1.f, 1.f);
			colour_t circle_outline = colour_t(circle_colour_outside.r(), circle_colour_outside.g(), circle_colour_outside.b(), 70);
			colour_t circle_colour_inside = colour_t(circle_colour_outside.r(), circle_colour_outside.g(), circle_colour_outside.b(), 10);
			interfaces::engineclient->get_screen_size(w, h);

			float mon_fov = (float(w) / float(h) / (4.0f / 3.0f));
			float fov_real = RAD2DEG(2 * atanf(mon_fov * tanf(DEG2RAD(vars::visuals::real_fov / 2))));
			float radius = tan(DEG2RAD(float(vars::aimbot::field_of_view)) / 2) / tan(DEG2RAD(fov_real) / 2) * (w);

			draw::draw_circle_outlined(w / 2, h / 2, radius, circle_outline);
			draw::draw_filled_circle(w / 2, h / 2, radius, circle_colour_inside);
		}

		for (int i = 0; i <= interfaces::engineclient->get_max_clients(); i++) {
			c_base_player* entity = reinterpret_cast<c_base_player*>(interfaces::entitylist->get_client_entity(i));
			if (!entity || entity->is_dead()) {
				continue;
			}
			
			if (!entity->is_dormant()) {
				if (vars::visuals::no_cloak) {
					if (utilities::has_condition(entity, 16)) { //remove cloak
						entity->set_condition(entity->get_condition() & ~16);
					}
				}
			}

			if (entity->is_dormant() && player_alpha[i] > 0) {
				player_alpha[i] -= .2f;
			} else if (!entity->is_dormant() && player_alpha[i] < 255) {
				player_alpha[i] = 255.0f;
			} clamp(player_alpha[i], 0.5f, 255.f);

			if (entity == ctx::local_player && !ctx::in_thirdperson) {
				continue;
			}

			if (utilities::is_developer(entity)) {
				create_player_esp(entity);
			}

			if (!utilities::is_entity_visible(ctx::local_player, entity, ctx::local_player->get_eye_position(), entity->get_world_space_center()) && vars::visuals::visible_only)
				continue;

			create_player_esp(entity);
		}

		for (int i = 1; i < interfaces::entitylist->get_highest_entity_index(); i++) {
			c_base_player* entity = reinterpret_cast<c_base_player*>(interfaces::entitylist->get_client_entity(i));
			if (!entity || entity->is_dead() || entity->is_dormant()) {
				continue;
			}

			if (entity->get_client_class()->class_id == 86 || entity->get_client_class()->class_id == 88 || entity->get_client_class()->class_id == 89) {
				if (entity->get_level() == -1 || entity->is_carried() || entity->is_placing()) {
					continue;
				}

				HANDLE entity_handle = entity->get_builder();
				if (!entity_handle) {
					continue;
				}

				c_base_player* entity_builder = (c_base_player*)interfaces::entitylist->get_client_entity_from_handle(entity_handle);
				if (!entity_builder) {
					continue;
				}

				if (entity->get_team_num() == ctx::local_player->get_team_num() && entity_builder != ctx::local_player && !utilities::is_developer(entity_builder)) {
					if (vars::visuals::building_enemyonly) {
						continue;
					}
				}

				create_building_esp(entity);
			}
		}

		for (int i = 1; i < interfaces::entitylist->get_highest_entity_index(); i++) {
			c_base_player* entity = reinterpret_cast<c_base_player*>(interfaces::entitylist->get_client_entity(i));
			if (!entity || entity->is_dead() || entity->is_dormant()) {
				continue;
			}

			create_item_esp(entity);
		}

		
	}
	void visuals::create_player_esp(c_base_player* entity) {
		player_info_t entity_info;
		if (!interfaces::engineclient->get_player_info(entity->get_index(), &entity_info)) {
			return;
		}

		if (!entity || !ctx::local_player || entity->is_dead()) { //don't store weird dormants
			return;
		}

		const matrix3x4_t& vec_matrix = entity->get_rgfl_coordinate_frame();
		vec3_t vec_minimum = entity->get_collideable_mins(), vec_maximum = entity->get_collideable_maxs();

		vec3_t vec_pointlist[] = {
			vec3_t(vec_minimum.x, vec_minimum.y, vec_minimum.z),
			vec3_t(vec_minimum.x, vec_maximum.y, vec_minimum.z),
			vec3_t(vec_maximum.x, vec_maximum.y, vec_minimum.z),
			vec3_t(vec_maximum.x, vec_minimum.y, vec_minimum.z),

			vec3_t(vec_maximum.x, vec_maximum.y, vec_maximum.z),
			vec3_t(vec_minimum.x, vec_maximum.y, vec_maximum.z),
			vec3_t(vec_minimum.x, vec_minimum.y, vec_maximum.z),
			vec3_t(vec_maximum.x, vec_minimum.y, vec_maximum.z)
		}, vec_transformed[8];

		for (int i = 0; i < 8; i++) {
			for (int j = 0; j < 3; j++) {
				vec_transformed[i][j] = vec_pointlist[i].dot((vec3_t&)vec_matrix[j]) + vec_matrix[j][3];
			}
		}

		vec3_t flb, brt, blb, frt, frb, brb, blt, flt;
		if (!world_to_screen(vec_transformed[3], flb) ||
			!world_to_screen(vec_transformed[0], blb) ||
			!world_to_screen(vec_transformed[2], frb) ||
			!world_to_screen(vec_transformed[6], blt) ||
			!world_to_screen(vec_transformed[5], brt) ||
			!world_to_screen(vec_transformed[4], frt) ||
			!world_to_screen(vec_transformed[1], brb) ||
			!world_to_screen(vec_transformed[7], flt))
			return;

		vec3_t vec_array[] = { flb, brt, blb, frt, frb, brb, blt, flt };

		float	top = flb.y,
				right = flb.x,
				left = flb.x,
				bottom = flb.y;

		for (int i = 0; i < 8; i++) {
			if (left > vec_array[i].x) {
				left = vec_array[i].x;
			}
			if (top < vec_array[i].y) {
				top = vec_array[i].y;
			}
			if (right < vec_array[i].x) {
				right = vec_array[i].x;
			}
			if (bottom > vec_array[i].y) {
				bottom = vec_array[i].y;
			}
		}

		float x = left, y = bottom, w = right - left, h = top - bottom;
		x += ((right - left) / 8), w -= ((right - left) / 8) * 2;

		bool dormancy = entity->is_dormant();
		colour_t esp_colour = dormancy ? colour_t(91, 91, 91, player_alpha[entity->get_index()]) : 
										 colour_t(255, 255, 255, player_alpha[entity->get_index()]);

		bool is_teammate = entity->get_team_num() == ctx::local_player->get_team_num();
		bool is_visible = utilities::is_entity_visible(ctx::local_player, entity, ctx::local_player->get_eye_position(), entity->get_world_space_center());

		if (is_teammate ? vars::visuals::team_bones : vars::visuals::enemy_bones) {
			studiohdr_t* studio_model = interfaces::model_info->get_studio_model(entity->get_model());
			if (!studio_model) return;
			auto size_of_position_adjustment = position_adjustment::ticks[entity->get_index()].size() - 1;
			static matrix3x4_t bone_to_world[128];

			if (entity->setup_bones(bone_to_world, 128, 256, 0)) {
				for (int i = 0; i < studio_model->numbones; i++) {
					mstudiobone_t* bone = studio_model->get_bone(i);
					if (!bone || !(bone->flags & 256) || bone->parent == -1) {
						continue;
					}
					vec3_t bone_position_one, bone_position_two;
					vec3_t bone_to_world_one, bone_to_world_two;
					bone_to_world_two = vec3_t(bone_to_world[bone->parent][0][3], bone_to_world[bone->parent][1][3], bone_to_world[bone->parent][2][3]);
					bone_to_world_one = vec3_t(bone_to_world[i][0][3], bone_to_world[i][1][3], bone_to_world[i][2][3]);

					if (!world_to_screen(bone_to_world_one, bone_position_one)) {
						continue;
					}

					if (!world_to_screen(bone_to_world_two, bone_position_two)) {
						continue;
					}

					draw::draw_line((int)bone_position_one.x, (int)bone_position_one.y, (int)bone_position_two.x, (int)bone_position_two.y, esp_colour);

				}
			}
		}

		if (is_teammate ? vars::visuals::team_hitboxes : vars::visuals::enemy_hitboxes) {
			studiohdr_t* hdr = interfaces::model_info->get_studio_model(entity->get_model());
			mstudiohitboxset_t* set = hdr->get_hitbox_set(0);
			static matrix3x4_t bones[128];
			float hue = 0;
			float invishue = 0;
			if (is_teammate) {
					hue = vars::colour::team_hitbox_vis;
					invishue = vars::colour::team_hitbox_inv;
			}
			else if (!is_teammate) {
					hue = vars::colour::enemy_hitbox_vis;
					invishue = vars::colour::enemy_hitbox_inv;
			}
			colour_t temp; 
			if (entity->setup_bones(bones, 128, 256, interfaces::globals->curtime))
			{
				for (int i = 0; i < set->numhitboxes; i++)
				{
					mstudiobbox_t* hitbox = set->get_new_hitbox(i);

					if (!hitbox)
						continue;

					int bone = hitbox->bone;
					vec3_t v_min = hitbox->bbmin;
					vec3_t v_max = hitbox->bbmax;

					vec3_t center = (v_min + v_max) * 0.5f;
					vec3_t vhitbox;

					utilities::vector_transform(center, bones[hitbox->bone], vhitbox);

					bool visible = utilities::is_entity_visible(ctx::local_player, entity, ctx::local_player->get_eye_position(), vhitbox);

					if (!dormancy)
						temp = colour_t::from_hsb((visible ? hue : invishue) / 100, 1, 1);
					else if (dormancy)
						temp = esp_colour;

					hitbox_esp(bones, bone, v_min, v_max, dormancy ? temp : colour_t(temp.r(), temp.g(), temp.b(), 144));
				}
			}
		}

		if (is_teammate ? vars::visuals::team_box : vars::visuals::enemy_box) {
			colour_t enemy_visible = colour_t::from_hsb(vars::colour::enemy_box_vis / 100, 1.f, 1.f);
			colour_t enemy_invisible = colour_t::from_hsb(vars::colour::enemy_box_inv / 100, 1.f, 1.f);
			colour_t team_visible = colour_t::from_hsb(vars::colour::team_box_vis / 100, 1.f, 1.f);
			colour_t team_invisible = colour_t::from_hsb(vars::colour::team_box_invis / 100, 1.f, 1.f);

			draw::draw_outlined_rect(x - 1, y - 1, w + 2, h + 2, colour_t(0, 0, 0, player_alpha[entity->get_index()]));
			if (is_visible) {
				if (!is_teammate) { draw::draw_outlined_rect(x, y, w, h, dormancy ? colour_t(91, 91, 91, player_alpha[entity->get_index()]) : enemy_visible); }
				else { draw::draw_outlined_rect(x, y, w, h, dormancy ? colour_t(91, 91, 91, player_alpha[entity->get_index()]) : team_visible); }
			}
			else
			{
				if (!is_teammate) { draw::draw_outlined_rect(x, y, w, h, dormancy ? colour_t(91, 91, 91, player_alpha[entity->get_index()]) : enemy_invisible); }
				else { draw::draw_outlined_rect(x, y, w, h, dormancy ? colour_t(91, 91, 91, player_alpha[entity->get_index()]) : team_invisible); }
			}

			draw::draw_outlined_rect(x + 1, y + 1, w - 2, h - 2, colour_t(0, 0, 0, player_alpha[entity->get_index()])); //eeee
		}
		
		if (is_teammate ? vars::visuals::team_health : vars::visuals::enemy_health) {
			auto health_to_colour = [&](c_base_player* entity, int current_health) {
				float percentage = (double)current_health / entity->get_max_health() * 100;
				int32_t health = max(0, min(current_health, 100));
				if (percentage > 100 || entity->get_condnew() & TFCond_Overhealed) {
					return colour_t(192, 192, 255, 255);
				}
				else if (percentage <= 100) {
					colour_t colour = colour_t(
						min((510 * (100 - health)) / 100, 255),
						min((510 * health) / 100, 255),
						0,
						255
					);
					return colour;
				}	return colour_t(255, 255, 255, 255);
			};

			int health = clamp(entity->get_health(), 0, entity->get_max_health());
			int flBoxes = health / 10.f;
			int health_bar = h / entity->get_max_health() * health;
			int health_bar_delta = h - health_bar;
			int green = health * 2.55;
			int red = 255 - green;

			draw::draw_filled_rect	(x - 6, y - 1, 4, h + 2, colour_t(80, 80, 80, player_alpha[entity->get_index()] / 2));
			draw::draw_outlined_rect(x - 6, y - 1, 4, h + 2, colour_t(0, 0, 0, player_alpha[entity->get_index()]));
			draw::draw_filled_rect	(x - 5, y + health_bar_delta, 2, health_bar, dormancy ? colour_t(91, 91, 91, player_alpha[entity->get_index()]) : health_to_colour(entity, health));
			if (health != entity->get_max_health() || entity->get_condnew() & TFCond_Overhealed)
				draw::draw_string_centered (x - 5, y + health_bar_delta - 1, dormancy ? colour_t(91, 91, 91, player_alpha[entity->get_index()]) : colour_t(255, 255, 255, 255), font::smallfont, "%d", entity->get_health());
		}

		if (is_teammate ? vars::visuals::team_ammo : vars::visuals::enemy_ammo) { //colour_t(76, 130, 186, 255)
			//int curammo = entity->get_active_weapon()->get_clip_1();
			//int clip = entity->get_active_weapon()->get_clip_2();
			//int ammo_bar = w / clip * curammo;
			//int ammo_bar_delta = w + ammo_bar;
			//if (clip) {
			//	draw::draw_filled_rect(x - 1, y + h + 2, w + 2, 4, colour_t(80, 80, 80, player_alpha[entity->get_index()] / 2));
			//	draw::draw_outlined_rect(x - 1, y + h + 2, w + 2, 4, colour_t(0, 0, 0, player_alpha[entity->get_index()] / 2));
			//	draw::draw_filled_rect(x, y + h + 2, w + ammo_bar_delta, 8, dormancy ? colour_t(91, 91, 91, player_alpha[entity->get_index()]) : colour_t(76, 130, 186, 255));
			//}
		}

		if (is_teammate ? vars::visuals::team_name : vars::visuals::enemy_name) {
			static auto sanitize = [](char * name) -> std::string {
				name[127] = '\0'; std::string tmp(name);
				if (tmp.length() > 20) {
					tmp.erase(20, (tmp.length() - 20));
					tmp.append("..");
				} return tmp;
			};

			char* name = entity->is_player_bot() ? (xorstr("bot %s"), entity_info.name) : entity_info.name;

			std::string final_name = sanitize(name);
			std::transform(final_name.begin(), final_name.end(), final_name.begin(), tolower);
			//std::transform(final_name.begin(), final_name.end(), final_name.begin(), toupper);

			draw::draw_string_centered(x + (w / 2), y - 13, esp_colour, font::calibri, final_name.c_str());
			//dx_render::dx_render->text(final_name, x + (w / 2), y - 13, CENTER, dx_render::fonts::font_franklin, false, true, D3DCOLOR_ARGB(255, 255, 255, 255));

			std::string class_name;
			switch (entity->get_class_num()) {
				case 0: class_name = ""; break;
				case 1: class_name = xorstr("SCOUT"); break;
				case 2: class_name = xorstr("SNIPER"); break;
				case 3: class_name = xorstr("SOLDIER"); break;
				case 4: class_name = xorstr("DEMOMAN"); break;
				case 5: class_name = xorstr("MEDIC"); break;
				case 6: class_name = xorstr("HEAVY"); break;
				case 7: class_name = xorstr("PYRO"); break;
				case 8: class_name = xorstr("SPY"); break;
				case 9: class_name = xorstr("ENGINEER"); break;
			}
			draw::draw_string(x + w + 2, y - 2, esp_colour, font::smallfont, class_name.c_str());

			//conds!
			int cond_offset = 0;
			if (entity->get_condnew() & TFCond_Cloaked || entity->get_condnew() & TFCond_DeadRingered) { //in theory this cond doesn't need the offset!
				draw::draw_string(x + w + 2, y + 5, esp_colour, font::smallfont, std::string(xorstr("CLOAK")).c_str());
				cond_offset += 7;
			}
			if (entity->get_condnew() & TFCond_Disguised) {
				draw::draw_string(x + w + 2, y + 5 + cond_offset, esp_colour, font::smallfont, std::string(xorstr("DISGUISE")).c_str());
				cond_offset += 7;
			}
			if (entity->get_condnew() & TFCond_Ubercharged || entity->get_condnew() & TFCond_MegaHeal) {
				draw::draw_string(x + w + 2, y + 5 + cond_offset, esp_colour, font::smallfont, std::string(xorstr("UBER")).c_str());
				cond_offset += 7;
			}
			if (entity->get_condnew() & TFCond_Zoomed) {
				draw::draw_string(x + w + 2, y + 5 + cond_offset, dormancy ? esp_colour : colour_t(31, 255, 255), font::smallfont, std::string(xorstr("ZOOM")).c_str());
				cond_offset += 7;
			}
			if (entity->get_condnew() & TFCond_Healing) {
				draw::draw_string(x + w + 2, y + 5 + cond_offset, dormancy ? esp_colour : colour_t(146, 252, 146), font::smallfont, std::string(xorstr("REGEN")).c_str());
				cond_offset += 7;
			}
			if (entity->get_condnew() & TFCond_OnFire) {
				draw::draw_string(x + w + 2, y + 5 + cond_offset, dormancy ? esp_colour : fire(), font::smallfont, std::string(xorstr("FIRE")).c_str());
				cond_offset += 7;
			}
			if (entity->get_condnew() & TFCond_Jarated) {
				draw::draw_string(x + w + 2, y + 5 + cond_offset, dormancy ? esp_colour : colour_t(255, 255, 0), font::smallfont, std::string(xorstr("PISS")).c_str());
				cond_offset += 7;
			}
			if (entity->get_condnew() & TFCond_Milked) {
				draw::draw_string(x + w + 2, y + 5 + cond_offset, esp_colour, font::smallfont, std::string(xorstr("CUM")).c_str()); //no dormancy check needed becuz cum is white :^)
				cond_offset += 7;
			}
			if (entity->get_condnew() & TFCond_Taunting) {
				draw::draw_string(x + w + 2, y + 5 + cond_offset, dormancy ? esp_colour : colour_t(255, 209, 110), font::smallfont, std::string(xorstr("TAUNT")).c_str());
				cond_offset += 7;
			}
			if (entity != ctx::local_player) {
				if (!ctx::local_player)
					return;
				float distance = (dist(ctx::local_player->get_abs_origin(), entity->get_abs_origin()) * 0.0254f);
				draw::draw_string(x + w + 2, y + 5 + cond_offset, esp_colour, font::smallfont, std::string(xorstr("[%.1fM]")).c_str(), distance);
				cond_offset += 7;
			}


			//dev testing shits lol
			//draw::draw_string(x + w + 2, y + 5 + cond_offset, esp_colour, font::smallfont, xorstr("SEQ: %i"), entity->get_sequence());
			//cond_offset += 7;
			////draw::draw_string(x + w + 2, y + 5 + cond_offset, esp_colour, font::smallfont, xorstr("LCOUNT: %i"), entity->anim_layer_count()); //could be irrelevant? everyone results in a 7.
			////cond_offset += 7;
			////draw::draw_string(x + w + 2, y + 5 + cond_offset, esp_colour, font::smallfont, xorstr("ALSEQ: %i"), entity->anim_layer(entity->anim_layer_count()).sequence);
			//for (int i = 0; i < 15; i++)
			//{
			//	draw::draw_string(x + w + 2, y + 5 + cond_offset, esp_colour, font::smallfont, xorstr("L%i: %i"), i, entity->anim_layer(i).sequence);
			//	cond_offset += 7;
			//}
			if (vars::hvh::yaw_resolver && vars::aimbot::enabled) {
				draw::draw_string(x + w + 2, y + 5 + cond_offset, esp_colour, font::smallfont, xorstr("MISSED: %i"), resolver::shots_missed[entity->get_index()]);
				cond_offset += 7;
				draw::draw_string(x + w + 2, y + 5 + cond_offset, esp_colour, font::smallfont, xorstr("HIT: %i"), resolver::shots_hit[entity->get_index()]);
				cond_offset += 7;;
			}
		}

		if (is_teammate ? vars::visuals::team_weapon : vars::visuals::enemy_weapon) {
			c_base_weapon* entity_weapon = entity->get_active_weapon();
			if (!entity_weapon) { return; }
			std::string weapon_name = entity_weapon->get_name();
			auto weapon_i = entity_weapon->get_item_definition_index();

			auto replace_string = [&](std::string& input, const std::string& found, const std::string& desired) -> void {
				size_t index; index = input.find(found);
				while (index != std::string::npos) {
					input.replace(index, found.size(), desired);
					index = input.find(found, index + desired.size());
				}
			};

			replace_string(weapon_name, "#", ""); replace_string(weapon_name, "_", " ");
			replace_string(weapon_name, "TF", ""); replace_string(weapon_name, "Weapon", "");
			replace_string(weapon_name, "tf weapon", ""); replace_string(weapon_name, "medic", "");
			replace_string(weapon_name, "hwg", ""); replace_string(weapon_name, "soldier", "");
			replace_string(weapon_name, "primary", ""); replace_string(weapon_name, "secondary", "");
			replace_string(weapon_name, "rocketlauncher", "rocket launcher"); replace_string(weapon_name, "syringegun", "syringe gun");
			replace_string(weapon_name, "scout", ""); replace_string(weapon_name, "sniperrife", "sniper rifle");
			replace_string(weapon_name, "flaregun", "flare gun"); replace_string(weapon_name, "fireaxe", "axe");
			replace_string(weapon_name, "grenadelauncher", "grenade launcher"); replace_string(weapon_name, "builder", "sapper");
			replace_string(weapon_name, "pyro", "");

			if (weapon_i == WPN_RocketJumper)
				weapon_name = xorstr(" rocket jumper ");
			if (weapon_i == WPN_DragonsFury)
				weapon_name = xorstr(" dragons fury ");
			if (weapon_i == WPN_Knife)
				weapon_name = xorstr(" knife "); //should help remove that stupid space??

			std::transform(weapon_name.begin(), weapon_name.end(), weapon_name.begin(), ::toupper);

			draw::draw_string_centered(x + (w / 2) - 3, y + h + 1, esp_colour, font::smallfont, weapon_name.c_str());
		}

		if (is_teammate ? vars::visuals::team_steamid : vars::visuals::enemy_steamid) {
			draw::draw_string_centered(x + (w / 2), y + h + ((is_teammate ? vars::visuals::team_weapon : vars::visuals::enemy_weapon) ? 8 : 1), esp_colour, font::smallfont, "%s", entity_info.guid);
		}

		auto draw_my_epic_crosshair = [&](vec3_t screen) {
			interfaces::surface->draw_set_colour_t(colour_t(0, 0, 0));
			interfaces::surface->draw_filled_rect(screen[0] - 4, screen[1] - 2, screen[0] - 4 + 8, screen[1] - 2 + 4);
			interfaces::surface->draw_filled_rect(screen[0] - 2, screen[1] - 4, screen[0] - 2 + 4, screen[1] - 4 + 8);

			interfaces::surface->draw_set_colour_t(colour_t(255, 255, 255));
			interfaces::surface->draw_filled_rect(screen[0] - 3, screen[1] - 1, screen[0] - 3 + 6, screen[1] - 1 + 2);
			interfaces::surface->draw_filled_rect(screen[0] - 1, screen[1] - 3, screen[0] - 1 + 2, screen[1] - 3 + 6);
		};

		if (is_teammate ? vars::visuals::team_d_lights : vars::visuals::enemy_d_lights) {
			c_user_cmd* command = ctx::command;
			//if (entity != aimbot::get_best_target(global::local_player, command))
			//	return;

			dlight_t* d_light = interfaces::efx->alloc_dlight(entity->get_index());
			d_light->colour_t.r = 255;
			d_light->colour_t.g = 255;
			d_light->colour_t.b = 255;

			d_light->colour_t.exponent = 5;

			d_light->radius = (is_teammate? vars::visuals::team_d_lights_radius : vars::visuals::enemy_d_lights_radius);
			d_light->key = entity->get_index();

			d_light->die = interfaces::globals->curtime + 0.05f;
			d_light->decay = d_light->radius / 5.0f;

			d_light->origin = entity->get_abs_origin() + vec3_t(0, 0, 2);
		}

		if (vars::visuals::enemy_backtrack_visulization && !(utilities::is_projectile(ctx::local_player, ctx::local_player->get_active_weapon())) && vars::position_adjustment::enabled) {

			if (position_adjustment::ticks[entity->get_index()].empty()) {
				return;
			}

			if (is_teammate)
				return;

			if (entity->is_dead())
				return;

			c_user_cmd* command = ctx::command;
			if (entity != aimbot::get_closest_enemy(ctx::local_player, command, 85))
				return;
			if (vars::visuals::enemy_backtrack_visulization == 1) {
				for (unsigned int t = 0; t < position_adjustment::ticks[entity->get_index()].size(); t++) {

					vec3_t hitbox, screen;

					if (ctx::local_player->get_active_weapon()->get_slot() == 2)
						hitbox = position_adjustment::ticks[entity->get_index()].at(t).world_space_center + vec3_t(0, 0, 1);
					else
						hitbox = hitbox = position_adjustment::ticks[entity->get_index()].at(t).head_position + vec3_t(0, 0, 1);

					if (world_to_screen(hitbox, screen)) {
						draw::draw_string(screen[0], screen[1], colour_t(255, 255, 255), font::smallfont, xorstr("+"));
					}
				}
			}

			if (vars::visuals::enemy_backtrack_visulization == 2) {
				vec3_t lastscreen;
				bool prev = false;
				for (unsigned int t = 0; t < position_adjustment::ticks[entity->get_index()].size(); t++) {

					vec3_t hitbox, screen;
					if (ctx::local_player->get_active_weapon()->get_slot() == 2)
						hitbox = position_adjustment::ticks[entity->get_index()].at(t).world_space_center + vec3_t(0, 0, 1);
					else
						hitbox = hitbox = position_adjustment::ticks[entity->get_index()].at(t).head_position + vec3_t(0, 0, 1);

					if (world_to_screen(hitbox, screen) && prev) {
						draw::draw_line(lastscreen.x, lastscreen.y, screen.x, screen.y, colour_t::from_hsb((0.0833333 * t), 1, 1));
						draw::draw_line(lastscreen.x, lastscreen.y + 1, screen.x, screen.y + 1, colour_t::from_hsb((0.0833333 * t), 0.55f, 1)); //2 px wide :D
					}

					lastscreen = screen;
					prev = true;
				}
			}

			if (vars::visuals::enemy_backtrack_visulization == 3)
			{
				for (unsigned int t = 0; t < position_adjustment::ticks[entity->get_index()].size(); t++) {

					vec3_t hitbox, screen;

					if (ctx::local_player->get_active_weapon()->get_slot() == 2)
						hitbox = position_adjustment::ticks[entity->get_index()].at(t).world_space_center + vec3_t(0, 0, 1);
					else
						hitbox = hitbox = position_adjustment::ticks[entity->get_index()].at(t).head_position + vec3_t(0, 0, 1);

					if (world_to_screen(hitbox, screen)) {
						draw::draw_filled_rect(screen[0], screen[1], 4, 4, colour_t::from_hsb((0.0833333 * t), 1, 1));
						draw::draw_outlined_rect(screen[0], screen[1], 4, 4, colour_t::from_hsb((0.0833333 * t), 0.55f, 0.55f));
					}
				}
			}
		}

		if (vars::visuals::projectile_prediction) {
			if (ctx::local_player->get_life_state() != LIFE_ALIVE)
				return;
			c_base_weapon* local_weapon = ctx::local_player->get_active_weapon();
			if (local_weapon) {
				bool is_projectile_weapon = utilities::is_projectile(ctx::local_player, local_weapon);
				if (is_projectile_weapon) {
					c_user_cmd* command = ctx::command;
					if (entity == aimbot::get_closest_enemy(ctx::local_player, command, 85))
					{
						vec3_t predicted = entity->get_world_space_center(), space_center = entity->get_world_space_center(), screen, wsc_screen;
						aimbot::projectile_prediction(ctx::local_player, entity, local_weapon, predicted, QUALITY_MEDIUM);
						if (world_to_screen(predicted, screen) && world_to_screen(space_center, wsc_screen)) {
							draw::draw_line(wsc_screen[0], wsc_screen[1], screen[0], screen[1], colour_t(0, 0, 0, 144));
							draw::draw_line(wsc_screen[0], wsc_screen[1] + 1, screen[0], screen[1] + 1, colour_t(0, 0, 0, 144));
							draw::draw_line(wsc_screen[0], wsc_screen[1] - 1, screen[0], screen[1] - 1, colour_t(0, 0, 0, 144));
							draw::draw_filled_circle(screen[0], screen[1], 7, colour_t(0, 0, 0, 144));
							draw::draw_filled_circle(screen[0], screen[1], 5, esp_colour);
						}
					}
					//draw::draw_sphere(predicted, 8, 0, 1, esp_colour);
				}
			}
		}

		if (vars::visuals::out_of_fov_arrows) {

			if (entity != ctx::local_player) {
				if (entity->get_class_num() == ctx::local_player->get_team_num() && !vars::visuals::player_enemyonly)
					return;

				vec3_t client_viewangles, entity_hitbox = entity->get_world_space_center(), screen; float radius = 300.0f;
				int screen_width = 0.0f, screen_height = 0.0f;

				//if (world_to_screen(entity_hitbox, screen)) {
				//	return;
				//}

				interfaces::engineclient->get_view_angles(client_viewangles);
				interfaces::engineclient->get_screen_size(screen_width, screen_height);

				if (is_on_screen(entity_hitbox, screen))
					return;

				auto calc_angle = [&](vec3_t from, vec3_t to) {
					vec3_t return_value, subtracted = to - from;
					utilities::vector_angle(subtracted, return_value);
					return return_value;
				};

				const auto screen_center = vec3_t(screen_width / 2.f, screen_height / 2.f, 0);
				const auto rot = DEG2RAD(client_viewangles.y - calc_angle(ctx::local_player->get_eye_position(), entity_hitbox).y - 90);

				std::vector<vertex_t> vertices;
				vertices.push_back(vertex_t(vec2_t(screen_center.x + cosf(rot) * radius, screen_center.y + sinf(rot) * radius)));
				vertices.push_back(vertex_t(vec2_t(screen_center.x + cosf(rot + DEG2RAD(2)) * (radius - 16), screen_center.y + sinf(rot + DEG2RAD(2)) * (radius - 16))));
				vertices.push_back(vertex_t(vec2_t(screen_center.x + cosf(rot - DEG2RAD(2)) * (radius - 16), screen_center.y + sinf(rot - DEG2RAD(2)) * (radius - 16))));

				colour_t arrow_colour = colour_t(255, 255, 255, 255);

				auto draw_textured_polygon = [&](int points, std::vector<vertex_t> vertices, colour_t colour) -> void {
					static int texture_id = interfaces::surface->create_new_texture_id(true);
					static unsigned char buf[4] = { 255, 255, 255, 255 };
					interfaces::surface->draw_set_texture_rgba(texture_id, buf, 1, 1);
					interfaces::surface->draw_set_colour_t(colour);
					interfaces::surface->draw_set_texture(texture_id);
					interfaces::surface->draw_textured_polygon(points, vertices.data());
				};
				draw_textured_polygon(3, vertices, arrow_colour);
			}
		}

	}
	void visuals::create_building_esp(c_base_player* entity) {
		if (!entity || !ctx::local_player || !entity->get_model()) {
			return;
		}
		if (!utilities::is_entity_visible(ctx::local_player, entity, ctx::local_player->get_eye_position(), entity->get_world_space_center()) && vars::visuals::visible_only)
			return;
		const matrix3x4_t& vec_matrix = entity->get_rgfl_coordinate_frame();
		vec3_t vec_minimum = entity->get_collideable_mins(), vec_maximum = entity->get_collideable_maxs();
		vec3_t vec_pointlist[] = {
			vec3_t(vec_minimum.x, vec_minimum.y, vec_minimum.z),
			vec3_t(vec_minimum.x, vec_maximum.y, vec_minimum.z),
			vec3_t(vec_maximum.x, vec_maximum.y, vec_minimum.z),
			vec3_t(vec_maximum.x, vec_minimum.y, vec_minimum.z),

			vec3_t(vec_maximum.x, vec_maximum.y, vec_maximum.z),
			vec3_t(vec_minimum.x, vec_maximum.y, vec_maximum.z),
			vec3_t(vec_minimum.x, vec_minimum.y, vec_maximum.z),
			vec3_t(vec_maximum.x, vec_minimum.y, vec_maximum.z)
		}, vec_transformed[8];

		for (int i = 0; i < 8; i++) {
			for (int j = 0; j < 3; j++) {
				vec_transformed[i][j] = vec_pointlist[i].dot((vec3_t&)vec_matrix[j]) + vec_matrix[j][3];
			}
		}

		vec3_t flb, brt, blb, frt, frb, brb, blt, flt;
		if (!world_to_screen(vec_transformed[3], flb) ||
			!world_to_screen(vec_transformed[0], blb) ||
			!world_to_screen(vec_transformed[2], frb) ||
			!world_to_screen(vec_transformed[6], blt) ||
			!world_to_screen(vec_transformed[5], brt) ||
			!world_to_screen(vec_transformed[4], frt) ||
			!world_to_screen(vec_transformed[1], brb) ||
			!world_to_screen(vec_transformed[7], flt))
			return;

		vec3_t vec_array[] = { flb, brt, blb, frt, frb, brb, blt, flt };

		float	top = flb.y,
			right = flb.x,
			left = flb.x,
			bottom = flb.y;

		for (int i = 0; i < 8; i++) {
			if (left > vec_array[i].x) {
				left = vec_array[i].x;
			}
			if (top < vec_array[i].y) {
				top = vec_array[i].y;
			}
			if (right < vec_array[i].x) {
				right = vec_array[i].x;
			}
			if (bottom > vec_array[i].y) {
				bottom = vec_array[i].y;
			}
		}

		float x = left, y = bottom, w = right - left, h = top - bottom;
		x += ((right - left) / 8), w -= ((right - left) / 8) * 2;

		/*bool dormancy = entity->is_dormant();
		colour_t esp_colour = dormancy ? colour_t(91, 91, 91) :
										 colour_t(255, 255, 255);*/

		colour_t esp_colour = colour_t(255, 255, 255);

		HANDLE entity_handle = entity->get_builder();
		if (!entity_handle) {
			return;
		}

		c_base_player* entity_builder = (c_base_player*)interfaces::entitylist->get_client_entity_from_handle(entity_handle);
		if (!entity_builder) {
			return;
		}

		player_info_t builder;
		if (!interfaces::engineclient->get_player_info(entity_builder->get_index(), &builder)) {
			return;
		}

		if (utilities::is_developer(entity_builder)) {
			esp_colour = colour_t(240, 192, 215);
		}

		auto health_to_colour = [&](c_base_player* entity, int current_health) {
			float percentage = (double)current_health / entity->get_max_health() * 100;
			int32_t health = max(0, min(current_health, 100));
			if (percentage > 100) {
				return colour_t(192, 192, 255, 255);
			}
			else if (percentage <= 100) {
				colour_t colour = colour_t(
					min((510 * (100 - health)) / 100, 255),
					min((510 * health) / 100, 255),
					0,
					255
				);
				return colour;
			}	return colour_t(255, 255, 255, 255);
		};

		switch (entity->get_client_class()->class_id) {
			case CObjectDispenser: {
				if (vars::visuals::building_box) {
					draw::draw_outlined_rect(x - 1, y - 1, w + 2, h + 2, colour_t(0, 0, 0));
					draw::draw_outlined_rect(x + 1, y + 1, w - 2, h - 2, colour_t(0, 0, 0));
					draw::draw_outlined_rect(x, y, w, h, esp_colour);
				}

				if (vars::visuals::building_health) {
					int health = clamp(entity->get_object_health(), 0, entity->get_object_max_health());
					int health_bar = h / entity->get_object_max_health() * health;
					int health_bar_delta = h - health_bar;

					int green = health * 2.55;
					int red = 255 - green;

					draw::draw_filled_rect(x - 6, y - 1, 4, h + 2, colour_t(80, 80, 80));
					draw::draw_outlined_rect(x - 6, y - 1, 4, h + 2, colour_t(0, 0, 0));
					draw::draw_filled_rect(x - 5, y + health_bar_delta, 2, health_bar, health_to_colour(entity, health));
				}

				if (utilities::is_developer(entity_builder)) {
					draw::draw_string_centered(x + (w / 2), vars::visuals::building_name ? y - 20 : y - 13, esp_colour, font::smallfont, xorstr("DEVELOPER"));
				}

				if (vars::visuals::building_name) {
					draw::draw_string_centered(x + (w / 2), y - 13, esp_colour, font::calibri, xorstr("dispenser"));
				}

				if (vars::visuals::building_builder) {
					static auto sanitize = [](char * name) -> std::string {
						name[127] = '\0'; std::string tmp(name);
						if (tmp.length() > 20) {
							tmp.erase(20, (tmp.length() - 20));
							tmp.append("..");
						} return tmp;
					};

					char* name = builder.name; std::string final_name = sanitize(name);
					std::transform(final_name.begin(), final_name.end(), final_name.begin(), toupper);

					draw::draw_string_centered(x + (w / 2) - 3, y + h, esp_colour, font::smallfont, final_name.c_str());
				}

				if (vars::visuals::building_level) {
					int level = entity->get_level();
					draw::draw_string(x + w + 2, y - 2, esp_colour, font::smallfont, xorstr("LEVEL: %i"), level);
				}
			} break;
			case CObjectSentrygun: {
				if (vars::visuals::building_box) {
					draw::draw_outlined_rect(x - 1, y - 1, w + 2, h + 2, colour_t(0, 0, 0));
					draw::draw_outlined_rect(x + 1, y + 1, w - 2, h - 2, colour_t(0, 0, 0));
					draw::draw_outlined_rect(x, y, w, h, esp_colour);
				}

				if (vars::visuals::building_health) {
					int health = clamp(entity->get_object_health(), 0, entity->get_object_max_health());
					int health_bar = h / entity->get_object_max_health() * health;
					int health_bar_delta = h - health_bar;

					int green = health * 2.55;
					int red = 255 - green;

					draw::draw_filled_rect(x - 6, y - 1, 4, h + 2, colour_t(80, 80, 80));
					draw::draw_outlined_rect(x - 6, y - 1, 4, h + 2, colour_t(0, 0, 0));
					draw::draw_filled_rect(x - 5, y + health_bar_delta, 2, health_bar, health_to_colour(entity, health));
				}

				if (utilities::is_developer(entity_builder)) {
					draw::draw_string_centered(x + (w / 2), vars::visuals::building_name ? y - 20 : y - 13, esp_colour, font::smallfont, xorstr("DEVELOPER"));
				}

				if (vars::visuals::building_name) {
					draw::draw_string_centered(x + (w / 2), y - 13, esp_colour, font::calibri, xorstr("sentry")); //
				}

				if (vars::visuals::building_builder) {
					static auto sanitize = [](char * name) -> std::string {
						name[127] = '\0'; std::string tmp(name);
						if (tmp.length() > 20) {
							tmp.erase(20, (tmp.length() - 20));
							tmp.append("..");
						} return tmp;
					};

					char* name = builder.name; std::string final_name = sanitize(name);
					std::transform(final_name.begin(), final_name.end(), final_name.begin(), toupper);

					draw::draw_string_centered(x + (w / 2) - 3, y + h, esp_colour, font::smallfont, final_name.c_str());
				}

				if (vars::visuals::building_level) {
					int level = entity->get_level();
					draw::draw_string(x + w + 2, y - 2, esp_colour, font::smallfont, xorstr("LEVEL: %i"), level);
				}
			} break;
			case CObjectTeleporter: {
				if (vars::visuals::building_box) {
					draw::draw_outlined_rect(x - 1, y - 1, w + 2, h + 2, colour_t(0, 0, 0));
					draw::draw_outlined_rect(x + 1, y + 1, w - 2, h - 2, colour_t(0, 0, 0));
					draw::draw_outlined_rect(x, y, w, h, esp_colour);
				}

				if (vars::visuals::building_health) {
					int health = clamp(entity->get_object_health(), 0, entity->get_object_max_health());
					int health_bar = h / entity->get_object_max_health() * health;
					int health_bar_delta = h - health_bar;

					int green = health * 2.55;
					int red = 255 - green;

					draw::draw_filled_rect(x - 6, y - 1, 4, h + 2, colour_t(80, 80, 80));
					draw::draw_outlined_rect(x - 6, y - 1, 4, h + 2, colour_t(0, 0, 0));
					draw::draw_filled_rect(x - 5, y + health_bar_delta, 2, health_bar, health_to_colour(entity, health));
				}

				if (utilities::is_developer(entity_builder)) {
					draw::draw_string_centered(x + (w / 2), vars::visuals::building_name ? y - 20 : y - 13, esp_colour, font::smallfont, xorstr("DEVELOPER"));
				}

				if (vars::visuals::building_name) {
					draw::draw_string_centered(x + (w / 2), y - 13, esp_colour, font::calibri, xorstr("teleporter")); //
				}

				if (vars::visuals::building_builder) {
					static auto sanitize = [](char * name) -> std::string {
						name[127] = '\0'; std::string tmp(name);
						if (tmp.length() > 20) {
							tmp.erase(20, (tmp.length() - 20));
							tmp.append("..");
						} return tmp;
					};

					char* name = builder.name; std::string final_name = sanitize(name);
					std::transform(final_name.begin(), final_name.end(), final_name.begin(), toupper);

					draw::draw_string_centered(x + (w / 2), y + h, esp_colour, font::smallfont, final_name.c_str());
				}

				if (vars::visuals::building_level) {
					int level = entity->get_level();
					draw::draw_string(x + w + 2, y - 2, esp_colour, font::smallfont, xorstr("LEVEL: %i"), level);
				}
			} break;
			default: break;
		}
	}
	void visuals::create_item_esp(c_base_player* entity) {
		if (!entity || !ctx::local_player) {
			return;
		}

		const matrix3x4_t& vec_matrix = entity->get_rgfl_coordinate_frame();
		vec3_t vec_minimum = entity->get_collideable_mins(), vec_maximum = entity->get_collideable_maxs();
		vec3_t vec_pointlist[] = {
			vec3_t(vec_minimum.x, vec_minimum.y, vec_minimum.z),
			vec3_t(vec_minimum.x, vec_maximum.y, vec_minimum.z),
			vec3_t(vec_maximum.x, vec_maximum.y, vec_minimum.z),
			vec3_t(vec_maximum.x, vec_minimum.y, vec_minimum.z),

			vec3_t(vec_maximum.x, vec_maximum.y, vec_maximum.z),
			vec3_t(vec_minimum.x, vec_maximum.y, vec_maximum.z),
			vec3_t(vec_minimum.x, vec_minimum.y, vec_maximum.z),
			vec3_t(vec_maximum.x, vec_minimum.y, vec_maximum.z)
		}, vec_transformed[8];

		for (int i = 0; i < 8; i++) {
			for (int j = 0; j < 3; j++) {
				vec_transformed[i][j] = vec_pointlist[i].dot((vec3_t&)vec_matrix[j]) + vec_matrix[j][3];
			}
		}

		vec3_t flb, brt, blb, frt, frb, brb, blt, flt;
		if (!world_to_screen(vec_transformed[3], flb) ||
			!world_to_screen(vec_transformed[0], blb) ||
			!world_to_screen(vec_transformed[2], frb) ||
			!world_to_screen(vec_transformed[6], blt) ||
			!world_to_screen(vec_transformed[5], brt) ||
			!world_to_screen(vec_transformed[4], frt) ||
			!world_to_screen(vec_transformed[1], brb) ||
			!world_to_screen(vec_transformed[7], flt))
			return;

		vec3_t vec_array[] = { flb, brt, blb, frt, frb, brb, blt, flt };

		float	top = flb.y,
			right = flb.x,
			left = flb.x,
			bottom = flb.y;

		for (int i = 0; i < 8; i++) {
			if (left > vec_array[i].x) {
				left = vec_array[i].x;
			}
			if (top < vec_array[i].y) {
				top = vec_array[i].y;
			}
			if (right < vec_array[i].x) {
				right = vec_array[i].x;
			}
			if (bottom > vec_array[i].y) {
				bottom = vec_array[i].y;
			}
		}

		float x = left, y = bottom, w = right - left, h = top - bottom;
		x += ((right - left) / 8), w -= ((right - left) / 8) * 2;

		colour_t esp_colour = colour_t(255, 255, 255);


		const char* model_name = entity->get_model_name();
		int model_id = entity->get_client_class()->class_id;
		bool is_ammo = false, is_health = false, is_projectile = false, is_money = false, is_tank = false, is_cart = false;
		std::string name = "";
		if (!strcmp(model_name, "models/items/medkit_small.mdl") ||
			!strcmp(model_name, "models/items/medkit_small_bday.mdl") ||
			!strcmp(model_name, "models/props_halloween/halloween_medkit_small.mdl") ||
			!strcmp(model_name, "models/items/medkit_medium.mdl") ||
			!strcmp(model_name, "models/items/medkit_medium_bday.mdl") ||
			!strcmp(model_name, "models/props_halloween/halloween_medkit_medium.mdl") ||
			!strcmp(model_name, "models/items/medkit_large.mdl") ||
			!strcmp(model_name, "models/items/medkit_large_bday.mdl") ||
			!strcmp(model_name, "models/props_halloween/halloween_medkit_large.mdl") ||
			!strcmp(model_name, "models/items/plate.mdl") ||
			!strcmp(model_name, "models/items/plate_steak.mdl") ||
			!strcmp(model_name, "models/items/plate_robo_sandwich.mdl")) {
			name = "health"; is_health = true;
		}
		
		if (!strcmp(model_name, "models/items/ammopack_small.mdl") ||
			!strcmp(model_name, "models/items/ammopack_small_bday.mdl") ||
			!strcmp(model_name, "models/items/ammopack_medium.mdl") ||
			!strcmp(model_name, "models/items/ammopack_medium_bday.mdl") ||
			!strcmp(model_name, "models/items/ammopack_large.mdl") ||
			!strcmp(model_name, "models/items/ammopack_large_bday.mdl") ||
			!strcmp(model_name, "models/items/tf_gift.mdl") ||
			!strcmp(model_name, "models/props_halloween/halloween_gift.mdl")) {
			name = "ammo"; is_ammo = true;
		}


		if (model_id == CBaseProjectile || model_id == CTFBaseProjectile || model_id == CTFBaseRocket || 
			model_id == CTFProjectile_Arrow || model_id == CTFProjectile_BallOfFire|| model_id == CTFProjectile_Cleaver||
			model_id == CTFProjectile_EnergyBall || model_id == CTFProjectile_EnergyRing || model_id == CTFProjectile_Flare ||
			model_id == CTFProjectile_GrapplingHook || model_id == CTFProjectile_HealingBolt || model_id == CTFProjectile_Jar ||
			model_id == CTFProjectile_JarGas || model_id == CTFProjectile_JarMilk || model_id == CTFProjectile_MechanicalArmOrb ||
			model_id == CTFProjectile_Rocket || model_id == CTFProjectile_SentryRocket || model_id == CTFProjectile_Throwable ||
			model_id == CTFProjectile_ThrowableBreadMonster || model_id == CTFProjectile_ThrowableBrick || model_id == CTFProjectile_ThrowableRepel ||
			model_id == CBaseGrenade || model_id == CTFGrenadePipebombProjectile || model_id == CTFWeaponBaseGrenadeProj) {
			name = "projectile"; is_projectile = true;
		}

		if (model_id == CCurrencyPack) {
			name = "money"; is_money = true;
		}
		
		if (model_id == CTFTankBoss) {
			name = "tank"; is_tank = true;
		}

		//if (model_id == 67) {
		//	name = "cart"; is_cart = true;
		//}

		if (is_projectile && vars::visuals::world_projectile_chams)
		{
			draw::draw_sphere(entity->get_vec_origin(), 10, rand() % (180 - 0 + 1) + 0, 1, colour_t::from_hsb(vars::colour::projectile_chams / 100, 1.f, 1.f));
		}
		if (is_ammo || is_health || is_projectile || is_money || is_tank || is_cart) {
			if (vars::visuals::world_box) {
				draw::draw_outlined_rect(x - 1, y - 1, w + 2, h + 2, colour_t(0, 0, 0));
				draw::draw_outlined_rect(x + 1, y + 1, w - 2, h - 2, colour_t(0, 0, 0));
				draw::draw_outlined_rect(x, y, w, h, esp_colour);
			}

			if (vars::visuals::world_name) {
				draw::draw_string_centered(x + (w / 2), y - 13, esp_colour, font::calibri, name.c_str());
			}
		}
		//draw::draw_string_centered(x + (w / 2), y, esp_colour, font::calibri, model_name);
		//draw::draw_string_centered(x + (w / 2), y - 9, esp_colour, font::calibri, "id: %i", model_id);
		if (model_id == CTFGlow)
		{
			draw::draw_string_centered(x + (w / 2), y, esp_colour, font::calibri, xorstr("this nigga GLOWIN BRUH"));
		}
	}

	bool visuals::world_to_screen(vec3_t &origin, vec3_t &screen) {
		const matrix3x4_t& world_to_screen = interfaces::engineclient->world_to_screen_matrix();

		float w = world_to_screen[3][0] * origin[0] + world_to_screen[3][1] * origin[1] + world_to_screen[3][2] * origin[2] + world_to_screen[3][3];
		screen.z = 0;

		if (w > 0.001) {
			float fl1DBw = 1 / w;
			screen.x = (ctx::screen_width / 2) + (0.5 * ((world_to_screen[0][0] * origin[0] + world_to_screen[0][1] * origin[1] + world_to_screen[0][2] * origin[2] + world_to_screen[0][3]) * fl1DBw) * ctx::screen_width + 0.5);
			screen.y = (ctx::screen_height / 2) - (0.5 * ((world_to_screen[1][0] * origin[0] + world_to_screen[1][1] * origin[1] + world_to_screen[1][2] * origin[2] + world_to_screen[1][3]) * fl1DBw) * ctx::screen_height + 0.5);
			return true;
		}
		return false;
	}

	bool visuals::create_bounds(c_base_player* entity, float &x, float &y, float &w, float &h) {
		const matrix3x4_t& vec_matrix = entity->get_rgfl_coordinate_frame();
		vec3_t vec_minimum = entity->get_collideable_mins(), vec_maximum = entity->get_collideable_maxs();
		vec3_t vec_pointlist[] = {
			vec3_t(vec_minimum.x, vec_minimum.y, vec_minimum.z),
			vec3_t(vec_minimum.x, vec_maximum.y, vec_minimum.z),
			vec3_t(vec_maximum.x, vec_maximum.y, vec_minimum.z),
			vec3_t(vec_maximum.x, vec_minimum.y, vec_minimum.z),

			vec3_t(vec_maximum.x, vec_maximum.y, vec_maximum.z),
			vec3_t(vec_minimum.x, vec_maximum.y, vec_maximum.z),
			vec3_t(vec_minimum.x, vec_minimum.y, vec_maximum.z),
			vec3_t(vec_maximum.x, vec_minimum.y, vec_maximum.z)
		}, vec_transformed[8];

		for (int i = 0; i < 8; i++) {
			for (int j = 0; j < 3; j++) {
				vec_transformed[i][j] = vec_pointlist[i].dot((vec3_t&)vec_matrix[j]) + vec_matrix[j][3];
			}
		}

		vec3_t flb, brt, blb, frt, frb, brb, blt, flt;
		if (!world_to_screen(vec_transformed[3], flb) ||
			!world_to_screen(vec_transformed[0], blb) ||
			!world_to_screen(vec_transformed[2], frb) ||
			!world_to_screen(vec_transformed[6], blt) ||
			!world_to_screen(vec_transformed[5], brt) ||
			!world_to_screen(vec_transformed[4], frt) ||
			!world_to_screen(vec_transformed[1], brb) ||
			!world_to_screen(vec_transformed[7], flt))
			return false;

		vec3_t vec_array[] = { flb, brt, blb, frt, frb, brb, blt, flt };

		float	top = flb.y,
				right = flb.x,
				left = flb.x,
				bottom = flb.y;

		for (int i = 0; i < 8; i++) {
			if (left > vec_array[i].x) {
				left = vec_array[i].x;
			}
			if (top < vec_array[i].y) {
				top = vec_array[i].y;
			}
			if (right < vec_array[i].x) {
				right = vec_array[i].x;
			}
			if (bottom > vec_array[i].y) {
				bottom = vec_array[i].y;
			}
		}

		x = left, y = bottom, w = right - left, h = top - bottom;
		x += ((right - left) / 8), w -= ((right - left) / 8) * 2;

		return true;
	}

	void render_glow_on_pdus(c_base_player* local) {
		for (int i = 1; i < interfaces::entitylist->get_highest_entity_index(); i++) {
			c_base_player* entity = reinterpret_cast<c_base_player*>(interfaces::entitylist->get_client_entity(i));
			if (!entity || entity == ctx::local_player)
				continue;
			bool is_visible = utilities::is_entity_visible(ctx::local_player, entity, ctx::local_player->get_eye_position(), entity->get_world_space_center());
			bool is_teammate = (entity->get_team_num() == local->get_team_num());
			if (!is_visible && vars::visuals::visible_only)
				continue;

			if (is_teammate ? vars::visuals::team_glow : vars::visuals::enemy_glow) {
				if (entity->get_client_class()->class_id == CBasePlayer) {
					colour_t white = colour_t(255, 255, 255);
					if (!entity->has_glow_effect())
						entity->register_glow_object(white, true, true);

					entity->force_glow_effect(true);
				}
			}
		}
	}

	void render_glow_on_frs(c_base_player* local) {
		if (vars::visuals::team_glow || vars::visuals::enemy_glow)
		{
			for (int i = 0; interfaces::glow_object->m_glow_object_definitions.Count(); ++i) {
				glow_object_t& object = interfaces::glow_object->m_glow_object_definitions[i];

				if (object.m_n_next_free_slot != ENTRY_IN_USE)
					continue;

				c_base_player* ent = (c_base_player*)interfaces::entitylist->get_client_entity_from_handle(object.entity_handle);

				if (!ent || ent == nullptr)
					continue;

				bool is_visible = false; //utilities::is_entity_visible(ctx::local_player, ent, ctx::local_player->get_eye_position(), ent->get_world_space_center());
				bool is_teammate = (ent->get_team_num() == local->get_team_num());

				if (ent->get_client_class()->class_id == CBasePlayer) {
					if (is_teammate ? vars::visuals::team_glow : vars::visuals::enemy_glow) {

						colour_t colour; //get colour
						float hue;
						if (is_teammate) {
							if (is_visible)
								hue = vars::colour::team_glow_vis;
							else
								hue = vars::colour::team_glow_inv;
						}
						else if (!is_teammate) {
							if (is_visible)
								hue = vars::colour::enemy_glow_vis;
							else
								hue = vars::colour::enemy_glow_inv;
						}
						colour = colour_t::from_hsb(hue / 100, 1.f, 1.f, 255);

						object.m_glow_colour = vec3_t(colour.r(), colour.g(), colour.b());
						object.m_glow_alpha = 1.f;
					}

					else
					{
						interfaces::glow_object->m_glow_object_definitions.Remove(i);
					}
				}
			}
		}
		else if (interfaces::glow_object->m_glow_object_definitions.Count())
		{
			interfaces::glow_object->m_glow_object_definitions.RemoveAll();
		}
	}
}