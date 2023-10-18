//-----------------------------------------------------------------------------
//
// Copyright(C) 2014-2023 David Hill
//
// See COPYLIB for license information.
//
//-----------------------------------------------------------------------------
//
// Header for target library libacs.
//
// "Native" functions from ZDoom's ACS.
//
//-----------------------------------------------------------------------------

#ifndef __GDCC_Header__C__ACS_ZDoom__Native_h__
#define __GDCC_Header__C__ACS_ZDoom__Native_h__


//----------------------------------------------------------------------------|
// Macros                                                                     |
//

//
// ACS_NativeDecl
//
#if __GDCC__
#define ACS_NativeDecl(addr, oarg, ...) \
   [[__address(addr), __call("Native"), __optional_args(oarg)]] [[__VA_ARGS__]]
#else
#define ACS_NativeDecl(addr, oarg, ...)
#endif

//
// ACS_NativeDeclFull
//
#ifndef ACS_SHORT_NAMES
#define ACS_NativeDeclFull(addr, oarg, retn, name, args, ...) \
   ACS_NativeDeclPre(addr, oarg, retn, name, args, __VA_ARGS__)
#else
#define ACS_NativeDeclFull(addr, oarg, retn, name, args, ...) \
   ACS_NativeDeclPre(addr, oarg, retn, name, args, __VA_ARGS__); \
   ACS_NativeDeclRaw(addr, oarg, retn, name, args, __VA_ARGS__)
#endif

