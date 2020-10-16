#ifndef IGAMEMOVEMENT_H
#define IGAMEMOVEMENT_H
#ifdef _WIN32
#pragma once
#endif

class CMoveData {
public:
	bool m_bFirstRunOfFunctions : 1;
	bool m_bGameCodeMovedPlayer : 1;

	HANDLE m_nPlayerHandle; // edict index on server, client entity handle
								 // on client

	int m_nImpulseCommand;     // Impulse command issued.
	vec3_t m_vecViewAngles;    // Command view angles (local space)
	vec3_t m_vecAbsViewAngles; // Command view angles (world space)
	int m_nButtons;            // Attack buttons.
	int m_nOldButtons;         // From host_client->oldbuttons;
	float m_flForwardMove;
	float m_flSideMove;
	float m_flUpMove;

	float m_flMaxSpeed;
	float m_flClientMaxSpeed;

	// Variables from the player edict (sv_player) or entvars on the client.
	// These are copied in here before calling and copied out after calling.
	vec3_t m_vecVelocity; // edict::velocity    // Current movement
						  // direction.
	vec3_t m_vecAngles;   // edict::angles
	vec3_t m_vecOldAngles;

	// Output only
	float m_outStepHeight; // how much you climbed this move
	vec3_t m_outWishVel;   // This is where you tried
	vec3_t m_outJumpVel;   // This is your jump velocity

						   // Movement constraints (radius 0 means no constraint)
	vec3_t m_vecConstraintCenter;
	float m_flConstraintRadius;
	float m_flConstraintWidth;
	float m_flConstraintSpeedFactor;

	void SetAbsOrigin(const vec3_t& vec);
	const vec3_t& GetAbsOrigin() const;

private:
	vec3_t m_vecAbsOrigin; // edict::origin
};

class igame_movement {
public:
	virtual ~igame_movement(void) {}

	//process the current movement command
	virtual void process_movement(c_base_player* player, CMoveData* move) = 0;
	virtual void start_track_prediction_errors(c_base_player* player) = 0;
	virtual void finish_track_prediction_errors(c_base_player* player) = 0;
	virtual void diff_print(char const* fmt, ...) = 0;

	//allows other parts of the engine to find out the normal and ducked player
	//bbox sizes

	virtual vec3_t get_player_mins(bool ducked) const = 0;
	virtual vec3_t get_player_maxs(bool ducked) const = 0;
	virtual vec3_t get_player_view_offset(bool ducked) const = 0;
};

#endif