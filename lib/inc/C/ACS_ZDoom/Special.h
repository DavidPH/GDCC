//-----------------------------------------------------------------------------
//
// Copyright(C) 2014-2022 David Hill
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

ACS_SpecialDeclFull( 14, 0, int, Door_Animated, (int, int, int));
ACS_SpecialDeclFull( 15, 0, int, Autosave, (void));

ACS_SpecialDeclFull( 17, 1, int, Thing_Raise, (int, int));
ACS_SpecialDeclFull( 18, 1, int, StartConversation, (int, int));
ACS_SpecialDeclFull( 19, 0, int, Thing_Stop, (int));

ACS_SpecialDeclFull( 33, 0, int, ForceField, (void));
ACS_SpecialDeclFull( 34, 0, int, ClearForceField, (int));

ACS_SpecialDeclFull( 37, 1, int, Floor_MoveToValue, (int, int, int, int));
ACS_SpecialDeclFull( 38, 0, int, Ceiling_Waggle, (int, int, int, int, int));
ACS_SpecialDeclFull( 39, 0, int, Teleport_ZombieChanger, (int, int));

ACS_SpecialDeclFull( 47, 1, int, Ceiling_MoveToValue, (int, int, int, int));

ACS_SpecialDeclFull( 49, 2, int, GlassBreak, (int, int));

ACS_SpecialDeclFull( 51, 0, int, Sector_SetLink, (int, int, int, int));
ACS_SpecialDeclFull( 52, 0, int, Scroll_Wall, (int, int, int, int, int));
ACS_SpecialDeclFull( 53, 0, int, Line_SetTextureOffset, (int, int, int, int, int));
ACS_SpecialDeclFull( 54, 0, int, Sector_ChangeFlags, (int, int, int));
ACS_SpecialDeclFull( 55, 0, int, Line_SetBlocking, (int, int, int));
ACS_SpecialDeclFull( 56, 0, int, Line_SetTextureScale, (int, int, int, int, int));

ACS_SpecialDeclFull( 57, 0, int, Polyobj_OR_MoveToSpot, (int, int, int));

ACS_SpecialDeclFull( 75, 0, int, Teleport_EndGame, (void));
ACS_SpecialDeclFull( 76, 0, int, TeleportOther, (int, int, int));
ACS_SpecialDeclFull( 77, 0, int, TeleportGroup, (int, int, int, int, int));
ACS_SpecialDeclFull( 78, 1, int, TeleportInSector, (int, int, int, int, int));
ACS_SpecialDeclFull( 79, 0, int, Thing_SetConversation, (int, int));

ACS_SpecialDeclRaw(  84, 4, int, ACS_ExecuteWithResult, (int, int, int, int, int));
ACS_SpecialDeclRaw(  85, 0, int, ACS_LockedExecuteDoor, (int, int, int, int, int));
ACS_SpecialDeclFull( 86, 0, int, Polyobj_MoveToSpot, (int, int, int));
ACS_SpecialDeclFull( 87, 0, int, Polyobj_Stop, (int));
ACS_SpecialDeclFull( 88, 0, int, Polyobj_MoveTo, (int, int, int, int));
ACS_SpecialDeclFull( 89, 0, int, Polyobj_OR_MoveTo, (int, int, int, int));

ACS_SpecialDeclFull( 97, 2, int, Ceiling_LowerAndCrushDist, (int, int, int, int, int));
ACS_SpecialDeclFull( 98, 1, int, Sector_SetTranslucent, (int, int, int, int));

ACS_SpecialDeclFull(104, 1, int, Ceiling_CrushAndRaiseSilentDist, (int, int, int, int, int));

ACS_SpecialDeclFull(107, 0, int, Line_SetPortalTarget, (void));

ACS_SpecialDeclFull(117, 0, int, Light_Stop, (int));

ACS_SpecialDeclFull(119, 1, int, Thing_Damage, (int, int, int));

ACS_SpecialDeclFull(125, 1, int, Thing_Move, (int, int, int));

ACS_SpecialDeclFull(127, 3, int, Thing_SetSpecial, (int, int, int, int, int));
ACS_SpecialDeclFull(128, 0, int, ThrustThingZ, (int, int, int, int));

