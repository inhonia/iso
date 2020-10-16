#pragma once
#include "renderer.h"

dx_render::render* dx_render::dx_render = new dx_render::render();

using namespace dx_render;

namespace dx_render
{
	namespace fonts
	{
		LPD3DXFONT font_franklin;
	}
}

bool render::m_initialized = false;

void render::create_objs()
{
	D3DXCreateLine(m_device, &m_line);
	D3DXCreateSprite(m_device, &m_sprite);
	D3DXCreateFontA(m_device, 13, 0, 500, 0, 0, ANSI_CHARSET, OUT_DEFAULT_PRECIS, DRAFT_QUALITY, DEFAULT_PITCH, xorstr("Franklin Gothic"), &fonts::font_franklin);
}

void render::initialize()
{
	if (!m_initialized)
	{
		m_device = **(IDirect3DDevice9***)(utilities::find_signature(xorstr("shaderapidx9.dll"), xorstr("A1 ?? ?? ?? ?? 50 8B 08 FF 51 0C")) + 1);
		create_objs();
		m_initialized = true;
	}
}

void render::release_objs()
{
	if (!m_candraw)
		return;

	if (m_line)
		m_line->Release();

	if (m_sprite)
		m_sprite->Release();

	if (fonts::font_franklin)
		fonts::font_franklin->Release();
}

void render::begin()
{
	if (!m_candraw)
		return;
}

void render::end()
{
}

void render::reset()
{
	D3DVIEWPORT9 screen;
	m_device->GetViewport(&screen);
	m_screen.w = screen.Width;
	m_screen.h = screen.Height;
	m_screen.xc = screen.Width / 2.0f;
	m_screen.yc = screen.Height / 2.0f;
}

int render::string_width(ID3DXFont* font, std::string str)
{
	auto rect = RECT();
	font->DrawTextA(0, str.c_str(), strlen(str.c_str()), &rect, DT_CALCRECT, D3DCOLOR_ARGB(0, 0, 0, 0));
	return rect.right - rect.left;
}

int render::string_height(ID3DXFont* font, std::string str)
{
	auto rect = RECT();
	font->DrawTextA(0, str.c_str(), strlen(str.c_str()), &rect, DT_CALCRECT, D3DCOLOR_ARGB(0, 0, 0, 0));
	return rect.bottom - rect.top;
}

void render::text(std::string text, int x, int y, int align, LPD3DXFONT font, bool bordered, bool dropshadow, D3DCOLOR color)
{
	RECT rect;
	auto alpha = get_a(color);

	switch (align)
	{
	case LEFT:
		if (bordered)
		{
			SetRect(&rect, x - 1, y, x - 1, y);
			font->DrawTextA(NULL, text.c_str(), -1, &rect, DT_LEFT | DT_NOCLIP, D3DCOLOR_ARGB(alpha, 0, 0, 0));
			SetRect(&rect, x + 1, y, x + 1, y);
			font->DrawTextA(NULL, text.c_str(), -1, &rect, DT_LEFT | DT_NOCLIP, D3DCOLOR_ARGB(alpha, 0, 0, 0));
			SetRect(&rect, x, y - 1, x, y - 1);
			font->DrawTextA(NULL, text.c_str(), -1, &rect, DT_LEFT | DT_NOCLIP, D3DCOLOR_ARGB(alpha, 0, 0, 0));
			SetRect(&rect, x, y + 1, x, y + 1);
			font->DrawTextA(NULL, text.c_str(), -1, &rect, DT_LEFT | DT_NOCLIP, D3DCOLOR_ARGB(alpha, 0, 0, 0));
		}

		if (dropshadow) { 
			SetRect(&rect, x + 1, y + 1, x + 1, y + 1);  font->DrawTextA(NULL, text.c_str(), -1, &rect, DT_LEFT | DT_NOCLIP, D3DCOLOR_ARGB(alpha, 0, 0, 0)); 
		}

		SetRect(&rect, x, y, x, y);
		font->DrawTextA(NULL, text.c_str(), -1, &rect, DT_LEFT | DT_NOCLIP, color);
		break;

	case CENTER:
		if (bordered)
		{
			SetRect(&rect, x - 1, y, x - 1, y);
			font->DrawTextA(NULL, text.c_str(), -1, &rect, DT_CENTER | DT_NOCLIP, D3DCOLOR_ARGB(alpha, 0, 0, 0));
			SetRect(&rect, x + 1, y, x + 1, y);
			font->DrawTextA(NULL, text.c_str(), -1, &rect, DT_CENTER | DT_NOCLIP, D3DCOLOR_ARGB(alpha, 0, 0, 0));
			SetRect(&rect, x, y - 1, x, y - 1);
			font->DrawTextA(NULL, text.c_str(), -1, &rect, DT_CENTER | DT_NOCLIP, D3DCOLOR_ARGB(alpha, 0, 0, 0));
			SetRect(&rect, x, y + 1, x, y + 1);
			font->DrawTextA(NULL, text.c_str(), -1, &rect, DT_CENTER | DT_NOCLIP, D3DCOLOR_ARGB(alpha, 0, 0, 0));
		}

		if (dropshadow) { 
			SetRect(&rect, x + 1, y + 1, x + 1, y + 1);  font->DrawTextA(NULL, text.c_str(), -1, &rect, DT_CENTER | DT_NOCLIP, D3DCOLOR_ARGB(alpha, 0, 0, 0));
		}

		SetRect(&rect, x, y, x, y);
		font->DrawTextA(NULL, text.c_str(), -1, &rect, DT_CENTER | DT_NOCLIP, color);
		break;

	case RIGHT:
		if (bordered)
		{
			SetRect(&rect, x - 1, y, x - 1, y);
			font->DrawTextA(NULL, text.c_str(), -1, &rect, DT_RIGHT | DT_NOCLIP, D3DCOLOR_ARGB(alpha, 0, 0, 0));
			SetRect(&rect, x + 1, y, x + 1, y);
			font->DrawTextA(NULL, text.c_str(), -1, &rect, DT_RIGHT | DT_NOCLIP, D3DCOLOR_ARGB(alpha, 0, 0, 0));
			SetRect(&rect, x, y - 1, x, y - 1);
			font->DrawTextA(NULL, text.c_str(), -1, &rect, DT_RIGHT | DT_NOCLIP, D3DCOLOR_ARGB(alpha, 0, 0, 0));
			SetRect(&rect, x, y + 1, x, y + 1);
			font->DrawTextA(NULL, text.c_str(), -1, &rect, DT_RIGHT | DT_NOCLIP, D3DCOLOR_ARGB(alpha, 0, 0, 0));
		}
		if (dropshadow) { 
			SetRect(&rect, x + 1, y + 1, x + 1, y + 1);  font->DrawTextA(NULL, text.c_str(), -1, &rect, DT_RIGHT | DT_NOCLIP, D3DCOLOR_ARGB(alpha, 0, 0, 0));
		}
		SetRect(&rect, x, y, x, y);
		font->DrawTextA(NULL, text.c_str(), -1, &rect, DT_RIGHT | DT_NOCLIP, color);
		break;
	}
}

