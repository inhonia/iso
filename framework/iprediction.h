#ifndef IPREDICTION_H
#define IPREDICTION_H
#ifdef _WIN32
#pragma once
#endif

class iprediction {
	public:
		void setup_move(c_base_player* local, c_user_cmd* cmd, CMoveData* movedata) {
			using o_setup_move = void(__thiscall*)(iprediction*, c_base_player*, c_user_cmd*, class imove_helper*, CMoveData*);
			utilities::call_vfunc<o_setup_move>(this, 18)(this, local, cmd, nullptr, movedata);
		}

		void finish_move(c_base_player* local, c_user_cmd* cmd, CMoveData* movedata) {
			using o_finish_move = void(__thiscall*)(iprediction*, c_base_player*, c_user_cmd*, CMoveData*);
			utilities::call_vfunc<o_finish_move>(this, 19)(this, local, cmd, movedata);
		}
};

#endif