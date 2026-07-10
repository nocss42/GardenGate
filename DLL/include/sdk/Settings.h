#pragma once

#include <cstdint>

#include "sdk/Array.h"

namespace fb
{
namespace gw1
{
enum BackendType : __int32
{
    Backend_Lan = 0x0,
    Backend_Blaze = 0x1,
    Backend_Peer = 0x2,
    Backend_Local = 0x3,
    Backend_Playground = 0x4,
    Backend_Count_ = 0x5,
};

enum OnlineEnvironment : __int32
{
    OnlineEnvironment_Development = 0x0,
    OnlineEnvironment_Test = 0x1,
    OnlineEnvironment_Certification = 0x2,
    OnlineEnvironment_Production = 0x3,
    OnlineEnvironment_Count = 0x4,
};

enum GamePlatform : __int32
{
    GamePlatform_Ps3 = 0x0,
    GamePlatform_Win32 = 0x1,
    GamePlatform_Xenon = 0x2,
    GamePlatform_Gen4a = 0x3,
    GamePlatform_Gen4b = 0x4,
    GamePlatform_Any = 0x5,
    GamePlatform_Invalid = 0x6,
    GamePlatformCount = 0x7,
};

enum ServerTypeValues : int32_t
{
    ServerType_OFFICIAL = 0,
    ServerType_RANKED = 1,
    ServerType_UNRANKED = 2,
    ServerType_PRIVATE = 3
};

enum VoiceChannel : int32_t
{
    VoiceChannel_Off = 0,
    VoiceChannel_Team = 1,
    VoiceChannel_Squad = 2,
    VoiceChannel_Loopback = 3,
    VoiceChannel_Count = 4
};

struct DataContainer
{
    char __pad[16];
};

struct SystemSettings : DataContainer
{
    char* Name;
    GamePlatform Platform;
};

struct SyncedBFSettings : SystemSettings
{
    float TeamSwitchingCooldownTime;
    float GameModeCounterModifier;
    float RoundTimeLimit;
    float SpottedTime;
    float TimeRequiredToSpot;
    float SpottingInterval;
    float SpottingFov;
    float SpottingDistance;
    float SpotOnFireMultiplier;
    float SpotIconFadeDistance;
    float SpotIconFadeEndDistance;
    String JesusModeForPlayerTag;
    int32_t Team1Faction;
    int32_t Team2Faction;
    int32_t Team3Faction;
    int32_t Team4Faction;
    String FieldSpecLevels;
    ServerTypeValues ServerType;
    bool AllUnlocksUnlocked;
    bool NoMinimap;
    bool NoHud;
    bool NoMinimapSpotting;
    bool No3dSpotting;
    bool NoNameTag;
    bool OnlySquadLeaderSpawn;
    bool TeamSwitchingAllowed;
    bool CommanderEnabled;
    bool RevertBackToBF3KillerCamera;
    bool DisableHitIndicators;
    bool DisableVehicleCommanderActions;
    bool FieldUpgradeSystemActive;
};

struct OnlineSettings : SystemSettings
{
    BackendType Backend;
    BackendType PeerBackend;
    OnlineEnvironment Environment;
    intptr_t Provider;
    Array<intptr_t> Platforms;
    String ServiceNameOverride;
    int32_t BlazeLogLevel;
    int32_t DirtySockLogLevel;
    intptr_t RichPresence;
    intptr_t LicenseConfig;
    String MatchmakingOptions;
    String MatchmakingMode;
    String Region;
    String Country;
    String PingSite;
    String MatchmakingToken;
    uint32_t NegativeUserCacheRefreshPeriod;
    String ServerLoginEmail;
    String ServerLoginPassword;
    String ServerLoginPersonaName;
    int32_t BlazeServerConnectionTimeout;
    int32_t BlazeServerTimeout;
    int32_t BlazeClientConnectionTimeout;
    int32_t BlazeClientTimeout;
    bool ClientIsPresenceEnabled;
    bool ServerIsPresenceEnabled;
    bool UseExternalLoginFlow;
    bool IsSecure;
    bool EnableQoS;
    bool MatchmakeImmediately;
    bool ServerIsReconfigurable;
    bool SupportHostMigration;
    bool MultipleVirtualMachines;
    bool DirtySockVoipEnabled;
    bool ServerAllowAnyReputation;
    bool DebugSkipSignIn;
    bool IsOriginEnvProd;
};

struct NetworkSettings : DataContainer
{
    int32_t ProtocolVersion;
    String TitleId;
    uint32_t ClientPort;
    uint32_t ServerPort;
    uint32_t MaxGhostCount;
    uint32_t MaxClientToServerGhostCount;
    uint32_t MaxClientCount;
    uint32_t MaxClientFrameSize;
    uint32_t MaxServerFrameSize;
    String XlspAddress;
    String ServerAddress;
    String ClientConnectionDebugFilePrefix;
    String ServerConnectionDebugFilePrefix;
    float TimeNudgeGhostFrequencyFactor;
    float TimeNudgeBias;
    double ConnectTimeout;
    uint32_t MaxLocalPlayerCount;
    bool IncrementServerPortOnFail;
    bool UseFrameManager;
    bool TimeSyncEnabled;
};

struct ServerSettings : SystemSettings
{
    String InstancePath;
    uint32_t RemoteControlPort;
    uint32_t MaxQueriesPerSecond;
    String SavePoint;
    float TimeoutTime;
    uint32_t PlayerCountNeededForMultiplayer;
    String DebugMenuClick;
    float LoadingTimeout;
    float IngameTimeout;
    float OutgoingFrequency;
    uint32_t IncomingRate;
    uint32_t OutgoingRate;
    String Playlist;
    int32_t DedicatedServerCpu;
    uint32_t SaveGameVersion;
    String ServerName;
    String ServerPassword;
    float VehicleSpawnDelayModifier;
    float HumanHealthMultiplier;
    float RespawnTimeModifier;
    String AdministrationPassword;
    String RemoteAdministrationPort;
    bool QueryProviderEnabled;
    bool DebrisClusterEnabled;
    bool VegetationEnabled;
    bool WaterPhysicsEnabled;
    bool IsDesertingAllowed;
    bool IsRenderDamageEvents;
    bool RespawnOnDeathPosition;
    bool IsStatsEnabled;
    bool IsNetworkStatsEnabled;
    bool IsAiEnabled;
    bool IsDestructionEnabled;
    bool IsSoldierAnimationEnabled;
    bool IsSoldierDetailedCollisionEnabled;
    bool LoadSavePoint;
    bool DisableCutscenes;
    bool HavokVisualDebugger;
    bool HavokCaptureToFile;
    bool ShowTriggerDebugText;
    bool TimeoutGame;
    bool AILooksIntoCamera;
    bool DeathmatchDebugInfo;
    bool JobEnable;
    bool ThreadingEnable;
    bool DrawActivePhysicsObjects;
    bool IsRanked;
    bool UnlockResolver;
    bool ScoringLogEnabled;
    bool ForcePlaylist;
    bool AutoUnspawnBangers;
    bool RegulatedAIThrottle;
    bool EnableAnimationCulling;
    bool FallBackToSquadSpawn;
    bool VehicleSpawnAllowed;
    bool AdministrationEnabled;
    bool AdministrationLogEnabled;
    bool AdministrationTimeStampLogNames;
    bool AdministrationEventsEnabled;
    bool AdministrationServerNameRestricted;
};

struct BFServerSettings : SystemSettings
{
    String ForceTeamForPlayerTag;
    int32_t ForceTeamForPlayerTagTeam;
    String ForceSquadForPlayerTag;
    int32_t ForceSquadForPlayerTagSquad;
    String ForceTeamAndSquadForPlayers;
    float InActivityTimeOut;
    float InActivityThreshold;
    uint32_t NoInteractivityBanRoundCount;
    uint32_t TeamKillCountForKick;
    float TeamKillValueForKick;
    float TeamKillValueIncrease;
    float TeamKillValueDecreasePerSecond;
    uint32_t TeamKillKickForBan;
    int32_t RoundMaxPlayerCount;
    int32_t RoundMinPlayerCount;
    int32_t RoundLockdownCountdown;
    int32_t RoundRestartCountdown;
    int32_t RoundWarmupTimeout;
    String BannerUrl;
    String ServerPreset;
    String ServerType;
    String ServerExperience;
    String ServerDescription;
    String ServerMessage;
    String ServerAdministrationSettings;
    String ServerAdministrationMapRotation;
    String ServerAdministrationMapRotationInformation;
    String Spectators;
    String PunkBusterVersion;
    String PVZBlazePlaylist;
    uint32_t TeamSwitchImbalanceLimit;
    uint32_t GameSize;
    uint32_t CommanderSize;
    String GameMod;
    VoiceChannel DefaultVoiceChannel;
    int32_t DeathmatchFriendZoneFallbackCount;
    float TeamSwitchCooldownTime;
    int32_t ServerConfigurationRefreshInterval;
    String OverrideServerConfigurationStartTime;
    float AlternateEORTimeout;
    int32_t MaxNumSoldierCorpses;
    int32_t CommanderRankThreshold;
    bool AutoBalance;
    bool OverrideAutoBalance;
    bool DisableBattlePickups;
    bool IsManDownRotationEnabled;
    bool IsKillerCameraEnabled;
    bool MapSequencerEnabled;
    bool PVZBlazePlaylistEnabled;
    bool KickAPlayerOnVIPJoin;
    bool AlwaysAllowedToSpectate;
};
} // namespace gw1

namespace gw2
{
typedef struct SettingsBase
{
    uint8_t _opaque[16];
} SettingsBase;

typedef enum GamePlatform
{
    GamePlatform_Ps3 = 0,
    GamePlatform_Win32 = 1,
    GamePlatform_Xenon = 2,
    GamePlatform_Gen4a = 3,
    GamePlatform_Gen4b = 4,
    GamePlatform_Android = 5,
    GamePlatform_iOS = 6,
    GamePlatform_OSX = 7,
    GamePlatform_Linux = 8,
    GamePlatform_Any = 9,
    GamePlatform_Invalid = 10,
    GamePlatformCount = 11
} GamePlatform;

enum BackendType : uint32_t
{
    Backend_Lan = 0,
    Backend_Blaze = 1,
    Backend_Peer = 2,
    Backend_Local = 3,
    Backend_Playground = 4,
    Backend_LocalNoStorage = 5,
    Backend_Count_ = 6
};

enum LogLevelType : uint32_t
{
    LogLevel_Default = 0,
    LogLevel_Fatal = 1,
    LogLevel_Error = 2,
    LogLevel_Warn = 3,
    LogLevel_Info = 4,
    LogLevel_Debug = 5,
    LogLevel_Trace = 6
};

enum OnlineEnvironment : uint32_t
{
    OnlineEnvironment_Development = 0,
    OnlineEnvironment_Test = 1,
    OnlineEnvironment_Certification = 2,
    OnlineEnvironment_Production = 3,
    OnlineEnvironment_Count = 4
};

enum VoiceChannel : __int32
{
    VoiceChannel_Off = 0x0,
    VoiceChannel_Team = 0x1,
    VoiceChannel_Squad = 0x2,
    VoiceChannel_Loopback = 0x3,
    VoiceChannel_Count = 0x4,
};

struct SystemSettings : SettingsBase
{
    char* Name;
    GamePlatform Platform;
};

enum TeamId
{
    TeamNeutral = 0,
    Team1 = 1,
    Team2 = 2,
    Team3 = 3,
    Team4 = 4,
    Team5 = 5,
    Team6 = 6,
    Team7 = 7,
    Team8 = 8,
    Team9 = 9,
    Team10 = 10,
    Team11 = 11,
    Team12 = 12,
    Team13 = 13,
    Team14 = 14,
    Team15 = 15,
    Team16 = 16,
    TeamIdCount = 17
};

enum SKU
{
    WW = 0,
    EU = 1,
    US = 2,
    JPN = 3,
    ROA = 4,
    RU = 5,
    EN = 6
};

enum ServerTypeValues : int32_t
{
    ServerType_OFFICIAL = 0,
    ServerType_RANKED = 1,
    ServerType_UNRANKED = 2,
    ServerType_PRIVATE = 3
};

struct GameModeSettings : SystemSettings
{
    void* GameModeConfiguration;
    int RushInstantCaptureUpToFlag;
    unsigned int CapturePointShowDebugInfo;
    unsigned int MaxSelfRevives;
    unsigned int TeamVanquishKillCount;
    float ForceRespawnManDownTime;
    float SkipManDownTime;
    int OverrideRoundStartPlayerCount;
    unsigned int StoredDifficultyIndex;
    int SkipSpawnScreenClassIndex;
    int SkipSpawnScreenCostumeIndex;
    float SkipSpawnRandomCostumes_UnrevivableAliveTimeThreshold;
    int ForceHUBMissionPlants;
    int ForceHUBMissionZombies;
    int ForceKOTHWave;
    char* ForceHUBTimeOfDay;
    char* HUBOverrideAISpawningID;
    float HUBUnderworldChallengeTimer;
    int CommunityMeterForceScore;
    unsigned int MaxSimultaniousMinionHealers;
    unsigned int MaxSimultaniousHeroHealers;
    unsigned int MaxSimultaniousBossHealers;
    unsigned int MaxSquadSize;
    unsigned int LocalPlayMaxBotsPerTeam;
    unsigned int LocalPlaySelectedAICharacterSetZombiesUnlockId;
    unsigned int LocalPlaySelectedAICharacterSetPlantsUnlockId;
    List<void*> TeamHighlightColors;
    char* CrazyOptionsDataPath;
    char* AvailableCostumes;
    float CrazyOption_TimeBomb_FullTimer;
    float CrazyOption_TimeBomb_ShortTimer;
    float CrazyOption_TimeBomb_BumpTime;
    float CrazyOptionsGlobalScoreModifier;
    int ModeTeamId;
    char* SplashToLevel;
    char* SplashToLevelInclusion;
    int AttackingTeamId;
    int EndlessSkipToWaveIndex;
    int EndlessSkipIndexMax;
    int FakeEndlessMaxWaveCompleted;
    int PlantsThreatLevel;
    int ZombieThreatLevel;
    int SeasonalQuestCooldownTime;
    unsigned int XPFishStarCost;
    float XPFishXPGrant;
    float PauseRespawnWaitTime;
    bool SelfReviveEnabled;
    bool BoastsEnabled;
    bool EnableAIRevives;
    bool ShowAINametags;
    bool AlwaysShowNametags;
    bool UseCoopAIWeaponData;
    bool CountAIKills;
    bool RenderingHighlightAllCharacters;
    bool RenderingHighlightFriendlyCharacters;
    bool E3StageDemo;
    bool EnableCharacterSubRealms;
    bool ShowTeamSwitchWarning;
    bool UseInfiniteManDownTime;
    bool SkipManDown;
    bool AllowManualTeamSwitch;
    bool Force1HpCharacterHealth;
    bool UnlimitedPrimaryAmmo;
    bool SkipPreroundCountdown;
    bool SkipEOR;
    bool SkipSpawnRandomCostumes;
    bool DisableHUBAISpawns;
    bool ShowHUBAISpawningDebug;
    bool ForceEnableKOTH;
    bool SkipIntroHubNIS;
    bool ForceHUBCustomizeUnlock;
    bool HUBAISpawnDebug;
    bool HUBOverrideAISpawning;
    bool HUBForceShowSplitScreenPortal;
    bool HUBUnderworldOpenAllPortals;
    bool ShowCommunityChallengeDebug;
    bool ForceScoreOverride;
    bool LocalPlayAIWaitForFirstPlayerToSpawn;
    bool CrazyOption1;
    bool CrazyOption2;
    bool CrazyOption3;
    bool CrazyOption4;
    bool CrazyOption5;
    bool CrazyOption6;
    bool CrazyOption7;
    bool CrazyOption8;
    bool CrazyOption9;
    bool HasSeenIntroVideo;
    bool IsGnomeSummonedCharacter;
    bool IsBETA;
    bool HasSeenESRBScreen;
    bool HasSeenForcedMailBox;
    bool ForceDefaultCustomizations;
    bool DebugSpawnNet;
};

struct OriginSettings : SystemSettings
{
    char* ContentId;
    char Title;
    char MultiplayerId;
    char Language;
    bool Enabled;
    bool RequiredForOnline;
    bool Log;
    bool AllowProductionEnvironment;
    bool DisableInLiveEditMode;
    bool IsOriginEnvProd;
};

struct NetworkSettings : SettingsBase
{
    uint32_t ProtocolVersion;
    char* TitleId;
    uint32_t ClientPort;
    uint32_t ServerPort;
    uint32_t MaxGhostCount;
    uint32_t MaxClientToServerGhostCount;
    uint32_t MaxClientCount;
    uint32_t MaxClientFrameSize;
    uint32_t MaxServerFrameSize;
    char* XlspAddress;
    char* ServerAddress;
    char* ClientConnectionDebugFilePrefix;
    char* ServerConnectionDebugFilePrefix;
    float SinglePlayerTimeNudgeGhostFrequencyFactor;
    float SinglePlayerTimeNudgeBias;
    float SinglePlayerTimeNudge;
    float SinglePlayerTimeNudgeSmoothingTime;
    float SinglePlayerLatencyFactor;
    float MemorySocketTimeNudgeGhostFrequencyFactor;
    float MemorySocketTimeNudgeBias;
    float MemorySocketTimeNudge;
    float MemorySocketTimeNudgeSmoothingTime;
    float MemorySocketLatencyFactor;
    float LocalHostTimeNudgeGhostFrequencyFactor;
    float LocalHostTimeNudgeBias;
    float LocalHostTimeNudge;
    float LocalHostTimeNudgeSmoothingTime;
    float LocalHostLatencyFactor;
    float DefaultTimeNudgeGhostFrequencyFactor;
    float DefaultTimeNudgeBias;
    float DefaultTimeNudge;
    float DefaultTimeNudgeSmoothingTime;
    float DefaultLatencyFactor;
    float ConnectTimeout;
    float PacketLossLogInterval;
    uint32_t ValidLocalPlayersMask;
    uint32_t DesiredLocalPlayersMask;
    uint32_t PersistentLocalPlayersMask;
    bool SinglePlayerUserFrequencyFactorOverride;
    bool SinglePlayerAutomaticTimeNudge;
    bool MemorySocketUserFrequencyFactorOverride;
    bool MemorySocketAutomaticTimeNudge;
    bool LocalHostUserFrequencyFactorOverride;
    bool LocalHostAutomaticTimeNudge;
    bool DefaultUserFrequencyFactorOverride;
    bool DefaultAutomaticTimeNudge;
    bool IncrementServerPortOnFail;
    bool UseFrameManager;
    bool TimeSyncEnabled;
    bool MLUREnabled;
};

struct GameSettings : SystemSettings
{
    uint32_t MaxPlayerCount;
    uint32_t MaxSpectatorCount;
    uint32_t MinPlayerCountElimination;
    uint32_t LogFileCollisionMode;
    uint32_t LogFileRotationHistoryLength;
    char* Level;
    char* StartPoint;
    intptr_t InputConfiguration;
    char* ActiveGameModeViewDefinition;
    intptr_t GameModeViewDefinitions;
    TeamId DefaultTeamId;
    uint32_t PS3ContentRatingAge;
    uint32_t LogHistory;
    intptr_t Version;
    intptr_t LayerInclusionTable;
    char* DefaultLayerInclusion;
    float TimeBeforeSpawnIsAllowed;
    float LevelWarmUpTime;
    float TimeToWaitForQuitTaskCompletion;
    intptr_t Player;
    intptr_t DifficultySettings;
    int32_t DifficultyIndex;
    SKU CurrentSKU;
    intptr_t GameSettingsComponents;
    bool LogFileEnable;
    bool ResourceRefreshAlwaysAllowed;
    bool SpawnMaxLocalPlayersOnStartup;
    bool UseSpeedBasedDetailedCollision;
    bool UseSingleWeaponSelector;
    bool AutoAimEnabled;
    bool HasUnlimitedAmmo;
    bool HasUnlimitedMags;
    bool RotateLogs;
    bool AdjustVehicleCenterOfMass;
    bool AimAssistEnabled;
    bool AimAssistUsePolynomials;
    bool ForceFreeStreaming;
    bool ForceDisableFreeStreaming;
    bool IsGodMode;
    bool IsJesusMode;
    bool IsJesusModeAi;
    bool GameAdministrationEnabled;
    bool AllowDestructionOutsideCombatArea;
};

struct OnlineSettings : SystemSettings
{
    BackendType Backend;
    BackendType PeerBackend;
    OnlineEnvironment Environment;
    void* Provider;
    Array<intptr_t> Platforms;
    String ServiceNameOverride;
    LogLevelType LogLevel;
    int32_t BlazeLogLevel;
    int32_t DirtySockLogLevel;
    void* RichPresenceData;
    void* LicenseConfig;
    String Region;
    String Country;
    String PingSite;
    String MatchmakingToken;
    String MatchmakingTokenSuffix;
    String MatchmakingTokenPatchSuffix;
    uint32_t NegativeUserCacheRefreshPeriod;
    String ServerLoginEmail;
    String ServerLoginPassword;
    String ServerLoginPersonaName;
    int32_t BlazeServerConnectionTimeout;
    int32_t BlazeServerTimeout;
    int32_t BlazeClientConnectionTimeout;
    int32_t BlazeClientTimeout;
    int32_t BlazeClientSettingsRetryTime;
    int32_t PeerPort;
    uint32_t BlazeCachedUserRefreshInterval;
    int32_t DirtySockServerPacketQueueCapacity;
    String TrustedLoginCertFilename;
    String TrustedLoginKeyFilename;
    uint32_t MinPlayerCapacity;
    bool AssertOnPresenceRequestFailures;
    bool ClientIsPresenceEnabled;
    bool ServerIsPresenceEnabled;
    bool IsSecure;
    bool EnableQoS;
    bool ServerIsReconfigurable;
    bool SupportHostMigration;
    bool ServerAllowAnyReputation;
    bool EnableGamegroupInvites;
    bool IsTrialMode;
    bool EnableNucleusLtOverride;
};

struct PVZOnlineSettings : OnlineSettings
{
    struct BugSentryOnlineSettings
    {
        char* EnableKillswitchForCrashDumpsLicenseName;
        char* ForceProdModeLicenseName;
        int32_t CrashTimerInSeconds;
        char* CrashTimerLicenseName;
        bool EnableKillswitchForCrashDumps;
        bool EnableCrashDumps;
        bool ForceProdMode;
        bool CrashTimerDoDummyCrash;
        bool EnablePinTelemetry;
    };

