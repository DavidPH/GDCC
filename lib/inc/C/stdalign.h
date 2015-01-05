//-----------------------------------------------------------------------------
//
// Copyright(C) 2014 David Hill
//
// See COPYLIB for license information.
//
//-----------------------------------------------------------------------------
//
// Header for target library libc.
//
// Alignment.
//
//-----------------------------------------------------------------------------

#ifndef __GDCC_Header__C__stdalign_h__
#define __GDCC_Header__C__stdalign_h__


//----------------------------------------------------------------------------|
// Macros                                                                     |
//

//
// alignas
//
#ifndef __cplusplus
#define alignas _Alignas
#endif

//
// alignof
//
#ifndef __cplusplus
#define alignof _Alignof
#endif

//
// __alignas_is_defined
//
#define __alignas_is_defined 1

//
// __alignof_is_defined
//
#define __alignof_is_defined 1

#endif//__GDCC_Header__C__stdalign_h__

