#include "../main.h"
namespace hooks {
	bool __fastcall	hooks::create_move(iclientmodeshared* ecx, PVOID edx, float input_sample_time, c_user_cmd* command) {
		static auto original_create_move = hook::client_mode_hook->get_original_function<decltype(&hooks::create_move)>(21);
		original_create_move(ecx, edx, input_sample_time, command);

		ctx::local_player = reinterpret_cast<c_base_player*>(interfaces::entitylist->get_client_entity(interfaces::engineclient->get_local_player()));
		ctx::command = command; ctx::original_command = *command;

		uintptr_t _bp; __asm mov _bp, ebp;
		ctx::send_packet = (bool*)(***(uintptr_t***)_bp - 1);

		if (ctx::local_player == nullptr || ctx::command == nullptr || !ctx::command->command_number) {
			return false;
		}
		position_adjustment::do_position_adjustment();
		miscellanous::do_miscellanous();
		//miscellanous::do_anim_bug();
		prediction::run_prediction(ctx::command, ctx::local_player);
		triggerbot::do_triggerbot();
		aimbot::do_aimbot();
		aimbot::do_autorev();
		prediction::end_prediction(ctx::command, ctx::local_player);
		anti_aim::do_anti_aim();
		//dx_render::dx_render->reset();
		#if ISO_CHROMA
		chroma::get().in_game();
		#endif

		if (vars::visuals::thirdperson_angle == 0 && !*ctx::send_packet) {
			ctx::antiaim_angle = ctx::command->viewangles;
			ctx::real_angle = ctx::command->viewangles;
		}
		else if (vars::visuals::thirdperson_angle == 1 && *ctx::send_packet) {
			ctx::antiaim_angle = ctx::command->viewangles;
			ctx::fakelag_position = ctx::local_player->get_abs_origin();
			ctx::fake_angle = ctx::command->viewangles;
		}

		return false;
	}
	float __fastcall hooks::get_viewmodel_fov(iclientmodeshared* ecx, PVOID edx) {
		static auto original_get_viewmodel_fov = hook::client_mode_hook->get_original_function<decltype(&hooks::get_viewmodel_fov)>(32);
		return original_get_viewmodel_fov(ecx, edx);
	}
	void __fastcall hooks::override_mouse_input(float *_x, float *_y) {
		static auto original_override_mouse_input = hook::client_mode_hook->get_original_function<decltype(&hooks::override_mouse_input)>(20);
		original_override_mouse_input(_x, _y);
	}
	int __fastcall hooks::key_event(ibaseclientdll* ecx, PVOID edx, int event_code, int key_num, const char* current_binding) {
		static auto original_key_event = hook::client_hook->get_original_function<decltype(&hooks::key_event)>(20);

		return original_key_event(ecx, edx, event_code, key_num, current_binding);
	}

	unsigned int find_panel(unsigned int parent, const char* name) {
		if (!strcmp(interfaces::panel->get_name(parent), name))
			return parent;

		for (int i = 0; i < interfaces::panel->get_child_count(parent); ++i)
		{
			unsigned int panel = interfaces::panel->get_child(parent, i);
			if (panel = find_panel(panel, name))
				return panel;
		}

		return 0;
	}

