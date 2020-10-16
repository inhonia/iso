#ifndef PLAYER_H
#define PLAYER_H
#ifdef _WIN32
#pragma once
#endif

class c_base_weapon;

enum tf_cond
{
	TFCond_Slowed					= (1 << 0),		// Toggled when a player is slowed down. 
	TFCond_Zoomed					= (1 << 1),		// Toggled when a player is zoomed. 
	TFCond_Disguising				= (1 << 2),		// Toggled when a Spy is disguising.  
	TFCond_Disguised				= (1 << 3),		// Toggled when a Spy is disguised. 
	TFCond_Cloaked					= (1 << 4),		// Toggled when a Spy is invisible. 
	TFCond_Ubercharged				= (1 << 5),		// Toggled when a player is ÜberCharged. 
	TFCond_TeleportedGlow			= (1 << 6),		// Toggled when someone leaves a teleporter and has glow beneath their feet. 
	TFCond_Taunting					= (1 << 7),		// Toggled when a player is taunting. 
	TFCond_UberchargeFading			= (1 << 8),		// Toggled when the ÜberCharge is fading. 
	TFCond_CloakFlicker				= (1 << 9),		// Toggled when a Spy is visible during cloak. 
	TFCond_Teleporting				= (1 << 10),	// Only activates for a brief second when the player is being teleported; not very useful. 
	TFCond_Kritzkrieged				= (1 << 11),	// Toggled when a player is being crit buffed by the KritzKrieg. 
	TFCond_TmpDamageBonus			= (1 << 12),	// Unknown what this is for. Name taken from the AlliedModders SDK. 
	TFCond_DeadRingered				= (1 << 13),	// Toggled when a player is taking reduced damage from the Deadringer. 
	TFCond_Bonked					= (1 << 14),	// Toggled when a player is under the effects of The Bonk! Atomic Punch. 
	TFCond_Stunned					= (1 << 15),	// Toggled when a player's speed is reduced from airblast or a Sandman ball. 
	TFCond_Buffed					= (1 << 16),	// Toggled when a player is within range of an activated Buff Banner. 
	TFCond_Charging					= (1 << 17),	// Toggled when a Demoman charges with the shield. 
	TFCond_DemoBuff					= (1 << 18),	// Toggled when a Demoman has heads from the Eyelander. 
	TFCond_CritCola					= (1 << 19),	// Toggled when the player is under the effect of The Crit-a-Cola. 
	TFCond_InHealRadius				= (1 << 20),	// Unused condition, name taken from AlliedModders SDK. 
	TFCond_Healing					= (1 << 21),	// Toggled when someone is being healed by a medic or a dispenser. 
	TFCond_OnFire					= (1 << 22),	// Toggled when a player is on fire. 
	TFCond_Overhealed				= (1 << 23),	// Toggled when a player has >100% health. 
	TFCond_Jarated					= (1 << 24),	// Toggled when a player is hit with a Sniper's Jarate. 
	TFCond_Bleeding					= (1 << 25),	// Toggled when a player is taking bleeding damage. 
	TFCond_DefenseBuffed			= (1 << 26),	// Toggled when a player is within range of an activated Battalion's Backup. 
	TFCond_Milked					= (1 << 27),	// Player was hit with a jar of Mad Milk. 
	TFCond_MegaHeal					= (1 << 28),	// Player is under the effect of Quick-Fix charge. 
	TFCond_RegenBuffed				= (1 << 29),	// Toggled when a player is within a Concheror's range. 
	TFCond_MarkedForDeath			= (1 << 30),	// Player is marked for death by a Fan O'War hit. Effects are similar to TFCond_Jarated. 
	TFCond_NoHealingDamageBuff		= (1 << 31),	// Unknown what this is used for.
	TFCondEx_SpeedBuffAlly			= (1 << 0),		// Toggled when a player gets hit with the disciplinary action. 
	TFCondEx_HalloweenCritCandy		= (1 << 1),		// Only for Scream Fortress event maps that drop crit candy. 
	TFCondEx_CritCanteen			= (1 << 2),		// Player is getting a crit boost from a MVM canteen.
	TFCondEx_CritDemoCharge			= (1 << 3),		// From demo's shield
	TFCondEx_CritHype				= (1 << 4),		// Soda Popper crits. 
	TFCondEx_CritOnFirstBlood		= (1 << 5),		// Arena first blood crit buff. 
	TFCondEx_CritOnWin				= (1 << 6),		// End of round crits. 
	TFCondEx_CritOnFlagCapture		= (1 << 7),		// CTF intelligence capture crits. 
	TFCondEx_CritOnKill				= (1 << 8),		// Unknown what this is for. 
	TFCondEx_RestrictToMelee		= (1 << 9),		// Unknown what this is for. 
	TFCondEx_DefenseBuffNoCritBlock = (1 << 10),	// MvM Buff.
	TFCondEx_Reprogrammed			= (1 << 11),	// MvM Bot has been reprogrammed.
	TFCondEx_PyroCrits				= (1 << 12),	// Player is getting crits from the Mmmph charge. 
	TFCondEx_PyroHeal				= (1 << 13),	// Player is being healed from the Mmmph charge. 
	TFCondEx_FocusBuff				= (1 << 14),	// Player is getting a focus buff.
	TFCondEx_DisguisedRemoved		= (1 << 15),	// Disguised remove from a bot.
	TFCondEx_MarkedForDeathSilent	= (1 << 16),	// Player is under the effects of the Escape Plan/Equalizer or GRU.
	TFCondEx_DisguisedAsDispenser	= (1 << 17),	// Bot is disguised as dispenser.
	TFCondEx_Sapped					= (1 << 18),	// MvM bot is being sapped.
	TFCondEx_UberchargedHidden		= (1 << 19),	// MvM Related
	TFCondEx_UberchargedCanteen		= (1 << 20),	// Player is receiving ÜberCharge from a canteen.
	TFCondEx_HalloweenBombHead		= (1 << 21),	// Player has a bomb on their head from Merasmus.
	TFCondEx_HalloweenThriller		= (1 << 22),	// Players are forced to dance from Merasmus.
	TFCondEx_BulletCharge			= (1 << 26),	// Player is receiving 75% reduced damage from bullets.
	TFCondEx_ExplosiveCharge		= (1 << 27),	// Player is receiving 75% reduced damage from explosives.
	TFCondEx_FireCharge				= (1 << 28),	// Player is receiving 75% reduced damage from fire.
	TFCondEx_BulletResistance		= (1 << 29),	// Player is receiving 10% reduced damage from bullets.
	TFCondEx_ExplosiveResistance	= (1 << 30),	// Player is receiving 10% reduced damage from explosives.
	TFCondEx_FireResistance			= (1 << 31),	// Player is receiving 10% reduced damage from fire.
	TFCondEx2_Stealthed				= (1 << 0),
	TFCondEx2_MedigunDebuff			= (1 << 1),
	TFCondEx2_StealthedUserBuffFade = (1 << 2),
	TFCondEx2_BulletImmune			= (1 << 3),
	TFCondEx2_BlastImmune			= (1 << 4),
	TFCondEx2_FireImmune			= (1 << 5),
	TFCondEx2_PreventDeath			= (1 << 6),
	TFCondEx2_MVMBotRadiowave		= (1 << 7),
	TFCondEx2_HalloweenSpeedBoost	= (1 << 8),		// Wheel has granted player speed boost.
	TFCondEx2_HalloweenQuickHeal	= (1 << 9),		// Wheel has granted player quick heal.
	TFCondEx2_HalloweenGiant		= (1 << 10),	// Wheel has granted player giant mode.
	TFCondEx2_HalloweenTiny			= (1 << 11),	// Wheel has granted player tiny mode.
	TFCondEx2_HalloweenInHell		= (1 << 12),	// Wheel has granted player in hell mode.
	TFCondEx2_HalloweenGhostMode	= (1 << 13),	// Wheel has granted player ghost mode.
	TFCondEx2_Parachute				= (1 << 16),	// Player has deployed the BASE Jumper.
	TFCondEx2_BlastJumping			= (1 << 17),	// Player has sticky or rocket jumped.
	TFCond_MiniCrits				= (TFCond_Buffed | TFCond_CritCola),
	TFCond_IgnoreStates				= (TFCond_Ubercharged | TFCond_Bonked),
	TFCondEx_IgnoreStates			= (TFCondEx_PyroHeal)
};

enum tfclasses_t
{
	TF2_None = 0,
	TF2_Scout,
	TF2_Sniper,
	TF2_Soldier,
	TF2_Demoman,
	TF2_Medic,
	TF2_Heavy,
	TF2_Pyro,
	TF2_Spy,
	TF2_Engineer
};

enum ObserverModes_t {
	OBS_MODE_NONE = 0,		// not in spectator mode
	OBS_MODE_DEATHCAM,		// special mode for death cam animation
	OBS_MODE_FREEZECAM,		// zooms to a target, and freeze-frames on them
	OBS_MODE_FIXED,			// view from a fixed camera position
	OBS_MODE_FIRSTPERSON,	// follow a player in first person view
	OBS_MODE_THIRDPERSON,	// follow a player in third person view
	OBS_MODE_ROAMING,		// free roaming
};

