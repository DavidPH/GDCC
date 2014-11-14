//-----------------------------------------------------------------------------
//
// Copyright(C) 2014 David Hill
//
// See COPYLIB for license information.
//
//-----------------------------------------------------------------------------
//
// Number to string formatting functions.
//
//-----------------------------------------------------------------------------

#include <GDCC.h>


//----------------------------------------------------------------------------|
// Macros                                                                     |
//

#define BufferLen 8192

//
// FormatIntAltX
//
#define FormatIntAltX(c) \
   if(format.flags & __GDCC__FormatFlag_AltF) \
   { \
      *--ret.begin = (c); \
      *--ret.begin = '0'; \
      \
      if(!(format.flags & __GDCC__FormatFlag_Pad0) || \
         format.flags & __GDCC__FormatFlag_Left) \
      { \
         ret.len += 2; \
      } \
   }

//
// FormatIntCoreD
//
#define FormatIntCoreD() \
   /* Perform core conversion. */ \
   for(; data; data /= 10) \
      *--ret.begin = (int)(data % 10) + '0'; \
   \
   ret.len = ret.end - ret.begin;

//
// FormatIntCoreO
//
#define FormatIntCoreO() \
   /* Perform core conversion. */ \
   for(; data; data >>= 3) \
      *--ret.begin = ((int)data & 7) + '0'; \
   \
   if(format.flags & __GDCC__FormatFlag_AltF) \
   { \
      if(ret.begin == ret.end || *ret.begin != '0') \
         *--ret.begin = '0'; \
   } \
   \
   ret.len = ret.end - ret.begin;

//
// FormatIntCoreX
//
#define FormatIntCoreX(table) \
   /* Perform core conversion. */ \
   for(; data; data >>= 4) \
      *--ret.begin = table[(int)data & 0xF]; \
   \
   ret.len = ret.end - ret.begin; \
   \
   if(format.flags & __GDCC__FormatFlag_AltF && \
      format.flags & __GDCC__FormatFlag_Pad0 && \
      !(format.flags & __GDCC__FormatFlag_Left)) \
   { \
      ret.len += 2; \
   }

//
// FormatIntPreS
//
#define FormatIntPreS() \
   __GDCC__FormatRet ret; \
   ret.begin = ret.end = Buffer + BufferLen / 2; \
   \
   int sign; \
   if((sign = data < 0)) \
      data = -data;

//
// FormatIntPreU
//
#define FormatIntPreU() \
   __GDCC__FormatRet ret; \
   ret.begin = ret.end = Buffer + BufferLen / 2;

//
// FormatIntPrec
//
#define FormatIntPrec() \
   /* Pad to precision. */ \
   if(format.prec == -1) \
   { \
      if(!ret.len) \
         *--ret.begin = '0', ret.len = 1; \
      \
      /* When padding to width with 0, use width as precision. */ \
      if(format.flags & __GDCC__FormatFlag_Pad0 && \
         !(format.flags & __GDCC__FormatFlag_Left)) \
      { \
         while(ret.len < format.width) \
            *--ret.begin = '0', ++ret.len; \
      } \
   } \
   else \
   { \
      while(ret.len < format.prec) \
         *--ret.begin = '0', ++ret.len; \
   }

//
// FormatIntSign
//
#define FormatIntSign() \
   if(sign) \
      *--ret.begin = '-', ++ret.len; \
   else if(format.flags & __GDCC__FormatFlag_Sign) \
      *--ret.begin = '+', ++ret.len; \
   else if(format.flags & __GDCC__FormatFlag_PadS) \
      *--ret.begin = ' ', ++ret.len;

//
// FormatIntWidth
//
#define FormatIntWidth() \
   /* Pad to width. */ \
   if(format.flags & __GDCC__FormatFlag_Left) \
   { \
      while(ret.len < format.width) \
         *ret.end++ = ' ', ++ret.len; \
   } \
   else \
   { \
      while(ret.len < format.width) \
         *--ret.begin = ' ', ++ret.len; \
   }


//----------------------------------------------------------------------------|
// Types                                                                      |
//

typedef char __loc_ars BufferChar;
typedef BufferChar *BufferPtr;


//----------------------------------------------------------------------------|
// Static Variables                                                           |
//