	void __fastcall	hooks::paint_traverse(ipanel* ecx, PVOID edx, unsigned int vgui_panel, bool force_repaint, bool allow_force) {
		//so im gonna switch this over to the smart way that doesn't use fucking string comparison because i'm not an ape

		static unsigned int focus_overlay = 0, base_viewport = 0, hud_scope = 0, hud_death = 0,
			team_menu = 0, hud_chat = 0, class_selection = 0, hud_crosshair = 0;
		const char* panel = interfaces::panel->get_name(vgui_panel);
		if (vgui_panel)
		{
			if (!focus_overlay && !strcmp(panel, "FocusOverlayPanel")) //purpose: compares strings ONCE instead of every frame.
			{
				focus_overlay = vgui_panel;
			}
			else if (!base_viewport && interfaces::engineclient->is_in_game())
			{
				unsigned int parent = interfaces::panel->get_parent(focus_overlay);
				if (interfaces::panel->has_parent(focus_overlay, parent) && (base_viewport = find_panel(parent, "CBaseViewport"))) //I actually don't know if the get_child function uses strcmp
				{																												   //but it really doesn't matter, i don't give a shit.
					hud_scope = find_panel(base_viewport, "HudScope");															   //like, you know. it looks better.
					hud_death = find_panel(base_viewport, "HudDeathNotice");
					team_menu = find_panel(base_viewport, "team");
					hud_chat = find_panel(base_viewport, "HudChat");
					class_selection = find_panel(base_viewport, "ClassMenuSelect"); //doesn't work.
					hud_crosshair = find_panel(base_viewport, "HudCrosshair");
				}
			}
		}

		if (interfaces::engineclient->is_connected() && interfaces::engineclient->is_in_game()) {
			if (vars::visuals::no_scope && vgui_panel == hud_scope) {
				return;
			}

			if (vars::cheat::hud && base_viewport)
			{
				int count = interfaces::panel->get_child_count(base_viewport);
				for (int i = 0; i < count; ++i)
				{
					if (vgui_panel == interfaces::panel->get_child(base_viewport, i))
					{
						if (vgui_panel != hud_scope and vgui_panel != hud_death and vgui_panel != team_menu and vgui_panel != hud_chat and vgui_panel != class_selection \
							and vgui_panel != hud_crosshair)
							return;
					}
				}
			}
		}
	
		static auto original_paint_traverse = hook::panel_hook->get_original_function<decltype(&hooks::paint_traverse)>(41);
		original_paint_traverse(ecx, edx, vgui_panel, force_repaint, allow_force);

		static unsigned int vgui_focus_overlay;
		if (vgui_focus_overlay == NULL) {
			if (vgui_panel == focus_overlay) {
				vgui_focus_overlay = vgui_panel;
			}
		}

		auto quick_do_fonts = [&]() {
			font::welcome = interfaces::surface->create_font();
			interfaces::surface->set_font_glyph_set(font::welcome, xorstr("Tahoma"), 18, 500, 0, 0, FONTFLAG_OUTLINE);

			font::verdana = interfaces::surface->create_font();
			interfaces::surface->set_font_glyph_set(font::verdana, xorstr("Verdana"), 14, 400, 0, 0, FONTFLAG_DROPSHADOW);

			font::smallfont = interfaces::surface->create_font();
			interfaces::surface->set_font_glyph_set(font::smallfont, xorstr("Small Fonts"), 9, 100, 0, 0, FONTFLAG_OUTLINE);

			font::tahoma = interfaces::surface->create_font();
			interfaces::surface->set_font_glyph_set(font::tahoma, xorstr("Verdana Bold"), 12, 500, 0, 0, FONTFLAG_DROPSHADOW);

			font::verdana2 = interfaces::surface->create_font();
			interfaces::surface->set_font_glyph_set(font::verdana2, xorstr("Verdana"), 12, 400, 0, 0, FONTFLAG_DROPSHADOW);

			font::menu_font = interfaces::surface->create_font();
			interfaces::surface->set_font_glyph_set(font::menu_font, xorstr("Franklin Gothic"), 13, 500, 0, 0, FONTFLAG_DROPSHADOW); //did you know this looks exactly like tahoma? wild shit bruhs

			font::tab_font = interfaces::surface->create_font();
			interfaces::surface->set_font_glyph_set(font::tab_font, xorstr("Impact"), 36, 500, 0, 0, FONTFLAG_ANTIALIAS);

			font::calibri = interfaces::surface->create_font(); //name font!
			interfaces::surface->set_font_glyph_set(font::calibri, xorstr("Franklin Gothic"), 12, 500, 0, 0, FONTFLAG_DROPSHADOW | FONTFLAG_ANTIALIAS);

			font::hud = interfaces::surface->create_font();
			interfaces::surface->set_font_glyph_set(font::hud, xorstr("Roboto"), 129, 800, 0, 0, FONTFLAG_ANTIALIAS);

			font::hud2 = interfaces::surface->create_font();
			interfaces::surface->set_font_glyph_set(font::hud2, xorstr("Roboto"), 79, 800, 0, 0, FONTFLAG_ANTIALIAS);

			interfaces::engineclient->get_screen_size(ctx::screen_width, ctx::screen_height);
		};

		static bool once = true;
		if (once) {
			quick_do_fonts();
			CNetVars::get().Initialize();
			once = false;
		}

		if (vgui_focus_overlay == vgui_panel) {
			c_menu::get().draw(vgui_focus_overlay);
			visuals::render_watermark();
			if (vars::miscellanous::sv_pure_bypass) {
				bool check_active = false;
				if (!interfaces::engineclient->is_in_game() && !check_active) {
					void** pure_addr = nullptr;
					if (!pure_addr) {
						pure_addr = *reinterpret_cast<void***>(utilities::find_signature(xorstr("engine.dll"), xorstr("A1 ? ? ? ? 56 33 F6 85 C0")) + 1);
					} *pure_addr = (void*)0;
					check_active = true;
				}
			}
		}

		if (vgui_panel == interfaces::enginevgui->get_panel(PANEL_CLIENTDLL)) {
			static int width, height; interfaces::engineclient->get_screen_size(width, height);
			if (ctx::screen_width != width || ctx::screen_height != height) {
				interfaces::engineclient->get_screen_size(ctx::screen_width, ctx::screen_height);
				quick_do_fonts();
			}
			visuals::render_hud();
			visuals::render_esp();
			c_notification::get().draw();
			c_menu::get().draw_radar();
			c_menu::get().draw_spectatorlist();
			c_menu::get().draw_playerlist();
		}
	}

	void __fastcall hooks::draw_model_exeute(imodel_render* ecx, PVOID edx, model_render_info &entity_info, matrix3x4_t* custom_bones) {
		hook::render_hook->un_hook_function(19);
		hook::render_hook->hook_function(hooks::draw_model_exeute, 19);
	}