enum life_states
{
	LIFE_ALIVE,
	LIFE_DYING,
	LIFE_DEAD,
	LIFE_RESPAWNABLE,
	LIFE_DISCARDBODY,
};
enum c_base_activity
{
	ACT_INVALID = -1, ACT_RESET, ACT_IDLE, ACT_TRANSITION, ACT_COVER, ACT_COVER_MED, ACT_COVER_LOW, ACT_WALK, ACT_WALK_AIM,
	ACT_WALK_CROUCH, ACT_WALK_CROUCH_AIM, ACT_RUN_PROTECTED, ACT_SCRIPT_CUSTOM_MOVE, ACT_RANGE_ATTACK1, ACT_RANGE_ATTACK2, ACT_RANGE_ATTACK1_LOW, ACT_RANGE_ATTACK2_LOW,
	ACT_DIE_SIMPLE, ACT_DIEBACKWARD, ACT_DIE_FORWARD, ACT_DIE_VIOLENT, ACT_DIE_RAGDOLLL, ACT_FLY, ACT_HOVER, ACT_GLIDE, ACT_SWIM, ACT_JUMP,
	ACT_HOP, ACT_LEAP, ACT_LAND, ACT_CLIMB_UP, ACT_CLIMB_DOWN, ACT_CLIMB_DISMOUNT, ACT_SHIPLADDER_UP, ACT_SHIPLADDER_DOWN, ACT_STRAFE_LEFT,
	ACT_STRAFE_RIGHT, ACT_ROLL_LEFT, ACT_ROLL_RIGHT, ACT_TURN_LEFT, ACT_TURN_RIGHT, ACT_CROUCH, ACT_CROUCH_IDLE, ACT_STAND,
	ACT_USE, ACT_SIGNAL1, ACT_SIGNAL2, ACT_SIGNAL3,
};

enum class_id {
	CAI_BaseNPC = 0,
	CBaseAnimating = 1,
	CBaseAnimatingOverlay = 2,
	CBaseAttributableItem = 3,
	CBaseCombatCharacter = 4,
	CBaseCombatWeapon = 5,
	CBaseDoor = 6,
	CBaseEntity = 7,
	CBaseFlex = 8,
	CBaseGrenade = 9,
	CBaseObject = 10,
	CBaseObjectUpgrade = 11,
	CBaseParticleEntity = 12,
	CBasePlayer = 13,
	CBaseProjectile = 14,
	CBasePropDoor = 15,
	CBaseTeamObjectiveResource = 16,
	CBaseTempEntity = 17,
	CBaseViewModel = 18,
	CBeam = 19,
	CBoneFollower = 20,
	CBonusDuckPickup = 21,
	CBonusPack = 22,
	CBonusRoundLogic = 23,
	CBreakableProp = 24,
	CBreakableSurface = 25,
	CCaptureFlag = 26,
	CCaptureFlagReturnIcon = 27,
	CCaptureZone = 28,
	CColorCorrection = 29,
	CColorCorrectionVolume = 30,
	CCurrencyPack = 31,
	CDynamicLight = 32,
	CDynamicProp = 33,
	CEconEntity = 34,
	CEconWearable = 35,
	CEmbers = 36,
	CEntityDissolve = 37,
	CEntityFlame = 38,
	CEntityParticleTrail = 39,
	CEnvDetailController = 40,
	CEnvParticleScript = 41,
	CEnvProjectedTexture = 42,
	CEnvQuadraticBeam = 43,
	CEnvScreenEffect = 44,
	CEnvScreenOverlay = 45,
	CEnvTonemapController = 46,
	CEnvWind = 47,
	CEyeballBoss = 48,
	CFireSmoke = 49,
	CFireTrail = 50,
	CFish = 51,
	CFogController = 52,
	CFuncAreaPortalWindow = 55,
	CFuncConveyor = 56,
	CFuncForceField = 57,
	CFuncLadder = 58,
	CFuncMonitor = 59,
	CFuncOccluder = 60,
	CFuncPasstimeGoal = 61,
	CFuncReflectiveGlass = 62,
	CFuncRespawnRoom = 63,
	CFuncRespawnRoomVisualizer = 64,
	CFuncRotating = 65,
	CFuncSmokeVolume = 66,
	CFuncTrackTrain = 67,
	CFunc_Dust = 53,
	CFunc_LOD = 54,
	CGameRulesProxy = 68,
	CHalloweenGiftPickup = 69,
	CHalloweenPickup = 70,
	CHalloweenSoulPack = 71,
	CHandleTest = 72,
	CHeadlessHatman = 73,
	CHightower_TeleportVortex = 74,
	CInfoLadderDismount = 75,
	CInfoLightingRelative = 76,
	CInfoOverlayAccessor = 77,
	CLaserDot = 78,
	CLightGlow = 79,
	CMannVsMachineStats = 80,
	CMaterialModifyControl = 81,
	CMerasmus = 82,
	CMerasmusDancer = 83,
	CMonsterResource = 84,
	CObjectCartDispenser = 85,
	CObjectDispenser = 86,
	CObjectSapper = 87,
	CObjectSentrygun = 88,
	CObjectTeleporter = 89,
	CParticleFire = 90,
	CParticlePerformanceMonitor = 91,
	CParticleSystem = 92,
	CPasstimeBall = 93,
	CPasstimeGun = 94,
	CPhysBox = 95,
	CPhysBoxMultiplayer = 96,
	CPhysMagnet = 99,
	CPhysicsProp = 97,
	CPhysicsPropMultiplayer = 98,
	CPlasma = 100,
	CPlayerDestructionDispenser = 101,
	CPlayerResource = 102,
	CPointCamera = 103,
	CPointCommentaryNode = 104,
	CPoseController = 105,
	CPrecipitation = 106,
	CPropVehicleDriveable = 107,
	CRagdollManager = 108,
	CRagdollProp = 109,
	CRagdollPropAttached = 110,
	CRobotDispenser = 111,
	CRopeKeyframe = 112,
	CSceneEntity = 113,
	CShadowControl = 114,
	CSlideshowDisplay = 115,
	CSmokeStack = 116,
	CSniperDot = 117,
	CSpotlightEnd = 118,
	CSprite = 119,
	CSpriteOriented = 120,
	CSpriteTrail = 121,
	CSteamJet = 122,
	CSun = 123,
	CTEArmorRicochet = 128,
	CTEBSPDecal = 141,
	CTEBaseBeam = 129,
	CTEBeamEntPoint = 130,
	CTEBeamEnts = 131,
	CTEBeamFollow = 132,
	CTEBeamLaser = 133,
	CTEBeamPoints = 134,
	CTEBeamRing = 135,
	CTEBeamRingPoint = 136,
	CTEBeamSpline = 137,
	CTEBloodSprite = 138,
	CTEBloodStream = 139,
	CTEBreakModel = 140,
	CTEBubbleTrail = 143,
	CTEBubbles = 142,
	CTEClientProjectile = 144,
	CTEDecal = 145,
	CTEDust = 146,
	CTEDynamicLight = 147,
	CTEEffectDispatch = 148,
	CTEEnergySplash = 149,
	CTEExplosion = 150,
	CTEFireBullets = 151,
	CTEFizz = 152,
	CTEFootprintDecal = 153,
	CTEGaussExplosion = 154,
	CTEGlowSprite = 155,
	CTEImpact = 156,
	CTEKillPlayerAttachments = 157,
	CTELargeFunnel = 158,
	CTEMetalSparks = 160,
	CTEMuzzleFlash = 161,
	CTEParticleSystem = 162,
	CTEPhysicsProp = 163,
	CTEPlayerAnimEvent = 164,
	CTEPlayerDecal = 165,
	CTEProjectedDecal = 166,
	CTEShatterSurface = 167,
	CTEShowLine = 168,
	CTESmoke = 170,
	CTESparks = 171,
	CTESprite = 172,
	CTESpriteSpray = 173,
	CTETFBlood = 176,
	CTETFExplosion = 177,
	CTETFParticleEffect = 178,
	CTEWorldDecal = 179,
	CTFAmmoPack = 180,
	CTFBall_Ornament = 181,
	CTFBaseBoss = 182,
	CTFBaseProjectile = 183,
	CTFBaseRocket = 184,
	CTFBat = 185,
	CTFBat_Fish = 186,
	CTFBat_Giftwrap = 187,
	CTFBat_Wood = 188,
	CTFBonesaw = 189,
	CTFBotHintEngineerNest = 190,
	CTFBottle = 191,
	CTFBreakableMelee = 192,
	CTFBreakableSign = 193,
	CTFBuffItem = 194,
	CTFCannon = 195,
	CTFChargedSMG = 196,
	CTFCleaver = 197,
	CTFClub = 198,
	CTFCompoundBow = 199,
	CTFCrossbow = 200,
	CTFDRGPomson = 201,
	CTFDroppedWeapon = 202,
	CTFFireAxe = 203,
	CTFFists = 204,
	CTFFlameManager = 205,
	CTFFlameRocket = 206,
	CTFFlameThrower = 207,
	CTFFlareGun = 208,
	CTFFlareGun_Revenge = 209,
	CTFGameRulesProxy = 210,
	CTFGasManager = 211,
	CTFGenericBomb = 212,
	CTFGlow = 213,
	CTFGrapplingHook = 214,
	CTFGrenadeLauncher = 215,
	CTFGrenadePipebombProjectile = 216,
	CTFHalloweenMinigame = 217,
	CTFHalloweenMinigame_FallingPlatforms = 218,
	CTFHellZap = 219,
	CTFItem = 220,
	CTFJar = 221,
	CTFJarGas = 222,
	CTFJarMilk = 223,
	CTFKatana = 224,
	CTFKnife = 225,
	CTFLaserPointer = 226,
	CTFLunchBox = 227,
	CTFLunchBox_Drink = 228,
	CTFMechanicalArm = 229,
	CTFMedigunShield = 230,
	CTFMiniGame = 231,
	CTFMinigameLogic = 232,
	CTFMinigun = 233,
	CTFObjectiveResource = 234,
	CTFPEPBrawlerBlaster = 240,
	CTFParachute = 235,
	CTFParachute_Primary = 236,
	CTFParachute_Secondary = 237,
	CTFParticleCannon = 238,
	CTFPasstimeLogic = 239,
	CTFPipebombLauncher = 241,
	CTFPistol = 242,
	CTFPistol_Scout = 243,
	CTFPistol_ScoutPrimary = 244,
	CTFPistol_ScoutSecondary = 245,
	CTFPlayer = 246,
	CTFPlayerDestructionLogic = 247,
	CTFPlayerResource = 248,
	CTFPointManager = 249,
	CTFPowerupBottle = 250,
	CTFProjectile_Arrow = 251,
	CTFProjectile_BallOfFire = 252,
	CTFProjectile_Cleaver = 253,
	CTFProjectile_EnergyBall = 254,
	CTFProjectile_EnergyRing = 255,
	CTFProjectile_Flare = 256,
	CTFProjectile_GrapplingHook = 257,
	CTFProjectile_HealingBolt = 258,
	CTFProjectile_Jar = 259,
	CTFProjectile_JarGas = 260,
	CTFProjectile_JarMilk = 261,
	CTFProjectile_MechanicalArmOrb = 262,
	CTFProjectile_Rocket = 263,
	CTFProjectile_SentryRocket = 264,
	CTFProjectile_SpellBats = 265,
	CTFProjectile_SpellFireball = 266,
	CTFProjectile_SpellKartBats = 267,
	CTFProjectile_SpellKartOrb = 268,
	CTFProjectile_SpellLightningOrb = 269,
	CTFProjectile_SpellMeteorShower = 270,
	CTFProjectile_SpellMirv = 271,
	CTFProjectile_SpellPumpkin = 272,
	CTFProjectile_SpellSpawnBoss = 273,
	CTFProjectile_SpellSpawnHorde = 274,
	CTFProjectile_SpellSpawnZombie = 275,
	CTFProjectile_SpellTransposeTeleport = 276,
	CTFProjectile_Throwable = 277,
	CTFProjectile_ThrowableBreadMonster = 278,
	CTFProjectile_ThrowableBrick = 279,
	CTFProjectile_ThrowableRepel = 280,
	CTFPumpkinBomb = 281,
	CTFRagdoll = 282,
	CTFRaygun = 283,
	CTFReviveMarker = 284,
	CTFRevolver = 285,
	CTFRobotArm = 286,
	CTFRobotDestructionLogic = 290,
	CTFRobotDestruction_Robot = 287,
	CTFRobotDestruction_RobotGroup = 288,
	CTFRobotDestruction_RobotSpawn = 289,
	CTFRocketLauncher = 291,
	CTFRocketLauncher_AirStrike = 292,
	CTFRocketLauncher_DirectHit = 293,
	CTFRocketLauncher_Mortar = 294,
	CTFRocketPack = 295,
	CTFSMG = 305,
	CTFScatterGun = 296,
	CTFShotgun = 297,
	CTFShotgunBuildingRescue = 302,
	CTFShotgun_HWG = 298,
	CTFShotgun_Pyro = 299,
	CTFShotgun_Revenge = 300,
	CTFShotgun_Soldier = 301,
	CTFShovel = 303,
	CTFSlap = 304,
	CTFSniperRifle = 306,
	CTFSniperRifleClassic = 307,
	CTFSniperRifleDecap = 308,
	CTFSodaPopper = 309,
	CTFSpellBook = 310,
	CTFStickBomb = 311,
	CTFStunBall = 312,
	CTFSword = 313,
	CTFSyringeGun = 314,
	CTFTankBoss = 315,
	CTFTauntProp = 316,
	CTFTeam = 317,
	CTFThrowable = 318,
	CTFViewModel = 319,
	CTFWeaponBase = 320,
	CTFWeaponBaseGrenadeProj = 321,
	CTFWeaponBaseGun = 322,
	CTFWeaponBaseMelee = 323,
	CTFWeaponBaseMerasmusGrenade = 324,
	CTFWeaponBuilder = 325,
	CTFWeaponFlameBall = 326,
	CTFWeaponInvis = 327,
	CTFWeaponPDA = 328,
	CTFWeaponPDAExpansion_Dispenser = 332,
	CTFWeaponPDAExpansion_Teleporter = 333,
	CTFWeaponPDA_Engineer_Build = 329,
	CTFWeaponPDA_Engineer_Destroy = 330,
	CTFWeaponPDA_Spy = 331,
	CTFWeaponSapper = 334,
	CTFWearable = 335,
	CTFWearableCampaignItem = 336,
	CTFWearableDemoShield = 337,
	CTFWearableItem = 338,
	CTFWearableLevelableItem = 339,
	CTFWearableRazorback = 340,
	CTFWearableRobotArm = 341,
	CTFWearableVM = 342,
	CTFWrench = 343,
	CTeam = 124,
	CTeamRoundTimer = 126,
	CTeamTrainWatcher = 127,
	CTeamplayRoundBasedRulesProxy = 125,
	CTeleportVortex = 159,
	CTesla = 169,
	CTestTraceline = 175,
	CTest_ProxyToggle_Networkable = 174,
	CVGuiScreen = 344,
	CVoteController = 345,
	CWaterBullet = 346,
	CWaterLODControl = 347,
	CWeaponIFMBase = 348,
	CWeaponIFMBaseCamera = 349,
	CWeaponIFMSteadyCam = 350,
	CWeaponMedigun = 351,
	CWorld = 352,
	CZombie = 353,
	DustTrail = 354,
	MovieExplosion = 355,
	NextBotCombatCharacter = 356,
	ParticleSmokeGrenade = 357,
	RocketTrail = 358,
	SmokeTrail = 359,
	SporeExplosion = 360,
	SporeTrail = 361
};

