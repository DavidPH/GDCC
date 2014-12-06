//-----------------------------------------------------------------------------
//
// Copyright(C) 2014 David Hill
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
// Global Functions                                                           |
//

ACS_NativeDeclFull( 37, 0, int,    AnnouncerSound, __str, int);

ACS_NativeDeclFull(100, 0, int,    ResetMap, void);
ACS_NativeDeclFull(101, 0, int,    PlayerIsSpectator, int);
ACS_NativeDeclFull(102, 0, int,    ConsolePlayerNumber, void);
ACS_NativeDeclFull(103, 0, int,    GetTeamProperty, int, int);
ACS_NativeDeclFull(103, 0, _Accum, GetTeamPropertyFixed, int, int);
ACS_NativeDeclFull(103, 0, __str,  GetTeamPropertyString, int, int);
ACS_NativeDeclFull(104, 0, int,    GetPlayerLivesLeft, int);
ACS_NativeDeclFull(105, 0, int,    SetPlayerLivesLeft, int, int);
ACS_NativeDeclFull(106, 0, int,    KickFromGame, int, __str);
ACS_NativeDeclFull(107, 0, int,    GetGameModeState, void);
ACS_NativeDeclFull(118, 0, int,    SetDBEntry, __str, __str, int);
ACS_NativeDeclFull(118, 0, int,    SetDBEntryFixed, __str, __str, _Accum);
ACS_NativeDeclFull(119, 0, int,    GetDBEntry, __str, __str);
ACS_NativeDeclFull(119, 0, _Accum, GetDBEntryFixed, __str, __str);
ACS_NativeDeclFull(110, 0, int,    SetDBEntryString, __str, __str, __str);
ACS_NativeDeclFull(111, 0, __str,  GetDBEntryString, __str, __str);
ACS_NativeDeclFull(112, 0, int,    IncrementDBEntry, __str, __str, int);
ACS_NativeDeclFull(112, 0, int,    IncrementDBEntryFixed, __str, __str, _Accum);
ACS_NativeDeclFull(113, 0, int,    PlayerIsLoggedIn, int);
ACS_NativeDeclFull(114, 0, __str,  GetPlayerAccountName, int);
ACS_NativeDeclFull(115, 0, int,    SortDBEntries, __str, int, int, int);
ACS_NativeDeclFull(116, 0, int,    CountDBResults, int);
ACS_NativeDeclFull(117, 0, int,    FreeDBResults, int);
ACS_NativeDeclFull(118, 0, __str,  GetDBResultKeyString, int, int);
ACS_NativeDeclFull(119, 0, __str,  GetDBResultValueString, int, int);
ACS_NativeDeclFull(120, 0, int,    GetDBResultValue, int, int);
ACS_NativeDeclFull(120, 0, _Accum, GetDBResultValueFixed, int, int);
ACS_NativeDeclFull(121, 0, int,    GetDBEntryRank, __str, __str, int);
ACS_NativeDeclFull(122, 3, int,    RequestPukeScript, int, int, int, int);
ACS_NativeDeclFull(123, 0, int,    BeginDBTransaction, void);
ACS_NativeDeclFull(124, 0, int,    EndDBTransaction, void);
ACS_NativeDeclFull(125, 0, int,    GetDBEntries, __str);

#endif//__GDCC_Header__C__ACS_Zandronum__Native_h__

