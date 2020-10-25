#pragma once
class c_user_cmd;
class c_base_player;

namespace prediction {
	int get_tick_base(c_user_cmd* cmd, c_base_player* local);
	void run_prediction(c_user_cmd* cmd, c_base_player* local);
	void end_prediction(c_user_cmd* cmd, c_base_player* local);
};