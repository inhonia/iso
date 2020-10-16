#ifndef ICLIENTMODESHARED_H
#define ICLIENTMODESHARED_H
#ifdef _WIN32
#pragma once
#endif

class iclientmodeshared {
public:
	bool is_chat_panel_out_of_focus(void) { //DOES NOT WORK
		typedef PVOID(__thiscall* OriginalFn)(PVOID);
		PVOID c_hud_chat = utilities::call_vfunc<OriginalFn>(this, 19)(this);

		if (c_hud_chat) {
			return *(PFLOAT)((DWORD)c_hud_chat + 252) == 0;
		}

		return false;
	}
};

#endif // !ICLIENTMODESHARED_H