#define DMG_GENERIC			0			// generic damage -- do not use if you want players to flinch and bleed!
#define DMG_CRUSH			(1 << 0)	// crushed by falling or moving object. 
// NOTE: It's assumed crush damage is occurring as a result of physics collision, so no extra physics force is generated by crush damage.
// DON'T use DMG_CRUSH when damaging entities unless it's the result of a physics collision. You probably want DMG_CLUB instead.
#define DMG_BULLET			(1 << 1)	// shot
#define DMG_SLASH			(1 << 2)	// cut, clawed, stabbed
#define DMG_BURN			(1 << 3)	// heat burned
#define DMG_VEHICLE			(1 << 4)	// hit by a vehicle
#define DMG_FALL			(1 << 5)	// fell too far
#define DMG_BLAST			(1 << 6)	// explosive blast damage
#define DMG_CLUB			(1 << 7)	// crowbar, punch, headbutt
#define DMG_SHOCK			(1 << 8)	// electric shock
#define DMG_SONIC			(1 << 9)	// sound pulse shockwave
#define DMG_ENERGYBEAM		(1 << 10)	// laser or other high energy beam 
#define DMG_PREVENT_PHYSICS_FORCE		(1 << 11)	// Prevent a physics force 
#define DMG_NEVERGIB		(1 << 12)	// with this bit OR'd in, no damage type will be able to gib victims upon death
#define DMG_ALWAYSGIB		(1 << 13)	// with this bit OR'd in, any damage type can be made to gib victims upon death.
#define DMG_DROWN			(1 << 14)	// Drowning


#define DMG_PARALYZE		(1 << 15)	// slows affected creature down
#define DMG_NERVEGAS		(1 << 16)	// nerve toxins, very bad
#define DMG_POISON			(1 << 17)	// blood poisoning - heals over time like drowning damage
#define DMG_RADIATION		(1 << 18)	// radiation exposure
#define DMG_DROWNRECOVER	(1 << 19)	// drowning recovery
#define DMG_ACID			(1 << 20)	// toxic chemicals or acid burns
#define DMG_SLOWBURN		(1 << 21)	// in an oven

#define DMG_REMOVENORAGDOLL	(1<<22)		// with this bit OR'd in, no ragdoll will be created, and the target will be quietly removed.
										// use this to kill an entity that you've already got a server-side ragdoll for

#define DMG_PHYSGUN			(1<<23)		// Hit by manipulator. Usually doesn't do any damage.
#define DMG_PLASMA			(1<<24)		// Shot by Cremator
#define DMG_AIRBOAT			(1<<25)		// Hit by the airboat's gun

#define DMG_DISSOLVE		(1<<26)		// Dissolving!
#define DMG_BLAST_SURFACE	(1<<27)		// A blast on the surface of water that cannot harm things underwater
#define DMG_DIRECT			(1<<28)
#define DMG_BUCKSHOT		(1<<29)		// not quite a bullet. Little, rounder, different.

// NOTE: DO NOT ADD ANY MORE CUSTOM DMG_ TYPES. MODS USE THE DMG_LASTGENERICFLAG BELOW, AND
//		 IF YOU ADD NEW DMG_ TYPES, THEIR TYPES WILL BE HOSED. WE NEED A BETTER SOLUTION.

// TODO: keep this up to date so all the mod-specific flags don't overlap anything.
#define DMG_LASTGENERICFLAG	DMG_BUCKSHOT

#define DMG_USE_HITLOCATIONS					(DMG_AIRBOAT)
#define DMG_HALF_FALLOFF						(DMG_RADIATION)
#define DMG_CRITICAL							(DMG_ACID)
#define DMG_RADIUS_MAX							(DMG_ENERGYBEAM)
#define DMG_IGNITE								(DMG_PLASMA)
#define DMG_USEDISTANCEMOD						(DMG_SLOWBURN)		// NEED TO REMOVE CALTROPS
#define DMG_NOCLOSEDISTANCEMOD					(DMG_POISON)
#define DMG_FROM_OTHER_SAPPER					(DMG_IGNITE)		// USED TO DAMAGE SAPPERS ON MATCHED TELEPORTERS
#define DMG_MELEE								(DMG_BLAST_SURFACE)
#define DMG_DONT_COUNT_DAMAGE_TOWARDS_CRIT_RATE	(DMG_DISSOLVE)		// DON'T USE THIS FOR EXPLOSION DAMAGE YOU WILL MAKE BRANDON SAD AND KYLE SADDER

