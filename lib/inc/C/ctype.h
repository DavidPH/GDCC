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
// Character handling.
//
//-----------------------------------------------------------------------------

#ifndef __GDCC_Header__C__ctype_h__
#define __GDCC_Header__C__ctype_h__

#include <bits/features.h>


//----------------------------------------------------------------------------|
// Global Functions                                                           |
//

#ifdef __cplusplus
extern "C" {
#endif

//
// Character classification functions.
//

int isalnum(int _c);
int isalpha(int _c);
int isblank(int _c);
int iscntrl(int _c);
int isdigit(int _c);
int isgraph(int _c);
int islower(int _c);
int isprint(int _c);
int ispunct(int _c);
int isspace(int _c);
int isupper(int _c);
int isxdigit(int _c);

//
// Character case mapping functions.
//

int tolower(int _c);
int toupper(int _c);

#ifdef __cplusplus
};
#endif

#endif//__GDCC_Header__C__ctype_h__

