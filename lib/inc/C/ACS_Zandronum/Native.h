//-----------------------------------------------------------------------------
//
// Copyright(C) 2014-2024 David Hill
//
// See COPYLIB for license information.
//
//-----------------------------------------------------------------------------
//
// Header for target library libacs.
//
// "Native" functions from Zandronum's ACS.
//
//-----------------------------------------------------------------------------

#ifndef __GDCC_Header__C__ACS_Zandronum__Native_h__
#define __GDCC_Header__C__ACS_Zandronum__Native_h__

#include <ACS_ZDoom/Native.h>


//----------------------------------------------------------------------------|
// Extern Functions                                                           |
//

ACS_NativeDeclFull( 37, 0, int,    AnnouncerSound, (__str, int));

ACS_NativeDeclFull(100, 0, int,    ResetMap, (void));
ACS_NativeDeclFull(101, 0, int,    PlayerIsSpectator, (int));
ACS_NativeDeclFull(102, 0, int,    ConsolePlayerNumber, (void));
ACS_NativeDeclFull(103, 0, int,    GetTeamProperty, (int, int));
ACS_NativeDeclFull(103, 0, _Accum, GetTeamPropertyFixed, (int, int));
ACS_NativeDeclFull(103, 0, __str,  GetTeamPropertyString, (int, int));
ACS_NativeDeclFull(104, 0, int,    GetPlayerLivesLeft, (int));
ACS_NativeDeclFull(105, 0, int,    SetPlayerLivesLeft, (int, int));
ACS_NativeDeclFull(106, 0, int,    KickFromGame, (int, __str));
ACS_NativeDeclFull(107, 0, int,    GetGameModeState, (void));
ACS_NativeDeclFull(108, 0, int,    SetDBEntry, (__str, __str, int));
ACS_NativeDeclFull(108, 0, int,    SetDBEntryFixed, (__str, __str, _Accum));
ACS_NativeDeclFull(109, 0, int,    GetDBEntry, (__str, __str));
ACS_NativeDeclFull(109, 0, _Accum, GetDBEntryFixed, (__str, __str));
ACS_NativeDeclFull(110, 0, int,    SetDBEntryString, (__str, __str, __str));
ACS_NativeDeclFull(111, 0, __str,  GetDBEntryString, (__str, __str));
ACS_NativeDeclFull(112, 0, int,    IncrementDBEntry, (__str, __str, int));
ACS_NativeDeclFull(112, 0, int,    IncrementDBEntryFixed, (__str, __str, _Accum));
ACS_NativeDeclFull(113, 0, int,    PlayerIsLoggedIn, (int));
ACS_NativeDeclFull(114, 0, __str,  GetPlayerAccountName, (int));
ACS_NativeDeclFull(115, 0, int,    SortDBEntries, (__str, int, int, int));
ACS_NativeDeclFull(116, 0, int,    CountDBResults, (int));
ACS_NativeDeclFull(117, 0, int,    FreeDBResults, (int));
ACS_NativeDeclFull(118, 0, __str,  GetDBResultKeyString, (int, int));
ACS_NativeDeclFull(119, 0, __str,  GetDBResultValueString, (int, int));
ACS_NativeDeclFull(120, 0, int,    GetDBResultValue, (int, int));
ACS_NativeDeclFull(120, 0, _Accum, GetDBResultValueFixed, (int, int));
ACS_NativeDeclFull(121, 0, int,    GetDBEntryRank, (__str, __str, int));
ACS_NativeDeclFull(122, 0, int,    RequestScriptPuke, (int, int, int, int, int));
ACS_NativeDeclFull(123, 0, int,    BeginDBTransaction, (void));
ACS_NativeDeclFull(124, 0, int,    EndDBTransaction, (void));
ACS_NativeDeclFull(125, 0, int,    GetDBEntries, (__str));
ACS_NativeDeclFull(126, 0, int,    NamedRequestScriptPuke, (__str, int, int, int, int));
ACS_NativeDeclFull(127, 0, int,    SystemTime, (void));
ACS_NativeDeclFull(128, 0, int,    GetTimeProperty, (int, int, int));
ACS_NativeDeclFull(129, 0, __str,  Strftime, (int, __str, int));
ACS_NativeDeclFull(130, 0, __str,  SetDeadSpectator, (int, int));
ACS_NativeDeclFull(131, 0, __str,  SetActivatorToPlayer, (int));
ACS_NativeDeclFull(132, 0, int,    SetCurrentGamemode, (__str));
ACS_NativeDeclFull(133, 0, __str,  GetCurrentGamemode, (void));
ACS_NativeDeclFull(134, 0, int,    SetGamemodeLimit, (int, int));
ACS_NativeDeclFull(135, 0, int,    SetPlayerClass, (int, __str, int));
ACS_NativeDeclFull(136, 0, int,    SetPlayerChasecam, (int, int));
ACS_NativeDeclFull(137, 0, int,    GetPlayerChasecam, (int));
ACS_NativeDeclFull(138, 0, int,    SetPlayerScore, (int, int, int));
ACS_NativeDeclFull(139, 0, int,    GetPlayerScore, (int, int));
ACS_NativeDeclFull(140, 0, int,    InDemoMode, (void));
ACS_NativeDeclFull(144, 4, int,    ExecuteClientScript, (int, int, int, int, int, int));
ACS_NativeDeclFull(145, 4, int,    NamedExecuteClientScript, (__str, int, int, int, int, int));
ACS_NativeDeclFull(146, 1, int,    SendNetworkString, (int, __str, int));
ACS_NativeDeclFull(147, 1, int,    NamedSendNetworkString, (__str, __str, int));
ACS_NativeDeclFull(148, 0, __str,  GetChatMessage, (int, int));
ACS_NativeDeclFull(149, 0, int,    GetMapRotationSize, (void));
ACS_NativeDeclFull(150, 0, int,    GetMapRotationInfo, (int, int));
ACS_NativeDeclFull(150, 0, __str,  GetMapRotationInfoString, (int, int));
ACS_NativeDeclFull(151, 0, int,    GetCurrentMapPosition, (void));
ACS_NativeDeclFull(152, 0, int,    GetEventResult, (void));
ACS_NativeDeclFull(153, 0, __str,  GetActorSectorLocation, (int, int));
ACS_NativeDeclFull(154, 1, int,    ChangeTeamScore, (int, int, int, int));
ACS_NativeDeclFull(155, 0, int,    SetGameplaySetting, (__str, int));
ACS_NativeDeclFull(156, 0, int,    SetCustomPlayerValue, (__str, int, int));
ACS_NativeDeclFull(156, 0, int,    SetCustomPlayerValueFixed, (__str, int, _Accum));
ACS_NativeDeclFull(156, 0, int,    SetCustomPlayerValueString, (__str, int, __str));
ACS_NativeDeclFull(157, 0, int,    GetCustomPlayerValue, (__str, int));
ACS_NativeDeclFull(157, 0, _Accum, GetCustomPlayerValueFixed, (__str, int));
ACS_NativeDeclFull(157, 0, __str,  GetCustomPlayerValueString, (__str, int));
ACS_NativeDeclFull(158, 0, int,    ResetCustomDataToDefault, (__str, int));