    struct MetricsTelemetryOnlineSettings
    {
        void* EnabledRealms;
        void* EnabledDisabledEvents;
        bool EnableTelemetry;
    };

    struct ServerTransactionTelemetryOnlineSettings
    {
        char* EnableTelemetryLicenseOverride;
        char* EnableNonPlayerEventsLicenseOverride;
        char* EnablePlayerEventsLicenseOverride;
        char* EnablePlayerStatesLicenseOverride;
        void* EnabledCategories;
        void* DisabledEvents;
        bool EnableTelemetry;
        bool EnableNonPlayerEvents;
        bool EnablePlayerEvents;
        bool EnablePlayerStates;
    };

    struct TelemetryOnlineSettings
    {
        char* DownloadSettingsLicenseOverride;
        char* EnableTelemetryLicenseOverride;
        MetricsTelemetryOnlineSettings MetricsSettings;
        char* TransactionLogSettingsPrefix;
        ServerTransactionTelemetryOnlineSettings PeerHostedServerTransactionSettings;
        ServerTransactionTelemetryOnlineSettings DedicatedServerTransactionSettings;
        int32_t NumSecondsBetweenMemoryMetricUpdates;
        bool DownloadSettings;
        bool EnableTelemetry;
        bool CompressTransactions;
    };

