#ifndef MENU_H
#define MENU_H
#ifdef _WIN32
#pragma once
#endif

struct ColorV2
{
	float red, green, blue, alpha;
};

struct multidropdownitem_t {
	bool enabled;
	std::string name;

	multidropdownitem_t(bool n_enabled, std::string n_name) {
		enabled = n_enabled;
		name = n_name;
	}
};

enum tabs {
	aimbot_tab = 0,
	visual_tab = 1,
	misc_tab = 2,
	anti_aim_tab = 3,
	chroma_tab = 4,
	config_tab = 5
};

class c_menu : public singleton<c_menu> {
public:
	bool menu_opened;
	vec2_t mouse_pos();
	bool mouse_in_params(int x, int y, int x2, int y2);
	void draw(unsigned int panel);
	void draw_radar();
	void draw_spectatorlist();
	void draw_playerlist();
private:
	vec3_t _pos = vec3_t(500, 200, 0);
	vec3_t _pos_radar = vec3_t(0, 0, 0);
	vec2_t _pos_spec = vec2_t(300, 800);
	int radar_width;
	int radar_height;
	int screen_width;
	int screen_height;
	int y_offset;
	int x_offset;
	int groupbox_height;
	int menu_alpha_main;
	int menu_alpha_text;
	int groupbox_scroll_add;
	int groupbox_width;
	int groupbox_bottom;
	int groupbox_top;
	colour_t custom_ui_colour;
	colour_t custom_ui_colour_noalpha;
	bool combobox_open;
	void groupbox(int x, int y, int w, int h, std::string name);
	void checkbox(std::string name, bool* item);
	void checkbox_colour(std::string name, bool* item, float* colour);
	void checkbox_colour_dual(std::string name, bool* item, float* colour1, float* colour2);
	void button(std::string name, bool* item, bool rebound);
	void label(std::string name);
	void combobox(int size, std::string name, std::string *itemname, int* item);
	void multicombo(std::vector<bool> bools, std::string name, std::string *itemnames);
	void keybind(int item);
	void slider(int max, std::string name, int * item, std::string suffix, int step);
	void slider(int min, int max, std::string name, int* item, std::string suffix, int step);
	void colourpicker(std::string name, colour_t item);
};

#endif // !MENU_H