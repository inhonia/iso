enum iscrit_t
{
	// Server observed us firing too many crits
	ISCRIT_OBSERVED_CAP = -5,
	// Crit bucket is empty
	ISCRIT_BUCKET_EMPTY = -4,
	// CanFireCritShot() returns false
	ISCRIT_CANNOT_CRIT = -3,
	// Seed reused
	ISCRIT_REUSED_SEED = -2,
	// Just finished streaming crits, wait 1 sec
	ISCRIT_STREAM_WAIT = -1,
	// Next  crit is determined by random chance
	ISCRIT_RANDOM = 0,
	// Currently critboosted
	ISCRIT_BOOSTED = 3,
	// Currently streaming crits
	ISCRIT_STREAMING = 4,
};

namespace critmanager {
	//void setup()
}