#pragma once
#include <windows.h>
#include <string_view>

namespace fb
{
enum SecureReason : int32_t
{
    SecureReason_Ok = 0,
    SecureReason_WrongProtocolVersion = 1,
    SecureReason_WrongTitleVersion = 2,
    SecureReason_ServerFull = 3,
    SecureReason_KickedOut = 4,
    SecureReason_Banned = 5,
    SecureReason_GenericError = 6,
    SecureReason_WrongPassword = 7,
    SecureReason_KickedOutDemoOver = 8,
    SecureReason_RankRestricted = 9,
    SecureReason_ConfigurationNotAllowed = 10,
    SecureReason_ServerReclaimed = 11,
    SecureReason_MissingContent = 12,
    SecureReason_NotVerified = 13,
    SecureReason_TimedOut = 14,
    SecureReason_ConnectFailed = 15,
    SecureReason_NoReply = 16,
    SecureReason_AcceptFailed = 17,
    SecureReason_MismatchingContent = 18,
    SecureReason_InteractivityTimeout = 19,
    SecureReason_KickedFromQueue = 20,
    SecureReason_TeamKills = 21,
    SecureReason_KickedByAdmin = 22,
    SecureReason_KickedViaPunkBuster = 23,
    SecureReason_KickedOutServerFull = 24,
    SecureReason_ESportsMatchStarting = 25,
    SecureReason_NotInESportsRosters = 26,
    SecureReason_ESportsMatchEnding = 27,
    SecureReason_VirtualServerExpired = 28,
    SecureReason_VirtualServerRecreate = 29,
    SecureReason_ESportsTeamFull = 30,
    SecureReason_ESportsMatchAborted = 31,
    SecureReason_ESportsMatchWalkover = 32,
    SecureReason_ESportsMatchWarmupTimedOut = 33,
    SecureReason_NotAllowedToSpectate = 34,
    SecureReason_NoSpectateSlotAvailable = 35,
    SecureReason_InvalidSpectateJoin = 36,
    SecureReason_KickedViaFairFight = 37,
    SecureReason_KickedCommanderOnLeave = 38,
    SecureReason_KickedCommanderAfterMutiny = 39,
    SecureReason_ServerMaintenance = 40
};

[[nodiscard]] constexpr std::string_view secure_reason_message(SecureReason reason) noexcept
{
    switch (reason)
    {
    case SecureReason_Ok:
        return "Connection established.";

    case SecureReason_WrongProtocolVersion:
        return "Client and server protocol versions do not match.";

    case SecureReason_WrongTitleVersion:
        return "Your game version is not compatible with this server.";

    case SecureReason_ServerFull:
    case SecureReason_KickedOutServerFull:
        return "The server is full.";

    case SecureReason_KickedOut:
        return "You were disconnected from the server.";

    case SecureReason_Banned:
        return "You are banned from this server.";

    case SecureReason_GenericError:
        return "A connection error occurred.";

    case SecureReason_WrongPassword:
        return "The server password is incorrect.";

    case SecureReason_KickedOutDemoOver:
        return "The demo session has ended.";

    case SecureReason_RankRestricted:
        return "Your rank does not meet the server requirements.";

    case SecureReason_ConfigurationNotAllowed:
        return "Your configuration is not allowed on this server.";

    case SecureReason_ServerReclaimed:
        return "The server was reclaimed by its owner.";

    case SecureReason_MissingContent:
        return "Required content is missing.";

    case SecureReason_NotVerified:
        return "Your account could not be verified.";

    case SecureReason_TimedOut:
    case SecureReason_InteractivityTimeout:
        return "Connection timed out.";

    case SecureReason_ConnectFailed:
        return "Failed to connect to the server.";

    case SecureReason_NoReply:
        return "No response from the server.";

    case SecureReason_AcceptFailed:
        return "The server rejected the connection.";

    case SecureReason_MismatchingContent:
        return "Installed content does not match the server.";

    case SecureReason_KickedFromQueue:
        return "You were removed from the server queue.";

    case SecureReason_TeamKills:
        return "You were kicked for excessive team killing.";

    case SecureReason_KickedByAdmin:
        return "You were kicked by a server administrator.";

    case SecureReason_KickedViaPunkBuster:
        return "You were kicked by PunkBuster.";

    case SecureReason_ESportsMatchStarting:
        return "The eSports match is starting.";

    case SecureReason_NotInESportsRosters:
        return "You are not part of the required eSports roster.";

    case SecureReason_ESportsMatchEnding:
        return "The eSports match has ended.";

    case SecureReason_VirtualServerExpired:
        return "The virtual server has expired.";

    case SecureReason_VirtualServerRecreate:
        return "The virtual server is being recreated.";

    case SecureReason_ESportsTeamFull:
        return "The eSports team is full.";

    case SecureReason_ESportsMatchAborted:
        return "The eSports match was aborted.";

    case SecureReason_ESportsMatchWalkover:
        return "The eSports match ended in a walkover.";

    case SecureReason_ESportsMatchWarmupTimedOut:
        return "The eSports match warmup timed out.";

    case SecureReason_NotAllowedToSpectate:
        return "You are not allowed to spectate this match.";

    case SecureReason_NoSpectateSlotAvailable:
        return "No spectator slots are available.";

    case SecureReason_InvalidSpectateJoin:
        return "Unable to join as a spectator.";

    case SecureReason_KickedViaFairFight:
        return "You were kicked by FairFight.";

    case SecureReason_KickedCommanderOnLeave:
        return "You were removed as commander after leaving.";

    case SecureReason_KickedCommanderAfterMutiny:
        return "You were removed as commander after a mutiny.";

    case SecureReason_ServerMaintenance:
        return "The server is undergoing maintenance.";

    default:
        return "An unknown connection error occurred.";
    }
}

[[nodiscard]] inline bool show_disconnected_dialog(SecureReason reason) noexcept
{
    const auto msg = secure_reason_message(reason);

    int r = ::MessageBoxA(nullptr, msg.data(), "Disconnected", MB_ICONWARNING | MB_SYSTEMMODAL | MB_RETRYCANCEL | MB_SETFOREGROUND);

    return r == IDRETRY;
}
} // namespace fb
