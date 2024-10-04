//-----------------------------------------------------------------------------
//
// Header for target library libacs.
//
// Defines from K8Vavoom's ACS.
//
//-----------------------------------------------------------------------------

#ifndef __GDCC_Header__C__ACS_K8Vavoom__defs_h__
#define __GDCC_Header__C__ACS_K8Vavoom__defs_h__

#include <ACS_Zandronum/defs.h>


//----------------------------------------------------------------------------|
// Macros                                                                     |
//

#define POBJ_FLAG_CRUSH            0x00000001
#define POBJ_FLAG_HURT_ON_TOUCH    0x00000002
#define POBJ_FLAG_NO_CARRY_THINGS  0x00000004
#define POBJ_FLAG_NO_ANGLE_CHANGE  0x00000008
#define POBJ_FLAG_SIDE_CRUSH       0x00000010

#define POBJ_FLAGS_CLEAR    0
#define POBJ_FLAGS_SET      1
#define POBJ_FLAGS_REPLACE -1

#define POBJ_MOVE_NORMAL    0x00000000
#define POBJ_MOVE_OVERRIDE  0x00000001
#define POBJ_MOVE_NOLINK    0x00000002
#define POBJ_MOVE_INDROT    0x00000004
#define POBJ_MOVE_POANGLE   0x00000008
#define POBJ_MOVE_MIRRORED  0x00000010
#define POBJ_MOVE_PERPETUAL 0x00000020

#endif//__GDCC_Header__C__ACS_K8Vavoom__defs_h__