ACS_SpecialDeclFull(139, 2, int, Thing_SpawnFacing, (int, int, int, int));

ACS_SpecialDeclFull(134, 0, int, Player_RemoveItem, (int, int));
ACS_SpecialDeclFull(135, 0, int, Player_GiveItem, (int, int));

ACS_SpecialDeclFull(150, 0, int, Line_SetHealth, (int, int));
ACS_SpecialDeclFull(151, 0, int, Sector_SetHealth, (int, int, int));

ACS_SpecialDeclFull(154, 1, int, Teleport_NoStop, (int, int, int));

ACS_SpecialDeclFull(157, 0, int, SetGlobalFogParameter, (int, int));
ACS_SpecialDeclFull(158, 3, int, FS_Execute, (int, int, int, int));
ACS_SpecialDeclFull(159, 0, int, Sector_SetPlaneReflection, (int, int, int));

ACS_SpecialDeclFull(168, 2, int, Ceiling_CrushAndRaiseDist, (int, int, int, int, int));
ACS_SpecialDeclFull(169, 0, int, Generic_Crusher2, (int, int, int, int, int));
ACS_SpecialDeclFull(170, 0, int, Sector_SetCeilingScale2, (int, int, int));
ACS_SpecialDeclFull(171, 0, int, SectorSetFloorScale2, (int, int, int));
ACS_SpecialDeclFull(172, 0, int, Plat_UpNearestWaitDownStay, (int, int, int));
ACS_SpecialDeclFull(173, 0, int, NoiseAlert, (int, int));
ACS_SpecialDeclFull(174, 0, int, SendToCommunicator, (int, int, int, int));
ACS_SpecialDeclFull(175, 0, int, Thing_ProjectileIntercept, (int, int, int, int, int));
ACS_SpecialDeclFull(176, 0, int, Thing_ChangeTID, (int, int));
ACS_SpecialDeclFull(177, 1, int, Thing_Hate, (int, int, int));
ACS_SpecialDeclFull(178, 1, int, Thing_ProjectileAimed, (int, int, int, int, int));
ACS_SpecialDeclFull(179, 0, int, ChangeSkill, (int));
ACS_SpecialDeclFull(180, 0, int, Thing_SetTranslation, (int, int));

ACS_SpecialDeclFull(183, 0, int, Line_AlignCeiling, (int, int));
ACS_SpecialDeclFull(184, 0, int, Line_AlignFloor, (int, int));
ACS_SpecialDeclFull(185, 0, int, Sector_SetRotation, (int, int, int));
ACS_SpecialDeclFull(186, 0, int, Sector_SetCeilingPanning, (int, int, int, int, int));
ACS_SpecialDeclFull(187, 0, int, Sector_SetFloorPanning, (int, int, int, int, int));
ACS_SpecialDeclFull(188, 0, int, Sector_SetCeilingScale, (int, int, int, int, int));
ACS_SpecialDeclFull(189, 0, int, Sector_SetFloorScale, (int, int, int, int, int));

ACS_SpecialDeclFull(191, 0, int, SetPlayerProperty, (int, int, int));
ACS_SpecialDeclFull(192, 0, int, Ceiling_LowerToHighestFloor, (int, int));
ACS_SpecialDeclFull(193, 0, int, Ceiling_LowerInstant, (int, int, int));
ACS_SpecialDeclFull(194, 0, int, Ceiling_RaiseInstant, (int, int, int));
ACS_SpecialDeclFull(195, 1, int, Ceiling_CrushRaiseAndStayA, (int, int, int, int, int));
ACS_SpecialDeclFull(196, 1, int, Ceiling_CrushAndRaiseA, (int, int, int, int, int));
ACS_SpecialDeclFull(197, 1, int, Ceiling_CrushAndRaiseSilentA, (int, int, int, int, int));
ACS_SpecialDeclFull(198, 0, int, Ceiling_RaiseByValueTimes8, (int, int, int));
ACS_SpecialDeclFull(199, 0, int, Ceiling_LowerByValueTimes8, (int, int, int));
ACS_SpecialDeclFull(200, 0, int, Generic_Floor, (int, int, int, int, int));
ACS_SpecialDeclFull(201, 0, int, Generic_Ceiling, (int, int, int, int, int));
ACS_SpecialDeclFull(202, 0, int, Generic_Door, (int, int, int, int, int));
ACS_SpecialDeclFull(203, 0, int, Generic_Lift, (int, int, int, int, int));
ACS_SpecialDeclFull(204, 0, int, Generic_Stairs, (int, int, int, int, int));
ACS_SpecialDeclFull(205, 0, int, Generic_Crusher, (int, int, int, int, int));
ACS_SpecialDeclFull(206, 1, int, Plat_DownWaitUpStayLip, (int, int, int, int, int));
ACS_SpecialDeclFull(207, 0, int, Plat_PerpetualRaiseLip, (int, int, int, int));
ACS_SpecialDeclFull(208, 1, int, TranslucentLine, (int, int, int));