    struct PingSiteSettings
    {
        int32_t GoodPingThreshold;
        int32_t OkayPingThreshold;
    };

    struct GameBrowserConfig
    {
        float MinimumInterval;
        float RefreshInterval;
        bool RefreshEnabled;
    };

    struct TelemetryPinSettings
    {
        char* ServerAddress;
        int32_t ServerPort;
        char* Environment;
        char* TitleIdType;
        char* TitleId;
        char* ReleaseType;
        void* EnableEventType;
        bool EnableTelemetry;
    };

    struct MandatedVersionSettings
    {
        int32_t Current;
        int32_t Required;
        bool ExactMatch;
    };

    struct BlazeClientBlackMarketManagerSettings
    {
        int32_t MinimumForceOverdueNotificationDelaySeconds;
        int32_t MaximumForceOverdueNotificationDelaySeconds;
        uint32_t MaximumNotificationHandlingAttempts;
    };

    struct BlazeClientNotificationManagerSettings
    {
        int32_t ForceNotificationCooldownSeconds;
        int32_t MinimumForceOverdueFeatureStateNotificationDelaySeconds;
        int32_t MaximumForceOverdueFeatureStateNotificationDelaySeconds;
        int32_t ForceRepeatFeatureStateNotificationIntervalSeconds;
    };

