#include "../main.h"

int spectator_count = 0;
int player_count = 0;

//set up the palette
//these are not constexprs simply because of intellisense
//but unfortunately there's nothing better you can get at the moment.
#define menu_accent colour_t(218, 23, 91, menu_alpha_main)
#define menu_accent_bottomgrad colour_t(129, 14, 54, menu_alpha_main)
#define menu_background colour_t(23, 20, 27, menu_alpha_main)
#define menu_background_bottomgrad colour_t(33, 30, 37, menu_alpha_main)
#define menu_text colour_t(186, 183, 194, menu_alpha_text)
#define menu_div1 colour_t(136, 93, 113, menu_alpha_main)
#define menu_div2 colour_t(59, 56, 63, menu_alpha_main)

// This is only ever used here, so I'm moving it from 
// main.h to here to speed up compile time.
#ifndef _AUTOMATED
#define USER xorstr("alexis hyland")
#endif


int get_fakelag_ticks() {
	unsigned int tick_amount = interfaces::convar->find_variable(xorstr("sv_maxusrcmdprocessticks"))->get_float();
	//if (!global::local_player || !interfaces::engineclient->is_connected())
	//	return 14;
	//else if (tick_amount == 0) //unlimited power!!! lmao
	//	return 62;
	//else
	//	return tick_amount;
	return 14;
}