ACS_NativeDeclFull(168, 2, int,    AddBot, (__str, int));
ACS_NativeDeclFull(169, 1, int,    RemoveBot, (__str));
ACS_NativeDeclFull(170, 0, int,    OpenMenu, (__str));
ACS_NativeDeclFull(171, 0, int,    CloseMenu, (void));
ACS_NativeDeclFull(172, 1, int,    BanFromGame, (int, int, __str));
ACS_NativeDeclFull(173, 0, int,    GetPlayerStatus, (int));
ACS_NativeDeclFull(174, 1, int,    SetPlayerWeaponZoomFactor, (int, _Accum, int));
ACS_NativeDeclFull(175, 1, int,    SetPlayerSkin, (int, __str, int));
ACS_NativeDeclFull(176, 0, __str,  GetPlayerSkin, (int, int));
ACS_NativeDeclFull(177, 0, __str,  GetPlayerCountry, (int, int));
ACS_NativeDeclFull(178, 0, int,    SetNextMapPosition, (int, int));
ACS_NativeDeclFull(179, 0, int,    GivePlayerMedal, (int, __str, int));
ACS_NativeDeclFull(180, 0, int,    GetPlayerJoinQueuePosition, (int));
ACS_NativeDeclFull(181, 0, int,    SkipJoinQueue, (int));
ACS_NativeDeclFull(182, 0, int,    GetControlPointInfo, (int, int));
ACS_NativeDeclFull(182, 0, _Accum, GetControlPointInfoFixed, (int, int));
ACS_NativeDeclFull(182, 0, __str,  GetControlPointInfoString, (int, int));
ACS_NativeDeclFull(183, 0, int,    SetControlPointInfo, (int, int, int));
ACS_NativeDeclFull(183, 0, int,    SetControlPointInfoFixed, (int, int, _Accum));
ACS_NativeDeclFull(183, 0, int,    SetControlPointInfoString, (int, int, __str));
ACS_NativeDeclFull(184, 2, int,    GetSkinProperty, (int, __str, int, int));
ACS_NativeDeclFull(184, 2, _Accum, GetSkinPropertyFixed, (int, __str, int, int));
ACS_NativeDeclFull(184, 2, __str,  GetSkinPropertyString, (int, __str, int, int));
ACS_NativeDeclFull(185, 0, int,    IsPlayerContestingControlPoint, (int, int));

#endif//__GDCC_Header__C__ACS_Zandronum__Native_h__

