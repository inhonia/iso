#pragma once
#include "main.h"

#define get_a(col) (((col)&0xff000000)>>24)
#define get_r(col) (((col)&0x00ff0000)>>16)
#define get_g(col) (((col)&0x0000ff00)>>8)
#define get_b(col) ((col)&0x000000ff)

enum circle_type
{
	FULL, HALF, QUARTER
};

enum text_align
{
	LEFT, CENTER, RIGHT
};

struct vertex
{
	float x, y, z, rhw;
	unsigned long color;
};

namespace dx_render
{
	namespace fonts
	{
		extern LPD3DXFONT font_franklin;
	}

	class render
	{
	public:
		void initialize();
		void reset();

		inline bool is_initialized()
		{
			return m_initialized;
		};

		void create_objs();
		void release_objs();

		void begin();
		void end();

		int string_width(ID3DXFont* font, std::string str);
		int string_height(ID3DXFont* font, std::string str);

		void line(int x, int y, int x2, int y2, D3DCOLOR color);
		void rect(int x, int y, int w, int h, D3DCOLOR color);

		void rect_filled(int x, int y, int w, int h, D3DCOLOR color);

		void text(std::string text, int x, int y, int align, LPD3DXFONT font, bool bordered, bool dropshadow, D3DCOLOR color); //align: LEFT, CENTER, RIGHT

	private:
		struct screen_t
		{
			float w;
			float h;
			float xc;
			float yc;
		} m_screen;

		LPDIRECT3DDEVICE9			m_device;
		LPDIRECT3DVERTEXBUFFER9		m_vb;
		LPDIRECT3DINDEXBUFFER9		m_ib;
		IDirect3DStateBlock9*		m_stateblock;

		LPD3DXLINE		m_line;
		LPD3DXSPRITE	m_sprite;

		bool m_candraw = false;
		static bool m_initialized;
	};

	extern render* dx_render;
}
