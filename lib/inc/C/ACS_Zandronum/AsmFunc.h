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
// "AsmFunc" functions from Zandronum's ACS.
//
//-----------------------------------------------------------------------------

#ifndef __GDCC_Header__C__ACS_Zandronum__AsmFunc_h__
#define __GDCC_Header__C__ACS_Zandronum__AsmFunc_h__

#include <ACS_ZDoom/AsmFunc.h>


//----------------------------------------------------------------------------|
// Global Functions                                                           |
//

ACS_AsmFuncDeclFull(118,    , 0, int,  IsMultiplayer, void);
ACS_AsmFuncDeclFull(119,    , 0, int,  PlayerTeam, void);
ACS_AsmFuncDeclFull(120,    , 0, int,  PlayerHealth, void);
ACS_AsmFuncDeclFull(121,    , 0, int,  PlayerArmorPoints, void);

ACS_AsmFuncDeclFull(124,    , 0, int,  BlueTeamCount, void);
ACS_AsmFuncDeclFull(125,    , 0, int,  RedTeamCount, void);
ACS_AsmFuncDeclFull(126,    , 0, int,  BlueTeamScore, void);
ACS_AsmFuncDeclFull(127,    , 0, int,  RedTeamScore, void);
ACS_AsmFuncDeclFull(128,    , 0, int,  IsOneFlagCTF, void);
ACS_AsmFuncDeclFull(129,    , 0, int,  GetInvasionWave, void);
ACS_AsmFuncDeclFull(130,    , 0, int,  GetInvasionState, void);

ACS_AsmFuncDeclFull(132,    , 0, void, Music_Change, __str, int);
ACS_AsmFuncDeclFull(134, 133, 2, void, ConsoleCommand, __str, int, int);
ACS_AsmFuncDeclFull(135,    , 0, int,  SinglePlayer, void);

#endif//__GDCC_Header__C__ACS_Zandronum__AsmFunc_h__

