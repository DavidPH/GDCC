//-----------------------------------------------------------------------------
//
// Copyright(C) 2014-2025 David Hill
//
// See COPYLIB for license information.
//
//-----------------------------------------------------------------------------
//
// Header for target library libGDCC.
//
// General declarations.
//
//-----------------------------------------------------------------------------

#ifndef __GDCC_Header__C__GDCC_h__
#define __GDCC_Header__C__GDCC_h__

#include <bits/types.h>


//----------------------------------------------------------------------------|
// Extern Functions                                                           |
//

#ifdef __cplusplus
extern "C" {
#endif

// Dynamic memory allocation. Use null for a new allocation and 0 to free.
[[call("StkCall")]]
extern void __sta *__GDCC__alloc(void __sta *_ptr, __size_t _size);

// As in __GDCC__alloc, but block is freed on detection of hub transition.
[[call("StkCall")]]
extern void __sta *__GDCC__alloc_auto(void __sta *_ptr, __size_t _size);

// Writes the state of the allocator.
[[call("StkCall")]]
extern void __GDCC__alloc_dump(void);

#ifdef __cplusplus
}
#endif

#endif//__GDCC_Header__C__GDCC_h__

