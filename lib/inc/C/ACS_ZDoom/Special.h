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
// "Special" functions from ZDoom's ACS.
//
//-----------------------------------------------------------------------------

#ifndef __GDCC_Header__C__ACS_ZDoom__Special_h__
#define __GDCC_Header__C__ACS_ZDoom__Special_h__

#include <ACS_Hexen/Special.h>


//----------------------------------------------------------------------------|
// Global Functions                                                           |
//

ACS_SpecialDeclFull( 14, 0, void, Door_Animated, int, int, int);
ACS_SpecialDeclFull( 15, 0, void, Autosave, void);

ACS_SpecialDeclFull( 17, 0, void, Thing_Raise, int);
ACS_SpecialDeclFull( 18, 1, void, StartConversation, int, int);
ACS_SpecialDeclFull( 19, 0, void, Thing_Stop, int);

ACS_SpecialDeclFull( 33, 0, void, ForceField, void);
ACS_SpecialDeclFull( 34, 0, void, ClearForceField, int);

ACS_SpecialDeclFull( 37, 1, void, Floor_MoveToValue, int, int, int, int);
ACS_SpecialDeclFull( 38, 0, void, Ceiling_Waggle, int, int, int, int, int);
ACS_SpecialDeclFull( 39, 0, void, Teleport_ZombieChanger, int, int);

ACS_SpecialDeclFull( 47, 1, void, Ceiling_MoveToValue, int, int, int, int);

ACS_SpecialDeclFull( 49, 1, void, GlassBreak, int);

ACS_SpecialDeclFull( 51, 0, void, Sector_SetLink, int, int, int, int);
ACS_SpecialDeclFull( 52, 0, void, Scroll_Wall, int, int, int, int, int);
ACS_SpecialDeclFull( 53, 0, void, Line_SetTextureOffset, int, int, int, int, int);
ACS_SpecialDeclFull( 54, 0, void, Sector_ChangeFlags, int, int, int);
ACS_SpecialDeclFull( 55, 0, void, Line_SetBlocking, int, int, int);
ACS_SpecialDeclFull( 56, 0, void, Line_SetTextureScale, int, int, int, int, int);

ACS_SpecialDeclFull( 57, 0, void, Polyobj_OR_MoveToSpot, int, int, int);

ACS_SpecialDeclFull( 75, 0, void, Teleport_EndGame, void);
ACS_SpecialDeclFull( 76, 0, void, TeleportOther, int, int, int);
ACS_SpecialDeclFull( 77, 0, void, TeleportGroup, int, int, int, int, int);
ACS_SpecialDeclFull( 78, 1, void, TeleportInSector, int, int, int, int, int);
ACS_SpecialDeclFull( 79, 0, void, Thing_SetConversation, int, int);

ACS_SpecialDeclRaw(  84, 4, int,  ACS_ExecuteWithResult, int, int, int, int, int);
ACS_SpecialDeclRaw(  85, 0, void, ACS_LockedExecuteDoor, int, int, int, int, int);
ACS_SpecialDeclFull( 86, 0, void, Polyobj_MoveToSpot, int, int, int);
ACS_SpecialDeclFull( 87, 0, void, Polyobj_Stop, int);
ACS_SpecialDeclFull( 88, 0, void, Polyobj_MoveTo, int, int, int, int);
ACS_SpecialDeclFull( 89, 0, void, Polyobj_OR_MoveTo, int, int, int, int);

ACS_SpecialDeclFull( 97, 2, void, Ceiling_LowerAndCrushDist, int, int, int, int, int);
ACS_SpecialDeclFull( 98, 1, void, Sector_SetTranslucent, int, int, int, int);

ACS_SpecialDeclFull(104, 1, void, Ceiling_CrushAndRaiseSilentDist, int, int, int, int, int);

ACS_SpecialDeclFull(117, 0, void, Light_Stop, int);

ACS_SpecialDeclFull(119, 1, void, Thing_Damage, int, int, int);

ACS_SpecialDeclFull(125, 1, void, Thing_Move, int, int, int);

ACS_SpecialDeclFull(127, 3, void, Thing_SetSpecial, int, int, int, int, int);
ACS_SpecialDeclFull(128, 0, void, ThrustThingZ, int, int, int, int);

ACS_SpecialDeclFull(139, 2, void, Thing_SpawnFacing, int, int, int, int);

