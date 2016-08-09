//-----------------------------------------------------------------------------
//
// Copyright(C) 2014-2016 David Hill
//
// See COPYLIB for license information.
//
//-----------------------------------------------------------------------------
//
// Header for target library libacs.
//
// "AsmFunc" functions from ZDoom's ACS.
//
//-----------------------------------------------------------------------------

#ifndef __GDCC_Header__C__ACS_ZDoom__AsmFunc_h__
#define __GDCC_Header__C__ACS_ZDoom__AsmFunc_h__

#include <ACS_Hexen/AsmFunc.h>


//----------------------------------------------------------------------------|
// Global Functions                                                           |
//

ACS_AsmFuncDeclFull( 85,    , 0, void,   BeginHudMessage, void);
ACS_AsmFuncDeclFull( 85,    , 0, void,   BeginHudMessageBold, void);
ACS_AsmFuncDeclFull( 85,    , 0, void,   BeginLog, void);
ACS_AsmFuncDeclFull( 85,    , 0, void,   BeginStrParam, void);

ACS_AsmFuncDeclFull(102,    , 0, void,   ActivatorSound, __str, int);
ACS_AsmFuncDeclFull(103,    , 0, void,   LocalAmbientSound, __str, int);
ACS_AsmFuncDeclFull(104,    , 0, void,   SetLineMonsterBlocking, int, int);

ACS_AsmFuncDeclFull(118,    , 0, int,    IsNetworkGame, void);

ACS_AsmFuncDeclFull(122,    , 0, int,    PlayerFrags, void);

ACS_AsmFuncDeclFull(131,    , 0, void,   PrintName, int);

ACS_AsmFuncDeclFull(136,    , 0, int,    FixedMul, int, int);
ACS_AsmFuncDeclFull(137,    , 0, int,    FixedDiv, int, int);
ACS_AsmFuncDeclFull(138, 139, 0, void,   SetGravity, _Accum);
ACS_AsmFuncDeclFull(140, 141, 0, void,   SetAirControl, _Accum);
ACS_AsmFuncDeclFull(142,    , 0, void,   ClearInventory, void);
ACS_AsmFuncDeclFull(143, 144, 0, void,   GiveInventory, __str, int);
ACS_AsmFuncDeclFull(145, 146, 0, void,   TakeInventory, __str, int);
ACS_AsmFuncDeclFull(147, 148, 0, int,    CheckInventory, __str);
ACS_AsmFuncDeclFull(149, 150, 2, int,    Spawn, __str, _Accum, _Accum, _Accum, int, int);
ACS_AsmFuncDeclFull(151, 152, 2, int,    SpawnSpot, __str, int, int, int);
ACS_AsmFuncDeclFull(153, 154, 2, void,   SetMusic, __str, int, int);
ACS_AsmFuncDeclFull(155, 156, 2, void,   LocalSetMusic, __str, int, int);
ACS_AsmFuncDeclFull(157,    , 0, void,   PrintFixed, _Accum);
ACS_AsmFuncDeclFull(158,    , 0, void,   PrintLocalized, __str);
ACS_AsmFuncDeclFull(159,    , 0, void,   MoreHudMessage, void);
ACS_AsmFuncDeclFull(160,    , 0, void,   OptHudMessage, int, int, int, _Accum, _Accum, _Accum);
ACS_AsmFuncDeclFull(160,    , 0, void,   OptHudMessageS, int, int, __str, _Accum, _Accum, _Accum);
ACS_AsmFuncDeclFull(161,    , 0, void,   EndHudMessage, ...);
ACS_AsmFuncDeclFull(161,    , 0, void,   EndHudMessageX, _Accum);
ACS_AsmFuncDeclFull(161,    , 0, void,   EndHudMessageXX, _Accum, _Accum);
ACS_AsmFuncDeclFull(161,    , 0, void,   EndHudMessageXXX, _Accum, _Accum, _Accum);
ACS_AsmFuncDeclFull(162,    , 0, void,   EndHudMessageBold, ...);
ACS_AsmFuncDeclFull(162,    , 0, void,   EndHudMessageBoldX, _Accum);
ACS_AsmFuncDeclFull(162,    , 0, void,   EndHudMessageBoldXX, _Accum, _Accum);
ACS_AsmFuncDeclFull(162,    , 0, void,   EndHudMessageBoldXXX, _Accum, _Accum, _Accum);