struct studiohdr_t;
struct mstudiohitboxset_t;
#define MAX_DATATABLES 1024 // must be a power of 2.
#define MAX_DATATABLE_PROPS 4096

#define MAX_ARRAY_ELEMENTS 2048 // a network array should have more that 1024 elements

#define HIGH_DEFAULT -121121.121121f

#define BITS_FULLRES -1    // Use the full resolution of the type being encoded.
#define BITS_WORLDCOORD -2 // Encode as a world coordinate.

#define DT_MAX_STRING_BITS 9
#define DT_MAX_STRING_BUFFERSIZE (1 << DT_MAX_STRING_BITS) // Maximum length of a string that can be sent.

#define STRINGBUFSIZE(className, varName) sizeof (((className *)0)->varName)

// Gets the size of a variable in a class.
#define PROPSIZEOF(className, varName) sizeof (((className *)0)->varName)

// SendProp::m_Flags.
#define SPROP_UNSIGNED (1 << 0) // Unsigned integer data.

#define SPROP_COORD (1 << 1) // If this is set, the float/vector is treated like a world coordinate.
// Note that the bit count is ignored in this case.

#define SPROP_NOSCALE (1 << 2) // For floating point, don't scale into range, just take value as is.

#define SPROP_ROUNDDOWN (1 << 3) // For floating point, limit high value to range minus one bit unit

#define SPROP_ROUNDUP (1 << 4) // For floating point, limit low value to range minus one bit unit

#define SPROP_NORMAL (1 << 5) // If this is set, the vector is treated like a normal (only valid for vectors)

#define SPROP_EXCLUDE (1 << 6) // This is an exclude prop (not excludED, but it points at another prop to be excluded).

#define SPROP_XYZE (1 << 7) // Use XYZ/Exponent encoding for vectors.

#define SPROP_INSIDEARRAY (1 << 8) // This tells us that the property is inside an array, so it shouldn't be put into the
// flattened property list. Its array will point at it when it needs to.

#define SPROP_PROXY_ALWAYS_YES (1 << 9) // Set for datatable props using one of the default datatable proxies like
// SendProxy_DataTableToDataTable that always send the data to all clients.

#define SPROP_CHANGES_OFTEN (1 << 10) // this is an often changed field, moved to head of sendtable so it gets a small index

#define SPROP_IS_A_VECTOR_ELEM (1 << 11) // Set automatically if SPROP_VECTORELEM is used.

#define SPROP_COLLAPSIBLE (1 << 12) // Set automatically if it's a datatable with an offset of 0 that doesn't change the pointer
// (ie: for all automatically-chained base classes).
// In this case, it can get rid of this SendPropDataTable altogether and spare the
// trouble of walking the hierarchy more than necessary.

#define SPROP_COORD_MP (1 << 13)              // Like SPROP_COORD, but special handling for multiplayer games
#define SPROP_COORD_MP_LOWPRECISION (1 << 14) // Like SPROP_COORD, but special handling for multiplayer games where the fractional component only gets a 3 bits instead of 5
#define SPROP_COORD_MP_INTEGRAL (1 << 15)     // SPROP_COORD_MP, but coordinates are rounded to integral boundaries
#define SPROP_NUMFLAGBITS_NETWORKED 16

// This is server side only, it's used to mark properties whose SendProxy_* functions encode against gpGlobals->tickcount (the only ones that currently do this are
//  m_flAnimTime and m_flSimulationTime.  MODs shouldn't need to mess with this probably
#define SPROP_ENCODED_AGAINST_TICKCOUNT (1 << 16)

// See SPROP_NUMFLAGBITS_NETWORKED for the ones which are networked
#define SPROP_NUMFLAGBITS 17

// Used by the SendProp and RecvProp functions to disable debug checks on type sizes.
#define SIZEOF_IGNORE -1

// Use this to extern send and receive datatables, and reference them.
#define EXTERN_SEND_TABLE(tableName) \
	namespace tableName {            \
	extern SendTable g_SendTable;    \
	}
#define EXTERN_RECV_TABLE(tableName) \
	namespace tableName {            \
	extern RecvTable g_RecvTable;    \
	}

#define REFERENCE_SEND_TABLE(tableName) tableName::g_SendTable
#define REFERENCE_RECV_TABLE(tableName) tableName::g_RecvTable

class SendProp;

typedef enum {
	DPT_Int = 0,
	DPT_Float,
	DPT_Vector,
	DPT_VectorXY,
	DPT_String,
	DPT_Array, // An array of the base types (can't be of datatables).
	DPT_DataTable,
#if 0 // We can't ship this since it changes the size of DTVariant to be 20 bytes instead of 16 and that breaks MODs!!!
	DPT_Quaternion,
#endif
	DPT_NUMSendPropTypes
} SendPropType;

class DVariant
{
public:
	DVariant()
	{
		m_Type = DPT_Float;
	}
	DVariant(float val)
	{
		m_Type = DPT_Float;
		m_Float = val;
	}

	//	const char *ToString()
	//	{
	//		static char text[128];
	//
	//		switch (m_Type) {
	//		case DPT_Int:
	//			V_snprintf(text, sizeof(text), "%i", m_Int);
	//			break;
	//		case DPT_Float:
	//			V_snprintf(text, sizeof(text), "%.3f", m_Float);
	//			break;
	//		case DPT_Vector:
	//			V_snprintf(text, sizeof(text), "(%.3f,%.3f,%.3f)", m_Vector[0], m_Vector[1], m_Vector[2]);
	//			break;
	//#if 0 // We can't ship this since it changes the size of DTVariant to be 20 bytes instead of 16 and that breaks MODs!!!
	//		case DPT_Quaternion:
	//			snprintf(text, sizeof(text), "(%.3f,%.3f,%.3f %.3f)",
	//				m_Vector[0], m_Vector[1], m_Vector[2], m_Vector[3]);
	//			break;
	//#endif
	//		case DPT_String:
	//			if (m_pString)
	//				return m_pString;
	//			else
	//				return "NULL";
	//			break;
	//		case DPT_Array:
	//			V_snprintf(text, sizeof(text), "Array");
	//			break;
	//		case DPT_DataTable:
	//			V_snprintf(text, sizeof(text), "DataTable");
	//			break;
	//		default:
	//			V_snprintf(text, sizeof(text), "DVariant type %i unknown", m_Type);
	//			break;
	//		}
	//
	//		return text;
	//	}

	union
	{
		float m_Float;
		long  m_Int;
		char *m_pString;
		void *m_pData; // For DataTables.
#if 0                  // We can't ship this since it changes the size of DTVariant to be 20 bytes instead of 16 and that breaks MODs!!!
		float	m_Vector[4];
#else
		float m_Vector[3];
#endif
	};
	SendPropType m_Type;
};

// This can be used to set the # of bits used to transmit a number between 0 and nMaxElements-1.
inline int NumBitsForCount(int nMaxElements)
{
	int nBits = 0;
	while (nMaxElements > 0) {
		++nBits;
		nMaxElements >>= 1;
	}
	return nBits;
}

#define ADDRESSPROXY_NONE -1

class RecvTable;
class RecvProp;

// This is passed into RecvProxy functions.
class CRecvProxyData
{
public:
	const RecvProp *m_pRecvProp; // The property it's receiving.

	DVariant m_Value; // The value given to you to store.

	int m_iElement; // Which array element you're getting.

	int m_ObjectID; // The object being referred to.
};

//-----------------------------------------------------------------------------
// pStruct = the base structure of the datatable this variable is in (like C_BaseEntity)
// pOut    = the variable that this this proxy represents (like C_BaseEntity::m_SomeValue).
//
// Convert the network-standard-type value in m_Value into your own format in pStruct/pOut.
//-----------------------------------------------------------------------------
typedef void(*RecvVarProxyFn)(const CRecvProxyData *pData, void *pStruct, void *pOut);

// ------------------------------------------------------------------------ //
// ArrayLengthRecvProxies are optionally used to get the length of the
// incoming array when it changes.
// ------------------------------------------------------------------------ //
typedef void(*ArrayLengthRecvProxyFn)(void *pStruct, int objectID, int currentArrayLength);

// NOTE: DataTable receive proxies work differently than the other proxies.
// pData points at the object + the recv table's offset.
// pOut should be set to the location of the object to unpack the data table into.
// If the parent object just contains the child object, the default proxy just does *pOut = pData.
// If the parent object points at the child object, you need to dereference the pointer here.
// NOTE: don't ever return null from a DataTable receive proxy function. Bad things will happen.
typedef void(*DataTableRecvVarProxyFn)(const RecvProp *pProp, void **pOut, void *pData, int objectID);

// This is used to fork over the standard proxy functions to the engine so it can
// make some optimizations.
class CStandardRecvProxies
{
public:
	CStandardRecvProxies();

	RecvVarProxyFn m_Int32ToInt8;
	RecvVarProxyFn m_Int32ToInt16;
	RecvVarProxyFn m_Int32ToInt32;
	RecvVarProxyFn m_FloatToFloat;
	RecvVarProxyFn m_VectorToVector;
};
extern CStandardRecvProxies g_StandardRecvProxies;

class CRecvDecoder;

class RecvProp
{
	// This info comes from the receive data table.
public:
	RecvProp();

	void InitArray(int nElements, int elementStride);

	int GetNumElements() const;
	void SetNumElements(int nElements);

	int GetElementStride() const;
	void SetElementStride(int stride);

	int GetFlags() const;

	const char *GetName() const;
	SendPropType GetType() const;

