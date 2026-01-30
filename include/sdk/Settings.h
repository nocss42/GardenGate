#pragma once

#include <cstdint>

#include "sdk/Array.h"

namespace fb
{
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
            char *IncomingReplicationStatusReportFilter;
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
            char *AwardName;
            bool Active;
        } ActivationOverride;

        typedef struct CompletionOverride
        {
            char *CriteriaName;
            float CompletionValue;
        } CompletionOverride;

        typedef struct ConsumableLimitSettings
        {
            int32_t RainbowStarLimit;
        } ConsumableLimitSettings;

        typedef struct AwardSettings
        {
            ActivationOverride *ActivationOverrides;
            CompletionOverride *CompletionOverrides;
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
            char *EventDescription;
            int32_t RetryTimerOnWin;
            int32_t BossHuntLoserEasyBase_Full;
            int32_t RushLoserBase_5;
            int32_t GnomeBombLoserBase;
            int32_t BossHuntWinnerHardBase;
            int32_t BossHuntLoserHardBase_Quarter;
            int32_t BossHuntLoserrMediumBase_Full;
            int32_t RushWinnerBase_3;
            char *EventImageUrl;
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
            char *EventTitle;
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
            Array<char *> DisabledKillSwitches;
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
            GameModeTeamSize *Teams;
            uint32_t TeamImbalanceLimit;
        } GameModeSizeInfo;

        typedef struct GameModeConfiguration
        {
            char *FrontEndGameModeOption;
            char **PlantCoOpGameModes;
            char **ZombieCoOpGameModes;
            void *GameModes;
            char **MPGameModes;
            GameModeSizeInfo **GameModeSizes;
            GameModeSizeInfo *FallbackSizeInfo;
            void *HostedModeBackends;
            char *SplashGameModeOption;
            char *HostedModeCriterionName;
            char *SocialGameModeOption;
            char *PeerHostedModeOption;
            char *LocalHostedModeOption;
            char *GameModeCriterionName;
            char *SoloPlayCriterionName;
            char *ServerHostedModeOption;
            char *SoloPlayMode;
        } GameModeConfiguration;

        typedef struct GameModeSettings : SettingsBase
        {
            int32_t ShouldLoadAutoTestSubLevels;
            TeamColor *TeamHighlightColors;
            GameModeConfiguration *GameModeConfiguration;
            int32_t *ForceSpawnClassIndices;
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
            char *SplashToLevelDSub;
            int32_t LocalPlayBotsPerTeamOverride;
            char *CrazyOptionsDataPath;
            float CrazyOption_TimeBomb_ShortTimer;
            int32_t SandboxFreeRoamSpawnPositionIndex;
            int32_t AttackingTeamId;
            int32_t ForceHUBSeason;
            int32_t ModeTeamId;
            float CrazyOptionsGlobalScoreModifier;
            int32_t SkipSpawnScreenClassIndex;
            char *SplashToLevelInclusion;
            char *AvailableCostumes;
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
            char *SplashToLevelStartPoint;
            int32_t AutoTestAutoplayerBehavior;
            uint32_t CapturePointShowDebugInfo;
            char *SplashToLevel;

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
            char *InstallationLevel;
            intptr_t Player;
            intptr_t Version;
            intptr_t GameModeViewDefinitions;
            intptr_t LayerInclusionTable;
            intptr_t GameSettingsComponents;

            uint32_t LogFileRotationHistoryLength;
            uint32_t MaxCorrectionUpdateCount;
            uint32_t MoveManagerOutgoingFrequencyDivider;

            char *DefaultLayerInclusion;
            char *InitialDSubLevel;
            char *ActiveGameModeViewDefinition;
            char *InstallationStartPoint;

            float MaxAllowedLatency;

            char *Level;
            char *StartPoint;

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
            void *RichPresenceData;
            void *Platforms;
            void *Provider;

            int32_t BlazeServerConnectionTimeout;
            uint32_t BlazeClientTunnelSocketRecvBufSize;
            uint32_t BlazeOutgoingBufferSize;
            uint32_t BlazeServerTunnelSocketRecvBufSize;
            int32_t BlazeClientConnectionTimeout;

            char *ServerLoginEmail;
            uint32_t MaxLocalUserCount;
            char *TrustedLoginKeyFilename;
            uint32_t BlazeServerTunnelSocketSendBufSize;
            char *ServerLoginProjectTag;

            uint32_t LogLevel;
            uint32_t MinPlayerCapacity;

            char *Country;
            char *ServiceNameOverride;
            char *Region;
            char *TrustedLoginPath;

            int32_t DirtySockLogLevel;
            char *TrustedLoginCertFilename;
            int32_t BlazeLogLevel;

            char *ServerLoginPassword;
            char *MatchmakingToken;
            char *DebugMessageCallstackTypeList;

            uint32_t NegativeUserCacheRefreshPeriod;
            uint32_t BlazeClientTunnelSocketSendBufSize;

            char *ServerLoginPersonaName;
            int32_t DirtySockServerPacketQueueCapacity;

            uint32_t ServerBackend;
            int32_t BlazeClientTimeout;
            uint32_t Environment;

            char *PingSite;

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
            char *PingSiteName;
            char *PingSiteHalId;
        } PingSiteNamePair;

        typedef struct PingSiteSettings
        {
            PingSiteNamePair *PingSiteNamePairs;
            int32_t GoodPingThreshold;
            int32_t OkayPingThreshold;
        } PingSiteSettings;

        typedef struct GameBrowserConfig
        {
            float MinimumInterval;
            float RefreshInterval;
            char *GameGroupConfig;
            char *AnyGameConfig;
            char *GameSessionConfig;
            char *GameGroupListConfig;
            bool RefreshEnabled;
        } GameBrowserConfig;

        typedef struct TelemetryOnlineSettingsPair
        {
            char *Item;
            char *ItemLicenseOverride;
        } TelemetryOnlineSettingsPair;

        typedef struct OnlineSettingsTelemetryEvent
        {
            char **Realms;
            char **Scopes;
            char *Module;
            char *Group;
            char *String;
            bool Enabled;
        } OnlineSettingsTelemetryEvent;

        typedef struct MetricsTelemetryOnlineSettings
        {
            char **EnabledRealms;
            OnlineSettingsTelemetryEvent *EnabledDisabledEvents;
            bool EnableTelemetry;
        } MetricsTelemetryOnlineSettings;

        typedef struct ServerTransactionTelemetryOnlineSettings
        {
            TelemetryOnlineSettingsPair *EnabledCategories;
            TelemetryOnlineSettingsPair *DisabledEvents;

            char *EnableTelemetryLicenseOverride;
            char *EnableNonPlayerEventsLicenseOverride;
            char *EnablePlayerStatesLicenseOverride;
            char *EnablePlayerEventsLicenseOverride;

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

            char *DownloadSettingsLicenseOverride;
            char *EnableTelemetryLicenseOverride;
            char *TransactionLogSettingsPrefix;

            int32_t NumSecondsBetweenMemoryMetricUpdates;
            int32_t TelemetryContextRefreshPeriodSeconds;

            bool EnableTelemetry;
            bool DownloadSettings;
            bool CompressTransactions;
        } TelemetryOnlineSettings;

        typedef struct TelemetryPinSettingsPair
        {
            char *Name;
            uint32_t StreamId;
            bool Enabled;
        } TelemetryPinSettingsPair;

        typedef struct TelemetryPinSettings
        {
            TelemetryPinSettingsPair *EnableEventType;
            char *TitleIdType;
            float FpsThreshold;
            char *Environment;
            char *ReleaseType;
            char *TitleId;
            float FpsCoolDownInterval;
            int32_t ServerPort;
            float PlayerHealthAbsoluteThreshold;
            char *ServerAddress;
            float PlayerTickInterval;
            uint32_t UIInteractMinModeDurSec;
            float FpsSplitScreenThreshold;
            int32_t TimerEventInterval;
            bool EnableTelemetry;
            bool EnableKillSwitchforPlayerStatTelemetry;
        } TelemetryPinSettings;

        typedef struct BugSentryOnlineSettings
        {
            char *EnableKillswitchForCrashDumpsLicenseName;
            char *CrashTimerLicenseName;
            char *ForceProdModeLicenseName;
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
            char *SeasonName;
            char *SeasonStatKey;
            int32_t CalibrationGames;
            char *ActivationTimestamp;
        } BattleArenaSeasonInfo;

        typedef struct EntitlementData
        {
            char *EntitlementTag;
            char *License;
            char *GroupName;
            char *ProjectId;
            char *ProductId;
            EntitlementType UsageType;
            bool VerifyOwnership;
        } EntitlementData;

        typedef struct EntitlementQuery
        {
            char *EntitlementTag;
            EntitlementData *EntitlementsData;
            char **GroupNames;
            GamePlatform Platform;
            char *ProductId;
            char *ProjectId;
        } EntitlementQuery;

        typedef struct PVZOnlineSettings : OnlineSettings
        {
            TelemetryOnlineSettings TelemetrySettings;
            TelemetryPinSettings TelemetryPinSettings;
            PingSiteSettings PingSiteSettings;

            uint64_t StreamingDataCacheSizePerWrite;
            BattleArenaSeasonInfo *BattleArenaSeasonInfos;

            uint64_t StreamingDataCacheMaxAgeSeconds;
            EntitlementQuery **EntitlementQueries;

            GameBrowserConfig GameBrowser;
            BugSentryOnlineSettings BugSentrySettings;
            BlazeClientNotificationManagerSettings BlazeClientNotificationManager;
            MandatedVersionSettings MandatedVersion;

            float BattleArenaMaxPenalty;
            char *SocialSpaceDSubName;
            uint32_t StreamingDataCacheMaxSizeMb;
            char *TrialModeExpiredLicenseString;
            char *MatchmakingTokenPatchSuffix;
            char *SocialSpaceStartPoint;
            char *BattleArenaPenaltyExpiryStatId;
            char *PurchaseCoinsButtonLicenseOverride;
            float TrialTimeDataStartTime;
            int32_t GameReportInterval;
            char *GameConfigurationUrl;
            char *ConnectionLevel;
            int32_t StoreRefreshRetryMaxDelay;
            char *CreditsPath;
            char *EditorialRootUrl;
            int32_t StoreRefreshRetryDelay;
            float MaxTrialTime;
            char *TrialModePurchaseId;
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
            char *PodiumDSubName;
            float RespawnDelayTimeoutSeconds;
            int32_t ClientStoreEntityRefreshDelaySeconds;
            int32_t OriginProfileRefreshPeriodSeconds;
            int32_t StoreRefreshPeriodSeconds;
            int32_t RpcRetryDelay;
            int32_t BlazeClientSettingsRetryTime;
            int32_t StoreInterruptedPurchaseBusySeconds;
            char *SocialSpaceGameModeInclusion;
            int32_t RpcRandomOffsetMaxDelay;
            float DisconnectFailSafeTimer;
            char *ClientGameConfigurationOverride;
            float PrivilegeCacheTimeOut;
            char *ContentBaseUrl;
            char *LevelRootName;
            int32_t ClientStoreEntityTimerExpiredRefreshDelaySeconds;
            char *MatchmakingTokenSuffix;
            char *StreamingDataCacheSubDir;
            float GroupSwitchTimeOut;
            int32_t GroupReservationRenewPeriodWhileQueued;
            char *ConnectionLevelLayerInclusion;

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
    }
}
