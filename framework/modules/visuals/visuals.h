#pragma once

namespace visuals {
	void render_esp();
	void render_welcome();
	void render_hud();
	void render_watermark();
	void create_player_esp(c_base_player* entity);
	void create_building_esp(c_base_player* entity);
	void create_item_esp(c_base_player* entity);

	bool world_to_screen(vec3_t &origin, vec3_t &screen);
	bool create_bounds(c_base_player* entity, float &x, float &y, float &w, float &h);
	void render_glow_on_pdus(c_base_player* local);
	void render_glow_on_frs(c_base_player* local);
	void render_fake_hitboxes();
}