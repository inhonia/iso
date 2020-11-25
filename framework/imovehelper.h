#pragma once

class CMoveHelper
{
public:
	void SetHost(c_base_player* host)
	{
		typedef void(__thiscall* FN)(PVOID, c_base_player*);
		utilities::call_vfunc<FN>(this, 0)(this, host);
	}
};