	void __fastcall hooks::frame_stage_notify(ibaseclientdll* ecx, PVOID edx, client_frame_stage_t stage) {
		static auto original_frame_stage = hook::client_hook->get_original_function<decltype(&hooks::frame_stage_notify)>(35);
		if (interfaces::engineclient->is_connected() && interfaces::engineclient->is_in_game() && ctx::local_player) {
			switch (stage) {
				case FRAME_START: break;
				case FRAME_NET_UPDATE_START: break;
				case FRAME_NET_UPDATE_POSTDATAUPDATE_START: {
					
					resolver::do_resolve();
					//visuals::render_glow_on_pdus(ctx::local_player);

				} break;
				case FRAME_NET_UPDATE_POSTDATAUPDATE_END: {
					static int starting = 100;
					if (starting != vars::visuals::nightmode_value) {
						starting  = vars::visuals::nightmode_value;

						for (material_handle i = interfaces::material_system->first_material(); i != interfaces::material_system->invalid_material(); i = interfaces::material_system->next_material(i)) {
							imaterial* material = interfaces::material_system->get_material(i);
							if (!material || material->is_error_material()) {
								continue;
							}

							float final_nightmode_value = vars::visuals::nightmode_value / 100.0f;
							if (strstr(material->get_texture_group_name(), xorstr("StaticProp")) || strstr(material->get_texture_group_name(), xorstr("World"))) {
								if (final_nightmode_value < 1.f)
									material->colour_modulate(final_nightmode_value - 0.10f, final_nightmode_value - 0.10f, final_nightmode_value + 0.05f);
								else 
									material->colour_modulate(final_nightmode_value, final_nightmode_value, final_nightmode_value);
							}
						}
					}
				} break;
				case FRAME_NET_UPDATE_END: break;
				case FRAME_RENDER_START: {
					if (ctx::in_thirdperson) {
						float &eye_angles_y = *reinterpret_cast<float*>(reinterpret_cast<DWORD>(ctx::local_player) + CNetVars::get().get_offset(xorstr("DT_BasePlayer"), xorstr("pl"), xorstr("deadflag")) + 8);
						float &eye_angles_x = *reinterpret_cast<float*>(reinterpret_cast<DWORD>(ctx::local_player) + CNetVars::get().get_offset(xorstr("DT_BasePlayer"), xorstr("pl"), xorstr("deadflag")) + 4);

						if (ctx::local_player && !ctx::local_player->is_dead()) {
							eye_angles_x = ctx::antiaim_angle.x;
							eye_angles_y = ctx::antiaim_angle.y;
							visuals::render_fake_hitboxes();
						}
					}
					//visuals::render_glow_on_frs(ctx::local_player);

					//experimental walkway fix....
					//if (interfaces::engineclient->is_connected() && interfaces::engineclient->is_in_game()) {
					//	for (int i = 0; i <= interfaces::engineclient->get_max_clients(); i++) {
					//		c_base_player* entity = reinterpret_cast<c_base_player*>(interfaces::entitylist->get_client_entity(i));
					//		if (!entity || entity->is_dead() || entity->get_client_class()->class_id != CTFPlayer) {
					//			continue;
					//		}

					//		c_base_player* ground_ent = (c_base_player*)interfaces::entitylist->get_client_entity_from_handle(entity->get_ground_entity());

					//		AnimState_t* animstate = entity->get_animstate();
					//		
					//		if (animstate)
					//		{ 
					//			if (ground_ent->get_conveyor_speed() > 0.f)
					//			{
					//				animstate->m_PoseParameterData.m_iMoveX = 0;
					//				animstate->m_PoseParameterData.m_iMoveY = 0;
					//			}
					//		}
					//	}
					//}

				} break;
				case FRAME_RENDER_END: break;
			}
		}
		original_frame_stage(ecx, edx, stage);
	}
	void __fastcall hooks::override_view(void* ecx, void* edx, c_view_setup* setup) {
		static auto original_override_view = hook::client_mode_hook->get_original_function<decltype(&hooks::override_view)>(16);
		original_override_view(ecx, edx, setup);

		if (interfaces::engineclient->is_connected() && interfaces::engineclient->is_in_game()) {

			//collecting data... 
			vars::visuals::real_fov = setup->m_fov;
			ctx::camera_x = setup->x;
			ctx::camera_y = setup->y;
			ctx::camera_origin = setup->origin;
			ctx::camera_angs = setup->angles;

			if (ctx::local_player && !ctx::local_player->is_dead()) {
				if (utilities::has_condition(ctx::local_player, 2)) {
					ctx::is_scoped = true;
					if (!vars::visuals::no_zoom) {
						//vars::visuals::real_fov = 20;
						
					}
				} else {
					ctx::is_scoped = false;
				}

				if (vars::visuals::no_zoom) {
					if (utilities::has_condition(ctx::local_player, 2)) {
						ctx::local_player->set_condition(ctx::local_player->get_condition() & ~2);
					}
					
					setup->m_fov = vars::visuals::forced_fov;
					vars::visuals::real_fov = setup->m_fov;
				} else if (setup->m_fov != 20.0f && setup->m_fov != vars::visuals::forced_fov) {
					setup->m_fov = vars::visuals::forced_fov;
					vars::visuals::real_fov = setup->m_fov;
				}

				if (vars::miscellanous::nopunch)
					setup->angles -= ctx::local_player->get_punch_angles();

				if (vars::visuals::thirdperson)
				{

				}
			}
		}
	}