ACS_SpecialDeclFull(212, 1, int, Sector_SetColor, (int, int, int, int, int));
ACS_SpecialDeclFull(213, 0, int, Sector_SetFade, (int, int, int, int));
ACS_SpecialDeclFull(214, 0, int, Sector_SetDamage, (int, int, int));

ACS_SpecialDeclFull(216, 0, int, Sector_SetGravity, (int, int, int));
ACS_SpecialDeclFull(217, 0, int, Stairs_BuildUpDoom, (int, int, int, int, int));
ACS_SpecialDeclFull(218, 0, int, Sector_SetWind, (int, int, int, int));
ACS_SpecialDeclFull(219, 0, int, Sector_SetFriction, (int, int));
ACS_SpecialDeclFull(220, 0, int, Sector_SetCurrent, (int, int, int, int));
ACS_SpecialDeclFull(221, 0, int, Scroll_Texture_Both, (int, int, int, int, int));

ACS_SpecialDeclFull(223, 0, int, Scroll_Floor, (int, int, int, int));
ACS_SpecialDeclFull(224, 0, int, Scroll_Ceiling, (int, int, int, int));

ACS_SpecialDeclRaw( 226, 3, int, ACS_ExecuteAlways, (int, int, int, int, int));

ACS_SpecialDeclFull(228, 0, int, Plat_RaiseAndStarTx0, (int, int));
ACS_SpecialDeclFull(229, 1, int, Thing_SetGoal, (int, int, int, int));
ACS_SpecialDeclFull(230, 0, int, Plat_UpByValueStayTx, (int, int, int));
ACS_SpecialDeclFull(231, 0, int, Plat_ToggleCeiling, (int));
ACS_SpecialDeclFull(232, 0, int, Light_StrobeDoom, (int, int, int));
ACS_SpecialDeclFull(233, 0, int, Light_MinNeighbor, (int));
ACS_SpecialDeclFull(234, 0, int, Light_MaxNeighbor, (int));
ACS_SpecialDeclFull(235, 0, int, Floor_TransferTrigger, (int));
ACS_SpecialDeclFull(236, 0, int, Floor_TransferNumeric, (int));
ACS_SpecialDeclFull(237, 0, int, ChangeCamera, (int, int, int));
ACS_SpecialDeclFull(238, 0, int, Floor_RaiseToLowestCeiling, (int, int));
ACS_SpecialDeclFull(239, 0, int, Floor_RaiseByValueTxTy, (int, int, int));
ACS_SpecialDeclFull(240, 0, int, Floor_RaiseByTexture, (int, int));
ACS_SpecialDeclFull(241, 0, int, Floor_LowerToLowestTxTy, (int, int));
ACS_SpecialDeclFull(242, 1, int, Floor_LowerToHighest, (int, int, int, int));
ACS_SpecialDeclFull(243, 0, int, Exit_Normal, (int));
ACS_SpecialDeclFull(244, 0, int, Exit_Secret, (int));
ACS_SpecialDeclFull(245, 0, int, Elevator_RaiseToNearest, (int, int));
ACS_SpecialDeclFull(246, 0, int, Elevator_MoveToFloor, (int, int));
ACS_SpecialDeclFull(247, 0, int, Elevator_LowerToNearest, (int, int));
ACS_SpecialDeclFull(248, 1, int, HealThing, (int, int));
ACS_SpecialDeclFull(249, 1, int, Door_CloseWaitOpen, (int, int, int, int));
ACS_SpecialDeclFull(250, 0, int, Floor_Donut, (int, int, int));
ACS_SpecialDeclFull(251, 0, int, FloorAndCeiling_LowerRaise, (int, int, int));
ACS_SpecialDeclFull(252, 0, int, Ceiling_RaiseToNearest, (int, int));
ACS_SpecialDeclFull(253, 0, int, Ceiling_LowerToLowest, (int, int));
ACS_SpecialDeclFull(254, 0, int, Ceiling_LowerToFloor, (int, int));
ACS_SpecialDeclFull(255, 1, int, Ceiling_CrushRaiseAndStaySilA, (int, int, int, int, int));
ACS_SpecialDeclFull(256, 1, int, Floor_LowerToHighestEE, (int, int, int));
ACS_SpecialDeclFull(257, 1, int, Floor_RaiseToLowest, (int, int, int));
ACS_SpecialDeclFull(258, 1, int, Floor_LowerToLowestCeiling, (int, int, int));
ACS_SpecialDeclFull(259, 3, int, Floor_RaiseToCeiling, (int, int, int, int, int));
ACS_SpecialDeclFull(260, 3, int, Floor_ToCeilingInstant, (int, int, int, int));
ACS_SpecialDeclFull(261, 1, int, Floor_LowerByTexture, (int, int, int));
ACS_SpecialDeclFull(262, 1, int, Ceiling_RaiseToHighest, (int, int, int));
ACS_SpecialDeclFull(263, 2, int, Ceiling_ToHighestInstant, (int, int, int));
ACS_SpecialDeclFull(264, 2, int, Ceiling_LowerToNearest, (int, int, int, int));
ACS_SpecialDeclFull(265, 1, int, Ceiling_RaiseToLowest, (int, int, int));
ACS_SpecialDeclFull(266, 1, int, Ceiling_RaiseToHighestFloor, (int, int, int));
ACS_SpecialDeclFull(267, 3, int, Ceiling_ToFloorInstant, (int, int, int, int));
ACS_SpecialDeclFull(268, 1, int, Ceiling_RaiseByTexture, (int, int, int));
ACS_SpecialDeclFull(269, 2, int, Ceiling_LowerByTexture, (int, int, int, int));
ACS_SpecialDeclFull(270, 0, int, Stairs_BuildDownDoom, (int, int, int, int, int));
ACS_SpecialDeclFull(271, 0, int, Stairs_BuildUpDoomSync, (int, int, int, int));
ACS_SpecialDeclFull(272, 0, int, Stairs_BuildDownDoomSync, (int, int, int, int));
ACS_SpecialDeclFull(273, 0, int, Stairs_BuildUpDoomCrush, (int, int, int, int, int));
ACS_SpecialDeclFull(274, 0, int, Door_AnimatedClose, (int, int));
ACS_SpecialDeclFull(275, 0, int, Floor_Stop, (int));
ACS_SpecialDeclFull(276, 0, int, Ceiling_Stop, (int));
ACS_SpecialDeclFull(277, 0, int, Sector_SetFloorGlow, (int, int, int, int, int));
ACS_SpecialDeclFull(278, 0, int, Sector_SetCeilingGlow, (int, int, int, int, int));
ACS_SpecialDeclFull(279, 1, int, Floor_MoveToValueAndCrush, (int, int, int, int, int));
ACS_SpecialDeclFull(280, 1, int, Ceiling_MoveToValueAndCrush, (int, int, int, int, int));
ACS_SpecialDeclFull(281, 0, int, Line_SetAutomapFlags, (int, int, int));
ACS_SpecialDeclFull(282, 0, int, Line_SetAutomapStyle, (int, int));
ACS_SpecialDeclFull(283, 0, int, Polyobj_StopSound, (int));

#endif//__GDCC_Header__C__ACS_ZDoom__Special_h__

