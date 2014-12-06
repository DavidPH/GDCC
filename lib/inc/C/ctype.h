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
// Character handling.
//
//-----------------------------------------------------------------------------

#ifndef __GDCC_Header__C__ctype_h__
#define __GDCC_Header__C__ctype_h__


//----------------------------------------------------------------------------|
// Global Functions                                                           |
//

#ifdef __cplusplus
extern "C" {
#endif

//
// Character classification functions.
//

int isalnum(int c);
int isalpha(int c);
int isblank(int c);
int iscntrl(int c);
int isdigit(int c);
int isgraph(int c);
int islower(int c);
int isprint(int c);
int ispunct(int c);
int isspace(int c);
int isupper(int c);
int isxdigit(int c);

//
// Character case mapping functions.
//

int tolower(int c);
int toupper(int c);

#ifdef __cplusplus
};
#endif

#endif//__GDCC_Header__C__ctype_h__

