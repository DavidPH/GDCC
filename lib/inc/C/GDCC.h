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
// Macros                                                                     |
//

//
// __GDCC__FormatDecl
//
#define __GDCC__FormatDecl(group, fmt, T) \
   [[call("StkCall")]] \
   extern __GDCC__FormatRet (__GDCC__Format##group##_##fmt)( \
      T data, __GDCC__FormatArg format)

//
// __GDCC__FormatFlag_*
//
#define __GDCC__FormatFlag_Left 0x0001
#define __GDCC__FormatFlag_Sign 0x0002
#define __GDCC__FormatFlag_PadS 0x0004
#define __GDCC__FormatFlag_AltF 0x0008
#define __GDCC__FormatFlag_Pad0 0x0010

//
// __GDCC__FormatGenInteg
//
#define __GDCC__FormatGenInteg(data, format, group, fmt) \
   (_Generic(data, \
      int:                __GDCC__Format##group##_##fmt, \
      unsigned:           __GDCC__Format##group##_##fmt, \
      long int:           __GDCC__Format##group##_##fmt##l, \
      long unsigned:      __GDCC__Format##group##_##fmt##l, \
      long long int:      __GDCC__Format##group##_##fmt##ll, \
      long long unsigned: __GDCC__Format##group##_##fmt##ll) \
   ((data), (format)))

//
// __GDCC__FormatI_*_g
//
#define __GDCC__FormatI_d_g(data, format) \
   __GDCC__FormatGenInteg((data), (format), I, d)

//
// __GDCC__FormatU_*_g
//
#define __GDCC__FormatU_d_g(data, format) \
   __GDCC__FormatGenInteg((data), (format), U, d)
#define __GDCC__FormatU_o_g(data, format) \
   __GDCC__FormatGenInteg((data), (format), U, o)
#define __GDCC__FormatU_X_g(data, format) \
   __GDCC__FormatGenInteg((data), (format), U, X)
#define __GDCC__FormatU_x_g(data, format) \
   __GDCC__FormatGenInteg((data), (format), U, x)


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

//
// __GDCC__FormatArg
//
typedef struct __GDCC__FormatArg
{
   unsigned int flags;
   unsigned int prec;
   unsigned int width;
} __GDCC__FormatArg;

//
// __GDCC__FormatRet
//
typedef struct __GDCC__FormatRet
{
   char __loc_ars *begin;
   char __loc_ars *end;
   unsigned int    len;
} __GDCC__FormatRet;


//----------------------------------------------------------------------------|
// Global Functions                                                           |
//

[[call("StkCall")]]
extern void __loc_ars *__GDCC__alloc(void __loc_ars *ptr, size_t size);

[[call("StkCall")]]
extern void __GDCC__alloc_dump(void);

__GDCC__FormatDecl(I, d, int);
__GDCC__FormatDecl(I, dl, long int);
__GDCC__FormatDecl(I, dll, long long int);

__GDCC__FormatDecl(U, d, unsigned);
__GDCC__FormatDecl(U, dl, long unsigned);
__GDCC__FormatDecl(U, dll, long long unsigned);

__GDCC__FormatDecl(U, o, unsigned);
__GDCC__FormatDecl(U, ol, long unsigned);
__GDCC__FormatDecl(U, oll, long long unsigned);

__GDCC__FormatDecl(U, X, unsigned);
__GDCC__FormatDecl(U, Xl, long unsigned);
__GDCC__FormatDecl(U, Xll, long long unsigned);
__GDCC__FormatDecl(U, x, unsigned);
__GDCC__FormatDecl(U, xl, long unsigned);
__GDCC__FormatDecl(U, xll, long long unsigned);

#endif//__GDCC_Header__C_GDCC_h__