ACS_SpecialDeclFull(134, 0, void, Player_RemoveItem, int, int);
ACS_SpecialDeclFull(135, 0, void, Player_GiveItem, int, int);

ACS_SpecialDeclFull(154, 1, void, Teleport_NoStop, int, int, int);

ACS_SpecialDeclFull(157, 0, void, SetGlobalFogParameter, int, int);
ACS_SpecialDeclFull(158, 3, void, FS_Execute, int, int, int, int);
ACS_SpecialDeclFull(159, 0, void, Sector_SetPlaneReflection, int, int, int);

ACS_SpecialDeclFull(168, 2, void, Ceiling_CrushAndRaiseDist, int, int, int, int, int);
ACS_SpecialDeclFull(169, 0, void, Generic_Crusher2, int, int, int, int, int);
ACS_SpecialDeclFull(170, 0, void, Sector_SetCeilingScale2, int, int, int);
ACS_SpecialDeclFull(171, 0, void, SectorSetFloorScale2, int, int, int);
ACS_SpecialDeclFull(172, 0, void, Plat_UpNearestWaitDownStay, int, int, int);
ACS_SpecialDeclFull(173, 0, void, NoiseAlert, int, int);
ACS_SpecialDeclFull(174, 0, void, SendToCommunicator, int, int, int, int);
ACS_SpecialDeclFull(175, 0, void, Thing_ProjectileIntercept, int, int, int, int, int);
ACS_SpecialDeclFull(176, 0, void, Thing_ChangeTID, int, int);
ACS_SpecialDeclFull(177, 1, void, Thing_Hate, int, int, int);
ACS_SpecialDeclFull(178, 1, void, Thing_ProjectileAimed, int, int, int, int, int);
ACS_SpecialDeclFull(179, 0, void, ChangeSkill, int);
ACS_SpecialDeclFull(180, 0, void, Thing_SetTranslation, int, int);

ACS_SpecialDeclFull(183, 0, void, Line_AlignCeiling, int, int);
ACS_SpecialDeclFull(184, 0, void, Line_AlignFloor, int, int);
ACS_SpecialDeclFull(185, 0, void, Sector_SetRotation, int, int, int);
ACS_SpecialDeclFull(186, 0, void, Sector_SetCeilingPanning, int, int, int, int, int);
ACS_SpecialDeclFull(187, 0, void, Sector_SetFloorPanning, int, int, int, int, int);
ACS_SpecialDeclFull(188, 0, void, Sector_SetCeilingScale, int, int, int, int, int);
ACS_SpecialDeclFull(189, 0, void, Sector_SetFloorScale, int, int, int, int, int);

ACS_SpecialDeclFull(191, 0, void, SetPlayerProperty, int, int, int);
ACS_SpecialDeclFull(192, 0, void, Ceiling_LowerToHighestFloor, int, int);
ACS_SpecialDeclFull(193, 0, void, Ceiling_LowerInstant, int, int, int);
ACS_SpecialDeclFull(194, 0, void, Ceiling_RaiseInstant, int, int, int);
ACS_SpecialDeclFull(195, 1, void, Ceiling_CrushRaiseAndStayA, int, int, int, int, int);
ACS_SpecialDeclFull(196, 1, void, Ceiling_CrushAndRaiseA, int, int, int, int, int);
ACS_SpecialDeclFull(197, 1, void, Ceiling_CrushAndRaiseSilentA, int, int, int, int, int);
ACS_SpecialDeclFull(198, 0, void, Ceiling_RaiseByValueTimes8, int, int, int);
ACS_SpecialDeclFull(199, 0, void, Ceiling_LowerByValueTimes8, int, int, int);
ACS_SpecialDeclFull(200, 0, void, Generic_Floor, int, int, int, int, int);
ACS_SpecialDeclFull(201, 0, void, Generic_Ceiling, int, int, int, int, int);
ACS_SpecialDeclFull(202, 0, void, Generic_Door, int, int, int, int, int);
ACS_SpecialDeclFull(203, 0, void, Generic_Lift, int, int, int, int, int);
ACS_SpecialDeclFull(204, 0, void, Generic_Stairs, int, int, int, int, int);
ACS_SpecialDeclFull(205, 0, void, Generic_Crusher, int, int, int, int, int);
ACS_SpecialDeclFull(206, 1, void, Plat_DownWaitUpStayLip, int, int, int, int, int);
ACS_SpecialDeclFull(207, 0, void, Plat_PerpetualRaiseLip, int, int, int, int);
ACS_SpecialDeclFull(208, 1, void, TranslucentLine, int, int, int);

