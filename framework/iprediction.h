#ifndef IPREDICTION_H
#define IPREDICTION_H
#ifdef _WIN32
#pragma once
#endif

class IPrediction
{
public:
public:
	virtual		 ~IPrediction(void) {};
	virtual void Init(void) = 0;
	virtual void Shutdown(void) = 0;
	virtual void Update
	(
		int startframe,
		bool validframe,
		int incoming_acknowledged,
		int outgoing_command
	) = 0;

	virtual void PreEntityPacketReceived(int commands_acknowledged, int current_world_update_packet) = 0;
	virtual void PostEntityPacketReceived(void) = 0;
	virtual void PostNetworkDataReceived(int commands_acknowledged) = 0;
	virtual void OnReceivedUncompressedPacket(void) = 0;
	virtual void GetViewOrigin(vec3_t& org) = 0;
	virtual void SetViewOrigin(vec3_t& org) = 0;
	virtual void GetViewAngles(vec3_t& ang) = 0;
	virtual void SetViewAngles(vec3_t& ang) = 0;
	virtual void GetLocalViewAngles(vec3_t& ang) = 0;
	virtual void SetLocalViewAngles(vec3_t& ang) = 0;
};

class C_Prediction : public IPrediction
{
public:
	virtual		 ~C_Prediction();
	virtual void Init();
	virtual void Shutdown();
	virtual void Update
	(
		int startframe,
		bool validframe,
		int incoming_acknowledged,
		int outgoing_command
	);

	virtual void OnReceivedUncompressedPacket();
	virtual void PreEntityPacketReceived(int commands_acknowledged, int current_world_update_packet);
	virtual void PostEntityPacketReceived();
	virtual void PostNetworkDataReceived(int commands_acknowledged);
	virtual bool InPrediction();
	virtual bool IsFirstTimePredicted();
	virtual int	 GetIncomingPacketNumber();
	virtual void GetViewOrigin(vec3_t& org);
	virtual void SetViewOrigin(vec3_t& org);
	virtual void GetViewAngles(vec3_t& ang);
	virtual void SetViewAngles(vec3_t& ang);
	virtual void GetLocalViewAngles(vec3_t& ang);
	virtual void SetLocalViewAngles(vec3_t& ang);
	virtual void RunCommand(c_base_player* player, c_user_cmd* ucmd, CMoveHelper* moveHelper);
	virtual void SetupMove(c_base_player* player, c_user_cmd* ucmd, CMoveHelper* pHelper, CMoveData* move);
	virtual void FinishMove(c_base_player* player, c_user_cmd* ucmd, CMoveData* move);
	virtual void SetIdealPitch(c_base_player* player, const vec3_t& origin, const vec3_t& angles, const vec3_t& viewheight);
	virtual void _Update
	(
		bool received_new_world_update,
		bool validframe,
		int incoming_acknowledged,
		int outgoing_command
	);

	//CHandle< c_base_player > m_hLastGround;
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

#define VCLIENT_PREDICTION_INTERFACE_VERSION "VClientPrediction001"


#endif