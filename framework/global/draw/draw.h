#ifndef DRAW_H
#define DRAW_H
#ifdef _WIN32
#pragma once
#endif

namespace draw {
	extern void draw_outlined_rect(int x, int y, int w, int h, colour_t col);
	extern void draw_circle_outlined(int x, int y, int r, colour_t col);
	extern void draw_textured_polygon(int points, vertex_t* vertices, colour_t colour);
	extern void draw_filled_circle(int x, int y, int r, colour_t col);
	extern void draw_gradient_circle(int x, int y, int r, colour_t col);
	extern void draw_line(int x, int y, int x1, int y1, colour_t col);
	extern void draw_filled_rect(int x, int y, int w, int h, colour_t col);
	extern void draw_string(int x, int y, colour_t col, unsigned long font, const char* text, ...);
	extern void draw_string_centered(int x, int y, colour_t col, unsigned long font, const char* text, ...);
	extern void draw_rect_gradient(int x, int y, int w, int h, colour_t first, colour_t second, bool horizontal);
	extern void draw_3d_box(vec3_t* points, colour_t colour);
	extern void draw_sphere(vec3_t origin, float radius, float angle, float scale, colour_t color);
}

#endif // !DRAW_H