//
// ACS_NativeDeclPre
//
#define ACS_NativeDeclPre(addr, oarg, retn, name, args, ...) \
   ACS_NativeDecl(addr, oarg, __VA_ARGS__) extern retn (ACS_##name) args

//
// ACS_NativeDeclRaw
//
#define ACS_NativeDeclRaw(addr, oarg, retn, name, args, ...) \
   ACS_NativeDecl(addr, oarg, __VA_ARGS__) extern retn (name) args


//----------------------------------------------------------------------------|
// Extern Functions                                                           |
//

ACS_NativeDeclFull(  1, 0, int,    GetLineUDMFInt, (int, __str));
ACS_NativeDeclFull(  2, 0, _Accum, GetLineUDMFFixed, (int, __str));
ACS_NativeDeclFull(  3, 0, int,    GetThingUDMFInt, (int, __str));
ACS_NativeDeclFull(  4, 0, _Accum, GetThingUDMFFixed, (int, __str));
ACS_NativeDeclFull(  5, 0, int,    GetSectorUDMFInt, (int, __str));
ACS_NativeDeclFull(  6, 0, _Accum, GetSectorUDMFFixed, (int, __str));
ACS_NativeDeclFull(  7, 0, int,    GetSideUDMFInt, (int, int, __str));
ACS_NativeDeclFull(  8, 0, _Accum, GetSideUDMFFixed, (int, int, __str));
ACS_NativeDeclFull(  9, 0, _Accum, GetActorVelX, (int));
ACS_NativeDeclFull( 10, 0, _Accum, GetActorVelY, (int));
ACS_NativeDeclFull( 11, 0, _Accum, GetActorVelZ, (int));
ACS_NativeDeclFull( 12, 1, int,    SetActivator, (int, int));
ACS_NativeDeclFull( 13, 0, int,    SetActivatorToTarget, (int));
ACS_NativeDeclFull( 14, 0, _Accum, GetActorViewHeight, (int));
ACS_NativeDeclFull( 15, 0, int,    GetChar, (__str, int));
ACS_NativeDeclFull( 16, 0, int,    GetAirSupply, (int));
ACS_NativeDeclFull( 17, 0, int,    SetAirSupply, (int, int));
ACS_NativeDeclFull( 18, 0, int,    SetSkyScrollSpeed, (int, _Accum));
ACS_NativeDeclFull( 19, 0, int,    GetArmorType, (__str, int));
ACS_NativeDeclFull( 20, 2, int,    SpawnSpotForced, (__str, int, int, int));
ACS_NativeDeclFull( 21, 1, int,    SpawnSpotFacingForced, (__str, int, int));
ACS_NativeDeclFull( 22, 0, int,    CheckActorProperty, (int, int, int));
ACS_NativeDeclFull( 22, 0, int,    CheckActorPropertyFixed, (int, int, _Accum));
ACS_NativeDeclFull( 22, 0, int,    CheckActorPropertyString, (int, int, __str));
ACS_NativeDeclFull( 23, 0, int,    SetActorVelocity, (int, _Accum, _Accum, _Accum, int, int));
ACS_NativeDeclFull( 24, 0, int,    SetUserVariable, (int, __str, int));
ACS_NativeDeclFull( 24, 0, int,    SetUserVariableFixed, (int, __str, _Accum));
ACS_NativeDeclFull( 25, 0, int,    GetUserVariable, (int, __str));
ACS_NativeDeclFull( 25, 0, _Accum, GetUserVariableFixed, (int, __str));
ACS_NativeDeclFull( 25, 0, __str,  GetUserVariableString, (int, __str));
ACS_NativeDeclFull( 26, 0, int,    Radius_Quake2, (int, int, int, int, int, __str));
ACS_NativeDeclFull( 27, 0, int,    CheckActorClass, (int, __str));
ACS_NativeDeclFull( 28, 0, int,    SetUserArray, (int, __str, int, int));
ACS_NativeDeclFull( 28, 0, int,    SetUserArrayFixed, (int, __str, int, _Accum));
ACS_NativeDeclFull( 29, 0, int,    GetUserArray, (int, __str, int));
ACS_NativeDeclFull( 29, 0, _Accum, GetUserArrayFixed, (int, __str, int));
ACS_NativeDeclFull( 29, 0, __str,  GetUserArrayString, (int, __str, int));
ACS_NativeDeclFull( 30, 0, int,    SoundSequenceOnActor, (int, __str));
ACS_NativeDeclFull( 31, 0, int,    SoundSequenceOnSector, (int, __str, int));
ACS_NativeDeclFull( 32, 0, int,    SoundSequenceOnPolyobj, (int, __str));
ACS_NativeDeclFull( 33, 0, _Accum, GetPolyobjX, (int));
ACS_NativeDeclFull( 34, 0, _Accum, GetPolyobjY, (int));
ACS_NativeDeclFull( 35, 0, int,    CheckSight, (int, int, int));
ACS_NativeDeclFull( 36, 2, int,    SpawnForced, (__str, _Accum, _Accum, _Accum, int, int));

ACS_NativeDeclFull( 38, 2, int,    SetPointer, (int, int, int, int));
ACS_NativeDeclRaw(  39, 3, int,    ACS_NamedExecute, (__str, int, int, int, int));
ACS_NativeDeclRaw(  40, 0, int,    ACS_NamedSuspend, (__str, int));
ACS_NativeDeclRaw(  41, 0, int,    ACS_NamedTerminate, (__str, int));
ACS_NativeDeclRaw(  42, 0, int,    ACS_NamedLockedExecute, (__str, int, int, int, int));
ACS_NativeDeclRaw(  43, 0, int,    ACS_NamedLockedExecuteDoor, (__str, int, int, int, int));
ACS_NativeDeclRaw(  44, 4, int,    ACS_NamedExecuteWithResult, (__str, int, int, int, int));
ACS_NativeDeclRaw(  45, 3, int,    ACS_NamedExecuteAlways, (__str, int, int, int, int));
ACS_NativeDeclFull( 46, 2, int,    UniqueTID, (int, int));
ACS_NativeDeclFull( 47, 0, int,    IsTIDUsed, (int));
ACS_NativeDeclFull( 48, 0, int,    Sqrt, (int));
ACS_NativeDeclFull( 49, 0, _Accum, FixedSqrt, (_Accum));
ACS_NativeDeclFull( 50, 0, _Accum, VectorLength, (_Accum, _Accum));
ACS_NativeDeclFull( 51, 2, int,    SetHudClipRect, (int, int, int, int, int, int));
ACS_NativeDeclFull( 52, 0, int,    SetHudWrapWidth, (int));
ACS_NativeDeclFull( 53, 0, int,    SetCVar, (__str, int));
ACS_NativeDeclFull( 53, 0, int,    SetCVarFixed, (__str, _Accum));
ACS_NativeDeclFull( 54, 0, int,    GetUserCVar, (int, __str));
ACS_NativeDeclFull( 54, 0, _Accum, GetUserCVarFixed, (int, __str));
ACS_NativeDeclFull( 55, 0, int,    SetUserCVar, (int, __str, int));
ACS_NativeDeclFull( 55, 0, int,    SetUserCVarFixed, (int, __str, _Accum));
ACS_NativeDeclFull( 56, 0, __str,  GetCVarString, (__str));
ACS_NativeDeclFull( 57, 0, int,    SetCVarString, (__str, __str));
ACS_NativeDeclFull( 58, 0, __str,  GetUserCVarString, (int, __str));
ACS_NativeDeclFull( 59, 0, int,    SetUserCVarString, (int, __str, __str));
ACS_NativeDeclFull( 60, 5, int,    LineAttack, (int, _Accum, _Accum, int, __str, __str, _Accum, int, int));
ACS_NativeDeclFull( 61, 5, int,    PlaySound, (int, __str, int, _Accum, int, _Accum, int));
ACS_NativeDeclFull( 62, 1, int,    StopSound, (int, int));
ACS_NativeDeclFull( 63, 1, int,    StrCmp, (__str, __str, int));
ACS_NativeDeclFull( 64, 1, int,    StrCaseCmp, (__str, __str, int));
ACS_NativeDeclFull( 64, 1, int,    StrICmp, (__str, __str, int));
ACS_NativeDeclFull( 65, 0, __str,  StrLeft, (__str, int));
ACS_NativeDeclFull( 66, 0, __str,  StrRight, (__str, int));
ACS_NativeDeclFull( 67, 0, __str,  StrMid, (__str, int, int));
ACS_NativeDeclFull( 68, 0, __str,  GetActorClass, (int));
ACS_NativeDeclFull( 69, 0, __str,  GetWeapon, (void));
ACS_NativeDeclFull( 70, 0, int,    SoundVolume, (int, int, _Accum));
ACS_NativeDeclFull( 71, 4, int,    PlayActorSound, (int, int, int, _Accum, int, _Accum));
ACS_NativeDeclFull( 72, 4, int,    SpawnDecal, (int, __str, int, _Accum, _Accum, _Accum));
ACS_NativeDeclFull( 73, 0, int,    CheckFont, (__str));
ACS_NativeDeclFull( 74, 0, int,    DropItem, (int, __str, int, int));
ACS_NativeDeclFull( 75, 0, int,    CheckFlag, (int, __str));
ACS_NativeDeclFull( 76, 1, int,    SetLineActivation, (int, int, int));
ACS_NativeDeclFull( 77, 0, int,    GetLineActivation, (int));
ACS_NativeDeclFull( 78, 0, int,    GetActorPowerupTics, (int, __str));
ACS_NativeDeclFull( 79, 1, int,    ChangeActorAngle, (int, _Accum, int));
ACS_NativeDeclFull( 80, 1, int,    ChangeActorPitch, (int, _Accum, int));
ACS_NativeDeclFull( 81, 0, int,    GetArmorInfo, (int));
ACS_NativeDeclFull( 81, 0, _Accum, GetArmorInfoFixed, (int));
ACS_NativeDeclFull( 81, 0, __str,  GetArmorInfoString, (int));
ACS_NativeDeclFull( 82, 0, int,    DropInventory, (int, __str));
ACS_NativeDeclFull( 83, 3, int,    PickActor, (int, _Accum, _Accum, _Accum, int, int, int, int));
ACS_NativeDeclFull( 84, 2, int,    IsPointerEqual, (int, int, int, int));
ACS_NativeDeclFull( 85, 0, int,    CanRaiseActor, (int));
ACS_NativeDeclFull( 86, 0, int,    SetActorTeleFog, (int, __str, __str));
ACS_NativeDeclFull( 87, 0, int,    SwapActorTeleFog, (int));
ACS_NativeDeclFull( 88, 0, int,    SetActorRoll, (int, _Accum));
ACS_NativeDeclFull( 89, 0, int,    ChangeActorRoll, (int, _Accum, int));
ACS_NativeDeclFull( 90, 0, _Accum, GetActorRoll, (int));
ACS_NativeDeclFull( 91, 4, int,    QuakeEx, (int, int, int, int, int, int, int, __str, int, _Accum, _Accum, _Accum));
ACS_NativeDeclFull( 92, 5, int,    Warp, (int, _Accum, _Accum, _Accum, _Accum, int, __str, int, _Accum, _Accum, _Accum));
ACS_NativeDeclFull( 93, 0, int,    GetMaxInventory, (int, __str));
ACS_NativeDeclFull( 94, 3, int,    SetSectorDamage, (int, int, __str, int, int));
ACS_NativeDeclFull( 95, 0, int,    SetSectorTerrain, (int, int, __str));
ACS_NativeDeclFull( 96,14, int,    SpawnParticle, (int, int, int, int, _Accum, _Accum, _Accum, _Accum, _Accum, _Accum, _Accum, _Accum, _Accum, int, int));
ACS_NativeDeclFull( 97, 0, int,    SetMusicVolume, (_Accum));
ACS_NativeDeclFull( 98, 3, int,    CheckProximity, (int, __str, _Accum, int, int, int));
ACS_NativeDeclFull( 99, 1, int,    CheckActorState, (int, __str, int));

ACS_NativeDeclFull(159, 2, int,    LumpOpen, (__str, int, int));
ACS_NativeDeclFull(160, 1, int,    LumpRead, (int, int, int));
ACS_NativeDeclFull(160, 1, _Accum, LumpReadFixed, (int, int, int));
ACS_NativeDeclFull(161, 1, __str,  LumpReadString, (int, int, int));
ACS_NativeDeclFull(162, 2, int,    LumpReadLocalCharArray, (int, int, int, int, int));
ACS_NativeDeclFull(163, 2, int,    LumpReadMapCharArray, (int, int, int, int, int));
ACS_NativeDeclFull(164, 2, int,    LumpReadWorldCharArray, (int, int, int, int, int));
ACS_NativeDeclFull(165, 2, int,    LumpReadGlobalCharArray, (int, int, int, int, int));
ACS_NativeDeclFull(166, 0, int,    LumpGetInfo, (int, int));
ACS_NativeDeclFull(166, 0, __str,  LumpGetInfoString, (int, int));
ACS_NativeDeclFull(167, 0, int,    LumpClose, (int));

ACS_NativeDeclFull(200, 2, int,    CheckClass, (__str, int, int));
ACS_NativeDeclFull(201, 0, int,    DamageActor, (int, int, int, int, int, __str));
ACS_NativeDeclFull(202, 0, int,    SetActorFlag, (int, __str, int));
ACS_NativeDeclFull(203, 0, int,    SetTranslation, (int, __str));
ACS_NativeDeclFull(204, 0, __str,  GetActorFloorTexture, (int));
ACS_NativeDeclFull(205, 0, __str,  GetActorFloorTerrain, (int));
ACS_NativeDeclFull(206, 0, int,    StrArg, (__str));
ACS_NativeDeclFull(207, 0, _Accum, Floor, (_Accum));
ACS_NativeDeclFull(208, 0, _Accum, Round, (_Accum));
ACS_NativeDeclFull(209, 0, _Accum, Ceil, (_Accum));
ACS_NativeDeclFull(210, 0, int,    ScriptCall, (__str, __str, ...));
ACS_NativeDeclFull(210, 0, _Accum, ScriptCallFixed, (__str, __str, ...));
ACS_NativeDeclFull(210, 0, __str,  ScriptCallString, (__str, __str, ...));
ACS_NativeDeclFull(211, 0, int,    StartSlideshow, (__str));
ACS_NativeDeclFull(212, 0, int,    GetSectorHealth, (int, int));
ACS_NativeDeclFull(213, 0, int,    GetLineHealth, (int));
ACS_NativeDeclFull(214, 0, int,    SetSubtitleNumber, (int, __str));

ACS_NativeDeclFull(300, 0, _Accum, GetLineX, (int, _Accum, _Accum));
ACS_NativeDeclFull(301, 0, _Accum, GetLineY, (int, _Accum, _Accum));
ACS_NativeDeclFull(302, 0, int,    SetAirFriction, (_Accum));

ACS_NativeDeclFull(400, 0, int,    SetSectorGlow, (int, int, int, int, int, int));
ACS_NativeDeclFull(401, 0, int,    SetFogDensity, (int, int));

#endif//__GDCC_Header__C__ACS_ZDoom__Native_h__