	RecvTable *GetDataTable() const;
	void SetDataTable(RecvTable *pTable);

	RecvVarProxyFn GetProxyFn() const;
	void SetProxyFn(RecvVarProxyFn fn);

	DataTableRecvVarProxyFn GetDataTableProxyFn() const;
	void SetDataTableProxyFn(DataTableRecvVarProxyFn fn);

	int GetOffset() const;
	void SetOffset(int o);

	// Arrays only.
	RecvProp *GetArrayProp() const;
	void SetArrayProp(RecvProp *pProp);

	// Arrays only.
	void SetArrayLengthProxy(ArrayLengthRecvProxyFn proxy);
	ArrayLengthRecvProxyFn GetArrayLengthProxy() const;

	bool IsInsideArray() const;
	void SetInsideArray();

	// Some property types bind more data to the prop in here.
	const void *GetExtraData() const;
	void SetExtraData(const void *pData);

	// If it's one of the numbered "000", "001", etc properties in an array, then
	// these can be used to get its array property name for debugging.
	const char *GetParentArrayPropName();
	void SetParentArrayPropName(const char *pArrayPropName);

public:
	const char *m_pVarName;
	SendPropType m_RecvType;
	int m_Flags;
	int m_StringBufferSize;
	RecvVarProxyFn m_ProxyFn;

private:
	bool m_bInsideArray; // Set to true by the engine if this property sits inside an array.

						 // Extra data that certain special property types bind to the property here.
	const void *m_pExtraData;

	// If this is an array (DPT_Array).
	RecvProp *m_pArrayProp;
	ArrayLengthRecvProxyFn m_ArrayLengthProxy;

	DataTableRecvVarProxyFn m_DataTableProxyFn; // For RDT_DataTable.

	RecvTable *m_pDataTable; // For RDT_DataTable.
	int m_Offset;

	int m_ElementStride;
	int m_nElements;

	// If it's one of the numbered "000", "001", etc properties in an array, then
	// these can be used to get its array property name for debugging.
	const char *m_pParentArrayPropName;
};

class RecvTable
{
public:
	typedef RecvProp PropType;

	RecvTable();
	RecvTable(RecvProp *pProps, int nProps, const char *pNetTableName);
	~RecvTable();

	void Construct(RecvProp *pProps, int nProps, const char *pNetTableName);

	int GetNumProps();
	RecvProp *GetPropTable(int i);

	const char *GetName();

	// Used by the engine while initializing array props.
	void SetInitialized(bool bInitialized);
	bool IsInitialized() const;

	// Used by the engine.
	void SetInMainList(bool bInList);
	bool IsInMainList() const;

public:
	// Properties described in a table.
	RecvProp * m_pProps;
	int m_nProps;

	// The decoder. NOTE: this covers each RecvTable AND all its children (ie: its children
	// will have their own decoders that include props for all their children).
	CRecvDecoder *m_pDecoder;

	const char *m_pNetTableName; // The name matched between client and server.

private:
	bool m_bInitialized;
	bool m_bInMainList;
};

inline int RecvTable::GetNumProps()
{
	return m_nProps;
}

inline RecvProp *RecvTable::GetPropTable(int i)
{
	// Assert( i >= 0 && i < m_nProps );
	return &m_pProps[i];
}

inline const char *RecvTable::GetName()
{
	return m_pNetTableName;
}

inline void RecvTable::SetInitialized(bool bInitialized)
{
	m_bInitialized = bInitialized;
}

inline bool RecvTable::IsInitialized() const
{
	return m_bInitialized;
}

inline void RecvTable::SetInMainList(bool bInList)
{
	m_bInMainList = bInList;
}

inline bool RecvTable::IsInMainList() const
{
	return m_bInMainList;
}

// ------------------------------------------------------------------------------------------------------ //
// See notes on BEGIN_SEND_TABLE for a description. These macros work similarly.
// ------------------------------------------------------------------------------------------------------ //
#define BEGIN_RECV_TABLE( className, tableName )                                                                                                                                                                                                                                                                                                                                                               \
    BEGIN_RECV_TABLE_NOBASE( className, tableName )                                                                                                                                                                                                                                                                                                                                                            \
    RecvPropDataTable( "baseclass", 0, 0, className::BaseClass::m_pClassRecvTable, DataTableRecvProxy_StaticDataTable ),

#define BEGIN_RECV_TABLE_NOBASE( className, tableName )                                                                                                                                                                                                                                                                                                                                                        \
    template < typename T >                                                                                                                                                                                                                                                                                                                                                                                    \
    int ClientClassInit( T * );                                                                                                                                                                                                                                                                                                                                                                                \
    namespace tableName                                                                                                                                                                                                                                                                                                                                                                                        \
    {                                                                                                                                                                                                                                                                                                                                                                                                          \
    struct ignored;                                                                                                                                                                                                                                                                                                                                                                                            \
    }                                                                                                                                                                                                                                                                                                                                                                                                          \
    template <>                                                                                                                                                                                                                                                                                                                                                                                                \
    int ClientClassInit< tableName::ignored >( tableName::ignored * );                                                                                                                                                                                                                                                                                                                                         \
    namespace tableName                                                                                                                                                                                                                                                                                                                                                                                        \
    {                                                                                                                                                                                                                                                                                                                                                                                                          \
    RecvTable g_RecvTable;                                                                                                                                                                                                                                                                                                                                                                                     \
    int g_RecvTableInit = ClientClassInit( (tableName::ignored *)NULL );                                                                                                                                                                                                                                                                                                                                       \
    }                                                                                                                                                                                                                                                                                                                                                                                                          \
    template <>                                                                                                                                                                                                                                                                                                                                                                                                \
    int ClientClassInit< tableName::ignored >( tableName::ignored * )                                                                                                                                                                                                                                                                                                                                          \
    {                                                                                                                                                                                                                                                                                                                                                                                                          \
        typedef className currentRecvDTClass;                                                                                                                                                                                                                                                                                                                                                                  \
        const char *pRecvTableName = #tableName;                                                                                                                                                                                                                                                                                                                                                               \
        RecvTable &RecvTable = tableName::g_RecvTable;                                                                                                                                                                                                                                                                                                                                                         \
        static RecvProp RecvProps[] = { RecvPropInt( xorstr("should_never_see_this"), 0, sizeof( int ) ), // It adds a dummy property at the start so you can define "empty" SendTables.

#define END_RECV_TABLE()                                                                                                                                                                                                                                                                                                                                                                                       \
    }                                                                                                                                                                                                                                                                                                                                                                                                          \
    ;                                                                                                                                                                                                                                                                                                                                                                                                          \
    RecvTable.Construct( RecvProps + 1, sizeof( RecvProps ) / sizeof( RecvProp ) - 1, pRecvTableName );                                                                                                                                                                                                                                                                                                        \
    return 1;                                                                                                                                                                                                                                                                                                                                                                                                  \
    }

#define RECVINFO( varName ) #varName, offsetof( currentRecvDTClass, varName ), sizeof( ( (currentRecvDTClass *)0 )->varName )
#define RECVINFO_NAME( varName, remoteVarName ) #remoteVarName, offsetof( currentRecvDTClass, varName ), sizeof( ( (currentRecvDTClass *)0 )->varName )
#define RECVINFO_STRING( varName ) #varName, offsetof( currentRecvDTClass, varName ), STRINGBUFSIZE( currentRecvDTClass, varName )
#define RECVINFO_BASECLASS( tableName ) RecvPropDataTable( xorstr("this"), 0, 0, &REFERENCE_RECV_TABLE( tableName ) )
#define RECVINFO_ARRAY( varName ) #varName, offsetof( currentRecvDTClass, varName ), sizeof( ( (currentRecvDTClass *)0 )->varName[ 0 ] ), sizeof( ( (currentRecvDTClass *)0 )->varName ) / sizeof( ( (currentRecvDTClass *)0 )->varName[ 0 ] )

// Just specify the name and offset. Used for strings and data tables.
#define RECVINFO_NOSIZE( varName ) #varName, offsetof( currentRecvDTClass, varName )
#define RECVINFO_DT( varName ) RECVINFO_NOSIZE( varName )
#define RECVINFO_DTNAME( varName, remoteVarName ) #remoteVarName, offsetof( currentRecvDTClass, varName )

void RecvProxy_FloatToFloat(const CRecvProxyData *pData, void *pStruct, void *pOut);
void RecvProxy_VectorToVector(const CRecvProxyData *pData, void *pStruct, void *pOut);
void RecvProxy_QuaternionToQuaternion(const CRecvProxyData *pData, void *pStruct, void *pOut);
void RecvProxy_Int32ToInt8(const CRecvProxyData *pData, void *pStruct, void *pOut);
void RecvProxy_Int32ToInt16(const CRecvProxyData *pData, void *pStruct, void *pOut);
void RecvProxy_StringToString(const CRecvProxyData *pData, void *pStruct, void *pOut);
void RecvProxy_Int32ToInt32(const CRecvProxyData *pData, void *pStruct, void *pOut);

// StaticDataTable does *pOut = pData.
void DataTableRecvProxy_StaticDataTable(const RecvProp *pProp, void **pOut, void *pData, int objectID);

// PointerDataTable does *pOut = *((void**)pData)   (ie: pData is a pointer to the object to decode into).
void DataTableRecvProxy_PointerDataTable(const RecvProp *pProp, void **pOut, void *pData, int objectID);

RecvProp RecvPropFloat(const char *pVarName,
	int offset,
	int sizeofVar = SIZEOF_IGNORE, // Handled by RECVINFO macro, but set to SIZEOF_IGNORE if you don't want to bother.
	int flags = 0,
	RecvVarProxyFn varProxy = RecvProxy_FloatToFloat);

