#include "prediction.h"
#include "main.h"
float cur_time;
float frame_time;
CMoveData move_data;
namespace prediction {
	void run_prediction(c_user_cmd* cmd, c_base_player* local) {
		if (vars::aimbot::smoothing)//engine pred on smooth aimbot looks icky ngl
			return;

		//set up everything
		cur_time = interfaces::globals->curtime;
		frame_time = interfaces::globals->frametime;

		memset(&move_data, 0, sizeof(CMoveData)); //make movedata!

		//correct time
		interfaces::globals->curtime = local->get_tick_base() * interfaces::globals->interval_per_tick;
		interfaces::globals->frametime = interfaces::globals->interval_per_tick;

		//set host
		local->set_current_command(cmd);

		//start prediction
		interfaces::game_movement->start_track_prediction_errors(local);
		interfaces::prediction->setup_move(local, cmd, &move_data);
		interfaces::game_movement->process_movement(local, &move_data);
		interfaces::prediction->finish_move(local, cmd, &move_data);

	}

	void end_prediction(c_user_cmd* cmd, c_base_player* local) {
		if (vars::aimbot::smoothing)
			return;

		//finish tracking prediction errors lol
		interfaces::game_movement->finish_track_prediction_errors(local);

		//restore the times
		interfaces::globals->curtime = cur_time;
		interfaces::globals->frametime = frame_time;

		//reset the host
		local->set_current_command(nullptr);

		//adjust tickbase
		local->update_tickbase();
	}
}