ACS_AsmFuncDeclFull(165, 166, 0, void,   SetFont, __str);

ACS_AsmFuncDeclFull(180,    , 5, void,   SetThingSpecial, int, int, int, int, int, int, int);

ACS_AsmFuncDeclFull(190,    , 0, void,   FadeTo, int, int, int, _Accum, _Accum);
ACS_AsmFuncDeclFull(191,    , 0, void,   FadeRange, int, int, int, _Accum, int, int, int, _Accum, _Accum);
ACS_AsmFuncDeclFull(192,    , 0, void,   CancelFade, void);
ACS_AsmFuncDeclFull(193,    , 0, int,    PlayMovie, __str);
ACS_AsmFuncDeclFull(194,    , 5, void,   SetFloorTrigger, int, int, int, int, int, int, int, int);
ACS_AsmFuncDeclFull(195,    , 5, void,   SetCeilingTrigger, int, int, int, int, int, int, int, int);
ACS_AsmFuncDeclFull(196,    , 0, _Accum, GetActorX, int);
ACS_AsmFuncDeclFull(197,    , 0, _Accum, GetActorY, int);
ACS_AsmFuncDeclFull(198,    , 0, _Accum, GetActorZ, int);
ACS_AsmFuncDeclFull(199,    , 0, void,   CreateTranslationStart, int);
ACS_AsmFuncDeclFull(200,    , 0, void,   CreateTranslationPalette, int, int, int, int);
ACS_AsmFuncDeclFull(201,    , 0, void,   CreateTranslationRGB, int, int, int, int, int, int, int, int);
ACS_AsmFuncDeclFull(202,    , 0, void,   CreateTranslationEnd, void);

ACS_AsmFuncDeclFull(220,    , 0, _Accum, Sin, _Accum);
ACS_AsmFuncDeclFull(221,    , 0, _Accum, Cos, _Accum);
ACS_AsmFuncDeclFull(222,    , 0, _Accum, VectorAngle, _Accum, _Accum);
ACS_AsmFuncDeclFull(223,    , 0, int,    CheckWeapon, __str);
ACS_AsmFuncDeclFull(224,    , 0, int,    SetWeapon, __str);

ACS_AsmFuncDeclFull(244,    , 0, void,   SetMarineWeapon, int, int);
ACS_AsmFuncDeclFull(245,    , 0, void,   SetActorProperty, int, int, int);
ACS_AsmFuncDeclFull(245,    , 0, void,   SetActorPropertyFixed, int, int, _Accum);
ACS_AsmFuncDeclFull(245,    , 0, void,   SetActorPropertyString, int, int, __str);
ACS_AsmFuncDeclFull(246,    , 0, int,    GetActorProperty, int, int);
ACS_AsmFuncDeclFull(246,    , 0, _Accum, GetActorPropertyFixed, int, int);
ACS_AsmFuncDeclFull(246,    , 0, __str,  GetActorPropertyString, int, int);
ACS_AsmFuncDeclFull(247,    , 0, int,    PlayerNumber, void);
ACS_AsmFuncDeclFull(248,    , 0, int,    ActivatorTID, void);
ACS_AsmFuncDeclFull(249,    , 0, void,   SetMarineSprite, int, __str);
ACS_AsmFuncDeclFull(250,    , 0, int,    GetScreenWidth, void);
ACS_AsmFuncDeclFull(251,    , 0, int,    GetScreenHeight, void);
ACS_AsmFuncDeclFull(252,    , 3, void,   Thing_Projectile2, int, int, int, int, int, int, int);
ACS_AsmFuncDeclFull(253,    , 0, int,    StrLen, __str);
ACS_AsmFuncDeclFull(254,    , 1, void,   SetHudSize, int, int, int);
ACS_AsmFuncDeclFull(255,    , 0, int,    GetCVar, __str);
ACS_AsmFuncDeclFull(255,    , 0, _Accum, GetCVarFixed, __str);

