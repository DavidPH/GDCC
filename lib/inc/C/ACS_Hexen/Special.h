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
// "Special" functions from Hexen's ACS.
//
//-----------------------------------------------------------------------------

#ifndef __GDCC_Header__C__ACS_Hexen__Special_h__
#define __GDCC_Header__C__ACS_Hexen__Special_h__


//----------------------------------------------------------------------------|
// Macros                                                                     |
//

//
// ACS_SpecialDecl
//
#define ACS_SpecialDecl(addr, optArgs) \
   [[__address(addr), __call("Special"), __optional_args(optArgs)]]

//
// ACS_SpecialDeclFull
//
#ifndef ACS_SHORT_NAMES
#define ACS_SpecialDeclFull(addr, optArgs, Tret, name, ...) \
   ACS_SpecialDeclPre(addr, optArgs, Tret, name, __VA_ARGS__)
#else
#define ACS_SpecialDeclFull(addr, optArgs, Tret, name, ...) \
   ACS_SpecialDeclPre(addr, optArgs, Tret, name, __VA_ARGS__); \
   ACS_SpecialDeclRaw(addr, optArgs, Tret, name, __VA_ARGS__)
#endif

//
// ACS_SpecialDeclPre
//
#define ACS_SpecialDeclPre(addr, optArgs, Tret, name, ...) \
   ACS_SpecialDecl(addr, optArgs) extern Tret (ACS_##name)(__VA_ARGS__)

//
// ACS_SpecialDeclRaw
//
#define ACS_SpecialDeclRaw(addr, optArgs, Tret, name, ...) \
   ACS_SpecialDecl(addr, optArgs) extern Tret (name)(__VA_ARGS__)


//----------------------------------------------------------------------------|
// Global Functions                                                           |
//

ACS_SpecialDeclFull(  2, 0, void, Polyobj_RotateLeft, int, int, int);
ACS_SpecialDeclFull(  3, 0, void, Polyobj_RotateRight, int, int, int);
ACS_SpecialDeclFull(  4, 0, void, Polyobj_Move, int, int, int, int);

ACS_SpecialDeclFull(  6, 0, void, Polyobj_MoveTimes8, int, int, int, int);
ACS_SpecialDeclFull(  7, 0, void, Polyobj_DoorSwing, int, int, int, int);
ACS_SpecialDeclFull(  8, 0, void, Polyobj_DoorSlide, int, int, int, int, int);

ACS_SpecialDeclFull( 10, 1, void, Door_Close, int, int, int);
ACS_SpecialDeclFull( 11, 1, void, Door_Open, int, int, int);
ACS_SpecialDeclFull( 12, 1, void, Door_Raise, int, int, int, int);
ACS_SpecialDeclFull( 13, 1, void, Door_LockedRaise, int, int, int, int, int);

ACS_SpecialDeclFull( 20, 0, void, Floor_LowerByValue, int, int, int);
ACS_SpecialDeclFull( 21, 0, void, Floor_LowerToLowest, int, int);
ACS_SpecialDeclFull( 22, 0, void, Floor_LowerToNearest, int, int);
ACS_SpecialDeclFull( 23, 0, void, Floor_RaiseByValue, int, int, int);
ACS_SpecialDeclFull( 24, 0, void, Floor_RaiseToHighest, int, int);
ACS_SpecialDeclFull( 25, 0, void, Floor_RaiseToNearest, int, int);
ACS_SpecialDeclFull( 26, 0, void, Stairs_BuildDown, int, int, int, int, int);
ACS_SpecialDeclFull( 27, 0, void, Stairs_BuildUp, int, int, int, int, int);
ACS_SpecialDeclFull( 28, 1, void, Floor_RaiseAndCrush, int, int, int, int);
ACS_SpecialDeclFull( 29, 0, void, Pillar_Build, int, int, int);
ACS_SpecialDeclFull( 30, 0, void, Pillar_Open, int, int, int, int);
ACS_SpecialDeclFull( 31, 0, void, Stairs_BuildDownSync, int, int, int, int);
ACS_SpecialDeclFull( 32, 0, void, Stairs_BuildUpSync, int, int, int, int);

ACS_SpecialDeclFull( 35, 0, void, Floor_RaiseByValueTimes8, int, int, int);
ACS_SpecialDeclFull( 36, 0, void, Floor_LowerByValueTimes8, int, int, int);

ACS_SpecialDeclFull( 40, 0, void, Ceiling_LowerByValue, int, int, int);
ACS_SpecialDeclFull( 41, 0, void, Ceiling_RaiseByValue, int, int, int);
ACS_SpecialDeclFull( 42, 1, void, Ceiling_CrushAndRaise, int, int, int, int);
ACS_SpecialDeclFull( 43, 1, void, Ceiling_LowerAndCrush, int, int, int, int);
ACS_SpecialDeclFull( 44, 0, void, Ceiling_CrushStop, int);
ACS_SpecialDeclFull( 45, 1, void, Ceiling_CrushRaiseAndStay, int, int, int, int);
ACS_SpecialDeclFull( 46, 0, void, Floor_CrushStop, int);

ACS_SpecialDeclFull( 60, 0, void, Plat_PerpetualRaise, int, int, int);
ACS_SpecialDeclFull( 61, 0, void, Plat_Stop, int);
ACS_SpecialDeclFull( 62, 0, void, Plat_DownWaitUpStay, int, int, int);
ACS_SpecialDeclFull( 63, 0, void, Plat_DownByValue, int, int, int, int);
ACS_SpecialDeclFull( 64, 0, void, Plat_UpWaitDownStay, int, int, int);
ACS_SpecialDeclFull( 65, 0, void, Plat_UpByValue, int, int, int, int);
ACS_SpecialDeclFull( 66, 0, void, Floor_LowerInstant, int, int, int);
ACS_SpecialDeclFull( 67, 0, void, Floor_RaiseInstant, int, int, int);
ACS_SpecialDeclFull( 68, 0, void, Floor_MoveToValueTimes8, int, int, int, int);
ACS_SpecialDeclFull( 69, 0, void, Ceiling_MoveToValueTimes8, int, int, int, int);
ACS_SpecialDeclFull( 70, 2, void, Teleport, int, int, int);
ACS_SpecialDeclFull( 71, 2, void, Teleport_NoFog, int, int, int);
ACS_SpecialDeclFull( 72, 2, void, ThrustThing, int, int, int, int);
ACS_SpecialDeclFull( 73, 1, void, DamageThing, int, int);
ACS_SpecialDeclFull( 74, 1, void, Teleport_NewMap, int, int, int);

ACS_SpecialDeclRaw(  80, 4, void, ACS_Execute, int, int, int, int, int);
ACS_SpecialDeclRaw(  81, 0, void, ACS_Suspend, int, int);
ACS_SpecialDeclRaw(  82, 0, void, ACS_Terminate, int, int);
ACS_SpecialDeclRaw(  83, 0, void, ACS_LockedExecute, int, int, int, int, int);

ACS_SpecialDeclFull( 90, 0, void, Polyobj_OR_RotateLeft, int, int, int);
ACS_SpecialDeclFull( 91, 0, void, Polyobj_OR_RotateRight, int, int, int);
ACS_SpecialDeclFull( 92, 0, void, Polyobj_OR_Move, int, int, int, int);
ACS_SpecialDeclFull( 93, 0, void, Polyobj_OR_MoveTimes8, int, int, int, int);
ACS_SpecialDeclFull( 94, 0, void, Pillar_BuildAndCrush, int, int, int, int);
ACS_SpecialDeclFull( 95, 0, void, FloorAndCeiling_LowerByValue, int, int, int);
ACS_SpecialDeclFull( 96, 0, void, FloorAndCeiling_RaiseByValue, int, int, int);

ACS_SpecialDeclFull(109, 0, void, Light_ForceLightning, int);
ACS_SpecialDeclFull(110, 0, void, Light_RaiseByValue, int, int);
ACS_SpecialDeclFull(111, 0, void, Light_LowerByValue, int, int);
ACS_SpecialDeclFull(112, 0, void, Light_ChangeToValue, int, int);
ACS_SpecialDeclFull(113, 0, void, Light_Fade, int, int, int);
ACS_SpecialDeclFull(114, 0, void, Light_Glow, int, int, int, int);
ACS_SpecialDeclFull(115, 0, void, Light_Flicker, int, int, int);
ACS_SpecialDeclFull(116, 0, void, Light_Strobe, int, int, int, int, int);

ACS_SpecialDeclFull(120, 0, void, Radius_Quake, int, int, int, int, int);

ACS_SpecialDeclFull(130, 0, void, Thing_Activate, int);
ACS_SpecialDeclFull(131, 0, void, Thing_Deactivate, int);
ACS_SpecialDeclFull(132, 0, void, Thing_Remove, int);
ACS_SpecialDeclFull(133, 2, void, Thing_Destroy, int, int, int);
ACS_SpecialDeclFull(134, 0, void, Thing_Projectile, int, int, int, int, int);
ACS_SpecialDeclFull(135, 1, void, Thing_Spawn, int, int, int, int);
ACS_SpecialDeclFull(136, 0, void, Thing_ProjectileGravity, int, int, int, int, int);
ACS_SpecialDeclFull(137, 1, void, Thing_SpawnNoFog, int, int, int, int);
ACS_SpecialDeclFull(138, 0, void, Floor_Waggle, int, int, int, int, int);

ACS_SpecialDeclFull(140, 0, void, Sector_ChangeSound, int, int);

#endif//__GDCC_Header__C__ACS_Hexen__Special_h__