	void __fastcall hooks::scene_end(void* ecx, void* edx) {
		static auto original_scene_end = hook::render_hook->get_original_function<decltype(&hooks::scene_end)>(9);
		original_scene_end(ecx, edx);

		if (!ctx::local_player || vars::chams::chams_activation == 0) {
			return;
		}

		auto force_material_thanks = [&](imaterial* material, colour_t colour, bool ignorez) {
			material->set_materialvar_flag(MATERIAL_VAR_IGNOREZ, ignorez);
			chams::force_material(colour, material);
		};

		imaterial* visible_mat = nullptr;
		imaterial* hidden_mat = nullptr;
		switch (vars::chams::chams_model) {
			case 0: { // Flat chams
				visible_mat = ctx::visible_flattened;
				hidden_mat = ctx::hidden_flattened;
			} break;
			case 1: { // Textured chams
				visible_mat = ctx::visible_textured;
				hidden_mat = ctx::hidden_textured;
			} break;
			case 2: { // Wireframe chams
				visible_mat = ctx::visible_wireframe;
				hidden_mat = ctx::hidden_wireframe;
			} break;
			case 3: { // Shiny chams (<3)
				visible_mat = ctx::visible_shine;
				hidden_mat = ctx::hidden_shine;
			} break;
			case 4: { // bro i forgot to mount css!!
				visible_mat = ctx::visible_error;
				hidden_mat = ctx::hidden_error;
			}
			default: break;
		}
		if (vars::visuals::enemy_chams || vars::visuals::team_chams) {
			for (int i = 1; i < interfaces::entitylist->get_highest_entity_index(); i++) {
				c_base_player* entity = reinterpret_cast<c_base_player*>(interfaces::entitylist->get_client_entity(i));

				if (!entity || entity->is_dead() || entity->is_dormant() || entity->get_client_class()->class_id != 246 || entity == ctx::local_player) {
					continue;
				}

				if ((entity->get_team_num() == ctx::local_player->get_team_num()) && !vars::visuals::team_chams && !utilities::is_developer(entity)) {
					continue;
				}

				if (utilities::has_condition(entity, 16) || utilities::has_condition(entity, 8192)) {
					continue;
				}

				bool is_teammate = entity->get_team_num() == ctx::local_player->get_team_num() && !utilities::is_developer(entity);

				colour_t chosenvisibleenemy = colour_t::from_hsb(vars::colour::enemy_chams_vis / 100, 1.f, 1.f);

				colour_t chosenvisibleteam = colour_t::from_hsb(vars::colour::team_chams_vis / 100, 1.f, 1.f);

				colour_t chosenhiddenenemy = colour_t::from_hsb(vars::colour::enemy_chams_inv / 100, 1.f, 1.f);

				colour_t chosenhiddenteam = colour_t::from_hsb(vars::colour::team_chams_invis / 100, 1.f, 1.f);

				colour_t visible_colour = is_teammate ? colour_t(chosenvisibleteam) : colour_t(chosenvisibleenemy);
				colour_t hidden_colour = is_teammate ? colour_t(chosenhiddenteam) : colour_t(chosenhiddenenemy);

				if (utilities::has_condition(entity, 32) || utilities::has_condition(entity, 16384) || utilities::has_condition(entity, 268435456)) {
					visible_colour = colour_t(125, 125, 125);
					hidden_colour = colour_t(85, 85, 85);

					if (vars::chams::chams_activation == 2) {
						if (vars::chams::chams_model != 5) {
							force_material_thanks(hidden_mat, hidden_colour, true);
						}
						else
						{
							force_material_thanks(hidden_mat, colour_t(255, 255, 255), true);
						}
						entity->draw_model(0x1);
						interfaces::model_render->force_material_override(nullptr);
					}
					if (vars::chams::chams_model != 5) {
						force_material_thanks(visible_mat, visible_colour, false);
					}
					else
					{
						force_material_thanks(visible_mat, colour_t(255, 255, 255), false);
					}
					entity->draw_model(0x1);
					interfaces::model_render->force_material_override(nullptr);
					continue;
				}

				if (utilities::has_condition(entity, 2048)) {
					visible_colour = colour_t(255, 150, 0);
					hidden_colour = colour_t(200, 100, 0);

					if (vars::chams::chams_activation == 2) {
						force_material_thanks(hidden_mat, hidden_colour, true);
						entity->draw_model(0x1);
						interfaces::model_render->force_material_override(nullptr);
					}
					force_material_thanks(visible_mat, visible_colour, false);
					entity->draw_model(0x1);
					interfaces::model_render->force_material_override(nullptr);
					continue;
				}

				if (utilities::is_developer(entity)) {
					visible_colour = colour_t(0, 100, 200);
					hidden_colour = colour_t(200, 0, 200);
				}

				if (vars::chams::chams_activation == 2) {
					force_material_thanks(hidden_mat, hidden_colour, true);
					entity->draw_model(0x1);
					interfaces::model_render->force_material_override(nullptr);
				}

				force_material_thanks(visible_mat, visible_colour, false);
				entity->draw_model(0x1);
				interfaces::model_render->force_material_override(nullptr);
			}
		}

		if (vars::chams::wearables && (vars::visuals::enemy_chams || vars::visuals::team_chams)) {
			for (int i = 1; i <= interfaces::entitylist->get_highest_entity_index(); i++) {
				c_base_player* entity = reinterpret_cast<c_base_player*>(interfaces::entitylist->get_client_entity(i));

				if (!entity || entity->is_dead() || entity->is_dormant()) {
					continue;
				}

				if ((entity->get_team_num() == ctx::local_player->get_team_num()) && !vars::visuals::team_chams) {
					continue;
				}

				const char* model_name = entity->get_model_name();

				colour_t chosenhiddenenemy = (entity->get_team_num() == ctx::local_player->get_team_num()) ? colour_t::from_hsb(vars::colour::team_chams_invis / 100, 1.f, 1.f) : colour_t::from_hsb(vars::colour::enemy_chams_inv / 100, 1.f, 1.f);

				if ((entity->get_client_class()->class_id == CTFWearable or entity->get_client_class()->class_id == CTFTauntProp or entity->get_client_class()->class_id == CTFPowerupBottle or (strstr(model_name, "/weapons/c_models") && !(strstr(model_name, "arms")) && !(entity->get_client_class()->class_id == CTFViewModel) && !(entity->get_client_class()->class_id == CTFDroppedWeapon))) and vars::chams::wearables) { //did you know "and" is an actual c++ keyword? that's funny isn't it?
					if (vars::chams::chams_activation == 2) {
						force_material_thanks(hidden_mat, chosenhiddenenemy, true);
						entity->draw_model(0x1);
						interfaces::model_render->force_material_override(nullptr);
					}
					force_material_thanks(visible_mat, chosenhiddenenemy, false);
					entity->draw_model(0x1);
					interfaces::model_render->force_material_override(nullptr);
				}
			}
		}

		if (vars::visuals::building_chams) {
			for (int i = 1; i <= interfaces::entitylist->get_highest_entity_index(); i++) {
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

					bool is_teammate = entity->get_team_num() == ctx::local_player->get_team_num() && entity_builder != ctx::local_player && !utilities::is_developer(entity_builder);
					colour_t visible_colour = is_teammate ? colour_t(200, 200, 0) : colour_t::from_hsb(vars::colour::building_chams_vis / 100, 1.f, 1.f);;
					colour_t hidden_colour = is_teammate ? colour_t(185, 0, 0) : colour_t::from_hsb(vars::colour::building_chams_inv / 100, 1.f, 1.f);;

					if (vars::visuals::building_enemyonly && is_teammate) {
						continue;
					}

					if (vars::chams::local_player && entity_builder == ctx::local_player) {
						visible_colour = colour_t(195, 140, 145);
						hidden_colour = colour_t(255, 255, 255);
					}

					if (vars::chams::chams_activation == 2) {
						force_material_thanks(hidden_mat, hidden_colour, true);
						entity->draw_model(0x1);
						interfaces::model_render->force_material_override(nullptr);

					}

					force_material_thanks(visible_mat, visible_colour, false);
					entity->draw_model(0x1);
					interfaces::model_render->force_material_override(nullptr);
				}
			}
		}
	
