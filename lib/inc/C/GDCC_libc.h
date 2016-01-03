//-----------------------------------------------------------------------------
//
// Copyright(C) 2015-2016 David Hill
//
// See COPYLIB for license information.
//
//-----------------------------------------------------------------------------
//
// Header for target library libc.
//
// This header declares functions, objects, and types which are specific and
// possibly internal to GDCC. Neither the contents or existence of this file
// should be relied upon by external projects.
//
//-----------------------------------------------------------------------------

#ifndef __GDCC_Header__C__GDCC_libc_h__
#define __GDCC_Header__C__GDCC_libc_h__

#include <GDCC.h>


#ifdef __cplusplus
extern "C" {
#endif

//----------------------------------------------------------------------------|
// Macros                                                                     |
//

//
// __GDCC__FormatBufLen
//
#define __GDCC__FormatBufLen 8192

//
// __GDCC__ForamtBufMid
//
#define __GDCC__FormatBufMid (__GDCC__FormatBuf + __GDCC__FormatBufLen / 2)

//
// __GDCC__FormatDecl
//
#define __GDCC__FormatDecl(group, fmt, T) \
   [[call("StkCall")]] \
   extern __GDCC__FormatRet (__GDCC__Format##group##_##fmt)( \
      T _data, __GDCC__FormatArg _format)

//
// __GDCC__FormatDefn
//
#define __GDCC__FormatDefn(group, fmt, T) \
   [[call("StkCall")]] \
   __GDCC__FormatRet (__GDCC__Format##group##_##fmt)( \
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
// __GDCC__FormatGenAccum
//
#define __GDCC__FormatGenAccum(data, format, group, fmt) \
   (_Generic(data, \
      _Accum:                __GDCC__Format##group##_##fmt, \
      unsigned _Accum:       __GDCC__Format##group##_##fmt, \
      short _Accum:          __GDCC__Format##group##_##fmt##h, \
      short unsigned _Accum: __GDCC__Format##group##_##fmt##h, \
      long _Accum:           __GDCC__Format##group##_##fmt##l, \
      long unsigned _Accum:  __GDCC__Format##group##_##fmt##l) \
   ((data), (format)))

//
// __GDCC__FormatGenFloat
//
#define __GDCC__FormatGenFloat(data, format, group, fmt) \
   (_Generic(data, \
      float:       __GDCC__Format##group##_##fmt, \
      double:      __GDCC__Format##group##_##fmt##l, \
      long double: __GDCC__Format##group##_##fmt##ll) \
   ((data), (format)))

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
// __GDCC__FormatF_*_g
//
#define __GDCC__FormatF_D_g(data, format) \
   __GDCC__FormatGenFloat((data), (format), F, D)
#define __GDCC__FormatF_E_g(data, format) \
   __GDCC__FormatGenFloat((data), (format), F, E)
#define __GDCC__FormatF_G_g(data, format) \
   __GDCC__FormatGenFloat((data), (format), F, G)
#define __GDCC__FormatF_X_g(data, format) \
   __GDCC__FormatGenFloat((data), (format), F, X)
#define __GDCC__FormatF_d_g(data, format) \
   __GDCC__FormatGenFloat((data), (format), F, d)
#define __GDCC__FormatF_e_g(data, format) \
   __GDCC__FormatGenFloat((data), (format), F, e)
#define __GDCC__FormatF_g_g(data, format) \
   __GDCC__FormatGenFloat((data), (format), F, g)
#define __GDCC__FormatF_x_g(data, format) \
   __GDCC__FormatGenFloat((data), (format), F, x)

//
// __GDCC__FormatI_*_g
//
#define __GDCC__FormatI_d_g(data, format) \
   __GDCC__FormatGenInteg((data), (format), I, d)

//
// __GDCC__FormatK_*_g
//
#define __GDCC__FormatK_d_g(data, format) \
   __GDCC__FormatGenAccum((data), (format), K, d)

//
// __GDCC__FormatU_*_g
//
#define __GDCC__FormatU_X_g(data, format) \
   __GDCC__FormatGenInteg((data), (format), U, X)
#define __GDCC__FormatU_b_g(data, format) \
   __GDCC__FormatGenInteg((data), (format), U, b)
#define __GDCC__FormatU_d_g(data, format) \
   __GDCC__FormatGenInteg((data), (format), U, d)
#define __GDCC__FormatU_o_g(data, format) \
   __GDCC__FormatGenInteg((data), (format), U, o)
#define __GDCC__FormatU_x_g(data, format) \
   __GDCC__FormatGenInteg((data), (format), U, x)

//
// __GDCC__FormatX_*_g
//
#define __GDCC__FormatX_d_g(data, format) \
   __GDCC__FormatGenAccum((data), (format), X, d)


//----------------------------------------------------------------------------|
// Types                                                                      |
//

//
// __GDCC__FormatArg
//
typedef struct __GDCC__FormatArg
{
   unsigned int _flags;
   unsigned int _prec;
   unsigned int _width;
} __GDCC__FormatArg;

//
// __GDCC__FormatRet
//
typedef struct __GDCC__FormatRet
{
   char __sta  *_begin;
   char __sta  *_end;
   unsigned int _len;
} __GDCC__FormatRet;


//----------------------------------------------------------------------------|
// Extern Objects                                                             |
//

extern char __GDCC__FormatBuf[__GDCC__FormatBufLen];


//----------------------------------------------------------------------------|
// Extern Functions                                                           |
//

__GDCC__FormatDecl(F, D, float);
__GDCC__FormatDecl(F, Dl, double);
__GDCC__FormatDecl(F, Dll, long double);

__GDCC__FormatDecl(F, E, float);
__GDCC__FormatDecl(F, El, double);
__GDCC__FormatDecl(F, Ell, long double);

__GDCC__FormatDecl(F, G, float);
__GDCC__FormatDecl(F, Gl, double);
__GDCC__FormatDecl(F, Gll, long double);

__GDCC__FormatDecl(F, X, float);
__GDCC__FormatDecl(F, Xl, double);
__GDCC__FormatDecl(F, Xll, long double);

__GDCC__FormatDecl(F, d, float);
__GDCC__FormatDecl(F, dl, double);
__GDCC__FormatDecl(F, dll, long double);

__GDCC__FormatDecl(F, e, float);
__GDCC__FormatDecl(F, el, double);
__GDCC__FormatDecl(F, ell, long double);

__GDCC__FormatDecl(F, g, float);
__GDCC__FormatDecl(F, gl, double);
__GDCC__FormatDecl(F, gll, long double);

__GDCC__FormatDecl(F, x, float);
__GDCC__FormatDecl(F, xl, double);
__GDCC__FormatDecl(F, xll, long double);

__GDCC__FormatDecl(I, d, int);
__GDCC__FormatDecl(I, dl, long int);
__GDCC__FormatDecl(I, dll, long long int);

__GDCC__FormatDecl(K, d, unsigned _Accum);
__GDCC__FormatDecl(K, dh, short unsigned _Accum);
__GDCC__FormatDecl(K, dl, long unsigned _Accum);

__GDCC__FormatDecl(U, X, unsigned);
__GDCC__FormatDecl(U, Xl, long unsigned);
__GDCC__FormatDecl(U, Xll, long long unsigned);

__GDCC__FormatDecl(U, b, unsigned);
__GDCC__FormatDecl(U, bl, long unsigned);
__GDCC__FormatDecl(U, bll, long long unsigned);

__GDCC__FormatDecl(U, d, unsigned);
__GDCC__FormatDecl(U, dl, long unsigned);
__GDCC__FormatDecl(U, dll, long long unsigned);

__GDCC__FormatDecl(U, o, unsigned);
__GDCC__FormatDecl(U, ol, long unsigned);
__GDCC__FormatDecl(U, oll, long long unsigned);

__GDCC__FormatDecl(U, x, unsigned);
__GDCC__FormatDecl(U, xl, long unsigned);
__GDCC__FormatDecl(U, xll, long long unsigned);

__GDCC__FormatDecl(X, d, _Accum);
__GDCC__FormatDecl(X, dh, short _Accum);
__GDCC__FormatDecl(X, dl, long _Accum);

char __sta *__GDCC__GetFormatBuf(void);

#ifdef __cplusplus
}
#endif

#endif//__GDCC_Header__C__GDCC_libc_h__