ACS_SpecialDeclFull(212, 1, void, Sector_SetColor, int, int, int, int, int);
ACS_SpecialDeclFull(213, 0, void, Sector_SetFade, int, int, int, int);
ACS_SpecialDeclFull(214, 0, void, Sector_SetDamage, int, int, int);

ACS_SpecialDeclFull(216, 0, void, Sector_SetGravity, int, int, int);
ACS_SpecialDeclFull(217, 0, void, Stairs_BuildUpDoom, int, int, int, int, int);
ACS_SpecialDeclFull(218, 0, void, Sector_SetWind, int, int, int, int);
ACS_SpecialDeclFull(219, 0, void, Sector_SetFriction, int, int);
ACS_SpecialDeclFull(220, 0, void, Sector_SetCurrent, int, int, int, int);
ACS_SpecialDeclFull(221, 0, void, Scroll_Texture_Both, int, int, int, int, int);

ACS_SpecialDeclFull(223, 0, void, Scroll_Floor, int, int, int, int);
ACS_SpecialDeclFull(224, 0, void, Scroll_Ceiling, int, int, int, int);

ACS_SpecialDeclRaw( 226, 3, void, ACS_ExecuteAlways, int, int, int, int, int);

ACS_SpecialDeclFull(228, 0, void, Plat_RaiseAndStarTx0, int, int);
ACS_SpecialDeclFull(229, 1, void, Thing_SetGoal, int, int, int, int);
ACS_SpecialDeclFull(230, 0, void, Plat_UpByValueStayTx, int, int, int);
ACS_SpecialDeclFull(231, 0, void, Plat_ToggleCeiling, int);
ACS_SpecialDeclFull(232, 0, void, Light_StrobeDoom, int, int, int);
ACS_SpecialDeclFull(233, 0, void, Light_MinNeighbor, int);
ACS_SpecialDeclFull(234, 0, void, Light_MaxNeighbor, int);
ACS_SpecialDeclFull(235, 0, void, Floor_TransferTrigger, int);
ACS_SpecialDeclFull(236, 0, void, Floor_TransferNumeric, int);
ACS_SpecialDeclFull(237, 0, void, ChangeCamera, int, int, int);
ACS_SpecialDeclFull(238, 0, void, Floor_RaiseToLowestCeiling, int, int);
ACS_SpecialDeclFull(239, 0, void, Floor_RaiseByValueTxTy, int, int, int);
ACS_SpecialDeclFull(240, 0, void, Floor_RaiseByTexture, int, int);
ACS_SpecialDeclFull(241, 0, void, Floor_LowerToLowestTxTy, int, int);
ACS_SpecialDeclFull(242, 1, void, Floor_LowerToHighest, int, int, int, int);
ACS_SpecialDeclFull(243, 0, void, Exit_Normal, int);
ACS_SpecialDeclFull(244, 0, void, Exit_Secret, int);
ACS_SpecialDeclFull(245, 0, void, Elevator_RaiseToNearest, int, int);
ACS_SpecialDeclFull(246, 0, void, Elevator_MoveToFloor, int, int);
ACS_SpecialDeclFull(247, 0, void, Elevator_LowerToNearest, int, int);
ACS_SpecialDeclFull(248, 1, void, HealThing, int, int);
ACS_SpecialDeclFull(249, 1, void, Door_CloseWaitOpen, int, int, int, int);
ACS_SpecialDeclFull(250, 0, void, Floor_Donut, int, int, int);
ACS_SpecialDeclFull(251, 0, void, FloorAndCeiling_LowerRaise, int, int, int);
ACS_SpecialDeclFull(252, 0, void, Ceiling_RaiseToNearest, int, int);
ACS_SpecialDeclFull(253, 0, void, Ceiling_LowerToLowest, int, int);
ACS_SpecialDeclFull(254, 0, void, Ceiling_LowerToFloor, int, int);
ACS_SpecialDeclFull(255, 1, void, Ceiling_CrushRaiseAndStaySilA, int, int, int, int, int);

#endif//__GDCC_Header__C__ACS_ZDoom__Special_h__

