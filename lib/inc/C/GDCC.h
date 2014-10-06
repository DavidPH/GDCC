//-----------------------------------------------------------------------------
//
// Copyright(C) 2014 David Hill
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

#ifndef __GDCC_Header__C_GDCC_h__
#define __GDCC_Header__C_GDCC_h__


//----------------------------------------------------------------------------|
// Types                                                                      |
//

#ifndef __GDCC_Have__size_t__
#define __GDCC_Have__size_t__
typedef unsigned size_t;
#endif

#ifndef __GDCC_Have__ssize_t__
#define __GDCC_Have__ssize_t__
typedef int ssize_t;
#endif


//----------------------------------------------------------------------------|
// Global Functions                                                           |
//

[[call("StkCall")]]
extern void __loc_ars *__GDCC__alloc(void __loc_ars *ptr, size_t size);

[[call("StkCall")]]
extern void __GDCC__alloc_dump(void);

#endif//__GDCC_Header__C_GDCC_h__