		for (int i = 1; i < interfaces::entitylist->get_highest_entity_index(); i++) {
			c_base_player* entity = reinterpret_cast<c_base_player*>(interfaces::entitylist->get_client_entity(i));
			if (!entity || entity->is_dead() || entity->is_dormant()) {
				continue;
			}

			colour_t visible_colour = colour_t(255, 255, 255);
			colour_t hidden_colour  = colour_t(255, 255, 255);
			bool is_health = false, is_ammo = false, is_projectile = false, is_money = false, is_tank = false, is_cart = false;
			const char* model_name = entity->get_model_name();
			int model_id = entity->get_client_class()->class_id;
			if ((!strcmp(model_name, "models/items/medkit_small.mdl") ||
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
				!strcmp(model_name, "models/items/plate_robo_sandwich.mdl")) && vars::visuals::world_health_chams) {
				visible_colour = colour_t::from_hsb(vars::colour::health_chams / 100, 1.f, 1.f);
				hidden_colour  = colour_t::from_hsb(vars::colour::health_chams / 100, 1.f, 1.f);
				is_health = true;
			}

			if ((!strcmp(model_name, "models/items/ammopack_small.mdl") ||
				!strcmp(model_name, "models/items/ammopack_small_bday.mdl") ||
				!strcmp(model_name, "models/items/ammopack_medium.mdl") ||
				!strcmp(model_name, "models/items/ammopack_medium_bday.mdl") ||
				!strcmp(model_name, "models/items/ammopack_large.mdl") ||
				!strcmp(model_name, "models/items/ammopack_large_bday.mdl") ||
				!strcmp(model_name, "models/items/tf_gift.mdl") ||
				!strcmp(model_name, "models/props_halloween/halloween_gift.mdl")) && vars::visuals::world_ammo_chams) {
				visible_colour = colour_t::from_hsb(vars::colour::ammo_chams / 100, 1.f, 1.f);
				hidden_colour  = colour_t::from_hsb(vars::colour::ammo_chams / 100, 1.f, 1.f);
				is_ammo = true;
			}

			if ((model_id == CBaseProjectile || model_id == CTFBaseProjectile || model_id == CTFBaseRocket ||
				model_id == CTFProjectile_Arrow || model_id == CTFProjectile_BallOfFire || model_id == CTFProjectile_Cleaver ||
				model_id == CTFProjectile_EnergyBall || model_id == CTFProjectile_EnergyRing || model_id == CTFProjectile_Flare ||
				model_id == CTFProjectile_GrapplingHook || model_id == CTFProjectile_HealingBolt || model_id == CTFProjectile_Jar ||
				model_id == CTFProjectile_JarGas || model_id == CTFProjectile_JarMilk || model_id == CTFProjectile_MechanicalArmOrb ||
				model_id == CTFProjectile_Rocket || model_id == CTFProjectile_SentryRocket || model_id == CTFProjectile_Throwable ||
				model_id == CTFProjectile_ThrowableBreadMonster || model_id == CTFProjectile_ThrowableBrick || model_id == CTFProjectile_ThrowableRepel ||
				model_id == CBaseGrenade || model_id == CTFGrenadePipebombProjectile || model_id == CTFWeaponBaseGrenadeProj) && vars::visuals::world_projectile_chams) {
				visible_colour = colour_t::from_hsb(vars::colour::projectile_chams / 100, 1.f, 1.f);
				hidden_colour = colour_t::from_hsb(vars::colour::projectile_chams / 100, 1.f, 1.f);
				is_projectile = true;
			}

			if (model_id == CCurrencyPack && vars::visuals::world_money_chams) {
				is_money = true;
				visible_colour = colour_t::from_hsb(vars::colour::money_chams / 100, 1.f, 1.f);
				hidden_colour = colour_t::from_hsb(vars::colour::money_chams / 100, 1.f, 1.f);
			}

			if ((model_id == CTFTankBoss or 
				!strcmp(model_name, "models/bots/boss_bot/tank_track_L.mdl") or
				!strcmp(model_name, "models/bots/boss_bot/tank_track_R.mdl")) && vars::visuals::world_tank_chams) {
				is_tank = true; 
				visible_colour = colour_t::from_hsb(vars::colour::tank_chams / 100, 1.f, 1.f);
				hidden_colour = colour_t::from_hsb(vars::colour::tank_chams / 100, 1.f, 1.f);
			}

			//if (model_id == 67) {
			//	is_cart = true;
			//}

			if (is_health || is_ammo || is_projectile || is_money || is_tank || is_cart) {
				if (vars::chams::chams_activation == 2) {
					force_material_thanks(hidden_mat, hidden_colour, true);
					entity->draw_model(0x1);
					interfaces::model_render->force_material_override(nullptr);
				}

				force_material_thanks(visible_mat, visible_colour, false);
				entity->draw_model(0x1);
				interfaces::model_render->force_material_override(nullptr);
			}

		}
		colour_t local = colour_t::from_hsb(vars::colour::chams_self / 100, 1.f, 1.f);

		if (vars::chams::local_player && (ctx::in_thirdperson or ctx::local_player->get_condnew() & TFCond_Taunting)) {
			if (vars::chams::chams_activation == 2) {
				force_material_thanks(visible_mat, local, false);
				ctx::local_player->draw_model(0x1);
				interfaces::model_render->force_material_override(nullptr);
			}

			force_material_thanks(visible_mat, local, false);
			ctx::local_player->draw_model(0x1);
			interfaces::model_render->force_material_override(nullptr);

		}

	}
}

