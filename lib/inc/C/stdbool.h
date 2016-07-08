//-----------------------------------------------------------------------------
//
// Copyright(C) 2014-2016 David Hill
//
// See COPYLIB for license information.
//
//-----------------------------------------------------------------------------
//
// Header for target library libc.
//
// Boolean type and values.
//
//-----------------------------------------------------------------------------

#ifndef __GDCC_Header__C__stdbool_h__
#define __GDCC_Header__C__stdbool_h__

#include <bits/features.h>


//----------------------------------------------------------------------------|
// Macros                                                                     |
//

//
// bool
//
#ifndef __cplusplus
#define bool _Bool
#endif

//
// true
//
#ifndef __cplusplus
#define true 1
#endif

//
// false
//
#ifndef __cplusplus
#define false 0
#endif

//
// __bool_true_false_are_defined
//
#define __bool_true_false_are_defined 1

#endif//__GDCC_Header__C__stdbool_h__