ACS_AsmFuncDeclFull(257,    , 0, void,   SetResultValue, int);
ACS_AsmFuncDeclFull(257,    , 0, void,   SetResultValueFixed, _Accum);
ACS_AsmFuncDeclFull(258,    , 0, int,    GetLineRowOffset, void);
ACS_AsmFuncDeclFull(259,    , 0, _Accum, GetActorFloorZ, int);
ACS_AsmFuncDeclFull(260,    , 0, _Accum, GetActorAngle, int);
ACS_AsmFuncDeclFull(261,    , 0, _Accum, GetSectorFloorZ, int, int, int);
ACS_AsmFuncDeclFull(262,    , 0, _Accum, GetSectorCeilingZ, int, int, int);

ACS_AsmFuncDeclFull(264,    , 0, int,    GetSigilPieces, void);
ACS_AsmFuncDeclFull(265,    , 0, int,    GetLevelInfo, int);
ACS_AsmFuncDeclFull(266,    , 0, void,   ChangeSky, __str, __str);
ACS_AsmFuncDeclFull(267,    , 0, int,    PlayerInGame, int);
ACS_AsmFuncDeclFull(268,    , 0, int,    PlayerIsBot, int);
ACS_AsmFuncDeclFull(269,    , 0, void,   SetCameraToTexture, int, __str, int);
ACS_AsmFuncDeclFull(270,    , 0, void,   EndLog, void);
ACS_AsmFuncDeclFull(271,    , 0, int,    GetAmmoCapacity, __str);
ACS_AsmFuncDeclFull(272,    , 0, void,   SetAmmoCapacity, __str, int);
ACS_AsmFuncDeclFull(273,    , 0, void,   PrintMapCharArray, int, int);
ACS_AsmFuncDeclFull(274,    , 0, void,   PrintWorldCharArray, int, int);
ACS_AsmFuncDeclFull(275,    , 0, void,   PrintGlobalCharArray, int, int);
ACS_AsmFuncDeclFull(276,    , 0, void,   SetActorAngle, int, _Accum);

ACS_AsmFuncDeclFull(280,    , 0, void,   SpawnProjectile, int, __str, int, int, int, int, int);
ACS_AsmFuncDeclFull(281,    , 0, int,    GetSectorLightLevel, int);
ACS_AsmFuncDeclFull(282,    , 0, _Accum, GetActorCeilingZ, int);
ACS_AsmFuncDeclFull(283,    , 1, int,    SetActorPosition, int, _Accum, _Accum, _Accum, int);
ACS_AsmFuncDeclFull(284,    , 0, void,   ClearActorInventory, int);
ACS_AsmFuncDeclFull(285,    , 0, void,   GiveActorInventory, int, __str, int);
ACS_AsmFuncDeclFull(286,    , 0, void,   TakeActorInventory, int, __str, int);
ACS_AsmFuncDeclFull(287,    , 0, int,    CheckActorInventory, int, __str);
ACS_AsmFuncDeclFull(288,    , 0, int,    ThingCountName, __str, int);
ACS_AsmFuncDeclFull(289,    , 1, int,    SpawnSpotFacing, __str, int, int);
ACS_AsmFuncDeclFull(290,    , 0, int,    PlayerClass, int);

ACS_AsmFuncDeclFull(326,    , 0, int,    GetPlayerInfo, int, int);
ACS_AsmFuncDeclFull(327,    , 0, void,   ChangeLevel, __str, int, int, int);
ACS_AsmFuncDeclFull(328,    , 0, void,   SectorDamage, int, int, __str, __str, int);
ACS_AsmFuncDeclFull(329,    , 1, void,   ReplaceTextures, __str, __str, int);