// TODO: Get netvar proxys to work.
namespace netvar_manager_hook {
	void netvar_manager_hook::yaw_angles_hook(const CRecvProxyData* data, void* structure, void* out) {
		float yaw = data->m_Value.m_Float;

		c_base_player* entity = (c_base_player*)structure;
		if (!ctx::local_player)
			return;

		if (!entity || entity->is_dead() || entity->is_dormant() || entity->get_client_class()->class_id != 246) {
			return;
		}

		if (GetAsyncKeyState(VK_MENU)) { //if im holding alt do what...?
			vec3_t viewangles; interfaces::engineclient->get_view_angles(viewangles);
			auto at_target_yaw = utilities::calc_angle(entity->get_abs_origin(), ctx::local_player->get_abs_origin()).y;

			auto normalize_yaw = [&](float yaw) {
				if (yaw > 180) {
					yaw -= (round(yaw / 360) * 360.f);
				} else if (yaw < -180) {
					yaw += (round(yaw / 360) * -360.f);
				} return yaw;
			};

			auto delta = normalize_yaw(viewangles.y - at_target_yaw);
			auto right_delta = at_target_yaw + 90.0f; auto left_delta = at_target_yaw - 90.0f;

			if (delta > 0) {
				yaw += right_delta;
			} else {
				yaw -= left_delta;
			}
		}

		*(float*)(out) = yaw;
	}
	void netvar_manager_hook::initialize_netvar_hooks() {
		netvar_manager_hook::hook(xorstr("DT_TFPlayer"), xorstr("m_angEyeAngles[1]"), yaw_angles_hook);
	}
	void netvar_manager_hook::hook(const char* table_to_hook, const char* prop_to_hook, netvar_hook_callback callback) {
		client_class* client_class = interfaces::client->get_all_classes();
		while (client_class) {
			const char* table_name = client_class->table->m_pNetTableName;
			if (!strcmp(table_name, table_to_hook)) {
				for (int i = 0; i < client_class->table->m_nProps; i++) {
					RecvProp &prop = client_class->table->m_pProps[i];
					const char* prop_name = prop.m_pVarName;
					if (!strcmp(prop_name, prop_to_hook)) {
						prop.m_ProxyFn = callback;
					}
				}
			}
		}
	}
}

bool is_headshot_weapon(c_base_player* local_player, c_base_weapon* local_weapon) {
	if (local_weapon->get_slot() == 0 && local_player->get_class_num() == TF2_Sniper)
		if (local_weapon->get_item_definition_index() != WPN_Huntsman && local_weapon->get_item_definition_index() != WPN_CompoundBow)
			return true;

	if (local_weapon->get_item_definition_index() == WPN_Ambassador || local_weapon->get_item_definition_index() == WPN_FestiveAmbassador)
		return true;

	return false;
}

class tf_matchgroup_description {
public:
	char pad0[4];
	int m_iID;
	char pad1[63];
	bool m_bForceCompetitiveSettings;
};

tf_matchgroup_description* get_match_group_description(int& idx)
{
	typedef tf_matchgroup_description* (_cdecl* get_them_fn)(int&);
	static get_them_fn get_them = (get_them_fn)(utilities::find_signature(xorstr("client.dll"), xorstr("55 8B EC 8B 45 08 8B 00 83 F8 FF")));
	return get_them(idx);
}

