//-----------------------------------------------------------------------------
//
// Copyright(C) 2014-2019 David Hill
//
// See COPYLIB for license information.
//
//-----------------------------------------------------------------------------
//
// Header for target library libacs.
//
// "AsmFunc" functions from Hexen's ACS.
//
//-----------------------------------------------------------------------------

#ifndef __GDCC_Header__C__ACS_Hexen__AsmFunc_h__
#define __GDCC_Header__C__ACS_Hexen__AsmFunc_h__


//----------------------------------------------------------------------------|
// Macros                                                                     |
//

//
// ACS_AsmFuncDecl
//
#if __GDCC__
#define ACS_AsmFuncDecl(addr, alit, oarg, ...) \
   [[__address(addr), __address_Lit(alit), __call("AsmFunc"), \
      __optional_args(oarg), __VA_ARGS__]]
#else
#define ACS_AsmFuncDecl(addr, alit, oarg, ...)
#endif

//
// ACS_AsmFuncDeclFull
//
#ifndef ACS_SHORT_NAMES
#define ACS_AsmFuncDeclFull(addr, alit, oarg, retn, name, args, ...) \
   ACS_AsmFuncDeclPre(addr, alit, oarg, retn, name, args, __VA_ARGS__)
#else
#define ACS_AsmFuncDeclFull(addr, alit, oarg, retn, name, args, ...) \
   ACS_AsmFuncDeclPre(addr, alit, oarg, retn, name, args, __VA_ARGS__); \
   ACS_AsmFuncDeclRaw(addr, alit, oarg, retn, name, args, __VA_ARGS__)
#endif

//
// ACS_AsmFuncDeclPre
//
#define ACS_AsmFuncDeclPre(addr, alit, oarg, retn, name, args, ...) \
   ACS_AsmFuncDecl(addr, alit, oarg, __VA_ARGS__) extern retn (ACS_##name) args

//
// ACS_AsmFuncDeclRaw
//
#define ACS_AsmFuncDeclRaw(addr, alit, oarg, retn, name, args, ...) \
   ACS_AsmFuncDecl(addr, alit, oarg, __VA_ARGS__) extern retn (name) args


//----------------------------------------------------------------------------|
// Extern Functions                                                           |
//

ACS_AsmFuncDeclFull( 55, 56, 0, void,   Delay, (int), delay);
ACS_AsmFuncDeclFull( 57, 58, 0, int,    Random, (int, int));
ACS_AsmFuncDeclFull( 57, 58, 0, _Accum, RandomFixed, (_Accum, _Accum));
ACS_AsmFuncDeclFull( 59, 60, 0, int,    ThingCount, (int, int));
ACS_AsmFuncDeclFull( 61, 62, 0, void,   TagWait, (int), delay);
ACS_AsmFuncDeclFull( 63, 64, 0, void,   PolyWait, (int), delay);
ACS_AsmFuncDeclFull( 65, 66, 0, void,   ChangeFloor, (int, __str));
ACS_AsmFuncDeclFull( 67, 68, 0, void,   ChangeCeiling, (int, __str));

ACS_AsmFuncDeclFull( 80,   , 0, int,    LineSide, (void));
ACS_AsmFuncDeclFull( 81, 82, 0, void,   ScriptWait, (int), delay);
ACS_AsmFuncDeclFull( 83,   , 0, void,   ClearLineSpecial, (void));

ACS_AsmFuncDeclFull( 85,   , 0, void,   BeginPrint, (void));
ACS_AsmFuncDeclFull( 85,   , 0, void,   BeginPrintBold, (void));
ACS_AsmFuncDeclFull( 86,   , 0, void,   EndPrint, (void));
ACS_AsmFuncDeclFull( 87,   , 0, void,   PrintString, (__str));
ACS_AsmFuncDeclFull( 88,   , 0, void,   PrintInt, (int));
ACS_AsmFuncDeclFull( 88,   , 0, void,   PrintNumber, (int));
ACS_AsmFuncDeclFull( 89,   , 0, void,   PrintChar, (char));
ACS_AsmFuncDeclFull( 90,   , 0, int,    PlayerCount, (void));
ACS_AsmFuncDeclFull( 91,   , 0, int,    GameType, (void));
ACS_AsmFuncDeclFull( 92,   , 0, int,    GameSkill, (void));
ACS_AsmFuncDeclFull( 93,   , 0, int,    Timer, (void));
ACS_AsmFuncDeclFull( 94,   , 0, void,   SectorSound, (__str, int));
ACS_AsmFuncDeclFull( 95,   , 0, void,   AmbientSound, (__str, int));
ACS_AsmFuncDeclFull( 96,   , 0, void,   SoundSequence, (__str));
ACS_AsmFuncDeclFull( 97,   , 0, void,   SetLineTexture, (int, int, int, __str));
ACS_AsmFuncDeclFull( 98,   , 0, void,   SetLineBlocking, (int, int));
ACS_AsmFuncDeclFull( 99,   , 5, void,   SetLineSpecial, (int, int, int, int, int, int, int));
ACS_AsmFuncDeclFull(100,   , 0, void,   ThingSound, (int, __str, int));
ACS_AsmFuncDeclFull(101,   , 0, void,   EndPrintBold, (void));

#endif//__GDCC_Header__C__ACS_Hexen__AsmFunc_h__

