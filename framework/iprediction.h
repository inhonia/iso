#ifndef IPREDICTION_H
#define IPREDICTION_H
#ifdef _WIN32
#pragma once
#endif

class iprediction {
	public:
		void set_local_view_angles(vec3_t& angs) {
			using set_local_view_angles = void(__thiscall*)(iprediction*, vec3_t&);
			utilities::call_vfunc<set_local_view_angles>(this, 16)(this, angs);
		}

		void setup_move(c_base_player* local, c_user_cmd* cmd, CMoveData* movedata) {
			using o_setup_move = void(__thiscall*)(iprediction*, c_base_player*, c_user_cmd*, class imove_helper*, CMoveData*);
			utilities::call_vfunc<o_setup_move>(this, 18)(this, local, cmd, nullptr, movedata);
		}

		void finish_move(c_base_player* local, c_user_cmd* cmd, CMoveData* movedata) {
			using o_finish_move = void(__thiscall*)(iprediction*, c_base_player*, c_user_cmd*, CMoveData*);
			utilities::call_vfunc<o_finish_move>(this, 19)(this, local, cmd, movedata);
		}

		uint32_t m_hLastGround;
		bool	 m_bInPrediction;
		bool	 m_bFirstTimePredicted;
		bool	 m_bOldCLPredictValue;
		bool	 m_bEnginePaused;
		int		 m_nPreviousStartFrame;
		int		 m_nCommandsPredicted;
		int		 m_nServerCommandsAcknowledged;
		int		 m_bPreviousAckHadErrors;
		int		 m_nIncomingPacketNumber;
		float	 m_flIdealPitch;
};

#endif