    char* ClientGameConfigurationOverride;
    void* EntitlementQueries;
    char* RequestedSettings;
    int32_t QueueCapacityOverride;
    uint32_t PingPeriod;
    BugSentryOnlineSettings BugSentrySettings;
    TelemetryOnlineSettings TelemetrySettings;
    int32_t GameReportInterval;
    void* OnlineGameInteractionKillSwitchList;
    PingSiteSettings PingSiteSettings;
    GameBrowserConfig GameBrowser;
    char* PurchaseCoinsButtonLicenseOverride;
    char* TrialModePurchaseId;
    float MaxTrialTime;
    float TrialTimeDataPollTime;
    float TrialTimeDataStartTime;
    char* TrialModeExpiredLicenseString;
    float TrialModeTimeModifier;
    TelemetryPinSettings TelemetryPinSettings;
    MandatedVersionSettings MandatedVersion;
    BlazeClientBlackMarketManagerSettings BlazeClientBlackMarketManager;
    BlazeClientNotificationManagerSettings BlazeClientNotificationManager;
    bool FairFightEnabled;
    bool DisableMiniGameBrowserAutoUpdate;
    bool PackExternalPurchasesKillSwitch;
    bool KickPlayerOnStatsDownloadFailure;
    bool OnlineGameInteractionMasterKillSwitch;
    bool PurchaseCoinsButtonEnabled;
    bool ForceDisableTrial;
    bool ForceEndTrial;
    bool TrialComingSoon;
};

struct ServerSettings : SystemSettings
{
    char* InstancePath;
    uint32_t RemoteControlPort;
    uint32_t MaxQueriesPerSecond;
    char* SavePoint;
    float TimeoutTime;
    uint32_t PlayerCountNeededForMultiplayer;
    char* DebugMenuClick;
    float LoadingTimeout;
    float IngameTimeout;
    float OutgoingFrequency;
    uint32_t IncomingRate;
    uint32_t OutgoingRate;
    char* Playlist;
    int32_t DedicatedServerCpu;
    uint32_t SaveGameVersion;
    char* ServerName;
    char* ServerPassword;
    float VehicleSpawnDelayModifier;
    float RespawnTimeModifier;
    char* AdministrationPassword;
    char* RemoteAdministrationPort;
    bool QueryProviderEnabled;
    bool DebrisClusterEnabled;
    bool VegetationEnabled;
    bool WaterPhysicsEnabled;
    bool IsDesertingAllowed;
    bool IsRenderDamageEvents;
    bool RespawnOnDeathPosition;
    bool IsStatsEnabled;
    bool IsNetworkStatsEnabled;
    bool IsAiEnabled;
    bool IsDestructionEnabled;
    bool IsSoldierAnimationEnabled;
    bool IsSoldierDetailedCollisionEnabled;
    bool LoadSavePoint;
    bool DisableCutscenes;
    bool HavokVisualDebugger;
    bool HavokCaptureToFile;
    bool ShowTriggerDebugText;
    bool TimeoutGame;
    bool AILooksIntoCamera;
    bool DeathmatchDebugInfo;
    bool JobEnable;
    bool ThreadingEnable;
    bool DrawActivePhysicsObjects;
    bool IsRanked;
    bool UnlockResolver;
    bool ScoringLogEnabled;
    bool ForcePlaylist;
    bool AutoUnspawnBangers;
    bool RegulatedAIThrottle;
    bool EnableAnimationCulling;
    bool FallBackToSquadSpawn;
    bool SaveGameUseProfileSaves;
    bool VehicleSpawnAllowed;
    bool AdministrationEnabled;
    bool AdministrationLogEnabled;
    bool AdministrationTimeStampLogNames;
    bool AdministrationEventsEnabled;
    bool AdministrationServerNameRestricted;
};

struct PVZServerSettings : SystemSettings
{
    float InActivityTimeOut;
    float ClientInActivityTimeOut;
    float ResetTimeInTeamInactivityTimeOut;
    char* PVZBlazePlaylist;
    uint32_t TeamSwitchImbalanceLimit;
    uint32_t GameSize;
    VoiceChannel DefaultVoiceChannel;
    int32_t DeathmatchFriendZoneFallbackCount;
    float TeamSwitchCooldownTime;
    int32_t ServerConfigurationRefreshInterval;
    char* OverrideServerConfigurationStartTime;
    bool AutoBalance;
    bool OverrideAutoBalance;
    bool KickIdlePlayers;
    bool MapSequencerEnabled;
    bool PlaylistEnabled;
};

struct SyncedPVZSettings : SystemSettings
{
    float TeamSwitchingCooldownTime;
    float GameModeCounterModifier;
    float SpottingInterval;
    float SpottingFov;
    float SpottingDistance;
    uint32_t CommanderSize;
    ServerTypeValues ServerType;
    float GlobalScoreMultiplier;
    float ForceExplosionDeathMagnitude;
    bool AllUnlocksUnlocked;
    bool CommanderEnabled;
    bool RenderAliveCharactersInGame;
    bool ForceExplosionDeath;
    bool UseDefaultCustomization;
};
} // namespace gw2

namespace gw3
{
typedef struct SettingsBase
{
    uint8_t _opaque[0x20];
} SettingsBase;

typedef enum GamePlatform
{
    GamePlatform_Win32 = 0,
    GamePlatform_Gen4a = 1,
    GamePlatform_Gen4b = 2,
    GamePlatform_Android = 3,
    GamePlatform_iOS = 4,
    GamePlatform_OSX = 5,
    GamePlatform_Linux = 6,
    GamePlatform_NX = 7,
    GamePlatform_Any = 8,
    GamePlatform_Invalid = 9,
    GamePlatformCount = 10
} GamePlatform;

typedef enum EntitlementType
{
    EntitlementType_Ignored = 0,
    EntitlementType_Write = 1,
    EntitlementType_Read = 2
} EntitlementType;

typedef enum NetObjectPriorityMethod
{
    NetObjectPriorityMethod_3DCone = 250379213,
    NetObjectPriorityMethod_2DDistance = 800242152,
    NetObjectPriorityMethod_Static = -1175905275,
    NetObjectPriorityMethod_DefaultDynamic = -266766269
} NetObjectPriorityMethod;

typedef enum NetObjectFilterMethod
{
    NetObjectFilterMethod_None = 262614181
} NetObjectFilterMethod;

typedef struct DeltaCompressionSettings
{
    uint32_t BaselineReuseCount;
    bool IsEnabled;
    bool ShareBaselinesAcrossConnections;
} DeltaCompressionSettings;

typedef struct NetObjectPrioritySettings
{
    float MaxFrequencyFactor;
    float MinFrequencyFactorRadius;
    float CameraFovBiasDegrees;
    float MinFrequencyFactor;
    float MaxFrequencyFactorRadius;
    float MaxFrequencyFactorConeRadius;
    float MinConeFrequencyFactor;
    float MaxCameraFovDegrees;
    float MinCameraFovDegrees;
} NetObjectPrioritySettings;

typedef struct NetObjectSystemDebugSettings
{
    float IncomingReplicationStatusReportViewDistance;
    float IncomingReplicationStatusReportMaxDelta;
    char* IncomingReplicationStatusReportFilter;
    uint32_t ReportReplicationWarningsAfterFrames;
    uint32_t InitialGraceTimeInFrames;

    bool IncomingReplicationStatusReportIncludeStatic;
    bool IncomingReplicationStatusReportIncludeNonSpatial;
    bool OutputObjectProtocols;
    bool IncomingReplicationStatusReportIncludeSpatial;
    bool EnableIncomingReplicationStatusReport;
    bool WarnOnMissingInitDependency;
    bool EnableReplicationWarnings;
    bool IncomingReplicationStatusReportDrawName;
    bool WarnOnNoStateCanBeSent;
    bool WarnOnTooLargeNetObject;
    bool WarnOnWaitingForCreationAck;
} NetObjectSystemDebugSettings;

typedef struct NetObjectSystemSettings
{
    uint8_t _opaque[0x18];

    NetObjectPrioritySettings PrioritySettings;
    NetObjectSystemDebugSettings Debug;
    DeltaCompressionSettings DeltaCompressionSettings;

    uint32_t MaxServerConnectionCount;
    uint32_t MaxClientConnectionCount;
    uint32_t MaxStaticNetObjectCount;
    uint32_t MaxRemoteAuthorityNetObjectCount;
    uint32_t MaxNetObjectCount;

    NetObjectPriorityMethod DefaultDynamicPriorityMethod;
    NetObjectFilterMethod DefaultFilterMethod;

    bool NetArrayEditorSanityCheck;
    bool InProcReplicationEnabled;
} NetObjectSystemSettings;

typedef struct ActivationOverride
{
    char* AwardName;
    bool Active;
} ActivationOverride;

typedef struct CompletionOverride
{
    char* CriteriaName;
    float CompletionValue;
} CompletionOverride;

typedef struct ConsumableLimitSettings
{
    int32_t RainbowStarLimit;
} ConsumableLimitSettings;

typedef struct AwardSettings
{
    ActivationOverride* ActivationOverrides;
    CompletionOverride* CompletionOverrides;
} AwardSettings;

typedef struct CommunityPortalSettings
{
    int32_t BossHuntLoserHardBase_Full;
    int32_t BossHuntWinnerEasyBase;
    int32_t BossHuntLoserrMediumBase_Quarter;
    int32_t VanquishConfirmedLoserBase;
    int32_t SuburbinationLoserBase;
    int32_t BossHuntLoserEasyBase_Quarter;
    int32_t RushLoserBase_6;
    int32_t GnomeBombWinnerBase;
    int32_t RushWinnerBase_2;
    int32_t RushWinnerBase_4;
    int32_t BossHuntLoserrMediumBase_Half;
    int32_t CrazyOption4;
    int32_t CrazyOption0;
    int32_t CvDWinnerBase;
    int32_t VanquishConfirmedWinnerBase;
    int32_t CrazyOption5;
    int32_t EndTime;
    uint32_t UnderworldMegaChestCost;
    int32_t BossHuntLoserEasyBase_Half;
    int32_t RetryTimerOnLoss;
    char* EventDescription;
    int32_t RetryTimerOnWin;
    int32_t BossHuntLoserEasyBase_Full;
    int32_t RushLoserBase_5;
    int32_t GnomeBombLoserBase;
    int32_t BossHuntWinnerHardBase;
    int32_t BossHuntLoserHardBase_Quarter;
    int32_t BossHuntLoserrMediumBase_Full;
    int32_t RushWinnerBase_3;
    char* EventImageUrl;
    int32_t RushWinnerBase_6;
    int32_t TeamVanquishWinnerBase;
    int32_t SuburbinationWinnerBase;
    int32_t RushLoserBase_4;
    int32_t RushWinnerBase_1;
    int32_t CvDLoserBase;
    int32_t GnomeBombPerBaseBonus;
    int32_t CrazyOption2;
    int32_t BossHuntWinnerMediumBase;
    int32_t RushLoserBase_2;
    int32_t BossHuntLoserHardBase_Half;
    int32_t CrazyOption3;
    int32_t CrazyOption1;
    int32_t StartTime;
    char* EventTitle;
    int32_t RushWinnerBase_5;
    int32_t RushLoserBase_3;
    int32_t RushLoserBase_1;
    int32_t TeamVanquishLoserBase;
    int32_t CvDPointsPerLoserRound;
    bool Widget;
} CommunityPortalSettings;

typedef struct PVZServerGameSettings : SettingsBase
{
    CommunityPortalSettings CommunityPortal;
    AwardSettings AwardSettings;
    Array<char*> DisabledKillSwitches;
    ConsumableLimitSettings ConsumableLimits;
    bool TrackWeeklyChallenges;
} PVZServerGameSettings;

typedef struct Vec4
{
    float x;
    float y;
    float z;
    float w;
} Vec4;

typedef uint32_t TeamId;

typedef struct TeamColor
{
    Vec4 Color;
    TeamId TeamId;
} TeamColor;

typedef struct GameModeTeamSize
{
    TeamId Team;
    uint32_t PlayerCount;
    bool IsAIOnly;
} GameModeTeamSize;

typedef struct GameModeSizeInfo
{
    GameModeTeamSize* Teams;
    uint32_t TeamImbalanceLimit;
} GameModeSizeInfo;

typedef struct GameModeConfiguration
{
    char* FrontEndGameModeOption;
    char** PlantCoOpGameModes;
    char** ZombieCoOpGameModes;
    void* GameModes;
    char** MPGameModes;
    GameModeSizeInfo** GameModeSizes;
    GameModeSizeInfo* FallbackSizeInfo;
    void* HostedModeBackends;
    char* SplashGameModeOption;
    char* HostedModeCriterionName;
    char* SocialGameModeOption;
    char* PeerHostedModeOption;
    char* LocalHostedModeOption;
    char* GameModeCriterionName;
    char* SoloPlayCriterionName;
    char* ServerHostedModeOption;
    char* SoloPlayMode;
} GameModeConfiguration;

typedef struct GameModeSettings : SettingsBase
{
    int32_t ShouldLoadAutoTestSubLevels;
    TeamColor* TeamHighlightColors;
    GameModeConfiguration* GameModeConfiguration;
    int32_t* ForceSpawnClassIndices;
    int32_t AutoplayersTestCaseOverride;
    uint32_t MaxSquadSize;
    int32_t SeasonalQuestCooldownTime;
    float SkipManDownTime;
    int32_t RushInstantCaptureUpToFlag;
    float OverridePreRoundTime;
    int32_t AutoplayersSoakTestLength;
    int32_t SandboxMissionBeatIndex;
    int32_t LoadHeatmapSublevel;
    int32_t ForceLevelVariation;
    uint32_t LocalPlaySelectedAICharacterSetZombiesUnlockId;
    float SkipSpawnRandomCostumes_UnrevivableAliveTimeThreshold;
    char* SplashToLevelDSub;
    int32_t LocalPlayBotsPerTeamOverride;
    char* CrazyOptionsDataPath;
    float CrazyOption_TimeBomb_ShortTimer;
    int32_t SandboxFreeRoamSpawnPositionIndex;
    int32_t AttackingTeamId;
    int32_t ForceHUBSeason;
    int32_t ModeTeamId;
    float CrazyOptionsGlobalScoreModifier;
    int32_t SkipSpawnScreenClassIndex;
    char* SplashToLevelInclusion;
    char* AvailableCostumes;
    uint32_t LocalPlayZombieBotsPerTeam;
    float CrazyOption_TimeBomb_BumpTime;
    int32_t OverrideRoundStartPlayerCount;
    uint32_t XPFishStarCost;
    uint32_t TeamVanquishKillCount;
    float XPFishXPGrant;
    int32_t WeeklyEventIndex;
    int32_t BattleMindMultiplier;
    float ForceRespawnManDownTime;
    float CrazyOption_TimeBomb_FullTimer;
    uint32_t StoredDifficultyIndex;
    uint32_t LocalPlaySelectedAICharacterSetPlantsUnlockId;
    float PauseRespawnWaitTime;
    uint32_t LocalPlayPlantBotsPerTeam;
    char* SplashToLevelStartPoint;
    int32_t AutoTestAutoplayerBehavior;
    uint32_t CapturePointShowDebugInfo;
    char* SplashToLevel;

    bool IsCoolLasers;
    bool LocalPlayAIWaitForFirstPlayerToSpawn;
    bool InstantCaptureEnabled;
    bool SkipPreroundCountdown;
    bool DebugMission_01;
    bool SocialHUBSkipLandingPage;
    bool Force1HpCharacterHealthPlants;
    bool SkipManDown;
    bool OpsBossRush;
    bool AutoplayersClient;
    bool CrazyOption11;
    bool AllowDropInSplitScreen;
    bool SocialHUBSkipStationTutorials;
    bool HasSeenESRBScreen;
    bool IsBETA;
    bool FeatureLockoff_Rux;
    bool UnlimitedPrimaryAmmoZombies;
    bool UnlimitedPrimaryAmmo;
    bool CrazyOption3;
    bool FeatureLockoff_BattleChest;
    bool ArenaIgnorePenalties;
    bool HumansDontDie;
    bool DebugMission_03;
    bool DebugMission_02;
    bool CrazyOption10;
    bool CrazyOption8;
    bool CountAIKills;
    bool SandboxFreeRoamSpawnAI;
    bool BattleMindDisplay;
    bool CrazyOption4;
    bool BackfillMpWithAI;
    bool PicnicBuildTechTest;
    bool DebugLaunchQuest;
    bool ForceImbalanceLimitFrom2to1;
    bool HasSeenIntroVideo;
    bool PicnicBuildClosedAlpha;
    bool ForceStreamingInstall;
    bool CrazyOption14;
    bool UseSandboxAIWeaponData;
    bool ArenaRemoveCharactersOnWin;
    bool FeatureLockoff_HUBSecrets;
    bool CrazyOption16;
    bool Force1HpCharacterHealthZombies;
    bool ShouldSkipHUBTutorial;
    bool RenderingHighlightAllCharacters;
    bool ShowHordeClasses;
    bool Force8v8to10v10;
    bool FeatureLockoff_Sandbox_DW;
    bool CrazyOption15;
    bool MatchmakeAtEOR;
    bool SandboxGnomeShrineUnlocked;
    bool CrazyOption1;
    bool FestivalBoothIsEnabled;
    bool CrazyOption9;
    bool isIntroTVM;
    bool UnlimitedPrimaryAmmoPlants;
    bool ShowAINametags;
    bool ForceAllowDropInSplitScreen;
    bool AutoplayersSplitscreenEnabled;
    bool IsGnomeSummonedCharacter;
    bool ShowEconomyEarnRateUI;
    bool CountPersistentAITowardsKillStreak;
    bool FeatureLockoff_Sandbox_CM;
    bool HasSeenForcedMailBox;
    bool ShouldPreventBotSpawnsInSocialHUB;
    bool EnableCharacterSubRealms;
    bool UseOpsAIWeaponData;
    bool CUOps_FreeStars;
    bool CrazyOption5;
    bool FeatureLockoff_Sandbox_TCP;
    bool EnableAIRevives;
    bool CrazyOption6;
    bool CrazyOption18;
    bool SkipSpawnRandomCostumes;
    bool RenderingHighlightFriendlyCharacters;
    bool IsSuburbinationKotH;
    bool SplashSearchSocial;
    bool CrazyOption2;
    bool CrazyOption12;
    bool FeatureLockoff_Ops;
    bool CrazyOption17;
    bool ShuffleTeamsByRank;
    bool ShowTeamSwitchWarning;
    bool SkipEOR;
    bool AlwaysShowNametags;
    bool AllowManualTeamSwitch;
    bool IsMeleeEnabled;
    bool CrazyOption7;
    bool Force1HpCharacterHealth;
    bool UseInfiniteManDownTime;
    bool BoastsEnabled;
    bool SplashShowDebugMenu;
    bool ForceDefaultCustomizations;
    bool FeatureLockoff_Sandbox_TCZ;
    bool CanAITaunt;
} GameModeSettings;

typedef struct GameSettings : SettingsBase
{
    char* InstallationLevel;
    intptr_t Player;
    intptr_t Version;
    intptr_t GameModeViewDefinitions;
    intptr_t LayerInclusionTable;
    intptr_t GameSettingsComponents;

    uint32_t LogFileRotationHistoryLength;
    uint32_t MaxCorrectionUpdateCount;
    uint32_t MoveManagerOutgoingFrequencyDivider;

    char* DefaultLayerInclusion;
    char* InitialDSubLevel;
    char* ActiveGameModeViewDefinition;
    char* InstallationStartPoint;

    float MaxAllowedLatency;

    char* Level;
    char* StartPoint;

    int LogFileCollisionMode;

    uint32_t MaxPlayerCount;
    uint32_t MaxSpectatorCount;

    float TimeToWaitForQuitTaskCompletion;

    uint32_t MoveManagerSinglePlayerOutgoingFrequencyDivider;

    bool EnableAutomaticCorrectionUpdateCount;
    bool ResourceRefreshAlwaysAllowed;
    bool LogFileEnable;
    bool DisableToggleEntryCamera;
    bool DebugFreeCamEnabled;
} GameSettings;

typedef struct OnlineSettings : SettingsBase
{
    uint32_t BlazeCachedUserRefreshInterval;
    void* RichPresenceData;
    void* Platforms;
    void* Provider;

    int32_t BlazeServerConnectionTimeout;
    uint32_t BlazeClientTunnelSocketRecvBufSize;
    uint32_t BlazeOutgoingBufferSize;
    uint32_t BlazeServerTunnelSocketRecvBufSize;
    int32_t BlazeClientConnectionTimeout;

    char* ServerLoginEmail;
    uint32_t MaxLocalUserCount;
    char* TrustedLoginKeyFilename;
    uint32_t BlazeServerTunnelSocketSendBufSize;
    char* ServerLoginProjectTag;

    uint32_t LogLevel;
    uint32_t MinPlayerCapacity;

    char* Country;
    char* ServiceNameOverride;
    char* Region;
    char* TrustedLoginPath;

    int32_t DirtySockLogLevel;
    char* TrustedLoginCertFilename;
    int32_t BlazeLogLevel;

    char* ServerLoginPassword;
    char* MatchmakingToken;
    char* DebugMessageCallstackTypeList;

    uint32_t NegativeUserCacheRefreshPeriod;
    uint32_t BlazeClientTunnelSocketSendBufSize;

    char* ServerLoginPersonaName;
    int32_t DirtySockServerPacketQueueCapacity;

    uint32_t ServerBackend;
    int32_t BlazeClientTimeout;
    uint32_t Environment;

    char* PingSite;

    int32_t DirtySockServerNetGameLinkQueueLength;
    int32_t BlazeServerTimeout;
    int32_t PeerPort;
    uint32_t DirtySockMaxConnectionCount;

    bool WaitForQoS;
    bool EnableNucleusLtOverride;
    bool IsSecure;
    bool ShouldControlBlaze;
    bool ClientIsPresenceEnabled;
    bool ServerAllowAnyReputation;
    bool EnableQoS;
    bool SupportHostMigration;
    bool ServerIsReconfigurable;
    bool AssertOnPresenceRequestFailures;
    bool ServerIsPresenceEnabled;
    bool EnableGamegroupInvites;
    bool ShouldControlDirtySock;
    bool ForceAdvertiseLocalAddress;

} OnlineSettings;

typedef struct PingSiteNamePair
{
    char* PingSiteName;
    char* PingSiteHalId;
} PingSiteNamePair;

typedef struct PingSiteSettings
{
    PingSiteNamePair* PingSiteNamePairs;
    int32_t GoodPingThreshold;
    int32_t OkayPingThreshold;
} PingSiteSettings;

typedef struct GameBrowserConfig
{
    float MinimumInterval;
    float RefreshInterval;
    char* GameGroupConfig;
    char* AnyGameConfig;
    char* GameSessionConfig;
    char* GameGroupListConfig;
    bool RefreshEnabled;
} GameBrowserConfig;

typedef struct TelemetryOnlineSettingsPair
{
    char* Item;
    char* ItemLicenseOverride;
} TelemetryOnlineSettingsPair;

typedef struct OnlineSettingsTelemetryEvent
{
    char** Realms;
    char** Scopes;
    char* Module;
    char* Group;
    char* String;
    bool Enabled;
} OnlineSettingsTelemetryEvent;

typedef struct MetricsTelemetryOnlineSettings
{
    char** EnabledRealms;
    OnlineSettingsTelemetryEvent* EnabledDisabledEvents;
    bool EnableTelemetry;
} MetricsTelemetryOnlineSettings;

typedef struct ServerTransactionTelemetryOnlineSettings
{
    TelemetryOnlineSettingsPair* EnabledCategories;
    TelemetryOnlineSettingsPair* DisabledEvents;

    char* EnableTelemetryLicenseOverride;
    char* EnableNonPlayerEventsLicenseOverride;
    char* EnablePlayerStatesLicenseOverride;
    char* EnablePlayerEventsLicenseOverride;

    bool EnableNonPlayerEvents;
    bool EnablePlayerStates;
    bool EnableTelemetry;
    bool EnablePlayerEvents;
} ServerTransactionTelemetryOnlineSettings;

typedef struct TelemetryOnlineSettings
{
    ServerTransactionTelemetryOnlineSettings PeerHostedServerTransactionSettings;
    ServerTransactionTelemetryOnlineSettings DedicatedServerTransactionSettings;
    MetricsTelemetryOnlineSettings MetricsSettings;

    char* DownloadSettingsLicenseOverride;
    char* EnableTelemetryLicenseOverride;
    char* TransactionLogSettingsPrefix;

    int32_t NumSecondsBetweenMemoryMetricUpdates;
    int32_t TelemetryContextRefreshPeriodSeconds;

    bool EnableTelemetry;
    bool DownloadSettings;
    bool CompressTransactions;
} TelemetryOnlineSettings;

typedef struct TelemetryPinSettingsPair
{
    char* Name;
    uint32_t StreamId;
    bool Enabled;
} TelemetryPinSettingsPair;

typedef struct TelemetryPinSettings
{
    TelemetryPinSettingsPair* EnableEventType;
    char* TitleIdType;
    float FpsThreshold;
    char* Environment;
    char* ReleaseType;
    char* TitleId;
    float FpsCoolDownInterval;
    int32_t ServerPort;
    float PlayerHealthAbsoluteThreshold;
    char* ServerAddress;
    float PlayerTickInterval;
    uint32_t UIInteractMinModeDurSec;
    float FpsSplitScreenThreshold;
    int32_t TimerEventInterval;
    bool EnableTelemetry;
    bool EnableKillSwitchforPlayerStatTelemetry;
} TelemetryPinSettings;

typedef struct BugSentryOnlineSettings
{
    char* EnableKillswitchForCrashDumpsLicenseName;
    char* CrashTimerLicenseName;
    char* ForceProdModeLicenseName;
    int32_t CrashTimerInSeconds;
    bool EnableCrashDumps;
    bool EnableKillswitchForCrashDumps;
    bool ForceProdMode;
    bool CrashTimerDoDummyCrash;
    bool EnablePinTelemetry;
} BugSentryOnlineSettings;

typedef struct BlazeClientNotificationManagerSettings
{
    int32_t ForceNotificationCooldownSeconds;
    int32_t MinimumForceOverdueFeatureStateNotificationDelaySeconds;
    int32_t MaximumForceOverdueFeatureStateNotificationDelaySeconds;
    int32_t ForceRepeatFeatureStateNotificationIntervalSeconds;
} BlazeClientNotificationManagerSettings;

typedef struct MandatedVersionSettings
{
    int32_t Current;
    int32_t Required;
    bool ExactMatch;
} MandatedVersionSettings;

typedef struct BattleArenaSeasonInfo
{
    int32_t SeasonNumber;
    char* SeasonName;
    char* SeasonStatKey;
    int32_t CalibrationGames;
    char* ActivationTimestamp;
} BattleArenaSeasonInfo;

typedef struct EntitlementData
{
    char* EntitlementTag;
    char* License;
    char* GroupName;
    char* ProjectId;
    char* ProductId;
    EntitlementType UsageType;
    bool VerifyOwnership;
} EntitlementData;

typedef struct EntitlementQuery
{
    char* EntitlementTag;
    EntitlementData* EntitlementsData;
    char** GroupNames;
    GamePlatform Platform;
    char* ProductId;
    char* ProjectId;
} EntitlementQuery;

typedef struct PVZOnlineSettings : OnlineSettings
{
    TelemetryOnlineSettings TelemetrySettings;
    TelemetryPinSettings TelemetryPinSettings;
    PingSiteSettings PingSiteSettings;

    uint64_t StreamingDataCacheSizePerWrite;
    BattleArenaSeasonInfo* BattleArenaSeasonInfos;

    uint64_t StreamingDataCacheMaxAgeSeconds;
    EntitlementQuery** EntitlementQueries;

    GameBrowserConfig GameBrowser;
    BugSentryOnlineSettings BugSentrySettings;
    BlazeClientNotificationManagerSettings BlazeClientNotificationManager;
    MandatedVersionSettings MandatedVersion;

    float BattleArenaMaxPenalty;
    char* SocialSpaceDSubName;
    uint32_t StreamingDataCacheMaxSizeMb;
    char* TrialModeExpiredLicenseString;
    char* MatchmakingTokenPatchSuffix;
    char* SocialSpaceStartPoint;
    char* BattleArenaPenaltyExpiryStatId;
    char* PurchaseCoinsButtonLicenseOverride;
    float TrialTimeDataStartTime;
    int32_t GameReportInterval;
    char* GameConfigurationUrl;
    char* ConnectionLevel;
    int32_t StoreRefreshRetryMaxDelay;
    char* CreditsPath;
    char* EditorialRootUrl;
    int32_t StoreRefreshRetryDelay;
    float MaxTrialTime;
    char* TrialModePurchaseId;
    int32_t RpcRetryMaxDelay;
    int32_t StoreInterruptedPurchaseRetries;
    uint32_t PingPeriod;
    int32_t QueueCapacityOverride;
    float TrialTimeDataPollTime;
    int32_t BattleArenaPointsBetweenRanks;
    float OnlinePeerGameReserveTimeOut;
    float ClientStartedAndWaitingFailSafeTimer;
    int32_t GroupReserveWaitForJoiningSessionTimeout;
    float TrialModeTimeModifier;
    float UpdatePermissionsTimeOut;
    char* PodiumDSubName;
    float RespawnDelayTimeoutSeconds;
    int32_t ClientStoreEntityRefreshDelaySeconds;
    int32_t OriginProfileRefreshPeriodSeconds;
    int32_t StoreRefreshPeriodSeconds;
    int32_t RpcRetryDelay;
    int32_t BlazeClientSettingsRetryTime;
    int32_t StoreInterruptedPurchaseBusySeconds;
    char* SocialSpaceGameModeInclusion;
    int32_t RpcRandomOffsetMaxDelay;
    float DisconnectFailSafeTimer;
    char* ClientGameConfigurationOverride;
    float PrivilegeCacheTimeOut;
    char* ContentBaseUrl;
    char* LevelRootName;
    int32_t ClientStoreEntityTimerExpiredRefreshDelaySeconds;
    char* MatchmakingTokenSuffix;
    char* StreamingDataCacheSubDir;
    float GroupSwitchTimeOut;
    int32_t GroupReservationRenewPeriodWhileQueued;
    char* ConnectionLevelLayerInclusion;

    bool ClientStoreEntityRefreshEnabled;
    bool AllowMissionActiveJoin;
    bool RestrictSplitscreenLeaders;
    bool GlobalMatchmaking;
    bool KickPlayerOnStatsDownloadFailure;
    bool ForceEndTrial;
    bool AllowFirstPartyGuestUser;
    bool StorePurchaseNotActiveRefreshEnabled;
    bool ForceDisableTrial;
    bool RichPresenceEnabled;
    bool TelemetryPCKillSwitch;
    bool TelemetryXBoneKillSwitch;
    bool NoPrivilegeMatchmaking;
    bool IsTrialMode;
    bool StoreFailedRefreshEnabled;
    bool PurchaseCoinsButtonEnabled;
    bool PackExternalPurchasesKillSwitch;
    bool TrialComingSoon;
    bool StorePurchaseFailRefreshEnabled;
    bool ClientStoreEntityTimerExpiredRefreshEnabled;
    bool GuestUserSharesUnlocksEnabled;
    bool StreamingDataCacheEnabled;
    bool StoreRefreshEnabled;
    bool TelemetryPS4KillSwitch;
    bool DisableMiniGameBrowserAutoUpdate;
    bool PrivilegeCheckEnabled;
} PVZOnlineSettings;

typedef struct PVZServerSettings : SettingsBase
{
    uint32_t GameSize;
    float ClientInActivityTimeOut;
    float InActivityTimeOut;
    float ResetTimeInTeamInactivityTimeOut;
    int32_t DeathmatchFriendZoneFallbackCount;
    float StatisticsManagerRequestTimeout;
    uint32_t TeamSwitchImbalanceLimit;
    float TeamSwitchCooldownTime;
    bool KickIdlePlayers;
    bool KickIdlePlayersInASpawn;
    bool OverrideAutoBalance;
    bool AutoBalance;
    bool MapSequencerEnabled;
    bool ValidatePlayerSelections;
} PVZServerSettings;
} // namespace gw3
} // namespace fb
