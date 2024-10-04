//-----------------------------------------------------------------------------
//
// Header for target library libacs.
//
// "Native" functions from K8Vavoom's ACS.
//
//-----------------------------------------------------------------------------

#ifndef __GDCC_Header__C__ACS_K8Vavoom__Native_h__
#define __GDCC_Header__C__ACS_K8Vavoom__Native_h__

#include <ACS_Zandronum/Native.h>


//----------------------------------------------------------------------------|
// Extern Functions                                                           |
//

ACS_NativeDeclFull(800, 0, int,    Polyobj_MoveEx, (int, int, int, int, int, int, int));
ACS_NativeDeclFull(801, 0, int,    Polyobj_MoveToEx, (int, int, int, int, int, int));
ACS_NativeDeclFull(802, 0, int,    Polyobj_MoveToSpotEx, (int, int, int, int));
ACS_NativeDeclFull(803, 0, _Accum, GetPolyobjZ, (int));
ACS_NativeDeclFull(804, 0, int,    Polyobj_GetFlagsEx, (int));
ACS_NativeDeclFull(805, 0, int,    Polyobj_SetFlagsEx, (int, int, int));
ACS_NativeDeclFull(806, 0, int,    Polyobj_IsBusy, (int));
ACS_NativeDeclFull(807, 0, _Accum, Polyobj_GetAngle, (int));
ACS_NativeDeclFull(808, 0, int,    Polyobj_MoveRotateEx, (int, int, int, int, int, int, _Accum, int));
ACS_NativeDeclFull(809, 0, int,    Polyobj_MoveToRotateEx, (int, int, int, int, int, _Accum, int));
ACS_NativeDeclFull(810, 0, int,    Polyobj_MoveToSpotRotateEx, (int, int, int, _Accum, int));
ACS_NativeDeclFull(811, 0, int,    Polyobj_RotateEx, (int, int, _Accum, int));

ACS_NativeDeclFull(860, 0, int,    CalcActorLight, (int, int, int));
ACS_NativeDeclFull(861, 0, int,    CalcPlayerLight, (int));

#endif//__GDCC_Header__C__ACS_K8Vavoom__Native_h__

