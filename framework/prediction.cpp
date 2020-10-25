#include "prediction.h"
#include "main.h"
float cur_time;
float frame_time;
int tick_count;
int flags;
CMoveData move_data;
namespace prediction {
	int get_tick_base(c_user_cmd* cmd, c_base_player* local)
	{
		static int nTick = 0;
		static c_user_cmd* pLastCmd = nullptr;

		if (cmd)
		{
			if (!pLastCmd || pLastCmd->has_been_predicted)
				nTick = local->get_tick_base();

			else nTick++;

			pLastCmd = cmd;
		}

		return nTick;
	}

	void run_prediction(c_user_cmd* cmd, c_base_player* local) {
		if (vars::aimbot::smoothing || local->get_life_state() != LIFE_ALIVE)
			return;

		local->set_current_command(cmd);

		*interfaces::random_seed = MD5_PseudoRandom(cmd->command_number) & INT_MAX;

		//set up everything
		cur_time = interfaces::globals->curtime;
		frame_time = interfaces::globals->frametime;
		tick_count = interfaces::globals->tickcount;
		flags = local->get_flags();

		const int old_tick_base = local->get_tick_base();
		const bool old_first_prediction = interfaces::prediction->m_bFirstTimePredicted;
		const bool old_in_prediction = interfaces::prediction->m_bInPrediction;

		memset(&move_data, 0, sizeof(CMoveData)); //make movedata!

		//correct time
		interfaces::globals->curtime = TICKS_TO_TIME(get_tick_base(cmd, local));
		interfaces::globals->frametime = interfaces::globals->interval_per_tick;
		interfaces::globals->tickcount = get_tick_base(cmd, local);

		interfaces::prediction->m_bFirstTimePredicted = false;
		interfaces::prediction->m_bInPrediction = true;

		//start prediction
		//interfaces::prediction->set_local_view_angles(cmd->viewangles);
		interfaces::game_movement->start_track_prediction_errors(local);
		interfaces::prediction->setup_move(local, cmd, &move_data);
		interfaces::game_movement->process_movement(local, &move_data);
		interfaces::prediction->finish_move(local, cmd, &move_data);

		local->set_tickbase(old_tick_base);

		interfaces::prediction->m_bFirstTimePredicted = old_first_prediction;
		interfaces::prediction->m_bInPrediction = old_in_prediction;
	}

	void end_prediction(c_user_cmd* cmd, c_base_player* local) {
		if (vars::aimbot::smoothing)
			return;

		//finish tracking prediction errors lol
		interfaces::game_movement->finish_track_prediction_errors(local);

		//restore the times
		interfaces::globals->curtime = cur_time;
		interfaces::globals->frametime = frame_time;
		interfaces::globals->tickcount = tick_count;

		local->set_current_command(nullptr);
	
		*interfaces::random_seed = -1;
	}
}