ACS_AsmFuncDeclFull(331,    , 0, _Accum, GetActorPitch, int);
ACS_AsmFuncDeclFull(332,    , 0, void,   SetActorPitch, int, _Accum);
ACS_AsmFuncDeclFull(333,    , 0, void,   PrintBind, __str);
ACS_AsmFuncDeclFull(334,    , 1, int,    SetActorState, int, __str, int);
ACS_AsmFuncDeclFull(335,    , 0, int,    Thing_Damage2, int, int, __str);
ACS_AsmFuncDeclFull(336,    , 0, int,    UseInventory, __str);
ACS_AsmFuncDeclFull(337,    , 0, int,    UseActorInventory, int, __str);
ACS_AsmFuncDeclFull(338,    , 0, int,    CheckActorCeilingTexture, int, __str);
ACS_AsmFuncDeclFull(339,    , 0, int,    CheckActorFloorTexture, int, __str);
ACS_AsmFuncDeclFull(340,    , 0, int,    GetActorLightLevel, int);
ACS_AsmFuncDeclFull(341,    , 0, void,   SetMugShotState, __str);
ACS_AsmFuncDeclFull(342,    , 0, int,    ThingCountSector, int, int, int);
ACS_AsmFuncDeclFull(343,    , 0, int,    ThingCountNameSector, __str, int, int);
ACS_AsmFuncDeclFull(344,    , 0, int,    CheckPlayerCamera, int);
ACS_AsmFuncDeclFull(345,    , 0, int,    MorphActor, int, __str, __str, int, int, __str, __str);
ACS_AsmFuncDeclFull(346,    , 1, int,    UnMorphActor, int, int);
ACS_AsmFuncDeclFull(347,    , 0, int,    GetPlayerInput, int, int);
ACS_AsmFuncDeclFull(347,    , 0, _Accum, GetPlayerInputFixed, int, int);
ACS_AsmFuncDeclFull(348,    , 0, int,    ClassifyActor, int);
ACS_AsmFuncDeclFull(349,    , 0, void,   PrintBinary, int);
ACS_AsmFuncDeclFull(350,    , 0, void,   PrintHex, int);

ACS_AsmFuncDeclFull(352,    , 0, __str,  EndStrParam, void);
ACS_AsmFuncDeclFull(353,    , 0, void,   PrintMapCharRange, int, int, int, int);
ACS_AsmFuncDeclFull(354,    , 0, void,   PrintWorldCharRange, int, int, int, int);
ACS_AsmFuncDeclFull(355,    , 0, void,   PrintGlobalCharRange, int, int, int, int);
ACS_AsmFuncDeclFull(356,    , 0, int,    StrCpyToMapCharRange, int, int, int, int, __str, int);
ACS_AsmFuncDeclFull(356,    , 0, int,    StrArsCpyToMapCharRange, int, int, int, int, char __str_ars const *);
ACS_AsmFuncDeclFull(357,    , 0, int,    StrCpyToWorldCharRange, int, int, int, int, __str, int);
ACS_AsmFuncDeclFull(357,    , 0, int,    StrArsCpyToWorldCharRange, int, int, int, int, char __str_ars const *);
ACS_AsmFuncDeclFull(358,    , 0, int,    StrCpyToGlobalCharRange, int, int, int, int, __str, int);
ACS_AsmFuncDeclFull(358,    , 0, int,    StrArsCpyToGlobalCharRange, int, int, int, int, char __str_ars const *);

ACS_AsmFuncDeclFull(361,    , 0, void,   NamedScriptWait, __str);
ACS_AsmFuncDeclFull(362,    , 0, void,   CreateTranslationDesat, int, int, _Accum, _Accum, _Accum, _Accum, _Accum, _Accum);

ACS_AsmFuncDeclFull(378,    , 0, void,   PrintLocalCharArray, int, int);
ACS_AsmFuncDeclFull(379,    , 0, void,   PrintLocalCharRange, int, int, int, int);
ACS_AsmFuncDeclFull(380,    , 0, int,    StrCpyToLocalCharRange, int, int, int, int, __str, int);

#endif//__GDCC_Header__C__ACS_ZDoom__AsmFunc_h__

