#ifndef FONTS_H
#define FONTS_H
#ifdef _WIN32
#pragma once
#endif

enum fontflags_t
{
	FONTFLAG_NONE,
	FONTFLAG_ITALIC = 0x001,
	FONTFLAG_UNDERLINE = 0x002,
	FONTFLAG_STRIKEOUT = 0x004,
	FONTFLAG_SYMBOL = 0x008,
	FONTFLAG_ANTIALIAS = 0x010,
	FONTFLAG_GAUSSIANBLUR = 0x020,
	FONTFLAG_ROTARY = 0x040,
	FONTFLAG_DROPSHADOW = 0x080,
	FONTFLAG_ADDITIVE = 0x100,
	FONTFLAG_OUTLINE = 0x200,
	FONTFLAG_CUSTOM = 0x400,
	FONTFLAG_BITMAP = 0x800,
};

namespace font {
	extern unsigned long welcome;
	extern unsigned long verdana;
	extern unsigned long verdana2;
	extern unsigned long smallfont;
	extern unsigned long tahoma;
	extern unsigned long calibri;

	extern unsigned long menu_font;
	extern unsigned long tab_font;
	extern unsigned long hud;
	extern unsigned long hud2;
}

#endif // !FONTS_H