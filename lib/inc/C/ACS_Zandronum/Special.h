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
// "Special" functions from Zandronum's ACS.
//
//-----------------------------------------------------------------------------

#ifndef __GDCC_Header__C__ACS_Zandronum__Special_h__
#define __GDCC_Header__C__ACS_Zandronum__Special_h__

#include <ACS_ZDoom/Special.h>


//----------------------------------------------------------------------------|
// Global Functions                                                           |
//

ACS_SpecialDeclFull(136, 0, void, Player_SetTeam, int);

ACS_SpecialDeclFull(152, 0, void, Team_Score, int, int);
ACS_SpecialDeclFull(153, 0, void, Team_GivePoints, int, int, int);

#endif//__GDCC_Header__C__ACS_Zandronum__Special_h__