RECT get_text_size(DWORD font, const char* text, ...) {
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

void c_menu::colourpicker(std::string name, colour_t item)
{
	
}

void c_menu::checkbox(std::string name, bool* item)
{
	if (groupbox_bottom <= y_offset + 16)
		return;

	if (groupbox_top >= y_offset + 16)
		return;

	int size = 12;

	static bool pressed = false;

	auto text_size = get_text_size(font::menu_font, name.c_str());

	if (c_menu::get().menu_opened) {
		if (!GetAsyncKeyState(VK_LBUTTON) && mouse_in_params(x_offset + 7, y_offset, size + text_size.right, size))
		{
			if (pressed)
				*item = !*item;
			pressed = false;
		}

		if (GetAsyncKeyState(VK_LBUTTON) && mouse_in_params(x_offset + 7, y_offset, size + text_size.right, size) && !pressed && !combobox_open) {
			pressed = true;
		}
	}

	if (*item == true) {
		draw::draw_rect_gradient(x_offset + 8, y_offset, size, size, menu_background, menu_background_bottomgrad, false);
		if (!vars::cheat::custom_ui_colour)
		{
			draw::draw_rect_gradient(x_offset + 8 + 2, y_offset + 2, size - 4, size - 4, menu_accent, menu_accent_bottomgrad, false);
		}
		else
		{
			draw::draw_filled_rect(x_offset + 8 + 2, y_offset + 2, size - 4, size - 4, custom_ui_colour);
		}
	}
	else
		draw::draw_rect_gradient(x_offset + 8, y_offset, size, size, menu_background, menu_background_bottomgrad, false);

	draw::draw_outlined_rect(x_offset + 8, y_offset, size, size, menu_div2);
	draw::draw_string(x_offset + 29, y_offset - 2, menu_text, font::menu_font, name.c_str());

	if (c_menu::get().menu_opened) {
		if (mouse_in_params(x_offset + 7, y_offset, size + text_size.right, size)) {
			draw::draw_outlined_rect(x_offset + 9, y_offset + 1, 10, 10, menu_div2);
		}
	}
	y_offset += 16;
}

void c_menu::checkbox_colour(std::string name, bool* item, float* colour)
{
	if (groupbox_bottom <= y_offset + 16)
		return;

	if (groupbox_top >= y_offset + 16)
		return;

	int size = 12;

	static bool pressed = false;
	static bool lpressed = false;
	static bool rpressed = false;

	auto text_size = get_text_size(font::menu_font, name.c_str());

	if (c_menu::get().menu_opened) {
		if (!GetAsyncKeyState(VK_LBUTTON) && mouse_in_params(x_offset + 7, y_offset, size + text_size.right, size))
		{
			if (pressed)
				*item = !*item;
			pressed = false;
		}

		if (GetAsyncKeyState(VK_LBUTTON) && mouse_in_params(x_offset + 7, y_offset, size + text_size.right, size) && !pressed && !combobox_open) {
			pressed = true;
		}

		if (!GetAsyncKeyState(VK_LBUTTON) && mouse_in_params(x_offset + 155 - 1, y_offset + 2 - 1, 16 + 2, 8 + 2))
		{
			if (lpressed)
				*colour += 5; //10
			if (*colour > 95) //90
				*colour = 0;
			lpressed = false;
		}

		if (GetAsyncKeyState(VK_LBUTTON) && mouse_in_params(x_offset + 155 - 1, y_offset + 2 - 1, 16 + 2, 8 + 2) && !lpressed)
			lpressed = true;

		if (!GetAsyncKeyState(VK_RBUTTON) && mouse_in_params(x_offset + 155 - 1, y_offset + 2 - 1, 16 + 2, 8 + 2))
		{
			if (rpressed)
				*colour -= 5; //10
			if (*colour < 0) 
				*colour = 95; //90
			rpressed = false;
		}

		if (GetAsyncKeyState(VK_RBUTTON) && mouse_in_params(x_offset + 155 - 1, y_offset + 2 - 1, 16 + 2, 8 + 2) && !rpressed)
			rpressed = true;
	}

	if (*item == true) {
		draw::draw_rect_gradient(x_offset + 8, y_offset, size, size, menu_background, menu_background_bottomgrad, false);
		if (!vars::cheat::custom_ui_colour)
		{
			draw::draw_rect_gradient(x_offset + 8 + 2, y_offset + 2, size - 4, size - 4, menu_accent, menu_accent_bottomgrad, false);
		}
		else
		{
			draw::draw_filled_rect(x_offset + 8 + 2, y_offset + 2, size - 4, size - 4, custom_ui_colour);
		}
		//draw::draw_rect_gradient(x_offset + 8 + 2, y_offset + 2, size - 4, size - 4, vars::cheat::custom_ui_colour ? custom_ui_colour : menu_accent, menu_accent_bottomgrad, menu_alpha_main);
	}
	else
		draw::draw_rect_gradient(x_offset + 8, y_offset, size, size, menu_background, menu_background_bottomgrad, false);

	draw::draw_outlined_rect(x_offset + 8, y_offset, size, size, menu_div2);
	draw::draw_string(x_offset + 29, y_offset - 2, menu_text, font::menu_font, name.c_str());

	if (c_menu::get().menu_opened) {
		if (mouse_in_params(x_offset + 7, y_offset, size + text_size.right, size)) {
			draw::draw_outlined_rect(x_offset + 9, y_offset + 1, 10, 10, menu_div2);
		}

		//if (mouse_in_params(x_offset + 155, y_offset + 2, 16, 8)) {
		//	draw::draw_string_centered(mouse_pos().x, mouse_pos().y + 15, colour_t(255, 255, 255), font::calibri, "mouse on colour");
		//}
		//if (lpressed || rpressed)
		//	draw::draw_string_centered(mouse_pos().x, mouse_pos().y + 24, colour_t(255, 255, 255), font::calibri, "pressed");
	}
	colour_t box_colour = colour_t::from_hsb((float)*colour / 100, 1.f, 1.f);
	colour_t box_colour_grad = colour_t::from_hsb((float)*colour / 100, 1.f, 0.7f);

	draw::draw_rect_gradient(x_offset + 155, y_offset + 2, 16, 8, colour_t(box_colour.r(), box_colour.g(), box_colour.b(), menu_alpha_main), colour_t(box_colour_grad.r(), box_colour_grad.g(), box_colour_grad.b(), menu_alpha_main), false);
	draw::draw_outlined_rect(x_offset + 155 - 1, y_offset + 2 - 1, 16 + 2, 8 + 2, menu_div2);

	y_offset += 16;
}

void c_menu::checkbox_colour_dual(std::string name, bool* item, float* colour1, float* colour2)
{
	if (groupbox_bottom <= y_offset + 16)
		return;

	if (groupbox_top >= y_offset + 16)
		return;

	int size = 12;

	static bool pressed = false;
	static bool lpressed1 = false;
	static bool rpressed1 = false;
	static bool lpressed2 = false;
	static bool rpressed2 = false;

	auto text_size = get_text_size(font::menu_font, name.c_str());

	if (c_menu::get().menu_opened) {
		if (!GetAsyncKeyState(VK_LBUTTON) && mouse_in_params(x_offset + 7, y_offset, size + text_size.right, size))
		{
			if (pressed)
				*item = !*item;
			pressed = false;
		}

		if (GetAsyncKeyState(VK_LBUTTON) && mouse_in_params(x_offset + 7, y_offset, size + text_size.right, size) && !pressed && !combobox_open) {
			pressed = true;
		}

		if (!GetAsyncKeyState(VK_LBUTTON) && mouse_in_params(x_offset + 135 - 1, y_offset + 2 - 1, 16 + 2, 8 + 2))
		{
			if (lpressed1)
				*colour1 += 5; //10
			if (*colour1 > 95) //90
				*colour1 = 0;
			lpressed1 = false;
		}

		if (GetAsyncKeyState(VK_LBUTTON) && mouse_in_params(x_offset + 135 - 1, y_offset + 2 - 1, 16 + 2, 8 + 2) && !lpressed1)
			lpressed1 = true;

		if (!GetAsyncKeyState(VK_RBUTTON) && mouse_in_params(x_offset + 135 - 1, y_offset + 2 - 1, 16 + 2, 8 + 2))
		{
			if (rpressed1)
				*colour1 -= 5; //10
			if (*colour1 < 0)
				*colour1 = 95; //90
			rpressed1 = false;
		}

		if (GetAsyncKeyState(VK_RBUTTON) && mouse_in_params(x_offset + 135 - 1, y_offset + 2 - 1, 16 + 2, 8 + 2) && !rpressed1)
			rpressed1 = true;


		if (!GetAsyncKeyState(VK_LBUTTON) && mouse_in_params(x_offset + 155 - 1, y_offset + 2 - 1, 16 + 2, 8 + 2))
		{
			if (lpressed2)
				*colour2 += 5; //10
			if (*colour2 > 95) //90
				*colour2 = 0;
			lpressed2 = false;
		}

		if (GetAsyncKeyState(VK_LBUTTON) && mouse_in_params(x_offset + 155 - 1, y_offset + 2 - 1, 16 + 2, 8 + 2) && !lpressed2)
			lpressed2 = true;

		if (!GetAsyncKeyState(VK_RBUTTON) && mouse_in_params(x_offset + 155 - 1, y_offset + 2 - 1, 16 + 2, 8 + 2))
		{
			if (rpressed2)
				*colour2 -= 5; //10
			if (*colour2 < 0)
				*colour2 = 95; //90
			rpressed2 = false;
		}

		if (GetAsyncKeyState(VK_RBUTTON) && mouse_in_params(x_offset + 155 - 1, y_offset + 2 - 1, 16 + 2, 8 + 2) && !rpressed2)
			rpressed2 = true;
	}

	if (*item == true) {
		draw::draw_rect_gradient(x_offset + 8, y_offset, size, size, menu_background, menu_background_bottomgrad, false);
		if (!vars::cheat::custom_ui_colour)
		{
			draw::draw_rect_gradient(x_offset + 8 + 2, y_offset + 2, size - 4, size - 4, menu_accent, menu_accent_bottomgrad, false);
		}
		else
		{
			draw::draw_filled_rect(x_offset + 8 + 2, y_offset + 2, size - 4, size - 4, custom_ui_colour);
		}
		//draw::draw_rect_gradient(x_offset + 8 + 2, y_offset + 2, size - 4, size - 4, vars::cheat::custom_ui_colour ? custom_ui_colour : menu_accent, menu_accent_bottomgrad, menu_alpha_main);
	}
	else
		draw::draw_rect_gradient(x_offset + 8, y_offset, size, size, menu_background, menu_background_bottomgrad, false);

	draw::draw_outlined_rect(x_offset + 8, y_offset, size, size, menu_div2);
	draw::draw_string(x_offset + 29, y_offset - 2, menu_text, font::menu_font, name.c_str());

	if (c_menu::get().menu_opened) {
		if (mouse_in_params(x_offset + 7, y_offset, size + text_size.right, size)) {
			draw::draw_outlined_rect(x_offset + 9, y_offset + 1, 10, 10, menu_div2);
		}

		//if (mouse_in_params(x_offset + 155, y_offset + 2, 16, 8)) {
		//	draw::draw_string_centered(mouse_pos().x, mouse_pos().y + 15, colour_t(255, 255, 255), font::calibri, "mouse on colour");
		//}
		//if (lpressed || rpressed)
		//	draw::draw_string_centered(mouse_pos().x, mouse_pos().y + 24, colour_t(255, 255, 255), font::calibri, "pressed");
	}
	colour_t box_colour1 = colour_t::from_hsb((float)*colour1 / 100, 1.f, 1.f);
	colour_t box_colour1_grad = colour_t::from_hsb((float)*colour1 / 100, 1.f, 0.7f);
	colour_t box_colour2 = colour_t::from_hsb((float)*colour2 / 100, 1.f, 1.f);
	colour_t box_colour2_grad = colour_t::from_hsb((float)*colour2 / 100, 1.f, 0.7f);
	draw::draw_rect_gradient(x_offset + 135, y_offset + 2, 16, 8, colour_t(box_colour1.r(), box_colour1.g(), box_colour1.b(), menu_alpha_main), colour_t(box_colour1_grad.r(), box_colour1_grad.g(), box_colour1_grad.b(), menu_alpha_main), false);
	draw::draw_outlined_rect(x_offset + 135 - 1, y_offset + 2 - 1, 16 + 2, 8 + 2, menu_div2);

	draw::draw_rect_gradient(x_offset + 155, y_offset + 2, 16, 8, colour_t(box_colour2.r(), box_colour2.g(), box_colour2.b(), menu_alpha_main), colour_t(box_colour2_grad.r(), box_colour2_grad.g(), box_colour2_grad.b(), menu_alpha_main), false);
	draw::draw_outlined_rect(x_offset + 155 - 1, y_offset + 2 - 1, 16 + 2, 8 + 2, menu_div2);

	y_offset += 16;
}

void c_menu::button(std::string name, bool* item, bool rebound) //
{
	if (groupbox_bottom <= y_offset + 16)
		return;

	if (groupbox_top >= y_offset + 16)
		return;

	int size = 16;

	static bool pressed = false;

	auto text_size = get_text_size(font::menu_font, name.c_str());


	if (c_menu::get().menu_opened) {
		if (GetAsyncKeyState(VK_LBUTTON) && mouse_in_params(x_offset + 24, y_offset, size + 117, size) && !pressed && !combobox_open) {
			pressed = true;
			if (rebound)
				*item = !*item; //and then undo it lol
		}

		else if (!GetAsyncKeyState(VK_LBUTTON) && mouse_in_params(x_offset + 24, y_offset, size + 117, size))
		{
			if (pressed)
				*item = !*item;
			pressed = false;
		}
	}

	if (*item == true)
		if (!vars::cheat::custom_ui_colour)
		{
			draw::draw_rect_gradient(x_offset + 25, y_offset, size + 117, size, menu_accent, menu_accent_bottomgrad, false);
		}
		else
		{
			draw::draw_filled_rect(x_offset + 25, y_offset, size + 117, size, custom_ui_colour);
		}
	else
		draw::draw_rect_gradient(x_offset + 25, y_offset, size + 117, size, menu_background, menu_background_bottomgrad, false);

	draw::draw_outlined_rect(x_offset + 25, y_offset, size + 117, size, menu_div2);

	draw::draw_string_centered(x_offset + 90, y_offset , menu_text, font::menu_font, name.c_str());

	if (c_menu::get().menu_opened) {
		if (mouse_in_params(x_offset + 24, y_offset, size + 117, size)) {
			draw::draw_outlined_rect(x_offset + 26, y_offset + 1, 14 + 117, 14, menu_div2);
		}
	}
	y_offset += 20;
}

void c_menu::label(std::string name)
{
	if (groupbox_bottom <= y_offset + 16)
		return;

	if (groupbox_top >= y_offset + 16)
		return;

	int size = 12;


	auto text_size = get_text_size(font::menu_font, name.c_str());


	draw::draw_string(x_offset + 8, y_offset - 2, menu_text, font::menu_font, name.c_str());
	y_offset += 16;
}

void c_menu::combobox(int size, std::string name, std::string* itemname, int* item)
{
	if (groupbox_bottom <= y_offset + 41)
		return;

	bool pressed = false;
	bool open = false;
	static bool selected_opened = false;
	static bool click_rest;
	static bool rest;
	static std::string name_selected;

	int ComboSize = 125; // 163

	if (c_menu::get().menu_opened) {
		if (open && GetAsyncKeyState(VK_LBUTTON) && !mouse_in_params(x_offset + 29, y_offset + 14 + 21, ComboSize, (size * 16)) && !click_rest) {
			name_selected = name;
			pressed = true;
			click_rest = true;
		}
		else if (GetAsyncKeyState(VK_LBUTTON) && mouse_in_params(x_offset + 25, y_offset + 14, ComboSize, 17) && !click_rest)
		{
			name_selected = name;
			pressed = true;
			click_rest = true;
		}
		else if (!GetAsyncKeyState(VK_LBUTTON) && mouse_in_params(x_offset + 25, y_offset + 14, ComboSize, 17)) {
			click_rest = false;
		}

		if (pressed)
		{
			if (!rest)
				selected_opened = !selected_opened;
			rest = true;
		}
		else
			rest = false;

		if (name_selected == name) {
			open = selected_opened;
			combobox_open = selected_opened;
		}

	}

	draw::draw_string(x_offset + 29, y_offset - 2, menu_text, font::menu_font, name.c_str());
	draw::draw_rect_gradient(x_offset + 29, y_offset + 14, ComboSize, 17, menu_background, menu_background_bottomgrad, false);
	draw::draw_outlined_rect(x_offset + 29, y_offset + 14, ComboSize, 17, menu_div2);

	if (open)
	{
		draw::draw_rect_gradient(x_offset + 29, y_offset + 14 + 21, ComboSize, (size * 16), menu_background, menu_background_bottomgrad, false);
		draw::draw_outlined_rect(x_offset + 29, y_offset + 14 + 21, ComboSize, (size * 16), menu_div2);

		for (int i = 0; i < size; i++)
		{
			if (GetAsyncKeyState(VK_LBUTTON) && mouse_in_params(x_offset + 29, y_offset + 34 + i * 16, ComboSize, 16))
				*item = i;

			if (i == *item)
				draw::draw_string(x_offset + 26 + 10, y_offset + 36 + i * 16, colour_t(255, 255, 255, menu_alpha_text), font::verdana2, itemname[i].c_str());
			else
				draw::draw_string(x_offset + 26 + 10, y_offset + 36 + i * 16, colour_t(155, 155, 155, menu_alpha_text), font::verdana2, itemname[i].c_str());

			if (mouse_in_params(x_offset + 26, y_offset + 35 + i * 16, ComboSize, 16)) {
				draw::draw_string(x_offset + 26 + 10, y_offset + 36 + i * 16, colour_t(200, 200, 200, menu_alpha_text), font::verdana2, itemname[i].c_str());
			}
		}
	}

	draw::draw_line(x_offset + 29 + ComboSize - 14, y_offset + 13 + 7 - 1, x_offset + 29 + ComboSize - 7, y_offset + 13 + 7 - 1, menu_div2); //top of triangle
	draw::draw_line(x_offset + 29 + ComboSize - 14, y_offset + 13 + 7 - 1, x_offset + 25 + ComboSize - 7, y_offset + 13 + 14 - 1, menu_div2); //left side
	draw::draw_line(x_offset + 29 + 6 + ComboSize - 14, y_offset + 13 + 7 - 1, x_offset + 25 + ComboSize - 7, y_offset + 13 + 14 - 1, menu_div2); //right side

	draw::draw_string(x_offset + 36, y_offset + 16, menu_text, font::verdana2, itemname[*item].c_str());

	if (open) 
		y_offset += 42 + (size * 16);
	else
		y_offset += 41;
}

void c_menu::multicombo(std::vector<bool> bools, std::string name, std::string *itemnames)
{
	if (groupbox_bottom <= y_offset + 41)
		return;

	bool pressed = false;
	bool open = false;
	static unsigned int selected_idx;
	static bool selected_opened = false;
	static bool click_rest;
	static bool rest;
	static std::string name_selected;

	int ComboSize = 125; // 163

	if (c_menu::get().menu_opened) { //
		if (open && GetAsyncKeyState(VK_LBUTTON) && !mouse_in_params(x_offset + 29, y_offset + 14 + 21, ComboSize, (bools.size() * 16)) && !click_rest) {
			name_selected = name;
			pressed = true;
			click_rest = true;
		}
		else if (GetAsyncKeyState(VK_LBUTTON) && mouse_in_params(x_offset + 25, y_offset + 14, ComboSize, 17) && !click_rest)
		{
			name_selected = name;
			pressed = true;
			click_rest = true;
		}
		else if (!GetAsyncKeyState(VK_LBUTTON) && mouse_in_params(x_offset + 25, y_offset + 14, ComboSize, 17)) {
			click_rest = false;
		}

		if (pressed)
		{
			if (!rest)
				selected_opened = !selected_opened;
			rest = true;
		}
		else
			rest = false;

		if (name_selected == name) {
			open = selected_opened;
			combobox_open = selected_opened;
		}

	}

	draw::draw_string(x_offset + 29, y_offset - 2, menu_text, font::menu_font, name.c_str());
	draw::draw_rect_gradient(x_offset + 29, y_offset + 14, ComboSize, 17, menu_background, menu_background_bottomgrad, false);
	draw::draw_outlined_rect(x_offset + 29, y_offset + 14, ComboSize, 17, menu_div2);

	if (open)
	{
		draw::draw_rect_gradient(x_offset + 29, y_offset + 14 + 21, ComboSize, (bools.size() * 16), menu_background, menu_background_bottomgrad, false);
		draw::draw_outlined_rect(x_offset + 29, y_offset + 14 + 21, ComboSize, (bools.size() * 16), menu_div2);

		for (int i = 0; i < bools.size(); i++)
		{
			interfaces::convar->console_print("%s: %s\n", itemnames[i].c_str(), bools[i] ? "true" : "false");

			if (GetAsyncKeyState(VK_LBUTTON) && mouse_in_params(x_offset + 29, y_offset + 34 + i * 16, ComboSize, 16))
				selected_idx = i;

			if (bools[i])
				draw::draw_string(x_offset + 26 + 10, y_offset + 36 + i * 16, colour_t(255, 255, 255, menu_alpha_text), font::verdana2, itemnames[i].c_str());
			else
				draw::draw_string(x_offset + 26 + 10, y_offset + 36 + i * 16, colour_t(155, 155, 155, menu_alpha_text), font::verdana2, itemnames[i].c_str());

			if (mouse_in_params(x_offset + 29, y_offset + 34 + i * 16, ComboSize, 16)) {
				draw::draw_string(x_offset + 26 + 10, y_offset + 36 + i * 16, colour_t(200, 200, 200, menu_alpha_text), font::verdana2, itemnames[i].c_str());
			}
		}
	}

	draw::draw_line(x_offset + 29 + ComboSize - 14, y_offset + 13 + 7 - 1, x_offset + 29 + ComboSize - 7, y_offset + 13 + 7 - 1, menu_div2); //top of triangle
	draw::draw_line(x_offset + 29 + ComboSize - 14, y_offset + 13 + 7 - 1, x_offset + 25 + ComboSize - 7, y_offset + 13 + 14 - 1, menu_div2); //left side
	draw::draw_line(x_offset + 29 + 6 + ComboSize - 14, y_offset + 13 + 7 - 1, x_offset + 25 + ComboSize - 7, y_offset + 13 + 14 - 1, menu_div2); //right side


	//draw::draw_string(x_offset + 38, y_offset + 15, menu_text, font::verdana2, itemnames[*item].c_str());

	if (open)
		y_offset += 42 + (bools.size() * 16);
	else
		y_offset += 41;
}

void c_menu::keybind(int item)
{
	if (groupbox_bottom <= y_offset + 41)
		return;
	static std::string keynames[253] = { "", "m1", "m2", "brk", "m3", "m4", "m5",
		"unk", "bspc", "tab", "unk", "unk", "unk", "enter", "unk", "unk", "shft",
		"ctrl", "alt","pause","caps", "unk", "unk", "unk", "unk", "unk", "unk",
		"esc", "unk", "unk", "unk", "unk", "space","pgup", "pgdown", "end", "home", "left",
		"up", "right", "down", "unk", "prnt", "unk", "prtscr", "ins","del", "unk", "0", "1",
		"2", "3", "4", "5", "6", "7", "8", "9", "unk", "unk", "unk", "unk", "unk", "unk",
		"unk", "a", "b", "c", "d", "e", "f", "g", "h", "i", "j", "k", "l", "m", "n", "o", "p", "q", "r", "s", "t", "u",
		"v", "w", "x", "y", "z", "lftwin", "rghtwin", "unk", "unk", "unk", "num0", "num1",
		"num2", "num3", "num4", "num5", "num6","num7", "num8", "num9", "*", "+", "unk", "-", ".", "/", "f1", "f2", "f3",
		"f4", "f5", "f6", "f7", "f8", "f9", "f10", "f11", "f12","f13", "f14", "f15", "f16", "f17", "f18", "f19", "f20", "f21",
		"f22", "f23", "f24", "unk", "unk", "unk", "unk", "unk","unk", "unk", "unk",
		"numlck", "scrllck", "unk", "unk", "unk", "unk", "unk", "unk", "unk",
		"unk", "unk","unk", "unk", "unk", "unk", "unk", "lshft", "rshft", "lctrl",
		"rctrl", "lmenu", "rmenu", "unk","unk", "unk","unk", "unk", "unk", "unk",
		"unk", "unk", "unk", "ntrk", "ptrk", "stop", "play", "unk", "unk",
		"unk", "unk", "unk", "unk", ";", "+", ",", "-", ".", "?", "~", "unk", "unk",
		"unk", "unk","unk", "unk", "unk", "unk", "unk", "unk", "unk",
		"unk", "unk", "unk", "unk", "unk", "unk", "unk", "unk","unk",
		"unk", "unk", "unk", "unk", "unk", "unk", "unk", "unk", "unk", "unk", "unk",
		"unk", "unk", "unk","unk", "unk", "unk", "unk", "unk", "unk",
		"unk", "unk", "unk", "unk", "unk", "unk", "unk", "unk", "unk",
		"unk", "unk", "unk", "unk", "unk", "unk", "unk", "unk", "unk",
		"unk", "unk" };
	bool pressed = false;
	static bool selected_opened = false;
	static bool click_rest;
	static bool rest;
	std::string key_name;
	static bool listening;
	static int key_index;

	int ComboSize = 125; // 163

	if (c_menu::get().menu_opened) {
		if (GetAsyncKeyState(VK_LBUTTON) && mouse_in_params(x_offset + 25, y_offset + 14, ComboSize, 17) && !click_rest)
		{
			pressed = true;
		}

		if (pressed)
		{
			listening = true;
		}

	}

	if (listening)
	{
		for (auto i = 1; i < 255; ++i)
		{
			if (GetAsyncKeyState(i))
			{
				if (i == VK_ESCAPE)
				{
					item = 0;
					key_index = 0;
					key_name = keynames[key_index];
					listening = false;
					break;
				}

				else
				{
					item = i;
					key_index = i;
					key_name = keynames[key_index];
					listening = false;
					break;
				}
			}
		}
		pressed = false;
	}

	draw::draw_string(x_offset + 29, y_offset - 2, menu_text, font::menu_font, xorstr("key"));
	draw::draw_filled_rect(x_offset + 29, y_offset + 14, ComboSize, 17, menu_background);
	draw::draw_outlined_rect(x_offset + 29, y_offset + 14, ComboSize, 17, menu_div2);


	draw::draw_string(x_offset + 38, y_offset + 15, menu_text, font::verdana2, key_name.c_str());

	y_offset += 41;
}

void c_menu::groupbox(int x, int y, int w, int h, std::string name)
{
	draw::draw_outlined_rect(_pos.x + x, _pos.y + y, w, h, colour_t(27, 27, 27, menu_alpha_main));
	draw::draw_outlined_rect(_pos.x + (x + 1), _pos.y + (y + 1), w - 2, h - 2, colour_t(0, 0, 0, menu_alpha_main));
	RECT text_size = get_text_size(font::menu_font, name.c_str());
	draw::draw_rect_gradient((((_pos.x + x) + (w / 2)) - (text_size.right / 2)) - 3, _pos.y + y, text_size.right + 6, 2, menu_background, menu_background, false);
	


	draw::draw_string((((_pos.x + x) + (w / 2)) - (text_size.right / 2)), (_pos.y + y - 6), menu_text, font::menu_font, name.c_str());

	/*
	Make scrollable!
	*/
	  
	y_offset = (_pos.y + (y + 12));
	x_offset = _pos.x + x;
	groupbox_bottom = (_pos.y + (y + h));
	groupbox_top = (_pos.y + y);
	groupbox_width = (_pos.x + (x + w));
}

void c_menu::slider(int max, std::string name, int* item, std::string suffix, int step)
{
	if (groupbox_bottom <= y_offset + 30)
		return;
	float SliderSize = 125; // 163
	float _pixel_value = max / SliderSize;
	if (c_menu::get().menu_opened) {
		if (GetAsyncKeyState(VK_LBUTTON) && mouse_in_params(x_offset + 29, y_offset + 14, SliderSize + 1, 10) && !combobox_open) 
			*item = abs(mouse_pos().x - (x_offset + 29)) * _pixel_value;

		static bool pressed_subtract = false;
		static bool pressed_add = false;

		/*
		subtract
		*/

		if (!GetAsyncKeyState(VK_LBUTTON) && mouse_in_params(x_offset + 21, y_offset + 12, 8, 10))
		{
			if (pressed_subtract)
				*item -= step;
			pressed_subtract = false;
		}

		if (GetAsyncKeyState(VK_LBUTTON) && mouse_in_params(x_offset + 21, y_offset + 12, 8, 10) && !pressed_subtract)
			pressed_subtract = true;

		/*
		add
		*/

		if (!GetAsyncKeyState(VK_LBUTTON) && mouse_in_params(x_offset + 22 + SliderSize, y_offset + 12, 15, 10))
		{
			if (pressed_add)
				*item += step;
			pressed_add = false;
		}

		if (GetAsyncKeyState(VK_LBUTTON) && mouse_in_params(x_offset + 22 + SliderSize, y_offset + 12, 15, 10) && !pressed_add)
			pressed_add = true;

		if (*item > max) {
			*item = max;
		}

		if (*item < 0) {
			*item = 0;
		}

	}
	
	std::string namedisplay;
	namedisplay += name.c_str();
	namedisplay += " (";
	namedisplay += std::to_string(*item);
	if (suffix.size() > 0) {
		namedisplay += " " + suffix;
	}
	namedisplay += ")";

	draw::draw_string(x_offset + 29, y_offset - 3, menu_text, font::menu_font, namedisplay.c_str());

	draw::draw_rect_gradient(x_offset + 29, y_offset + 14, SliderSize, 10, menu_background, menu_background_bottomgrad, false);

	if (*item)
	{
		if (*item == max) //to fix dumb math stufffff bec pixel size
			if (!vars::cheat::custom_ui_colour)
			{ 
				draw::draw_rect_gradient(x_offset + 29, y_offset + 14, SliderSize, 10, menu_accent, menu_accent_bottomgrad, false);
			}
			else
			{
				draw::draw_filled_rect(x_offset + 29, y_offset + 14, SliderSize, 10, custom_ui_colour);
			}
		else
			if (!vars::cheat::custom_ui_colour)
			{
				draw::draw_rect_gradient(x_offset + 29, y_offset + 14, (*item / _pixel_value), 10, menu_accent, menu_accent_bottomgrad, false);
			}
			else
			{
				draw::draw_filled_rect(x_offset + 29, y_offset + 14, (*item / _pixel_value), 10, custom_ui_colour);
			}
			
	}

	draw::draw_outlined_rect(x_offset + 29, y_offset + 14, SliderSize, 10, menu_div2);

	draw::draw_string(x_offset + 18, y_offset + 11, menu_text, font::menu_font, "<");
	draw::draw_string(x_offset + 20 + SliderSize + 12, y_offset + 11, menu_text, font::menu_font, ">");

	if (c_menu::get().menu_opened) {
		if (mouse_in_params(x_offset + 29, y_offset + 14, SliderSize + 1, 10)) {
			int _hovering_value = abs(mouse_pos().x - (x_offset + 29)) * _pixel_value;
			draw::draw_filled_rect(x_offset + 29, y_offset + 14, (_hovering_value / _pixel_value), 10, menu_div2);
			draw::draw_string(mouse_pos().x + 12, mouse_pos().y - 5, menu_text, font::menu_font, 
				(suffix.size() > 0) ? (std::to_string(_hovering_value) + " " + suffix).c_str() : std::to_string(_hovering_value).c_str());
		}
	}

	y_offset += 30;
}

void c_menu::draw_spectatorlist()
{
	if (!vars::visuals::spectators)
		return;
	spectator_count = 0;
	static int _drag_x = 300;
	static int _drag_y = 800;
	static int _width = 200;
	static int _height = 20;
	static bool _dragging = false;
	bool _click = false;

	if (GetAsyncKeyState(VK_LBUTTON))
		_click = true;

	vec2_t _mouse_pos = mouse_pos();

	if (_dragging && !_click)
		_dragging = false;

	if (menu_opened) {
		if (_dragging && _click)
		{
			vars::visuals::spectator_pos_x = _mouse_pos.x - _drag_x;
			vars::visuals::spectator_pos_y = _mouse_pos.y - _drag_y;
		}

		if (mouse_in_params(vars::visuals::spectator_pos_x, vars::visuals::spectator_pos_y, _width, _height) && !(mouse_in_params(vars::visuals::spectator_pos_x + 20, vars::visuals::spectator_pos_y + 20, _width - 10, _height - 10)))
		{
			_dragging = true;
			_drag_x = _mouse_pos.x - vars::visuals::spectator_pos_x;
			_drag_y = _mouse_pos.y - vars::visuals::spectator_pos_y;
		}
	}
	draw::draw_filled_rect(vars::visuals::spectator_pos_x, vars::visuals::spectator_pos_y, _width, _height, colour_t(menu_background.r(), menu_background.g(), menu_background.b(), 144));
	draw::draw_filled_rect(vars::visuals::spectator_pos_x, vars::visuals::spectator_pos_y + 20, _width, _height - 20, colour_t(menu_background.r(), menu_background.g(), menu_background.b(), 144));
	draw::draw_filled_rect(vars::visuals::spectator_pos_x, vars::visuals::spectator_pos_y + 20, _width, 1, vars::cheat::custom_ui_colour ? custom_ui_colour_noalpha : colour_t(menu_accent.r(), menu_accent.g(), menu_accent.b(), 255));

	draw::draw_outlined_rect(vars::visuals::spectator_pos_x, vars::visuals::spectator_pos_y, _width, _height, colour_t(44, 44, 44, 255 / 3));

	for (int i = 0; i <= interfaces::engineclient->get_max_clients(); i++) {
		c_base_player* entity = reinterpret_cast<c_base_player*>(interfaces::entitylist->get_client_entity(i));
		if (!entity || !entity->is_dead() || entity->is_dormant()) {
			continue;
		}

		if (!ctx::local_player || ctx::local_player->is_dead()) {
			continue;
		}

		auto do_spectator_list = [&]() -> void {
			player_info_t entity_info;
			if (!interfaces::engineclient->get_player_info(entity->get_index(), &entity_info)) {
				return;
			}

			HANDLE obs_targ = entity->get_observer_target();
			if (!obs_targ) {
				return;
			}

			c_base_player* entity_handle = (c_base_player*)interfaces::entitylist->get_client_entity_from_handle(obs_targ);
			if (!entity_handle || entity_handle->is_dormant()) {
				return;
			}
			int obs_mode = entity->get_observer_mode();
			if (entity_handle == ctx::local_player && !entity_handle->is_dead()) {
				static auto sanitize = [](char * name) -> std::string {
					name[127] = '\0'; std::string tmp(name);
					if (tmp.length() > 20) {
						tmp.erase(20, (tmp.length() - 20));
						tmp.append("..");
					} return tmp;
				};

				char* name = entity->is_player_bot() ? ("bot %s", entity_info.name) : entity_info.name;

				std::string final_name = sanitize(name);

				if (obs_mode == OBS_MODE_FIRSTPERSON) //what's a switch?
					final_name.append(xorstr(" -> first person"));
				else if (obs_mode == OBS_MODE_THIRDPERSON)
					final_name.append(xorstr(" -> third person"));
				else if (obs_mode == OBS_MODE_DEATHCAM)
					final_name.append(xorstr(" -> death cam"));
				else if (obs_mode == OBS_MODE_ROAMING)
					final_name.append(xorstr(" -> roaming"));
				else if (obs_mode == OBS_MODE_FREEZECAM)
					final_name.append(xorstr(" -> freeze cam"));
				else if (obs_mode == OBS_MODE_FIXED)
					final_name.append(xorstr(" -> fixed"));

				std::transform(final_name.begin(), final_name.end(), final_name.begin(), tolower);
				draw::draw_string(vars::visuals::spectator_pos_x + 10, vars::visuals::spectator_pos_y + 23 + spectator_count++ * 12, colour_t(255, 255, 255, 255), font::calibri, final_name.c_str());
				if (spectator_count >= 1)
					_height = 26 + spectator_count * 12;
			}
		};

		if (vars::visuals::spectators) {
			do_spectator_list();
		}
	}
	draw::draw_string(vars::visuals::spectator_pos_x + 10, vars::visuals::spectator_pos_y + 2, colour_t(menu_text.r(), menu_text.g(), menu_text.b(), 255), font::menu_font, xorstr("spectators (%i)"), spectator_count);
	if (spectator_count == 0)
		_height = 20;
}

void c_menu::draw_playerlist()
{
	if (!vars::visuals::player_list || !menu_opened || !interfaces::engineclient->is_in_game())
		return;
	
	player_count = 0;
	static int _drag_x = 300;
	static int _drag_y = 800;
	static int _width = 200;
	static int _height = 20;
	static bool _dragging = false;
	static int panel_x, panel_y;

	bool _click = false;

	if (GetAsyncKeyState(VK_LBUTTON))
		_click = true;

	vec2_t _mouse_pos = mouse_pos();

	if (_dragging && !_click)
		_dragging = false;

	if (menu_opened) {
		if (_dragging && _click)
		{
			vars::visuals::player_list_x = _mouse_pos.x - _drag_x;
			vars::visuals::player_list_y = _mouse_pos.y - _drag_y;
		}

		if (mouse_in_params(vars::visuals::player_list_x, vars::visuals::player_list_y, _width, _height) && !(mouse_in_params(vars::visuals::player_list_x + 20, vars::visuals::player_list_y + 20, _width - 10, _height - 10)))
		{
			_dragging = true;
			_drag_x = _mouse_pos.x - vars::visuals::player_list_x;
			_drag_y = _mouse_pos.y - vars::visuals::player_list_y;
		}
	}
	draw::draw_filled_rect(vars::visuals::player_list_x, vars::visuals::player_list_y, _width, _height, colour_t(menu_background.r(), menu_background.g(), menu_background.b(), 144));
	draw::draw_filled_rect(vars::visuals::player_list_x, vars::visuals::player_list_y + 20, _width, _height - 20, colour_t(menu_background.r(), menu_background.g(), menu_background.b(), 144));
	draw::draw_filled_rect(vars::visuals::player_list_x, vars::visuals::player_list_y + 20, _width, 1, vars::cheat::custom_ui_colour ? custom_ui_colour : colour_t(menu_accent.r(), menu_accent.g(), menu_accent.b(), 255));

	draw::draw_outlined_rect(vars::visuals::player_list_x, vars::visuals::player_list_y, _width, _height, colour_t(44, 44, 44, 255 / 3));

	colour_t enemy_visible = colour_t::from_hsb(vars::colour::enemy_box_vis / 100, 1.f, 1.f);
	colour_t team_visible = colour_t::from_hsb(vars::colour::team_box_vis / 100, 1.f, 1.f);

	for (int i = 0; i <= interfaces::engineclient->get_max_clients(); i++) {
		static bool panel_open = false;
		c_base_player* entity = reinterpret_cast<c_base_player*>(interfaces::entitylist->get_client_entity(i));
		if (!entity || entity == ctx::local_player) {
			continue;
		}

		if (!ctx::local_player) {
			continue;
		}

		auto player_list = [&]() -> void {
			player_info_t entity_info;
			if (!interfaces::engineclient->get_player_info(entity->get_index(), &entity_info)) {
				return;
			}

			if (ctx::local_player) {
				static auto sanitize = [](char * name) -> std::string {
					name[127] = '\0'; std::string tmp(name);
					if (tmp.length() > 20) {
						tmp.erase(20, (tmp.length() - 20));
						tmp.append("..");
					} return tmp;
				};

				static bool clicked = false;


				char* name = entity->is_player_bot() ? ("bot %s", entity_info.name) : entity_info.name;

				std::string final_name = sanitize(name);

				std::transform(final_name.begin(), final_name.end(), final_name.begin(), tolower);

				draw::draw_string(vars::visuals::player_list_x + 10, vars::visuals::player_list_y + 23 + player_count++ * 12, entity->get_team_num() == ctx::local_player->get_team_num() ? team_visible : enemy_visible, font::calibri, final_name.c_str());

				if (GetAsyncKeyState(VK_LBUTTON) && mouse_in_params(vars::visuals::player_list_x, vars::visuals::player_list_y + 23 + (player_count - 1) * 12, _width, 12) && !clicked) {
					panel_x = mouse_pos().x;
					panel_y = mouse_pos().y;
					panel_open = !panel_open;
					clicked = !clicked;
				}

				draw::draw_outlined_rect(vars::visuals::player_list_x, vars::visuals::player_list_y + 23 + (player_count - 1) * 12, _width, 12, colour_t::from_hsb(0 + i * 12, 1, 1));

				if (panel_open) {
					clicked = false;
					draw::draw_filled_rect(panel_x, panel_y, 150, 200, menu_background); //only draws when menu is open anyways who gives a fuck?
					draw::draw_string_centered(panel_x + 75, panel_y + 2, entity->get_team_num() == ctx::local_player->get_team_num() ? team_visible : enemy_visible, font::menu_font, final_name.c_str());
				}

				if (clicked && !panel_open)
					clicked = false;

				if (player_count >= 1)
					_height = 26 + player_count * 12;
			}
		};

		player_list();

	}

	draw::draw_string(vars::visuals::player_list_x + 10, vars::visuals::player_list_y + 2, colour_t(menu_text.r(), menu_text.g(), menu_text.b(), 255), font::menu_font, xorstr("player list"), player_count);
	if (player_count == 0)
		_height = 20;
}

void c_menu::draw_radar()
{
	if (!vars::visuals::radar) {
		return;
	}

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

	static int _drag_x = 300;
	static int _drag_y = 300;
	if (vars::visuals::radar_height <= 10)
		vars::visuals::radar_height = 300;
	if (vars::visuals::radar_width <= 10)
		vars::visuals::radar_width = 300;
	colour_t coloursarray[10] = { colour_t(0, 195, 255), colour_t(250, 0, 255), colour_t(174, 255, 0), colour_t(141, 83, 173), colour_t(209, 42, 224), colour_t(255, 191, 0), colour_t(255, 0, 97), colour_t(0, 55, 255), colour_t(246, 255, 0), colour_t(0, 195, 255) };

	static bool _dragging = false;
	bool _click = false;
	static bool _resizing = false;


	if (GetAsyncKeyState(VK_LBUTTON))
		_click = true;

	vec2_t _mouse_pos = mouse_pos();

	if (_dragging && !_click)
		_dragging = false;

	if (_resizing && !_click)
		_resizing = false;

	if (menu_opened) {
		if (_dragging && _click)
		{
			vars::visuals::radar_drag_x = _mouse_pos.x - _drag_x;
			vars::visuals::radar_drag_y = _mouse_pos.y - _drag_y;
		}

		if (_resizing && _click) {
			vars::visuals::radar_width = _mouse_pos.x - vars::visuals::radar_drag_x;
			vars::visuals::radar_height = _mouse_pos.y - vars::visuals::radar_drag_y;

			if (vars::visuals::radar_width < 200)
				vars::visuals::radar_width = 200;

			if (vars::visuals::radar_height < 200)
				vars::visuals::radar_height = 200;

			if (vars::visuals::radar_width > 600)
				vars::visuals::radar_width = 600;

			if (vars::visuals::radar_height > 600)
				vars::visuals::radar_height = 600;
		}
	}
	if (mouse_in_params(vars::visuals::radar_drag_x, vars::visuals::radar_drag_y, vars::visuals::radar_width, vars::visuals::radar_height) && !(mouse_in_params(vars::visuals::radar_drag_x + 20, vars::visuals::radar_drag_y + 20, vars::visuals::radar_width - 10, vars::visuals::radar_height - 10)))
	{
		_dragging = true;
		_drag_x = _mouse_pos.x - vars::visuals::radar_drag_x;
		_drag_y = _mouse_pos.y - vars::visuals::radar_drag_y;
	}

	if (mouse_in_params(vars::visuals::radar_drag_x, vars::visuals::radar_drag_y,vars::visuals::radar_width, vars::visuals::radar_height) && !(mouse_in_params(vars::visuals::radar_drag_x, vars::visuals::radar_drag_y, vars::visuals::radar_width - 30, vars::visuals::radar_height - 30))) {
		_resizing = true;
	}

	draw::draw_filled_rect(vars::visuals::radar_drag_x, vars::visuals::radar_drag_y, vars::visuals::radar_width, vars::visuals::radar_height, colour_t(menu_background.r(), menu_background.g(), menu_background.b(), 144));
	draw::draw_filled_rect(vars::visuals::radar_drag_x, vars::visuals::radar_drag_y + 20, vars::visuals::radar_width, vars::visuals::radar_height - 20, colour_t(menu_background.r(), menu_background.g(), menu_background.b(), 144));
	draw::draw_filled_rect(vars::visuals::radar_drag_x, vars::visuals::radar_drag_y + 20, vars::visuals::radar_width, 1, vars::cheat::custom_ui_colour ? custom_ui_colour_noalpha : colour_t(menu_accent.r(), menu_accent.g(), menu_accent.b(), 255));

	draw::draw_line(vars::visuals::radar_drag_x + vars::visuals::radar_width / 2, vars::visuals::radar_drag_y + vars::visuals::radar_height - 1, vars::visuals::radar_drag_x + vars::visuals::radar_width / 2, vars::visuals::radar_drag_y + 21, colour_t(125, 125, 125, 255));
	draw::draw_line(vars::visuals::radar_drag_x, (vars::visuals::radar_drag_y + vars::visuals::radar_height / 2) + 5, vars::visuals::radar_drag_x + vars::visuals::radar_width, (vars::visuals::radar_drag_y + vars::visuals::radar_height / 2) + 5, colour_t(125, 125, 125, 255));

	draw::draw_outlined_rect(vars::visuals::radar_drag_x, vars::visuals::radar_drag_y, vars::visuals::radar_width, vars::visuals::radar_height, colour_t(44, 44, 44, 255 / 3));

	draw::draw_string(vars::visuals::radar_drag_x + 10, vars::visuals::radar_drag_y + 2, colour_t(menu_text.r(), menu_text.g(), menu_text.b(), 255), font::menu_font, xorstr("radar"));

	if (interfaces::engineclient->is_connected() && interfaces::engineclient->is_in_game()) {
		c_base_player* local_player = ctx::local_player;
		if (!local_player) { return; }

		vec3_t local_angles; interfaces::engineclient->get_view_angles(local_angles);
		for (int i = 1; i <= interfaces::engineclient->get_max_clients(); i++) {
			c_base_player* entity = reinterpret_cast<c_base_player*>(interfaces::entitylist->get_client_entity(i));
			if (!entity || entity->is_dead() || entity->is_dormant() || entity == ctx::local_player) {
				continue;
			}
			player_info_t entity_info;
			if (!interfaces::engineclient->get_player_info(entity->get_index(), &entity_info)) {
				return;
			}
			if (vars::visuals::player_enemyonly && entity->get_team_num() == ctx::local_player->get_team_num()) {
				continue;
			}

			vec3_t	entity_position = entity->get_world_space_center(),
				local_position = local_player->get_world_space_center();

			if (entity_position.is_zero() || local_position.is_zero()) {
				continue;
			}

			float	angle = DEG2RAD(local_angles.y);

			float	radar_x = local_position.x - entity_position.x,
				radar_y = local_position.y - entity_position.y;

			float	rotate_x = radar_y * cos(angle) - radar_x * sin(angle),
				rotate_y = radar_x * cos(angle) + radar_y * sin(angle);

			float	final_x = (vars::visuals::radar_drag_x + (vars::visuals::radar_width / 2)) + rotate_x / 10,
				final_y = (vars::visuals::radar_drag_y + 5 + (vars::visuals::radar_height / 2)) + rotate_y / 10;

			if (final_x < vars::visuals::radar_drag_x) {
				final_x = vars::visuals::radar_drag_x;
			}

			if (final_x > vars::visuals::radar_drag_x + vars::visuals::radar_width) {
				final_x = vars::visuals::radar_drag_x + vars::visuals::radar_width - 3;
			}

			if (final_y < vars::visuals::radar_drag_y + 20) {
				final_y = vars::visuals::radar_drag_y + 22;
			}

			if (final_y > vars::visuals::radar_drag_y + vars::visuals::radar_height) {
				final_y = vars::visuals::radar_drag_y + vars::visuals::radar_height - 2;
			}
			bool is_visible = utilities::is_entity_visible(ctx::local_player, entity, ctx::local_player->get_eye_position(), entity->get_world_space_center());
			static auto sanitize = [](char * name) -> std::string {
				name[127] = '\0'; std::string tmp(name);
				if (tmp.length() > 20) {
					tmp.erase(20, (tmp.length() - 20));
					tmp.append("..");
				} return tmp;
			};
			char* name = entity->is_player_bot() ? (xorstr("bot %s"), entity_info.name) : entity_info.name;
			std::string final_name = sanitize(name);
			std::transform(final_name.begin(), final_name.end(), final_name.begin(), toupper);
			colour_t enemy_visible = colour_t::from_hsb(vars::colour::enemy_box_vis / 100, 1.f, 1.f);
			colour_t enemy_invisible = colour_t::from_hsb(vars::colour::enemy_box_inv / 100, 1.f, 1.f);
			colour_t team_visible = colour_t::from_hsb(vars::colour::team_box_vis / 100, 1.f, 1.f);
			colour_t team_invisible = colour_t::from_hsb(vars::colour::team_box_invis / 100, 1.f, 1.f);

			if (is_visible)
				draw::draw_filled_rect(final_x, final_y, 4, 4, (entity->get_team_num() != local_player->get_team_num()) ? enemy_visible : team_visible);
			else
				draw::draw_filled_rect(final_x, final_y, 4, 4, (entity->get_team_num() != local_player->get_team_num()) ? enemy_invisible : team_visible);

			draw::draw_outlined_rect(final_x - 1, final_y - 1, 6, 6, colour_t(0, 0, 0));
			draw::draw_string(final_x - 1, final_y + 5, colour_t(255, 255, 255), font::smallfont, final_name.c_str());
			draw::draw_string(final_x - 1, final_y + 13, health_to_colour(entity, entity->get_health()), font::smallfont, xorstr("H: %d"), entity->get_health());

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
			draw::draw_string(final_x - 1, final_y + 21, colour_t(255, 255, 255), font::smallfont, class_name.c_str());
		}
	}
}
vec2_t c_menu::mouse_pos()
{
	POINT mouse_position;
	GetCursorPos(&mouse_position);
	ScreenToClient(FindWindow(0, xorstr("Team Fortress 2")), &mouse_position);
	return { static_cast<float>(mouse_position.x), static_cast<float>(mouse_position.y) };
}
bool c_menu::mouse_in_params(int x, int y, int x2, int y2) {
	if (mouse_pos().x > x && mouse_pos().y > y && mouse_pos().x < x2 + x && mouse_pos().y < y2 + y)
		return true;
	return false;
}

bool test = false;
int testcombo = 0;
int test_slider;
void c_menu::draw(unsigned int panel)
{
	custom_ui_colour = colour_t::from_hsb(vars::colour::custom_ui_colour / 100, 1.f, 1.f, menu_alpha_main);
	custom_ui_colour_noalpha = colour_t::from_hsb(vars::colour::custom_ui_colour / 100, 1.f, 1.f, 255);

	static bool _pressed = true;
	static bool _secret_pressed = false;

	if (!_pressed && GetAsyncKeyState(VK_INSERT)) {
		_pressed = true;
	}
	else if (_pressed && !GetAsyncKeyState(VK_INSERT))
	{
		//chroma::get().do_menu_effect();
		_pressed = false;
		menu_opened = !menu_opened;
		interfaces::panel->set_mouse_input_enabled(panel, menu_opened);
	}
	if (!_secret_pressed && menu_opened && GetAsyncKeyState(VK_SCROLL))
		_secret_pressed = true;
	else if (_secret_pressed && !GetAsyncKeyState(VK_SCROLL))
	{
		_secret_pressed = false;
		interfaces::convar->console_print_colour(vars::cheat::custom_ui_colour ? custom_ui_colour : menu_accent, xorstr( //uh oh stinky poop ahead!
"thank you to all the homies that have helped me make this project possible:\n\
**catberg**: being an amazing help with the telegram bot \n\
fox: being fox \n\
v952: spoonfeeding me a bunch of shit \n\
all the gamers in my telegram: being around\nthanks folks ;)\n\
ALPHA TELEGRAM v1.1 BUILD\n"));
	}

	//just gonna do config shit here cuz why not i guess

	if (vars::config::save) {
		g_Config->Save();
	}
	if (vars::config::load) {
		g_Config->Load();
	}


	if (vars::cheat::unhook)
		setup::unhook(); //im sorry this is just becoming an easy place to put shit that doesnt belong in createmove :/

	//wrong place: continued :/
	static bool monoset = false;

	if (monoset != vars::cheat::monocromia)
		if (vars::cheat::monocromia) {
			interfaces::engineclient->client_command(xorstr("r_screenoverlay \"debug/yuv\""), true);
			monoset = vars::cheat::monocromia;
		}
		else {
			interfaces::engineclient->client_command(xorstr("r_screenoverlay \"\""), true);
			monoset = vars::cheat::monocromia;
		}

	screen_width  = ctx::screen_width;
	screen_height = ctx::screen_height;

	if (menu_opened)
		ctx::menu_open = true;
	else
		ctx::menu_open = false;
	//dx_render::dx_render->rect(150, 150, 100, 100, D3DCOLOR_ARGB(255, 0, 0, 0));
	//dx_render::dx_render->text("fuck you!!!!", 200, 200, CENTER, dx_render::fonts::font_franklin, false, true, D3DCOLOR_ARGB(255, 255, 255, 255));

	static vec2_t _mouse_pos;

	static int _drag_x = 300;
	static int _drag_y = 300;
	static int _width = 210;
	static int _height = 470;

	static bool _dragging = false;
	bool _click = false;
	static bool _resizing = false;
	if (menu_opened)
	{
		menu_alpha_main = min(menu_alpha_main + 10, 255);
		menu_alpha_text = min(menu_alpha_text + 10, 255);

		if (GetAsyncKeyState(VK_LBUTTON))
			_click = true;

		vec2_t _mouse_pos = mouse_pos();

		if (_dragging && !_click)
			_dragging = false;

		if (_dragging && _click)
		{
			_pos.x = _mouse_pos.x - _drag_x;
			_pos.y = _mouse_pos.y - _drag_y;
		}

		if (mouse_in_params(_pos.x - 85, _pos.y, _width + 85, _pos.y + 20) && !(mouse_in_params(_pos.x - 85, _pos.y + 20, _width + 85, _height)))
		{
			_dragging = true;
			_drag_x = _mouse_pos.x - _pos.x;
			_drag_y = _mouse_pos.y - _pos.y;
		}
	}
	else {
		menu_alpha_main = max(menu_alpha_main - 10, 0);
		menu_alpha_text = max(menu_alpha_text - 10, 0);
	}
	if (menu_alpha_main > 144) {
		draw::draw_filled_rect(_pos.x - 85, _pos.y, _width + 85, _height, colour_t(9, 9, 9, 144));
	}
	else
	{
		draw::draw_filled_rect(_pos.x - 85, _pos.y, _width + 85, _height, menu_background);
	}
	draw::draw_filled_rect(_pos.x, _pos.y + 20, _width, _height - 20, menu_background);

	draw::draw_filled_rect(_pos.x - 85, _pos.y + 20, 85, _height - 20, colour_t(23, 20, 27, menu_alpha_main / 1.5));

	draw::draw_filled_rect(_pos.x - 85, _pos.y + 20, _width + 85, 1, vars::cheat::custom_ui_colour ? custom_ui_colour : menu_accent);

	draw::draw_outlined_rect(_pos.x - 85, _pos.y, _width + 85, _height, colour_t(59, 56, 63, menu_alpha_main / 3));
	draw::draw_outlined_rect(_pos.x - 85, _pos.y - 1, _width + 85, _height + 1, menu_div2);
	draw::draw_string(_pos.x - 85 + 10, _pos.y + 2, colour_t(180, 180, 180, menu_alpha_text), font::menu_font, xorstr("Isopropyl | registered to: %s"), USER);
	int in_sizew = 70;
	static int tab_selected = 0;
	static const int tab_amount = 5;
	static std::string tab_names[tab_amount] = { xorstr("aimbot"), xorstr("visuals"), xorstr("miscellaneous"), xorstr("anti-aim"), xorstr("config") };

	for (int i = 0; i < tab_amount; i++)
	{
		RECT text_size = get_text_size(font::tahoma, tab_names[i].c_str());

		int tab_area[4] = { _pos.x - 90, _pos.y + 27 + i * (in_sizew / tab_amount), 100, (in_sizew / tab_amount) };

		//int tab_area[4] = { _pos.x + 5 + i * (in_sizew / tab_amount), _pos.y - 5, 100, (in_sizew / tab_amount) };

		//if (GetAsyncKeyState(VK_LBUTTON) && mouse_in_params(tab_area[0], tab_area[1] + 25, tab_area[2], tab_area[3] - 50))
		 if (GetAsyncKeyState(VK_LBUTTON) && mouse_in_params(tab_area[0], tab_area[1], tab_area[2], tab_area[3]))
			tab_selected = i;

		if (tab_selected == i)
			draw::draw_string(tab_area[0] + (tab_area[2] / 2) - (text_size.right / 2), tab_area[1] + (tab_area[3] / 2) - (text_size.bottom / 2), vars::cheat::custom_ui_colour ? custom_ui_colour : menu_accent, font::menu_font, tab_names[i].c_str());
		else
			draw::draw_string(tab_area[0] + (tab_area[2] / 2) - (text_size.right / 2), tab_area[1] + (tab_area[3] / 2) - (text_size.bottom / 2), menu_text, font::menu_font, tab_names[i].c_str());
	}

	static std::string  thirdperson_anglez[2] = { xorstr("real angle"), xorstr("fake angle") },
		chams_model[5] = { xorstr("flattened"), xorstr("textured"), xorstr("wireframe"), xorstr("shine"), xorstr("gradient") },

		chams_activation[3] = { xorstr("disabled"), xorstr("obey z"), xorstr("ignore z") },
		strafer_types[3] = { xorstr("off"), xorstr("legit"), xorstr("rage") },
		esp_type[4] = { xorstr("player enemy"), xorstr("player team"), xorstr("building"), xorstr("world") },

		fakelag_modes[6] = { xorstr("off"), xorstr("factor"), xorstr("switch"), xorstr("adaptive"), xorstr("random"), xorstr("pulse") },

		anti_aim_pitch[9] = { xorstr("off"), xorstr("down"), xorstr("up"), xorstr("fake-down"), xorstr("fake-up"), xorstr("fake zero up"), xorstr("fake zero down"), xorstr("lisp up"), xorstr("lisp down") },
		main_hitbox[5] = { xorstr("head"), xorstr("body"), xorstr("auto"), xorstr("hitscan"), xorstr("hitscan prefer body") },
		yaw_types[3] = { xorstr("static"), xorstr("edge"), xorstr("spin") },
		yaw_select[2] = { xorstr("real"), xorstr("fake") },

		keys[11] = { xorstr("always"), xorstr("mouse1"), xorstr("mouse2"), xorstr("mouse3"), xorstr("mouse4"), xorstr("mouse5"), xorstr("shift"), xorstr("alt"), xorstr("f"), xorstr("c"), xorstr("z") },
		colours[9] = { xorstr("baby blue"), xorstr("gay pink"), xorstr("skeet lime"), xorstr("night purple"), xorstr("lovell purple"), xorstr("agent orange"), xorstr("strawberry"), xorstr("royal blue"), xorstr("watersports yellow") },
		configs[4] = { xorstr("one"), xorstr("two"), xorstr("three"), xorstr("four") },
		backtracks[4] = { xorstr("off"), xorstr("cross"), xorstr("interpolated line"), xorstr("squares") },
		multidummy[4] = { xorstr("head"), xorstr("chest"), xorstr("pelvis"), xorstr("legs")};
	std::vector<bool> jeff = {vars::dummy::dummy1, vars::dummy::dummy2, vars::dummy::dummy3, vars::dummy::dummy4};
	switch (tab_selected) {
		case aimbot_tab: {
			static bool aim_tab_page = true;
			switch (aim_tab_page)
			{
				case true: {
					groupbox(16, 36, 180, 365, xorstr("aimbot")); {
						checkbox(xorstr("enabled"), &vars::aimbot::enabled);
						if (!vars::aimbot::smoothing) {
							slider(180, xorstr("fov"), &vars::aimbot::field_of_view, xorstr("deg"), 10);
						}
						else
						{
							slider(30, xorstr("fov"), &vars::aimbot::field_of_view, xorstr("deg"), 1);
							if (vars::aimbot::field_of_view > 30)
								vars::aimbot::field_of_view = 30;
						}
						slider(100, xorstr("point scale"), &vars::aimbot::pointscale, xorstr(""), 10);
						combobox(5, xorstr("bone"), main_hitbox, &vars::aimbot::main_hitbox);
						combobox(11, xorstr("key"), keys, &vars::aimbot::aimbot_key);
						checkbox(xorstr("smoothing"), &vars::aimbot::smoothing);
						if (!vars::aimbot::smoothing) {
							checkbox(xorstr("autoshoot"), &vars::aimbot::autoshoot);
							checkbox(xorstr("autoscope"), &vars::aimbot::autoscope);
							checkbox_colour(xorstr("draw fov"), &vars::aimbot::drawfov, &vars::colour::aimbot_fov);
							checkbox(xorstr("silent aim"), &vars::aimbot::silent_aim);
							checkbox(xorstr("perfect silent"), &vars::aimbot::bullet_time);
							checkbox(xorstr("wait for charge"), &vars::aimbot::wait_for_charge);
							checkbox(xorstr("backtracking"), &vars::position_adjustment::enabled);
							checkbox(xorstr("auto minigun rev"), &vars::aimbot::heavy_autorev);
							checkbox(xorstr("pitch resolver"), &vars::hvh::pitch_resolver);
							checkbox(xorstr("yaw resolver"), &vars::hvh::yaw_resolver);

						}
						else {
							slider(40, xorstr("vertical smoothing"), &vars::aimbot::smoothingx, "", 1);
							slider(40, xorstr("horizontal smoothing"), &vars::aimbot::smoothingy, "", 1);
							checkbox(xorstr("autoshoot"), &vars::aimbot::autoshoot);
							checkbox(xorstr("autoscope"), &vars::aimbot::autoscope);
							checkbox_colour(xorstr("draw fov"), &vars::aimbot::drawfov, &vars::colour::aimbot_fov);
							checkbox(xorstr("wait for charge"), &vars::aimbot::wait_for_charge);
							checkbox(xorstr("backtracking"), &vars::position_adjustment::enabled);
							checkbox(xorstr("auto minigun rev"), &vars::aimbot::heavy_autorev);
							label(xorstr("legitbot is on!"));
						}
						groupbox(16, 424, 180, 40, xorstr("page")); {
							button(xorstr("next page"), &aim_tab_page, false);
						}
					}
				} break;

				case false: {
					groupbox(16, 178, 180, 114, xorstr("targeting")); {
						checkbox(xorstr("sentries"), &vars::aimbot::aimbot_sentry);
						checkbox(xorstr("dispensers"), &vars::aimbot::aimbot_dispenser);
						checkbox(xorstr("teleporters"), &vars::aimbot::aimbot_teleporters);
						checkbox(xorstr("prefer medic"), &vars::aimbot::prefer_medic);
						checkbox(xorstr("ignore cloaked"), &vars::aimbot::ignore_cloaked);
						checkbox(xorstr("ignore disguised"), &vars::aimbot::ignore_disguised);
					}

					groupbox(16, 32, 180, 136, xorstr("triggerbot")); {
						checkbox(xorstr("enabled"), &vars::triggerbot::triggerbot_enabled);
						combobox(11, xorstr("key"), keys, &vars::triggerbot::triggerbot_key);
						slider(1000, xorstr("delay"), &vars::triggerbot::triggerbot_delay, xorstr("ms"), 10);
						checkbox(xorstr("auto detonate"), &vars::triggerbot::auto_detonate);
						checkbox(xorstr("auto airblast"), &vars::triggerbot::auto_airblast);
					}

					groupbox(16, 424, 180, 40, xorstr("page")); {
						button(xorstr("prev page"), &aim_tab_page, false);
					}
				} break;
			}
		} break;
		case visual_tab: {
			static bool vis_tab_page = true;
			switch (vis_tab_page) {
				case true: {
					groupbox(16, 424, 180, 40, xorstr("page")); {
						button(xorstr("next page"), &vis_tab_page, false);
					}

					groupbox(16, 288, 180, 132, xorstr("chameleon skins")); {
						combobox(3, xorstr("material z rules"), chams_activation, &vars::chams::chams_activation);
						combobox(5, xorstr("material type"), chams_model, &vars::chams::chams_model);
						checkbox_colour(xorstr("self chams"), &vars::chams::local_player, &vars::colour::chams_self);
						checkbox(xorstr("attachments"), &vars::chams::wearables);
					}

					groupbox(16, 36, 180, 245, xorstr("esp")); {
						combobox(4, xorstr("esp type"), esp_type, &vars::visuals::esp_type);
						switch (vars::visuals::esp_type) {
						case 0: { //enemy vis
							checkbox(xorstr("non-esp enemy only"), &vars::visuals::player_enemyonly);
							checkbox(xorstr("name"), &vars::visuals::enemy_name);
							checkbox_colour_dual(xorstr("box"), &vars::visuals::enemy_box, &vars::colour::enemy_box_vis, &vars::colour::enemy_box_inv);
							checkbox(xorstr("skeleton"), &vars::visuals::enemy_bones);
							checkbox(xorstr("health"), &vars::visuals::enemy_health);
							checkbox_colour_dual(xorstr("chams"), &vars::visuals::enemy_chams, &vars::colour::enemy_chams_vis, &vars::colour::enemy_chams_inv);
							checkbox(xorstr("weapon"), &vars::visuals::enemy_weapon);
							checkbox(xorstr("steamid"), &vars::visuals::enemy_steamid);
							combobox(4, xorstr("backtrack visualization"), backtracks, &vars::visuals::enemy_backtrack_visulization);
							checkbox_colour_dual(xorstr("hitboxes"), &vars::visuals::enemy_hitboxes, &vars::colour::enemy_hitbox_vis, &vars::colour::enemy_hitbox_inv);

						} break;
						case 1: { //team vis
							checkbox(xorstr("name"), &vars::visuals::team_name);
							checkbox_colour_dual(xorstr("box"), &vars::visuals::team_box, &vars::colour::team_box_vis, &vars::colour::team_box_invis);
							checkbox(xorstr("skeleton"), &vars::visuals::team_bones);
							checkbox(xorstr("health"), &vars::visuals::team_health);
							checkbox_colour_dual(xorstr("chams"), &vars::visuals::team_chams, &vars::colour::team_chams_vis, &vars::colour::team_chams_invis);
							checkbox(xorstr("weapon"), &vars::visuals::team_weapon);
							checkbox(xorstr("steamid"), &vars::visuals::team_steamid);
							checkbox_colour_dual(xorstr("hitboxes"), &vars::visuals::team_hitboxes, &vars::colour::team_hitbox_vis, &vars::colour::team_hitbox_inv);
						} break;
						case 2: { //building vis
							checkbox(xorstr("enemy only"), &vars::visuals::building_enemyonly);
							checkbox(xorstr("box"), &vars::visuals::building_box);
							checkbox(xorstr("health"), &vars::visuals::building_health);
							checkbox(xorstr("name"), &vars::visuals::building_name);
							checkbox(xorstr("builder"), &vars::visuals::building_builder);
							checkbox(xorstr("level"), &vars::visuals::building_level);
							checkbox_colour_dual(xorstr("chams"), &vars::visuals::building_chams, &vars::colour::building_chams_vis, &vars::colour::building_chams_inv);
						} break;
						case 3: { //world vis
							checkbox(xorstr("box"), &vars::visuals::world_box);
							checkbox(xorstr("name"), &vars::visuals::world_name);
							checkbox_colour(xorstr("health chams"), &vars::visuals::world_health_chams, &vars::colour::health_chams);
							checkbox_colour(xorstr("ammo chams"), &vars::visuals::world_ammo_chams, &vars::colour::ammo_chams);
							checkbox_colour(xorstr("projectile chams"), &vars::visuals::world_projectile_chams, &vars::colour::projectile_chams);
							checkbox_colour(xorstr("money chams"), &vars::visuals::world_money_chams, &vars::colour::money_chams);
							checkbox_colour(xorstr("tank chams"), &vars::visuals::world_tank_chams, &vars::colour::tank_chams);
						}
						}
					}

				} break;

				case false: {
					//2nd row
					groupbox(16, 36, 180, 341, xorstr("on-screen visuals")); {
						checkbox(xorstr("crosshair"), &vars::visuals::custom_crosshair);
						checkbox(xorstr("spectators list"), &vars::visuals::spectators);
						checkbox(xorstr("player list"), &vars::visuals::player_list);
						checkbox(xorstr("radar"), &vars::visuals::radar);
						slider(100, xorstr("nightmode value"), &vars::visuals::nightmode_value, "", 10);
						checkbox(xorstr("projectile pred"), &vars::visuals::projectile_prediction);
						checkbox(xorstr("out of fov arrows"), &vars::visuals::out_of_fov_arrows);
						checkbox(xorstr("thirdperson (h)"), &vars::visuals::thirdperson);
						combobox(2, xorstr("thirdperson angle"), thirdperson_anglez, &vars::visuals::thirdperson_angle);
						slider(500, xorstr("thirdperson distance"), &vars::visuals::thirdperson_dist, xorstr(""), 25);
						checkbox(xorstr("antiaim visualization"), &vars::visuals::aa_visual);
						checkbox(xorstr("no zoom"), &vars::visuals::no_zoom);
						checkbox(xorstr("scope lines"), &vars::visuals::no_scope);
						slider(150, xorstr("fov"), &vars::visuals::forced_fov, xorstr("deg"), 10);
						checkbox(xorstr("no cloak"), &vars::visuals::no_cloak);
						checkbox(xorstr("esp visible only"), &vars::visuals::visible_only);
					}

					groupbox(16, 424, 180, 40, xorstr("page")); {
						button(xorstr("prev page"), &vis_tab_page, false);
					}
				} break;

			}
		} break;
		case misc_tab: {

			groupbox(16, 172, 180, 36, xorstr("chatspam")); {
				checkbox(xorstr("enabled"), &vars::chatspam::chatspam_enabled);
			}

			groupbox(16, 36, 180, 125, xorstr("misc")); {
				checkbox(xorstr("taunt slide"), &vars::miscellanous::taunt_slide);
				checkbox(xorstr("bunnyhop"), &vars::miscellanous::bunnyhop);
				combobox(3, xorstr("auto strafer"), strafer_types, &vars::miscellanous::autostrafe);
				checkbox(xorstr("no aimpunch"), &vars::miscellanous::nopunch);
				checkbox(xorstr("fake crouch"), &vars::miscellanous::fake_crouch);
			}

		} break;

		case anti_aim_tab: {
			groupbox(16, 36, 180, 420, xorstr("hvh features")); {
				combobox(6, xorstr("fakelag mode"), fakelag_modes, &vars::hvh::fakelag_mode);
				slider(get_fakelag_ticks(), xorstr("fakelag value"), &vars::hvh::fakelag_value, xorstr("packets"), 1);
				slider(100, xorstr("fakelag jitter"), &vars::hvh::fakelag_jitter, xorstr(""), 10);
				combobox(9, xorstr("pitch"), anti_aim_pitch, &vars::hvh::anti_aim_pitch);
				checkbox(xorstr("yaw anti-aim"), &vars::hvh::anti_aim_yaw);
				combobox(2, xorstr("yaw selection"), yaw_select, &vars::hvh::anti_aim_menu_select);
				switch (vars::hvh::anti_aim_menu_select)
				{
					case 0: { //real stuff
						combobox(3, xorstr("type"), yaw_types, &vars::hvh::anti_aim_real_type);
						slider(30, xorstr("spin speed"), &vars::hvh::anti_aim_real_spinspeed, xorstr(""), 10);
						slider(180, xorstr("offset"), &vars::hvh::anti_aim_real_offset, xorstr("deg"), 5);
						slider(180, xorstr("add jitter"), &vars::hvh::anti_aim_real_jitter, xorstr("deg"), 10);
						slider(180, xorstr("add spin"), &vars::hvh::anti_aim_real_spin_range, xorstr("deg"), 10);
						slider(30, xorstr("add spin speed"), &vars::hvh::anti_aim_real_add_spin_speed, xorstr(""), 10);
					} break;

					case 1: { //le fakes
						combobox(3, xorstr("type"), yaw_types, &vars::hvh::anti_aim_fake_type);
						slider(30, xorstr("spin speed"), &vars::hvh::anti_aim_fake_spinspeed, xorstr(""), 10);
						slider(180, xorstr("offset"), &vars::hvh::anti_aim_fake_offset, xorstr("deg"), 5);
						slider(180, xorstr("add jitter"), &vars::hvh::anti_aim_fake_jitter, xorstr("deg"), 10);
						slider(180, xorstr("add spin"), &vars::hvh::anti_aim_fake_spin_range, xorstr("deg"), 10);
						slider(30, xorstr("add spin speed"), &vars::hvh::anti_aim_fake_add_spin_speed, xorstr(""), 10);
					}

				}

			}
		} break;

		//case chroma_tab: {
		//	groupbox(16, 36, 180, 80, xorstr("keyboard chroma")); {
		//		checkbox(xorstr("enabled"), &vars::chroma::enabled);
		//		checkbox(xorstr("in game effects"), &vars::chroma::in_game);
		//		checkbox(xorstr("up arrow crosshair"), &vars::chroma::up_arrow);
		//		checkbox(xorstr("display killstreak"), &vars::chroma::killstreak);
		//	}
		//} break;

		case config_tab: {
			groupbox(16, 36, 180, 51, xorstr("general")); {
				checkbox_colour(xorstr("custom ui colour"), &vars::cheat::custom_ui_colour, &vars::colour::custom_ui_colour);
				button(xorstr("ragequit"), &vars::cheat::unhook, true);
			}

			groupbox(16, 94, 180, 128, xorstr("config")); {
				combobox(4, xorstr("selection"), configs, &vars::config::selection);
				button(xorstr("save"), &vars::config::save, true);
				button(xorstr("load"), &vars::config::load, true);
				checkbox(xorstr("dev hud"), &vars::cheat::hud);
				checkbox(xorstr("monocromia"), &vars::cheat::monocromia);
			}
		} break;

		default: {
			groupbox(16, 36, 180, 51, xorstr("fuck")); {
				label(xorstr("oopsie poopsie!"));
				label(xorstr("ur not supposed to see this lol"));
			}

		} break;
	}
}