[[no_init]]
static BufferChar Buffer[BufferLen];


//----------------------------------------------------------------------------|
// Global Functions                                                           |
//

//
// __GDCC__FormatI_d
//
__GDCC__FormatDecl(I, d, int)
{
   FormatIntPreS();
   FormatIntCoreD();
   FormatIntPrec();
   FormatIntSign();
   FormatIntWidth();

   return ret;
}

//
// __GDCC__FormatI_dl
//
__GDCC__FormatDecl(I, dl, long int)
{
   FormatIntPreS();
   FormatIntCoreD();
   FormatIntPrec();
   FormatIntSign();
   FormatIntWidth();

   return ret;
}

//
// __GDCC__FormatI_dll
//
__GDCC__FormatDecl(I, dll, long long int)
{
   FormatIntPreS();
   FormatIntCoreD();
   FormatIntPrec();
   FormatIntSign();
   FormatIntWidth();

   return ret;
}

//
// __GDCC__FormatU_d
//
__GDCC__FormatDecl(U, d, unsigned)
{
   FormatIntPreU();
   FormatIntCoreD();
   FormatIntPrec();
   FormatIntWidth();

   return ret;
}

//
// __GDCC__FormatU_dl
//
__GDCC__FormatDecl(U, dl, long unsigned)
{
   FormatIntPreU();
   FormatIntCoreD();
   FormatIntPrec();
   FormatIntWidth();

   return ret;
}

//
// __GDCC__FormatU_dll
//
__GDCC__FormatDecl(U, dll, long long unsigned)
{
   FormatIntPreU();
   FormatIntCoreD();
   FormatIntPrec();
   FormatIntWidth();

   return ret;
}

//
// __GDCC__FormatU_o
//
__GDCC__FormatDecl(U, o, unsigned)
{
   FormatIntPreU();
   FormatIntCoreO();
   FormatIntPrec();
   FormatIntWidth();

   return ret;
}

//
// __GDCC__FormatU_ol
//
__GDCC__FormatDecl(U, ol, long unsigned)
{
   FormatIntPreU();
   FormatIntCoreO();
   FormatIntPrec();
   FormatIntWidth();

   return ret;
}

//
// __GDCC__FormatU_oll
//
__GDCC__FormatDecl(U, oll, long long unsigned)
{
   FormatIntPreU();
   FormatIntCoreO();
   FormatIntPrec();
   FormatIntWidth();

   return ret;
}

//
// __GDCC__FormatU_X
//
__GDCC__FormatDecl(U, X, unsigned)
{
   FormatIntPreU();
   FormatIntCoreX("0123456789ABCDEF");
   FormatIntPrec();
   FormatIntAltX('X');
   FormatIntWidth();

   return ret;
}

//
// __GDCC__FormatU_Xl
//
__GDCC__FormatDecl(U, Xl, long unsigned)
{
   FormatIntPreU();
   FormatIntCoreX("0123456789ABCDEF");
   FormatIntPrec();
   FormatIntAltX('X');
   FormatIntWidth();

   return ret;
}

//
// __GDCC__FormatU_Xll
//
__GDCC__FormatDecl(U, Xll, long long unsigned)
{
   FormatIntPreU();
   FormatIntCoreX("0123456789ABCDEF");
   FormatIntPrec();
   FormatIntAltX('X');
   FormatIntWidth();

   return ret;
}

//
// __GDCC__FormatU_x
//
__GDCC__FormatDecl(U, x, unsigned)
{
   FormatIntPreU();
   FormatIntCoreX("0123456789abcdef");
   FormatIntPrec();
   FormatIntAltX('x');
   FormatIntWidth();

   return ret;
}

//
// __GDCC__FormatU_xl
//
__GDCC__FormatDecl(U, xl, long unsigned)
{
   FormatIntPreU();
   FormatIntCoreX("0123456789abcdef");
   FormatIntPrec();
   FormatIntAltX('x');
   FormatIntWidth();

   return ret;
}

//
// __GDCC__FormatU_xll
//
__GDCC__FormatDecl(U, xll, long long unsigned)
{
   FormatIntPreU();
   FormatIntCoreX("0123456789abcdef");
   FormatIntPrec();
   FormatIntAltX('x');
   FormatIntWidth();

   return ret;
}

// EOF