RecvProp RecvPropVector(const char *pVarName,
	int offset,
	int sizeofVar = SIZEOF_IGNORE, // Handled by RECVINFO macro, but set to SIZEOF_IGNORE if you don't want to bother.
	int flags = 0,
	RecvVarProxyFn varProxy = RecvProxy_VectorToVector);

// This is here so the RecvTable can look more like the SendTable.
#define RecvPropQAngles RecvPropVector

#if 0 // We can't ship this since it changes the size of DTVariant to be 20 bytes instead of 16 and that breaks MODs!!!

RecvProp RecvPropQuaternion(
	const char *pVarName,
	int offset,
	int sizeofVar = SIZEOF_IGNORE,	// Handled by RECVINFO macro, but set to SIZEOF_IGNORE if you don't want to bother.
	int flags = 0,
	RecvVarProxyFn varProxy = RecvProxy_QuaternionToQuaternion
);
#endif

RecvProp RecvPropInt(const char *pVarName,
	int offset,
	int sizeofVar = SIZEOF_IGNORE, // Handled by RECVINFO macro, but set to SIZEOF_IGNORE if you don't want to bother.
	int flags = 0,
	RecvVarProxyFn varProxy = 0);

RecvProp RecvPropString(const char *pVarName, int offset, int bufferSize, int flags = 0, RecvVarProxyFn varProxy = RecvProxy_StringToString);

RecvProp RecvPropDataTable(const char *pVarName, int offset, int flags, RecvTable *pTable, DataTableRecvVarProxyFn varProxy = DataTableRecvProxy_StaticDataTable);

RecvProp RecvPropArray3(const char *pVarName, int offset, int sizeofVar, int elements, RecvProp pArrayProp, DataTableRecvVarProxyFn varProxy = DataTableRecvProxy_StaticDataTable);

// Use the macro to let it automatically generate a table name. You shouldn't
// ever need to reference the table name. If you want to exclude this array, then
// reference the name of the variable in varTemplate.
RecvProp InternalRecvPropArray(const int elementCount, const int elementStride, const char *pName, ArrayLengthRecvProxyFn proxy);

//
// Use this if you want to completely manage the way the array data is stored.
// You'll need to provide a proxy inside varTemplate that looks for 'iElement'
// to figure out where to store the specified element.
//
#define RecvPropVirtualArray( arrayLengthProxy, maxArrayLength, varTemplate, propertyName ) varTemplate, InternalRecvPropArray( maxArrayLength, 0, #propertyName, arrayLengthProxy )

// Use this and pass the array name and it will figure out the count and stride automatically.
#define RecvPropVariableLengthArray( arrayLengthProxy, varTemplate, arrayName ) varTemplate, InternalRecvPropArray( sizeof( ( (currentRecvDTClass *)0 )->arrayName ) / PROPSIZEOF( currentRecvDTClass, arrayName[ 0 ] ), PROPSIZEOF( currentRecvDTClass, arrayName[ 0 ] ), #arrayName, arrayLengthProxy )

// Use this and pass the array name and it will figure out the count and stride automatically.
#define RecvPropArray( varTemplate, arrayName ) RecvPropVariableLengthArray( 0, varTemplate, arrayName )

// Use this one to specify the element count and stride manually.
#define RecvPropArray2( arrayLengthProxy, varTemplate, elementCount, elementStride, arrayName ) varTemplate, InternalRecvPropArray( elementCount, elementStride, #arrayName, arrayLengthProxy )

// ---------------------------------------------------------------------------------------- //
// Inlines.
// ---------------------------------------------------------------------------------------- //

inline void RecvProp::InitArray(int nElements, int elementStride)
{
	m_RecvType = DPT_Array;
	m_nElements = nElements;
	m_ElementStride = elementStride;
}

inline int RecvProp::GetNumElements() const
{
	return m_nElements;
}

inline void RecvProp::SetNumElements(int nElements)
{
	m_nElements = nElements;
}

inline int RecvProp::GetElementStride() const
{
	return m_ElementStride;
}

inline void RecvProp::SetElementStride(int stride)
{
	m_ElementStride = stride;
}

inline int RecvProp::GetFlags() const
{
	return m_Flags;
}

inline const char *RecvProp::GetName() const
{
	return m_pVarName;
}

inline SendPropType RecvProp::GetType() const
{
	return m_RecvType;
}

inline RecvTable *RecvProp::GetDataTable() const
{
	return m_pDataTable;
}

inline void RecvProp::SetDataTable(RecvTable *pTable)
{
	m_pDataTable = pTable;
}

inline RecvVarProxyFn RecvProp::GetProxyFn() const
{
	return m_ProxyFn;
}

inline void RecvProp::SetProxyFn(RecvVarProxyFn fn)
{
	m_ProxyFn = fn;
}

inline DataTableRecvVarProxyFn RecvProp::GetDataTableProxyFn() const
{
	return m_DataTableProxyFn;
}

inline void RecvProp::SetDataTableProxyFn(DataTableRecvVarProxyFn fn)
{
	m_DataTableProxyFn = fn;
}

inline int RecvProp::GetOffset() const
{
	return m_Offset;
}

inline void RecvProp::SetOffset(int o)
{
	m_Offset = o;
}

inline RecvProp *RecvProp::GetArrayProp() const
{
	return m_pArrayProp;
}

inline void RecvProp::SetArrayProp(RecvProp *pProp)
{
	m_pArrayProp = pProp;
}

inline void RecvProp::SetArrayLengthProxy(ArrayLengthRecvProxyFn proxy)
{
	m_ArrayLengthProxy = proxy;
}

inline ArrayLengthRecvProxyFn RecvProp::GetArrayLengthProxy() const
{
	return m_ArrayLengthProxy;
}

inline bool RecvProp::IsInsideArray() const
{
	return m_bInsideArray;
}

inline void RecvProp::SetInsideArray()
{
	m_bInsideArray = true;
}

inline const void *RecvProp::GetExtraData() const
{
	return m_pExtraData;
}

inline void RecvProp::SetExtraData(const void *pData)
{
	m_pExtraData = pData;
}

inline const char *RecvProp::GetParentArrayPropName()
{
	return m_pParentArrayPropName;
}

inline void RecvProp::SetParentArrayPropName(const char *pArrayPropName)
{
	m_pParentArrayPropName = pArrayPropName;
}
class client_class
{
private:
	BYTE _chPadding[8];
public:
	char* name;
	RecvTable* table;
	client_class* next_class;
	int class_id;
};

template<typename T>
class singleton
{
protected:
	singleton() {}
	~singleton() {}

	singleton(const singleton&) = delete;
	singleton& operator=(const singleton&) = delete;

	singleton(singleton&&) = delete;
	singleton& operator=(singleton&&) = delete;

public:
	static T& get()
	{
		static T instance{};
		return instance;
	}
};
class CNetVars : public singleton<CNetVars>
{
	struct node;
	using map_type = std::unordered_map<std::string, std::shared_ptr<node>>;

	struct node {
		node(DWORD offset) : offset(offset) {}
		map_type nodes;
		DWORD offset;
	};

	map_type nodes;

public:
	void Initialize();

private:
	void populate_nodes(class RecvTable *recv_table, map_type *map);

	/**
	* get_offset_recursive - Return the offset of the final node
	* @map:	Node map to scan
	* @acc:	Offset accumulator
	* @name:	Netvar name to search for
	*
	* Get the offset of the last netvar from map and return the sum of it and accum
	*/
	DWORD get_offset_recursive(map_type &map, int acc, const char *name)
	{
		return acc + map[name]->offset;
	}

	/**
	* get_offset_recursive - Recursively grab an offset from the tree
	* @map:	Node map to scan
	* @acc:	Offset accumulator
	* @name:	Netvar name to search for
	* @args:	Remaining netvar names
	*
	* Perform tail recursion with the nodes of the specified branch of the tree passed for map
	* and the offset of that branch added to acc
	*/
	template<typename ...args_t>
	DWORD get_offset_recursive(map_type &map, int acc, const char *name, args_t ...args)
	{
		const auto &node = map[name];
		return get_offset_recursive(node->nodes, acc + node->offset, args...);
	}

public:
	/**
	* get_offset - Get the offset of a netvar given a list of branch names
	* @name:	Top level datatable name
	* @args:	Remaining netvar names
	*
	* Initiate a recursive search down the branch corresponding to the specified datable name
	*/
	template<typename ...args_t>
	DWORD get_offset(const char *name, args_t ...args)
	{
		const auto &node = nodes[name];
		return get_offset_recursive(node->nodes, node->offset, args...);
	}
};

template<typename T>
class CDynamicNetvar
{
	DWORD off;
	using sT = typename std::remove_reference<T>::type;
public:
	template<typename... args_t>
	CDynamicNetvar(args_t... a)
	{
		// just assume these wont be accessed until after netvars has been inited

		off = CNetVars::get().get_offset(a...);
	}
	template<typename... args_t>
	CDynamicNetvar(int offset, args_t... a)
	{
		off = CNetVars::get().get_offset(a...) + offset;
	}

	T GetValue(PVOID base) const
	{
		return *reinterpret_cast< T * >((DWORD)base + (DWORD)off);
	}
	void SetValue(PVOID base, T val) const
	{
		*reinterpret_cast<sT* >((DWORD)(base)+((DWORD)(off))) = val;
	}
};

#define DYNVAR(name, type, ...) static CDynamicNetvar<type> ##name( __VA_ARGS__ )
#define DYNVAR_RETURN(type, base, ...) DYNVAR(n, type, __VA_ARGS__); return n.GetValue(base)