void render::line(int x, int y, int x2, int y2, D3DCOLOR color)
{
	vertex pVertex[2] = { { x, y, 0.0f, 1.0f, color },{ x2, y2, 0.0f, 1.0f, color } };
	m_device->SetFVF(D3DFVF_XYZRHW | D3DFVF_DIFFUSE);
	m_device->SetTexture(0, nullptr);
	m_device->SetRenderState(D3DRS_LIGHTING, FALSE);
	m_device->SetRenderState(D3DRS_ZENABLE, D3DZB_FALSE);
	m_device->SetRenderState(D3DRS_FOGENABLE, FALSE);
	m_device->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
	m_device->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
	m_device->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	m_device->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
	m_device->SetRenderState(D3DRS_LIGHTING, FALSE);
	m_device->SetRenderState(D3DRS_STENCILENABLE, FALSE);
	DWORD colorwrite, srgbwrite;
	m_device->GetRenderState(D3DRS_COLORWRITEENABLE, &colorwrite);
	m_device->GetRenderState(D3DRS_SRGBWRITEENABLE, &srgbwrite);
	m_device->SetRenderState(D3DRS_COLORWRITEENABLE, 0xffffffff);
	m_device->SetRenderState(D3DRS_SRGBWRITEENABLE, false);
	m_device->DrawPrimitiveUP(D3DPT_LINELIST, 1, &pVertex, sizeof(vertex));
	m_device->SetRenderState(D3DRS_COLORWRITEENABLE, colorwrite);
	m_device->SetRenderState(D3DRS_SRGBWRITEENABLE, srgbwrite);
}

void render::rect(int x, int y, int w, int h, D3DCOLOR color) //unfilled rectangle, previously a BORDERED_RECT... bad naming
{
	int x2 = x + w;
	int y2 = y + h;
	line(x, y, x2, y, color);
	line(x2, y, x2, y2, color);
	line(x2, y2, x, y2, color);
	line(x, y2, x, y, color);
}

void render::rect_filled(int x, int y, int w, int h, D3DCOLOR color)
{
	vertex pVertex[4] = { { x, y + h, 0.0f, 1.0f, color },{ x, y, 0.0f, 1.0f, color },{ x + w, y + h, 0.0f, 1.0f, color },{ x + w, y, 0.0f, 1.0f, color } };
	m_device->SetFVF(D3DFVF_XYZRHW | D3DFVF_DIFFUSE);
	m_device->SetTexture(0, nullptr);
	m_device->SetRenderState(D3DRS_LIGHTING, FALSE);
	m_device->SetRenderState(D3DRS_ZENABLE, D3DZB_FALSE);
	m_device->SetRenderState(D3DRS_FOGENABLE, FALSE);
	m_device->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
	m_device->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
	m_device->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	m_device->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
	m_device->SetRenderState(D3DRS_LIGHTING, FALSE);
	m_device->SetRenderState(D3DRS_STENCILENABLE, FALSE);
	m_device->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, 2, pVertex, sizeof(vertex));
}

