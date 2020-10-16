#include "main.h"

//again, mega props to deadcell csgo guys.

void c_notification::add(bool display, colour_t colour, const std::string message, ...) {
	if (message.empty())
		return;

	va_list va;
	std::string buf;

	va_start(va, message);

	const int str_len = std::vsnprintf(nullptr, 0, message.c_str(), va);
	if (str_len < 0) {
		va_end(va);

		return;
	}

	buf.resize(str_len);
	std::vsnprintf(&buf[0], str_len + 1, message.c_str(), va);

	if (m_notification.size() > 15)
		m_notification.pop_front();

	if (display)
		m_notification.emplace_back(colour, colour_t(125, 0, 0, 0), interfaces::globals->curtime, buf);
}

void c_notification::clear() {
	m_notification.clear();
}

void c_notification::draw() {
	int adjust_height = 5;
	int x_add;

	for (size_t i = 0; i < m_notification.size(); ++i) {
		auto &notification = m_notification.at(i);

		colour_t custom_ui_colour;
		colour_t &colour = notification.colour;
		colour_t &shadow_colour = notification.background_colour;
		const float cur_time = notification.m_time;
		std::string message = notification.m_buf;
		custom_ui_colour = vars::cheat::custom_ui_colour ? colour_t::from_hsb(vars::colour::custom_ui_colour / 100, 1.f, 1.f, 255) : colour_t(218, 23, 91, 255);
		if (interfaces::globals->curtime - cur_time > m_text_duration) {
			int alpha = static_cast<int>(colour.a() - 255 / 1.f * max(interfaces::globals->frametime, 0.01f));

			if (alpha > 255)
				alpha = 255;
			if (alpha < 0)
				alpha = 0;

			if (!alpha) {
				m_notification.erase(m_notification.begin() + i);
				continue;
			}
			
			colour_t col = colour;
			colour = colour_t(col.r(), col.g(), col.b(), alpha);
			custom_ui_colour = vars::cheat::custom_ui_colour ? colour_t::from_hsb(vars::colour::custom_ui_colour / 100, 1.f, 1.f, alpha) : colour_t(218, 23, 91, alpha);
		}
		x_add = 56;
		draw::draw_string(8, adjust_height, custom_ui_colour, font::menu_font, "[Isopropyl] ");
		draw::draw_string(8 + x_add, adjust_height, colour, font::menu_font, message.c_str());
		adjust_height += 14;
	}
}