#define DYNVAR_OFF(name, type, offset, ...) static CDynamicNetvar<type> ##name(offset, __VA_ARGS__)
#define DYNVAR_OFF_RETURN(type, base, offset, ...) DYNVAR_OFF(n, type, offset, __VA_ARGS__); return n.GetValue(base)
#define DYNVAR_SET(type, base, value, ...) DYNVAR(n, type, __VA_ARGS__); n.SetValue(base,value)

struct MultiPlayerPoseData_t
{
	int		m_iMoveX;
	int		m_iMoveY;
	int		m_iAimYaw;
	int		m_iAimPitch;
	int		m_iBodyHeight;
	int		m_iMoveYaw;
	int		m_iMoveScale;
	float	m_flEstimateYaw;
	float	m_flLastAimTurnTime;
};

struct DebugPlayerAnimData_t
{
	float	m_flSpeed;
	float	m_flAimPitch;
	float	m_flAimYaw;
	float	m_flBodyHeight;
	vec2_t	m_vecMoveYaw;
};

struct MultiPlayerMovementData_t
{
	float m_flWalkSpeed;
	float m_flRunSpeed;
	float m_flSprintSpeed;
	float m_flBodyYawRate;
};

struct AnimState_t
{
	char pad[30];
	vec3_t	 					m_angRender;
	bool						m_bPoseParameterInit;
	MultiPlayerPoseData_t		m_PoseParameterData;
	DebugPlayerAnimData_t 		m_DebugAnimData;
	bool						m_bCurrentFeetYawInitialized;
	float						m_flLastAnimationStateClearTime;
	float 						m_flEyeYaw;
	float 						m_flEyePitch;
	float 						m_flGoalFeetYaw;
	float 						m_flCurrentFeetYaw;
	float 						m_flLastAimTurnTime;
	MultiPlayerMovementData_t	m_MovementData;
};

class c_base_player
{
public:

	void set_current_command(c_user_cmd* cmd) {
		DYNVAR_SET(c_user_cmd*, this - 4, cmd, "DT_BasePlayer", "localdata", "m_hConstraintEntity");
		//DYNVAR_OFF(n, c_user_cmd*, this - 4, "DT_BasePlayer", "localdata", "m_hConstraintEntity");
	}

	void update_tickbase() {
		DYNVAR(n, int, "DT_BasePlayer", "localdata", "m_nTickBase");
		n.SetValue(this, n.GetValue(this) + 1);
	}
	
	void set_tickbase(int tick) {
		DYNVAR_SET(int, this, tick, "DT_BasePlayer", "localdata", "m_nTickBase");
	}

	int get_ammo() {
		DYNVAR_RETURN(int, this, "DT_BasePlayer", "localdata", "m_iAmmo");
	}

	vec3_t get_vec_origin() {
		DYNVAR_RETURN(vec3_t, this, "DT_BaseEntity", "m_vecOrigin");
	}

	uintptr_t get_ehandle() {
		using original_fn = uintptr_t(__thiscall*)(PVOID);
		return utilities::call_vfunc<original_fn>(this, 1)(this);
	}

	client_networkable* get_client_networkable()
	{
		using original_fn = client_networkable * (__thiscall*)(PVOID);
		return utilities::call_vfunc<original_fn>(this, 4)(this);
	}

	client_renderable* get_client_renderable()
	{
		using original_fn = client_renderable * (__thiscall*)(PVOID);
		return utilities::call_vfunc<original_fn>(this, 5)(this);
	}

	PVOID get_collideable()
	{
		using original_fn = PVOID(__thiscall*)(PVOID);
		return utilities::call_vfunc<original_fn>(this, 3)(this);
	}

	client_class* get_client_class() {
		PVOID networkable = (PVOID)(this + 0x8);
		using original_fn = client_class*(__thiscall*)(PVOID);
		return utilities::call_vfunc<original_fn>(networkable, 2)(networkable);
	}

	int get_flags()
	{
		return *reinterpret_cast<int*>(uintptr_t(this) + 0x37C);
	}

	int get_team_num()
	{
		return *reinterpret_cast<int*>(uintptr_t(this) + 0xB0);
	}

	vec3_t get_velocity()
	{
		return *reinterpret_cast<vec3_t*>(uintptr_t(this) + 0x120);
	}

	int get_class_num()
	{
		return *reinterpret_cast<int*>(uintptr_t(this) + 0x154C);
	}

	int get_health()
	{
		using original_fn = int(__thiscall*)(PVOID);
		return utilities::call_vfunc<original_fn>(this, 106)(this);
	}

	int get_max_health() {
		using original_fn = int(__thiscall*)(PVOID);
		return utilities::call_vfunc<original_fn>(this, 107)(this);
	}

	BYTE get_life_state()
	{
		return *reinterpret_cast<BYTE*>(uintptr_t(this) + 0xA5);
	}

	bool is_dead() {
		return (this->get_life_state() != LIFE_ALIVE);
	}

	int get_condition() {
		return *reinterpret_cast<int*>(uintptr_t(this) + 0x18BC);
	}

	void set_condition(int force_condition) {
		*reinterpret_cast<int*>(uintptr_t(this) + 0x18BC) = force_condition;
	}

	float get_simulation_time() {
		return *reinterpret_cast<float*>(uintptr_t(this) + 0x6C);
	}

	vec3_t& get_abs_origin()
	{
		using original_fn = vec3_t&(__thiscall*)(PVOID);
		return utilities::call_vfunc<original_fn>(this, 9)(this);
	}


	vec3_t& get_abs_angles()
	{
		using original_fn = vec3_t & (__thiscall*)(PVOID);
		return utilities::call_vfunc<original_fn>(this, 10)(this);
	}

	void set_abs_origin(vec3_t angles) {
		using setabsoriginfn = void(__thiscall*)(void*, const vec3_t &angles);
		static setabsoriginfn setabsorigin;
		if (!setabsorigin) {
			setabsorigin = (setabsoriginfn)(utilities::find_signature("client.dll", "55 8B EC 56 57 8B F1 E8 ? ? ? ? 8B 7D 08 F3 0F 10 07"));
		}
		setabsorigin(this, angles);
	}

	HANDLE get_ground_entity()
	{
		DYNVAR_RETURN(HANDLE, this, "DT_BasePlayer", "m_hGroundEntity");
	}

	const vec3_t& get_render_angles()
	{
		void* renderable = (PVOID)(this + 0x4);
		typedef const vec3_t& (__thiscall* original_fn)(PVOID);
		return utilities::call_vfunc<original_fn>(renderable, 2)(renderable);
	}

	void set_render_angles(const vec3_t& v)
	{
		vec3_t* abs_angs = const_cast<vec3_t*>(&this->get_render_angles());
		*abs_angs = v;
	}

	void update_client_anim()
	{
		void* renderable = (PVOID)(this + 0x4);
		typedef void (__thiscall* original_fn)(PVOID);
		return utilities::call_vfunc<original_fn>(renderable, 3)(renderable);
	}

	float get_conveyor_speed()
	{
		DYNVAR_RETURN(float, this, "DT_FuncConveyor", "m_flConveyorSpeed");
	}

	vec3_t get_eye_position()
	{
		if (!this)
			return vec3_t(0, 0, 0);
		return *reinterpret_cast<vec3_t*>(uintptr_t(this) + 0xFC) + get_abs_origin(); // m_vecViewOffset[0]
	}

	void set_abs_angles(const vec3_t &v)
	{
		vec3_t* abs_angs = const_cast<vec3_t*>(&this->get_abs_angles());
		*abs_angs = v;
	}

	bool is_dormant()
	{
		using original_fn = bool(__thiscall*)(PVOID);
		return utilities::call_vfunc<original_fn>(get_client_networkable(), 8)(get_client_networkable());
	}

	int get_condnew() {
		DYNVAR_RETURN(int, this, "DT_TFPlayer", "m_Shared", "m_nPlayerCond");
	}

	vec3_t get_punch_angles()
	{
		return *reinterpret_cast<vec3_t*>(this + 0xE8C);
	}

	HANDLE get_wearable_owner() {
		DYNVAR_RETURN(HANDLE, this, "DT_TFWearable", "m_hWeaponAssociatedWith");
	}

	HANDLE get_owner() {
		DYNVAR_RETURN(HANDLE, this, "DT_BaseEntity", "m_hOwnerEntity");
	}

		HANDLE get_builder() {
			DYNVAR_RETURN(HANDLE, this, "DT_BaseObject", "m_hBuilder");
		}

		int get_level() {
			DYNVAR_RETURN(int, this, "DT_BaseObject", "m_iUpgradeLevel");
		}

		bool is_sapped() {
			DYNVAR_RETURN(bool, this, "DT_BaseObject", "m_bHasSapper");
		}

		bool is_carried() {
			DYNVAR_RETURN(bool, this, "DT_BaseObject", "m_bCarried");
		}

		bool is_placing() {
			DYNVAR_RETURN(bool, this, "DT_BaseObject", "m_bPlacing");
		}

		bool is_building() {
			DYNVAR_RETURN(bool, this, "DT_BaseObject", "m_bBuilding");
		}

		int get_state() {
			DYNVAR_RETURN(int, this, "DT_ObjectSentrygun", "m_iState");
		}

		float percent_constructed() {
			if (this->is_building()) {
				DYNVAR_RETURN(float, this, "DT_BaseObject", "m_flPercentageConstructed");
			} else {
				return 0.0f;
			}
		}

		int metal_upgraded() {
			DYNVAR_RETURN(int, this, "DT_BaseObject", "m_iUpgradeMetal");
		}

		int get_object_health() {
			DYNVAR_RETURN(int, this, "DT_BaseObject", "m_iHealth");
		}

		int get_object_max_health() {
			DYNVAR_RETURN(int, this, "DT_BaseObject", "m_iMaxHealth");
		}

