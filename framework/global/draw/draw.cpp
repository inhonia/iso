#include "../../main.h"


namespace draw {
	void draw_outlined_rect(int x, int y, int w, int h, colour_t col) {
		interfaces::surface->draw_set_colour_t(col);
		interfaces::surface->draw_outlined_rect(x, y, x + w, y + h);
	}

	void draw_line(int x, int y, int x1, int y1, colour_t col) {
		interfaces::surface->draw_set_colour_t(col);
		interfaces::surface->draw_line(x, y, x1, y1);
	}

	void draw_filled_rect(int x, int y, int w, int h, colour_t col) {
		interfaces::surface->draw_set_colour_t(col);
		interfaces::surface->draw_filled_rect(x, y, x + w, y + h);
	}

	void draw_rect_gradient(int x, int y, int w, int h, colour_t first, colour_t second, bool horizontal)
	{
		interfaces::surface->draw_set_colour_t(first);
		interfaces::surface->draw_filled_rect_fade(x, y, x + w, y + h, first.a(), first.a(), horizontal);
		interfaces::surface->draw_set_colour_t(second);
		interfaces::surface->draw_filled_rect_fade(x, y, x + w, y + h, 0, second.a(), horizontal);
	}

	void draw_textured_polygon(int points, vertex_t* vertices, colour_t colour) {
		static int texture_id = interfaces::surface->create_new_texture_id(true);
		static unsigned char buf[4] = { 255, 255, 255, 255 };
		interfaces::surface->draw_set_texture_rgba(texture_id, buf, 1, 1);
		interfaces::surface->draw_set_colour_t(colour);
		interfaces::surface->draw_set_texture(texture_id);
		interfaces::surface->draw_textured_polygon(points, vertices);
	};

	void draw_circle_outlined(int x, int y, int r, colour_t col)
	{
		interfaces::surface->draw_set_colour_t(col);
		interfaces::surface->draw_outlined_circle(x, y, r, 100); //was 48
	}

	void draw_filled_circle(int x, int y, int r, colour_t col)
	{
		static vertex_t shape[100];
		static float step = PI * 2.0f / 100;
		
		for (int i{}; i < 100; i++) {
			float theta = i * step;
			float x1 = x + r * cos(theta);
			float y1 = y + r * sin(theta);
			shape[i].initialize(vec2_t(x1, y1));
		}
		draw_textured_polygon(100, shape, col);
	}

	void draw_gradient_circle(int x, int y, int r, colour_t col)
	{
		for (int i = 0; i <= r; i++)
		{
			//colour_t temp = colour_t(col.r(), col.g(), col.b(), r - i / col.a());
			draw::draw_filled_circle(x, y, i, col);
		}
	}

	void draw_string(int x, int y, colour_t col, unsigned long font, const char* text, ...) {
		if (text == NULL) return;

		va_list			va_alist;
		char			buffer[1024];
		wchar_t			string[1024];

		va_start(va_alist, text);
		_vsnprintf(buffer, sizeof(buffer), text, va_alist);
		va_end(va_alist);

		MultiByteToWideChar(CP_UTF8, 0, buffer, 256, string, 256);

		interfaces::surface->draw_set_text_pos(x, y);
		interfaces::surface->draw_set_text_font(font);
		interfaces::surface->draw_set_text_colour_t(col.r(), col.g(), col.b(), col.a());
		interfaces::surface->draw_print_text(string, wcslen(string));
	}

	void draw_string_centered(int x, int y, colour_t col, unsigned long font, const char* text, ...) {
		if (text == NULL) return;

		va_list		va_alist;
		char		buffer[1024];
		wchar_t		string[1024];
		int			w, h;

		va_start(va_alist, text);
		_vsnprintf(buffer, sizeof(buffer), text, va_alist);
		va_end(va_alist);

		MultiByteToWideChar(CP_UTF8, 0, buffer, 256, string, 256);

		interfaces::surface->get_text_size(font, string, w, h);
		interfaces::surface->draw_set_text_pos(x - w / 2, y/* - h / 2*/);
		interfaces::surface->draw_set_text_font(font);
		interfaces::surface->draw_set_text_colour_t(col.r(), col.g(), col.b(), col.a());
		interfaces::surface->draw_print_text(string, wcslen(string));
	}

	void draw_3d_box(vec3_t* points, colour_t colour) {
		vec3_t start_pos, end_pos;
		for (int i = 0; i < 3; i++)
		{
			if (visuals::world_to_screen(points[i], start_pos))
				if (visuals::world_to_screen(points[i + 1], end_pos))
					draw_line(start_pos.x, start_pos.y, end_pos.x, end_pos.y, colour);
		}
		if (visuals::world_to_screen(points[0], start_pos))
			if (visuals::world_to_screen(points[3], end_pos))
				draw_line(start_pos.x, start_pos.y, end_pos.x, end_pos.y, colour);

		for (int i = 4; i < 7; i++)
		{
			if (visuals::world_to_screen(points[i], start_pos))
				if (visuals::world_to_screen(points[i + 1], end_pos))
					draw_line(start_pos.x, start_pos.y, end_pos.x, end_pos.y, colour);
		}
		if (visuals::world_to_screen(points[4], start_pos))
			if (visuals::world_to_screen(points[7], end_pos))
				draw_line(start_pos.x, start_pos.y, end_pos.x, end_pos.y, colour);

		if (visuals::world_to_screen(points[0], start_pos))
			if (visuals::world_to_screen(points[6], end_pos))
				draw_line(start_pos.x, start_pos.y, end_pos.x, end_pos.y, colour);

		if (visuals::world_to_screen(points[1], start_pos))
			if (visuals::world_to_screen(points[5], end_pos))
				draw_line(start_pos.x, start_pos.y, end_pos.x, end_pos.y, colour);

		if (visuals::world_to_screen(points[2], start_pos))
			if (visuals::world_to_screen(points[4], end_pos))
				draw_line(start_pos.x, start_pos.y, end_pos.x, end_pos.y, colour);

		if (visuals::world_to_screen(points[3], start_pos))
			if (visuals::world_to_screen(points[7], end_pos))
				draw_line(start_pos.x, start_pos.y, end_pos.x, end_pos.y, colour);
	}

	void draw_sphere(vec3_t origin, float radius, float angle, float scale, colour_t colour)
	{
		std::vector< vertex_t > vertices{};

		// compute angle step for input radius and precision.
		float step = (1.f / radius) + DEG2RAD(angle);

		for (float lat{}; lat < (PI * scale); lat += step) {
			// reset.
			vertices.clear();

			for (float lon{}; lon < (PI * 2); lon += step) {
				vec3_t point{
					origin.x + (radius * std::sin(lat) * std::cos(lon)),
					origin.y + (radius * std::sin(lat) * std::sin(lon)),
					origin.z + (radius * std::cos(lat))
				};

				vec3_t screen;
				if (visuals::world_to_screen(point, screen))
					vertices.emplace_back(vec2_t(screen.x, screen.y));
			}

			if (vertices.empty())
				continue;
			static int texture_id = interfaces::surface->create_new_texture_id(true);
			static unsigned char buf[4] = { 255, 255, 255, 255 };
			interfaces::surface->draw_set_texture_rgba(texture_id, buf, 1, 1);
			interfaces::surface->draw_set_colour_t(colour);
			interfaces::surface->draw_set_texture(texture_id);
			interfaces::surface->draw_textured_poly_line(vertices.data(), vertices.size());
		}
	}
}