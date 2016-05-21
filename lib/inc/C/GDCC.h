//-----------------------------------------------------------------------------
//
// Copyright(C) 2014-2015 David Hill
//
// See COPYLIB for license information.
//
//-----------------------------------------------------------------------------
//
// Header for target library libGDCC.
//
// This header declares functions, objects, and types which are specific and
// possibly internal to GDCC. Neither the contents or existence of this file
// should be relied upon by external projects.
//
//-----------------------------------------------------------------------------

#ifndef __GDCC_Header__C__GDCC_h__
#define __GDCC_Header__C__GDCC_h__

#include <bits/types.h>


//----------------------------------------------------------------------------|
// Macros                                                                     |
//

//
// __GDCC__Sta
//
#define __GDCC__Sta __glyph(int, "___GDCC__Sta")


//----------------------------------------------------------------------------|
// Extern Functions                                                           |
//

#ifdef __cplusplus
extern "C" {
#endif

[[call("StkCall")]]
extern void __sta *__GDCC__alloc(void __sta *_ptr, __size_t _size);

[[call("StkCall")]]
extern void __GDCC__alloc_dump(void);

#ifdef __cplusplus
}
#endif

#endif//__GDCC_Header__C__GDCC_h__

