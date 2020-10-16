#ifndef IPANEL_H
#define IPANEL_H
#ifdef _WIN32
#pragma once
#endif

class ipanel
{
public:
	int get_child_count(unsigned int vgui_panel) {
		typedef int(__thiscall* OriginalFn)(PVOID, int);
		return 	utilities::call_vfunc<OriginalFn>(this, 17)(this, vgui_panel);
	}
	unsigned int get_child(unsigned int vgui_panel, int index) {
		typedef int(__thiscall* OriginalFn)(PVOID, unsigned int, int);
		return 	utilities::call_vfunc<OriginalFn>(this, 18)(this, vgui_panel, index);
	}
	unsigned int get_parent(unsigned int vgui_panel) {
		typedef int(__thiscall* OriginalFn)(PVOID, unsigned int);
		return 	utilities::call_vfunc<OriginalFn>(this, 20)(this, vgui_panel);
	}
	bool has_parent(unsigned int vgui_panel, unsigned int parent) {
		using original_fn = bool(__thiscall*)(PVOID, unsigned int, unsigned int);
		return utilities::call_vfunc<original_fn>(this, 23)(this, vgui_panel, parent);
	}
	void set_mouse_input_enabled(unsigned int panel, bool state)
	{
		using original_fn = void(__thiscall*)(PVOID, unsigned int, bool);
		utilities::call_vfunc<original_fn>(this, 32)(this, panel, state);
	}
	const char* get_name(unsigned int vgui_panel)
	{
		using original_fn = const char*(__thiscall*)(PVOID, unsigned int);
		return utilities::call_vfunc<original_fn>(this, 36)(this, vgui_panel);
	}
};

#endif // !IPANEL_H