	vec3_t& get_collideable_mins()
	{
		using original_fn = vec3_t&(__thiscall*)(PVOID);
		return utilities::call_vfunc<original_fn>(get_collideable(), 1)(get_collideable());
	}

	vec3_t& get_collideable_maxs()
	{
		using original_fn = vec3_t&(__thiscall*)(PVOID);
		return utilities::call_vfunc<original_fn>(get_collideable(), 2)(get_collideable());
	}

	matrix3x4_t& get_rgfl_coordinate_frame()
	{
		using original_fn = matrix3x4_t&(__thiscall*)(PVOID);
		return utilities::call_vfunc<original_fn>(get_client_renderable(), 34)(get_client_renderable());
	}

	bool setup_bones(matrix3x4_t* bone_to_world_out, int max_bones, int bone_mask, float current_time)
	{
		PVOID renderable = (PVOID)(this + 0x4);
		using original_fn = bool(__thiscall*)(PVOID, matrix3x4_t*, int, int, float);
		return utilities::call_vfunc<original_fn>(renderable, 16)(renderable, bone_to_world_out, max_bones, bone_mask, current_time);
	}

	vec3_t get_bone_positon(int bone) {
		matrix3x4_t matrix[128];
		setup_bones(matrix, 128, 0x100, 0);
		return vec3_t(matrix[bone][0][3], matrix[bone][1][3], matrix[bone][2][3]);
	}

	int draw_model(int flags) {
		using original_fn = int(__thiscall*)(PVOID, int);
		return utilities::call_vfunc<original_fn>(get_client_renderable(), 10)(get_client_renderable(), flags);
	}

	bool is_player_bot() {
		DYNVAR_RETURN(bool, this, "DT_TFPlayer", "m_bIsABot");
	}

	/*void set_abs_origin(vec3_t &vector) {
		using original_fn = void(__thiscall*)(PVOID, vec3_t&);
		static original_fn original = (original_fn)utilities::find_signature("client.dll", "55 8B EC 56 57 8B F1 E8 ? ? ? ? 8B 7D 08 F3");
		return original(this, vector);
	}*/

	DWORD* get_model() {
		PVOID renderable = (PVOID)(this + 0x4);
		using original_fn = DWORD * (__thiscall*)(PVOID);
		return utilities::call_vfunc<original_fn>(renderable, 9)(renderable);
	}

	int get_index() {
		PVOID networkable = (PVOID)(this + 0x8);
		using original_fn = int(__thiscall*)(PVOID);
		return utilities::call_vfunc<original_fn>(networkable, 9)(networkable);
	}
	int get_tick_base() {
		return *reinterpret_cast<int*>(uintptr_t(this) + 0x114C);
	}
	void get_render_bounds(vec3_t &min, vec3_t &max) {
		PVOID renderable = (PVOID)(this + 0x4);
		using original_fn = void(__thiscall*)(PVOID, vec3_t&, vec3_t&);
		utilities::call_vfunc<original_fn>(renderable, 20)(renderable, min, max);
	}
	vec3_t get_world_space_center() {
		if (!this)
			return vec3_t(0, 0, 0);
		vec3_t min, max, out; this->get_render_bounds(min, max);
		out = this->get_abs_origin(); out[2] += (min[2] + max[2]) / 2;
		return out;
	}
	int get_pipe_type() {
		DYNVAR_RETURN(int, this, "DT_TFProjectile_Pipebomb", "m_iType");
	}
	vec3_t get_pipe_initv() {
		DYNVAR_RETURN(vec3_t, this, "DT_TFProjectile_pipebomb", "m_vInitialVelocity");
	}
	HANDLE get_thrower() {
		DYNVAR_RETURN(HANDLE, this, "DT_BaseGrenade", "m_hThrower");
	}
	void force_taunt_cam(bool forced) {
		DYNVAR(n, bool, "DT_TFPlayer", "m_nForceTauntCam");
		n.SetValue(this, forced);
	}
	int get_observer_mode() {
		DYNVAR_RETURN(int, this, "DT_BasePlayer", "m_iObserverMode");
	}
	HANDLE get_observer_target() {
		return *reinterpret_cast<HANDLE*>(uintptr_t(this) + 0x10A0);
	}
	void force_taunt_yaw(float forced_yaw) {
		DYNVAR(n, float, "DT_TFPlayer", "m_flTauntYaw");
		n.SetValue(this, forced_yaw);
	}
	void update_glow_effect() {
		using original_fn = void (__thiscall*)(PVOID);
		return utilities::call_vfunc<original_fn>(this, 226)(this);
	}
	void destroy_glow_effect() {
		using original_fn = void(__thiscall*)(PVOID);
		return utilities::call_vfunc<original_fn>(this, 227)(this);
	}
	void force_glow_effect(bool forced) {
		DYNVAR(n, bool, "DT_TFPlayer", "m_bGlowEnabled");
		n.SetValue(this, forced);
	}
	bool is_glow_effect_enabled()
	{
		DYNVAR(n, bool, "DT_TFPlayer", "m_bGlowEnabled");
		return n.GetValue(this);
	}

	float get_sim_time() {
		DYNVAR_RETURN(float, this, "DT_BaseEntity", "m_flSimulationTime");
	}
	float get_anim_time() {
		DYNVAR_RETURN(float, this, "DT_BaseEntity", "AnimTimeMustBeFirst", "m_flAnimTime");
	}
	float get_cycle() {
		DYNVAR_RETURN(float, this, "DT_BaseAnimating", "serveranimdata", "m_flCycle");
	}
	int get_tickbase_net() {
		DYNVAR_RETURN(int, this, "DT_BasePlayer", "localdata", "m_nTickBase");
	}
	int get_sequence() {
		DYNVAR_RETURN(int, this, "DT_BaseAnimating", "m_nSequence");
	}

	AnimState_t* get_animstate()
	{
		return *reinterpret_cast<AnimState_t**>(this + 0x1BF8);
	}

	c_base_weapon* get_active_weapon();
	const char* get_model_name();
	int register_glow_object(colour_t &colour, bool render_when_occluded, bool render_when_unoccluded);
	bool has_glow_effect();
};

class c_base_weapon : public c_base_player {
public:
	char* get_name() {
		using original_fn = char*(__thiscall*)(PVOID);
		return utilities::call_vfunc<original_fn>(this, 329)(this);
	}
	char* get_print_name() {
		using original_fn = char*(__thiscall*)(PVOID);
		return utilities::call_vfunc<original_fn>(this, 330)(this);
	}
	float get_projectile_speed() { //note this is "not modified by your actual item scheme"
		using original_fn = float(__thiscall*)(PVOID);
		return utilities::call_vfunc<original_fn>(this, 534)(this);
	}
	float get_projectile_gravity() { //note this is "not modified by your actual item scheme"
		using original_fn = float(__thiscall*)(PVOID);
		return utilities::call_vfunc<original_fn>(this, 535)(this);
	}
	int get_item_definition_index() {
		DYNVAR_RETURN(int, this, "DT_EconEntity", "m_AttributeManager", "m_Item", "m_iItemDefinitionIndex");
	}
	int get_slot() {
		using original_fn = int(__thiscall*)(PVOID);
		return utilities::call_vfunc<original_fn>(this, 327)(this);
	}
	HANDLE get_weapon_owner() {
		DYNVAR_RETURN(HANDLE, this, "DT_BaseCombatWeapon", "m_hOwner");
	}
	int get_clip_1() {
		DYNVAR_RETURN(int, this, "DT_BaseCombatWeapon", "LocalWeaponData", "m_iClip1");
	}
	int get_clip_2() {
		DYNVAR_RETURN(int, this, "DT_BaseCombatWeapon", "LocalWeaponData", "m_iClip2");
	}
	int get_prim_ammo_type() {
		DYNVAR_RETURN(int, this, "DT_BaseCombatWeapon", "LocalWeaponData", "m_iPrimaryAmmoType");
	}
	int get_sec_ammo_type() {
		DYNVAR_RETURN(int, this, "DT_BaseCombatWeapon", "LocalWeaponData", "m_iSecondaryAmmoType");
	}
	float last_fire_time() {
		DYNVAR_RETURN(float, this, "DT_TFWeaponBase", "LocalActiveTFWeaponData", "m_flLastFireTime");
	}
	float reload_prior_next_fire() {
		DYNVAR_RETURN(float, this, "DT_TFWeaponBase", "LocalActiveTFWeaponData", "m_flReloadPriorNextFire");
	}
	float get_next_attack() {
		DYNVAR_RETURN(float, this, "DT_BaseCombatWeapon", "LocalActiveWeaponData", "m_flNextPrimaryAttack");
	}
	float get_next_melee_attack() {
		DYNVAR_RETURN(float, this, "DT_BaseCombatWeapon", "LocalActiveWeaponData", "m_flNextSecondaryAttack");
	}
	bool ready_to_backstab() {
		DYNVAR_RETURN(bool, this, "DT_TFWeaponKnife", "m_bReadyToBackstab");
	}
	float get_charge() {
		DYNVAR_RETURN(float, this, "DT_TFSniperRifle", "SniperRifleLocalData", "m_flChargedDamage");
	}
	float begin_charge_time() {
		DYNVAR_RETURN(float, this, "DT_WeaponPipebombLauncher", "PipebombLauncherLocalData", "m_flChargeBeginTime");
	}
	vec3_t get_weapon_spread() {
		using original_fn = vec3_t(__thiscall*)(PVOID);
		return utilities::call_vfunc<original_fn>(this, 286)(this);
	}
	int get_damage_type() {
		using original_fn = int(__thiscall*)(PVOID);
		return utilities::call_vfunc<original_fn>(this, 378)(this);
	}
	bool can_shoot(c_base_player* local);
};
#endif // !PLAYER_H