namespace setup {
	void setup::setup_interfaces() {
		create_interface_fn client	= reinterpret_cast<create_interface_fn>(GetProcAddress(GetModuleHandleA(xorstr("client.dll")), xorstr("CreateInterface")));
		create_interface_fn engine	= reinterpret_cast<create_interface_fn>(GetProcAddress(GetModuleHandleA(xorstr("engine.dll")), xorstr("CreateInterface")));
		create_interface_fn vgui	= reinterpret_cast<create_interface_fn>(GetProcAddress(GetModuleHandleA(xorstr("vgui2.dll")), xorstr("CreateInterface")));
		create_interface_fn mat		= reinterpret_cast<create_interface_fn>(GetProcAddress(GetModuleHandleA(xorstr("vguimatsurface.dll")), xorstr("CreateInterface")));
		create_interface_fn mat_sys = reinterpret_cast<create_interface_fn>(GetProcAddress(GetModuleHandleA(xorstr("materialsystem.dll")), xorstr("CreateInterface")));
		create_interface_fn convar	= reinterpret_cast<create_interface_fn>(GetProcAddress(GetModuleHandleA(xorstr("vstdlib.dll")), xorstr("CreateInterface")));

		interfaces::client			= static_cast<ibaseclientdll*>(client(xorstr("VClient017"), nullptr));
		interfaces::entitylist		= static_cast<icliententitylist*>(client(xorstr("VClientEntityList003"), nullptr));
		interfaces::clientmode		= **(iclientmodeshared***)((*(DWORD**)interfaces::client)[10] + 0x5);
		interfaces::model_info		= static_cast<imodel_info*>(engine(xorstr("VModelInfoClient006"), nullptr));
		interfaces::model_render	= static_cast<imodel_render*>(engine(xorstr("VEngineModel016"), nullptr));
		interfaces::material_system = static_cast<imaterial_system*>(mat_sys(xorstr("VMaterialSystem081"), nullptr));
		interfaces::render_view		= static_cast<irender_view*>(engine(xorstr("VEngineRenderView014"), nullptr));
		interfaces::game_movement	= static_cast<igame_movement*>(client(xorstr("GameMovement001"), nullptr));
		interfaces::prediction		= static_cast<iprediction*>(client(xorstr("VClientPrediction001"), nullptr));

		interfaces::engineclient = static_cast<ivengineclient*>(engine(xorstr("VEngineClient013"), nullptr));
		interfaces::enginetrace	 = static_cast<ienginetrace*>(engine(xorstr("EngineTraceClient003"), nullptr));
		interfaces::debugoverlay = static_cast<ivdebugoverlay*>(engine(xorstr("VDebugOverlay003"), nullptr));
		interfaces::enginevgui	 = static_cast<ienginevgui*>(engine(xorstr("VEngineVGui001"), nullptr));
		interfaces::random_seed	 = *reinterpret_cast<int32_t**>(utilities::find_signature(xorstr("client.dll"), "C7 05 ? ? ? ? ? ? ? ? 5D C3 8B 40 34") + 0x2);
		interfaces::efx			 = static_cast<ivefx*>(engine(xorstr("VEngineEffects001"), nullptr));

		interfaces::globals		= *reinterpret_cast<global_vars_base**>(utilities::find_signature(xorstr("engine.dll"), xorstr("A1 ? ? ? ? 8B 11 68")) + 8);
		interfaces::glow_object = *reinterpret_cast<glow_manager_t**>   (utilities::find_signature(xorstr("client.dll"), xorstr("8B 0D ? ? ? ? A1 ? ? ? ? 56 8B 37")) + 0x2);
		//interfaces::d3ddevice	= **(DWORD***)(utilities::find_signature(xorstr("shaderapidx9.dll"), xorstr("A1 ?? ?? ?? ?? 50 8B 08 FF 51 0C")) + 1);
		interfaces::surface		= static_cast<isurface*>(mat(xorstr("VGUI_Surface030"), nullptr));
		interfaces::panel		= static_cast<ipanel*>(vgui(xorstr("VGUI_Panel009"), nullptr));

		interfaces::convar		= static_cast<iconvar*>(convar(xorstr("VEngineCvar004"), nullptr));
		interfaces::game_event  = static_cast<igame_event_manager*>(engine(xorstr("GAMEEVENTSMANAGER002"), nullptr));

		ctx::visible_flattened = chams::create_material(false, true, false, false, xorstr("vgui/white_additive"));//interfaces::material_system->find_material("dev/glow_color", TEXTURE_GROUP_MODEL); //chams::create_material(false);
		ctx::hidden_flattened = chams::create_material(true, true, false, false, xorstr("vgui/white_additive"));//interfaces::material_system->find_material("dev/glow_color", TEXTURE_GROUP_MODEL); //chams::create_material(true);

		ctx::visible_textured = chams::create_material(false, false, false, false, xorstr("vgui/white_additive"));//interfaces::material_system->find_material("shaded", "Model textures");
		ctx::hidden_textured  = chams::create_material(true, false, false, false, xorstr("vgui/white_additive"));//interfaces::material_system->find_material("shaded", "Model textures");

		ctx::visible_wireframe = chams::create_material(false, false, true, false, xorstr("vgui/white_additive"));
		ctx::hidden_wireframe = chams::create_material(true, false, true, false, xorstr("vgui/white_additive"));

		ctx::visible_shine = chams::create_material(false, false, false, true, xorstr("vgui/white_additive"));
		ctx::hidden_shine = chams::create_material(true, false, false, true, xorstr("vgui/white_additive"));

		ctx::visible_error = chams::create_material(false, false, false, true, xorstr("effects/conc_normal"));
		ctx::hidden_error = chams::create_material(true, false, false, true, xorstr("effects/conc_normal"));

		dx_render::render().initialize();
		g_Config->Setup();

		for (int i = 0; i < 12; i++) {
			tf_matchgroup_description* match_description = get_match_group_description(i);
			if (!match_description || match_description->m_iID > 9) { 
				continue; 
			}

			if (match_description->m_bForceCompetitiveSettings) { 
				match_description->m_bForceCompetitiveSettings = false; 
			}
		}

		//resolver::get().initialize();
		events::get().initialize();

		//#if ISO_CHROMA
		//chroma::get().initialize();
		//chroma::get().on_inject();
		//#endif

		interfaces::engineclient->client_command(xorstr("con_filter_enable 1"), true);//after initializing events, might as well make console clean right? :)
		interfaces::engineclient->client_command(xorstr(R"(con_filter_text_out "particle")"), true);
		interfaces::engineclient->client_command(xorstr(R"(con_filter_text_out "material")"), true);
		interfaces::engineclient->client_command(xorstr(R"(con_filter_text_out "muzzle")"), true);
		interfaces::engineclient->client_command(xorstr(R"(con_filter_text_out "Cannot")"), true);
		interfaces::engineclient->client_command(xorstr(R"(con_filter_text_out "doesn't")"), true);
		interfaces::engineclient->client_command(xorstr(R"(con_filter_text_out "failed")"), true);
		interfaces::engineclient->client_command(xorstr(R"(con_filter_text_out "Failed to load sound ")"), true);
		interfaces::engineclient->client_command(xorstr(R"(con_filter_text_out "No such")"), true);
		interfaces::engineclient->client_command(xorstr(R"(con_filter_text_out "DataTable")"), true);
		if (!interfaces::engineclient->is_console_visible()) //always open this shit so we can all see my beautiful logo ^^
			interfaces::engineclient->client_command(xorstr("toggleconsole; clear"), true);
		else 
			interfaces::engineclient->client_command(xorstr("clear"), true);
		Beep(523, 100); Sleep(100);
		Beep(1046, 100);
		interfaces::convar->console_print_colour(colour_t(218, 23, 91), xorstr("                    ISOPROPYL                    \n")); //i spent too much time on this ngl
		interfaces::convar->console_print_colour(colour_t(218, 23, 91), xorstr("                #################                \n"));
		interfaces::convar->console_print_colour(colour_t(218, 23, 91), xorstr("            ###)                 (###            \n"));
		interfaces::convar->console_print_colour(colour_t(218, 23, 91), xorstr("         ##                           ##         \n"));
		interfaces::convar->console_print_colour(colour_t(218, 23, 91), xorstr("       ###  ##                     ##  ###       \n"));
		interfaces::convar->console_print_colour(colour_t(218, 23, 91), xorstr("     ###     ####               ####     ###     \n"));
		interfaces::convar->console_print_colour(colour_t(218, 23, 91), xorstr("    ##       &#  ##*         ,##  ##       ##    \n"));
		interfaces::convar->console_print_colour(colour_t(218, 23, 91), xorstr("   ##         #&    ##     ##    /#         ##   \n"));
		interfaces::convar->console_print_colour(colour_t(218, 23, 91), xorstr("  ##           #      .## *      #           ##  \n"));
		interfaces::convar->console_print_colour(colour_t(218, 23, 91), xorstr(" .#            &#    ##& /##    ##           .#* \n"));
		interfaces::convar->console_print_colour(colour_t(218, 23, 91), xorstr(" ##             #(*##       ###*#             ## \n"));
		interfaces::convar->console_print_colour(colour_t(218, 23, 91), xorstr(" ##             ##             # .            ## \n"));
		interfaces::convar->console_print_colour(colour_t(218, 23, 91), xorstr(" ##          ##, &#    ISO    ##  ##          *# \n"));
		interfaces::convar->console_print_colour(colour_t(218, 23, 91), xorstr(" ##       &##     #*          #     &##       ## \n"));
		interfaces::convar->console_print_colour(colour_t(218, 23, 91), xorstr(" ##    ,##         #         #,        ##&    ## \n"));
		interfaces::convar->console_print_colour(colour_t(218, 23, 91), xorstr(" .#  #######################################. #/ \n"));
		interfaces::convar->console_print_colour(colour_t(218, 23, 91), xorstr("  ##                #*      #                ##  \n"));
		interfaces::convar->console_print_colour(colour_t(218, 23, 91), xorstr("   ##                #     #*               ##   \n"));
		interfaces::convar->console_print_colour(colour_t(218, 23, 91), xorstr("    ##               ##   &#               ##    \n"));
		interfaces::convar->console_print_colour(colour_t(218, 23, 91), xorstr("     *##              #   #              .##     \n"));
		interfaces::convar->console_print_colour(colour_t(218, 23, 91), xorstr("       ###            .# #&            *##       \n"));
		interfaces::convar->console_print_colour(colour_t(218, 23, 91), xorstr("         ###           ###           ###         \n"));
		interfaces::convar->console_print_colour(colour_t(218, 23, 91), xorstr("            ###,        #        .###.           \n"));
		interfaces::convar->console_print_colour(colour_t(218, 23, 91), xorstr("               .#######&.&#######,               \n"));
		interfaces::convar->console_print_colour(colour_t(218, 23, 91), xorstr("                  HAS INJECTED!                  \n"));

		if (interfaces::convar->find_variable(xorstr("cl_flipviewmodels"))) {
			if (interfaces::convar->find_variable(xorstr("cl_flipviewmodels"))->m_flags &   (1 << 22)) { //if the cvar has the flag FCVAR_NOT_CONNECTED (you can't change it while connected)
				interfaces::convar->find_variable(xorstr("cl_flipviewmodels"))->m_flags &= ~(1 << 22); //remove that shit dawg
			}
		}

		if (interfaces::convar->find_variable(xorstr("r_screenoverlay"))) {
			if (interfaces::convar->find_variable(xorstr("r_screenoverlay"))->m_flags &   (1 << 4)) {
				interfaces::convar->find_variable(xorstr("r_screenoverlay"))->m_flags &= ~(1 << 4); //unhide
			}
			if (interfaces::convar->find_variable(xorstr("r_screenoverlay"))->m_flags &   (1 << 14)) {
				interfaces::convar->find_variable(xorstr("r_screenoverlay"))->remove_flags(1 << 14); //unrestrict...?
			}
			if (!interfaces::convar->find_variable(xorstr("r_screenoverlay"))->is_flag_set(1 << 30)) {
				interfaces::convar->console_print_colour(colour_t(218, 23, 91), xorstr("DBGDBG: 1 << 30 on screenoverlay\n"));
				interfaces::convar->find_variable(xorstr("r_screenoverlay"))->add_flags(1 << 30);
			}
			
		}

	}
	void setup::setup_hooks() {

		if (interfaces::clientmode != nullptr) {
			hook::client_mode_hook = std::make_unique<vmt_manager>(interfaces::clientmode);
			//hook::client_mode_hook->hook_function(reinterpret_cast<void*>(hooks::override_mouse_input), 20);
			hook::client_mode_hook->hook_function(reinterpret_cast<bool*>(hooks::create_move), 21);
			//hook::client_mode_hook->hook_function(reinterpret_cast<float*>(hooks::get_viewmodel_fov), 32);
			hook::client_mode_hook->hook_function(reinterpret_cast<float*>(hooks::override_view), 16);
		}
		if (interfaces::client != nullptr) {
			hook::client_hook = std::make_unique<vmt_manager>(interfaces::client);
			//hook::client_hook->hook_function(reinterpret_cast<int*>(hooks::key_event), 20);
			hook::client_hook->hook_function(reinterpret_cast<int*>(hooks::frame_stage_notify), 35);
		}
		if (interfaces::panel != nullptr) {
			hook::panel_hook = std::make_unique<vmt_manager>(interfaces::panel);
			hook::panel_hook->hook_function(reinterpret_cast<void*>(hooks::paint_traverse), 41);
		}
		if (interfaces::render_view != nullptr) {
			hook::render_hook = std::make_unique<vmt_manager>(interfaces::render_view);
			hook::render_hook->hook_function(reinterpret_cast<void*>(hooks::scene_end), 9);
		}

	}

	void setup::unhook() {
		HMODULE module;
		//interfaces::panel->set_mouse_input_enabled(3, true);
		hook::client_mode_hook->un_hook_function(21);
		hook::client_mode_hook->un_hook_function(16);
		hook::client_mode_hook->un_hook_function(23);

		hook::client_hook->un_hook_function(35);

		hook::panel_hook->un_hook_function(41);

		hook::render_hook->un_hook_function(9);
		hook::render_hook->un_hook_function(19);
		chroma::get().kill();
		Sleep(150);
		//FreeLibraryAndExitThread(module, 0);
		exit(0);
	}

	bool setup::ready_to_blast() {
		if (interfaces::client == nullptr)
			return true;
		return false;
	}
}	