//-----------------------------------------------------------------------------
//
// Copyright(C) 2014-2015 David Hill
//
// See COPYLIB for license information.
//
//-----------------------------------------------------------------------------
//
// Input/output.
//
// Formatted output functions.
//
//-----------------------------------------------------------------------------

#include <stdio.h>

#include <GDCC.h>
#include <stdarg.h>

#if __GDCC_Target__ZDoom__
#include <ACS_ZDoom.h>
#endif


//----------------------------------------------------------------------------|
// Macros                                                                     |
//

//
// FormatC
//
#define FormatC() \
   switch(fmtLen) \
   { \
   case FL_: \
      fmtBuf[0]    = va_arg(arg, int); \
      fmtRet.begin = fmtBuf; \
      fmtRet.end   = fmtBuf + 1; \
      fmtRet.len   = 1; \
      break; \
      \
   default: return ~ret; \
   }

//
// FormatCases
//
#define FormatCases() \
   case 'A': FormatF(__GDCC__FormatF_X_g); break; \
   case 'B': FormatU(__GDCC__FormatU_b_g); break; \
   case 'E': return ~ret; /* TODO */ \
   case 'F': return ~ret; /* TODO */ \
   case 'G': return ~ret; /* TODO */ \
   case 'K': FormatK(__GDCC__FormatK_d_g); break; \
   case 'R': return ~ret; /* TODO */ \
   case 'S': FormatStr();                  break; \
   case 'X': FormatU(__GDCC__FormatU_X_g); break; \
   case 'a': FormatF(__GDCC__FormatF_x_g); break; \
   case 'c': FormatC();                    break; \
   case 'd': FormatI(__GDCC__FormatI_d_g); break; \
   case 'e': return ~ret; /* TODO */ \
   case 'f': return ~ret; /* TODO */ \
   case 'g': return ~ret; /* TODO */ \
   case 'i': FormatI(__GDCC__FormatI_d_g); break; \
   case 'k': FormatX(__GDCC__FormatX_d_g); break; \
   case 'n': FormatN();                    break; \
   case 'o': FormatU(__GDCC__FormatU_o_g); break; \
   case 'p': FormatP();                    break; \
   case 'r': return ~ret; /* TODO */ \
   case 's': FormatS();                    break; \
   case 'u': FormatU(__GDCC__FormatU_d_g); break; \
   case 'x': FormatU(__GDCC__FormatU_x_g); break;

//
// FormatDecl
//
#define FormatDecl() \
   __GDCC__FormatArg fmtArg; \
   __GDCC__FormatRet fmtRet; \
   FormatLen         fmtLen; \
   \
   int ret = 0; \
   \
   __str fmtStr; \
   char fmtBuf[1], fmtChr;

//
// FormatF
//
#define FormatF(fmt) \
   switch(fmtLen) \
   { \
   case FL_l: \
   case FL_:  fmtRet = fmt(va_arg(arg, double),        fmtArg); break; \
   case FL_L: fmtRet = fmt(va_arg(arg, long double),   fmtArg); break; \
   case FL_h: fmtRet = fmt((float)va_arg(arg, double), fmtArg); break; \
   default: return ~ret; \
   }

//
// FormatI
//
#define FormatI(fmt) \
   switch(fmtLen) \
   { \
   case FL_:   fmtRet = fmt(va_arg(arg, int),           fmtArg); break; \
   case FL_h:  fmtRet = fmt(va_arg(arg, int),           fmtArg); break; \
   case FL_hh: fmtRet = fmt(va_arg(arg, int),           fmtArg); break; \
   case FL_j:  fmtRet = fmt(va_arg(arg, intmax_t),      fmtArg); break; \
   case FL_l:  fmtRet = fmt(va_arg(arg, long int),      fmtArg); break; \
   case FL_ll: fmtRet = fmt(va_arg(arg, long long int), fmtArg); break; \
   case FL_t:  fmtRet = fmt(va_arg(arg, ptrdiff_t),     fmtArg); break; \
   case FL_z:  fmtRet = fmt(va_arg(arg, ssize_t),       fmtArg); break; \
   default: return ~ret; \
   }

//
// FormatK
//
#define FormatK(fmt) \
   switch(fmtLen) \
   { \
   case FL_:  fmtRet = fmt(va_arg(arg, unsigned _Accum),       fmtArg); break; \
   case FL_h: fmtRet = fmt(va_arg(arg, short unsigned _Accum), fmtArg); break; \
   case FL_l: fmtRet = fmt(va_arg(arg, long unsigned _Accum),  fmtArg); break; \
   default: return ~ret; \
   }

//
// FormatN
//
#define FormatN() \
   switch(fmtLen) \
   { \
   case FL_:   *va_arg(arg, int *)           = ret; continue; \
   case FL_h:  *va_arg(arg, short int *)     = ret; continue; \
   case FL_hh: *va_arg(arg, signed char *)   = ret; continue; \
   case FL_j:  *va_arg(arg, intmax_t *)      = ret; continue; \
   case FL_l:  *va_arg(arg, long int *)      = ret; continue; \
   case FL_ll: *va_arg(arg, long long int *) = ret; continue; \
   case FL_t:  *va_arg(arg, ptrdiff_t *)     = ret; continue; \
   case FL_z:  *va_arg(arg, ssize_t *)       = ret; continue; \
   default: return ~ret; \
   }

//
// FormatP
//
#define FormatP() \
   switch(fmtLen) \
   { \
   case FL_: \
      if(fmtArg.prec == -1) fmtArg.prec = 8; \
      fmtRet = __GDCC__FormatU_X_g((uintptr_t)va_arg(arg, void *), fmtArg); \
      break; \
      \
   default: return ~ret; \
   }

//
// FormatS
//
#define FormatS() \
   switch(fmtLen) \
   { \
   case FL_: \
      fmtRet.begin = fmtRet.end = (char *)va_arg(arg, char const *); \
      while(*fmtRet.end) ++fmtRet.end; \
      fmtRet.len = fmtRet.end - fmtRet.begin; \
      \
      if(fmtRet.len > fmtArg.prec) \
         fmtRet.len = fmtArg.prec; \
      \
      break; \
      \
   default: return ~ret; \
   }

//
// FormatStr
//
#if __GDCC_Target__ZDoom__
#define FormatStr() \
   switch(fmtLen) \
   { \
   case FL_: \
      fmtStr = va_arg(arg, __str); \
      fmtRet.len = ACS_StrLen(fmtStr); \
      goto fmt_str; \
   \
   case FL_L: \
      ACS_BeginPrint(); \
      ACS_PrintLocalized(va_arg(arg, __str)); \
      fmtStr = ACS_EndStrParam(); \
      fmtRet.len = ACS_StrLen(fmtStr); \
      goto fmt_str; \
   \
   case FL_j: \
      ACS_BeginPrint(); \
      ACS_PrintBind(va_arg(arg, __str)); \
      fmtStr = ACS_EndStrParam(); \
      fmtRet.len = ACS_StrLen(fmtStr); \
      goto fmt_str; \
   \
   case FL_t: \
      ACS_BeginPrint(); \
      ACS_PrintName(va_arg(arg, int)); \
      fmtStr = ACS_EndStrParam(); \
      fmtRet.len = ACS_StrLen(fmtStr); \
      goto fmt_str; \
   \
   default: return ~ret; \
   }
#else
#define FormatStr() return ~ret;
#endif

//
// FormatSwitch_f
//
#define FormatSwitch_f() \
   switch(fmtChr) \
   { \
      FormatCases(); \
      \
   case '%': \
      if(fputc(fmtChr, stream) == EOF) \
         return ~ret; \
      ++ret; \
      continue; \
      \
   fmt_str: \
      if(fmtRet.len > fmtArg.prec) \
         fmtRet.len = fmtArg.prec; \
      \
      /* Write result. */ \
      WriteResult_f(fmtStr, __fwrite_str); \
      \
      continue; \
      \
   default: return ~ret; \
   } \
   \
   /* Write result. */ \
   WriteResult_f(fmtRet.begin, fwrite);

//
// FormatSwitch_n
//
#define FormatSwitch_n() \
   switch(fmtChr) \
   { \
      FormatCases(); \
      \
   case '%': ACS_PrintChar('%'); ++ret; continue; \
      \
   default: return ~ret; \
      \
   fmt_str: \
      /* Write result. */ \
      WriteResult_n_str(); \
      \
      continue; \
   } \
   \
   /* Write result. */ \
   WriteResult_n()

//
// FormatX
//
#define FormatX(fmt) \
   switch(fmtLen) \
   { \
   case FL_:  fmtRet = fmt(va_arg(arg, _Accum),       fmtArg); break; \
   case FL_h: fmtRet = fmt(va_arg(arg, short _Accum), fmtArg); break; \
   case FL_l: fmtRet = fmt(va_arg(arg, long _Accum),  fmtArg); break; \
   default: return ~ret; \
   }

//
// FormatU
//
#define FormatU(fmt) \
   switch(fmtLen) \
   { \
   case FL_:   fmtRet = fmt(va_arg(arg, unsigned),           fmtArg); break; \
   case FL_h:  fmtRet = fmt(va_arg(arg, unsigned),           fmtArg); break; \
   case FL_hh: fmtRet = fmt(va_arg(arg, unsigned),           fmtArg); break; \
   case FL_j:  fmtRet = fmt(va_arg(arg, uintmax_t),          fmtArg); break; \
   case FL_l:  fmtRet = fmt(va_arg(arg, long unsigned),      fmtArg); break; \
   case FL_ll: fmtRet = fmt(va_arg(arg, long long unsigned), fmtArg); break; \
   case FL_t:  fmtRet = fmt(va_arg(arg, uptrdiff_t),         fmtArg); break; \
   case FL_z:  fmtRet = fmt(va_arg(arg, size_t),             fmtArg); break; \
   default: return ~ret; \
   }

//
// ParseFlags
//
#define ParseFlags() \
   for(fmtArg.flags = 0; (fmtChr = *format); ++format) \
   { \
      switch(fmtChr) \
      { \
      case '-': fmtArg.flags |= __GDCC__FormatFlag_Left; continue; \
      case '+': fmtArg.flags |= __GDCC__FormatFlag_Sign; continue; \
      case ' ': fmtArg.flags |= __GDCC__FormatFlag_PadS; continue; \
      case '#': fmtArg.flags |= __GDCC__FormatFlag_AltF; continue; \
      case '0': fmtArg.flags |= __GDCC__FormatFlag_Pad0; continue; \
      } \
      \
      break; \
   }

//
// ParseFormat
//
#define ParseFormat() \
   /* Read flags, if any. */ \
   ParseFlags(); \
   \
   /* Read field width, if any. */ \
   ParseWidth(); \
   \
   /* Read precision, if any. */ \
   ParsePrec(); \
   \
   /* Read length modifier, if any. */ \
   ParseLength(); \
   \
   /* Read format specifier. */ \
   fmtChr = *format++;

//
// ParseLength
//
#define ParseLength() \
   switch(*format) \
   { \
   case 'h': fmtLen = *++format == 'h' ? ++format, FL_hh : FL_h; break; \
   case 'l': fmtLen = *++format == 'l' ? ++format, FL_ll : FL_l; break; \
   case 'L': fmtLen = FL_L; ++format; break; \
   case 'j': fmtLen = FL_j; ++format; break; \
   case 't': fmtLen = FL_t; ++format; break; \
   case 'z': fmtLen = FL_z; ++format; break; \
   default:  fmtLen = FL_;            break; \
   }

//
// ParsePrec
//
#define ParsePrec() \
   if(*format == '.') \
   { \
      if(*++format == '*') \
      { \
         ++format; \
         int tmp; \
         if((tmp = va_arg(arg, int)) < 0) \
            fmtArg.prec = -1; \
         else \
            fmtArg.prec = tmp; \
      } \
      else for(fmtArg.prec = 0; '0' <= (fmtChr = *format) && fmtChr <= '9'; ++format) \
         fmtArg.prec = (fmtArg.prec * 10) + (fmtChr - '0'); \
   } \
   else \
      fmtArg.prec = -1;

//
// ParseWidth
//
#define ParseWidth() \
   if(*format == '*') \
   { \
      ++format; \
      int tmp; \
      if((tmp = va_arg(arg, int)) < 0) \
         fmtArg.width = -tmp, fmtArg.flags |= __GDCC__FormatFlag_Left; \
      else \
         fmtArg.width = tmp; \
   } \
   else for(fmtArg.width = 0; '0' <= (fmtChr = *format) && fmtChr <= '9'; ++format) \
      fmtArg.width = (fmtArg.width * 10) + (fmtChr - '0');

//
// WriteResult_f
//
#define WriteResult_f(res, write) \
   if(fmtRet.len < fmtArg.width) \
   { \
      ret += fmtArg.width; \
      fmtArg.width -= fmtRet.len; \
      \
      WriteWidthPre(f, fmtArg.width); \
      if(!write(res, fmtRet.len, 1, stream)) \
         return ~ret; \
      WriteWidthPro(f, fmtArg.width); \
   } \
   else \
   { \
      ret += fmtRet.len; \
      if(!write(res, fmtRet.len, 1, stream)) \
         return ~ret; \
   }

//
// WriteResult_n
//
#define WriteResult_n() \
   if(fmtRet.len < fmtArg.width) \
   { \
      ret += fmtArg.width; \
      fmtArg.width -= fmtRet.len; \
      \
      WriteWidthPre(n, fmtArg.width); \
      ACS_PrintGlobalCharRange((int)fmtRet.begin, __GDCC__Sta, 0, fmtRet.len); \
      WriteWidthPro(n, fmtArg.width); \
   } \
   else \
   { \
      ret += fmtRet.len; \
      ACS_PrintGlobalCharRange((int)fmtRet.begin, __GDCC__Sta, 0, fmtRet.len); \
   }

//
// WriteResult_n_str
//
#define WriteResult_n_str() \
   if(fmtRet.len > fmtArg.prec) \
   { \
      fmtRet.len = fmtArg.prec; \
      \
      if(fmtRet.len < fmtArg.width) \
      { \
         ret += fmtArg.width; \
         fmtArg.width -= fmtRet.len; \
         \
         WriteWidthPre(n, fmtArg.width); \
         for(unsigned i = 0; i != fmtRet.len; ++i) \
            ACS_PrintChar(fmtStr[i]); \
         WriteWidthPro(n, fmtArg.width); \
      } \
      else \
      { \
         ret += fmtRet.len; \
         for(unsigned i = 0; i != fmtRet.len; ++i) \
            ACS_PrintChar(fmtStr[i]); \
      } \
   } \
   else \
   { \
      if(fmtRet.len < fmtArg.width) \
      { \
         ret += fmtArg.width; \
         fmtArg.width -= fmtRet.len; \
         \
         WriteWidthPre(n, fmtArg.width); \
         ACS_PrintString(fmtStr); \
         WriteWidthPro(n, fmtArg.width); \
      } \
      else \
      { \
         ret += fmtRet.len; \
         ACS_PrintString(fmtStr); \
      } \
   }

//
// WriteStrEnt_s
//
#if __GDCC_Target__ZDoom__
#define WriteStrEnt_s(len) \
   do \
   { \
      ACS_StrCpyToGlobalCharRange((int)s, __GDCC__Sta, 0, (len), \
         fmtStr, (len)); \
      (s) += len; \
   } \
   while(0)
#else
#define WriteStrEnt_s(len) \
   do \
   { \
      for(unsigned i = 0; i != (len); ++i) \
         *s++ = fmtStr[i]; \
   } \
   while(0)
#endif

//
// WriteWidth_f
//
#define WriteWidth_f(len) \
   do if(fputc(' ', stream) == EOF) return ~ret; while(--len)

//
// WriteWidth_n
//
#define WriteWidth_n(len) \
   do ACS_PrintChar(' '); while(--len)

//
// WriteWidth_s
//
#define WriteWidth_s(len) \
   do *s++ = ' '; while(--len)

//
// WriteWidthPre
//
#define WriteWidthPre(group, len) \
   if(!(fmtArg.flags & __GDCC__FormatFlag_Left)) \
      {WriteWidth_##group(len);}

//
// WriteWidthPro
//
#define WriteWidthPro(group, len) \
   if(fmtArg.flags & __GDCC__FormatFlag_Left) \
      {WriteWidth_##group(len);}


//----------------------------------------------------------------------------|
// Types                                                                      |
//

//
// FormatLength
//
typedef enum FormatLen
{
   FL_,
   FL_L,
   FL_h,
   FL_hh,
   FL_j,
   FL_l,
   FL_ll,
   FL_t,
   FL_z,
} FormatLen;


//----------------------------------------------------------------------------|
// Global Functions                                                           |
//

//=========================================================
// Formatted output functions.
//

//
// fprintf
//
int fprintf(FILE *restrict stream, char const *restrict format, ...)
{
   va_list arg;
   int     ret;

   va_start(arg, format);
   ret = vfprintf(stream, format, arg);
   va_end(arg);

   return ret;
}

//
// printf
//
int printf(char const *restrict format, ...)
{
   va_list arg;
   int     ret;

   va_start(arg, format);
   ret = vfprintf(stdout, format, arg);
   va_end(arg);

   return ret;
}

//
// snprintf
//
int snprintf(char *restrict s, size_t n, char const *restrict format, ...)
{
   va_list arg;
   int     ret;

   va_start(arg, format);
   ret = vsnprintf(s, n, format, arg);
   va_end(arg);

   return ret;
}

//
// sprintf
//
int sprintf(char *restrict s, char const *restrict format, ...)
{
   va_list arg;
   int     ret;

   va_start(arg, format);
   ret = vsprintf(s, format, arg);
   va_end(arg);

   return ret;
}

//
// vfprintf
//
int vfprintf(FILE *restrict stream, char const *restrict format, __va_list arg)
{
   FormatDecl();

   while((fmtChr = *format++))
   {
      if(fmtChr != '%')
      {
         if(fputc(fmtChr, stream) == EOF)
            return ~ret;
         ++ret;
         continue;
      }

      // Read format args.
      ParseFormat();

      // Perform format.
      FormatSwitch_f();
   }

   return ret;
}

//
// vprintf
//
int vprintf(char const *restrict format, __va_list arg)
{
   return vfprintf(stdout, format, arg);
}

//
// vsnprintf
//
int vsnprintf(char *restrict s, size_t n, char const *restrict format, __va_list arg)
{
   FormatDecl();

   while((fmtChr = *format++))
   {
      if(fmtChr != '%') {if(ret < (int)n) *s++ = fmtChr; ++ret; continue;}

      // Read format args.
      ParseFormat();

      // Perform format.
      switch(fmtChr)
      {
         FormatCases();

      case '%': if(ret < (int)n) *s++ = '%'; ++ret; continue;

      fmt_str:
         if(fmtRet.len > fmtArg.prec)
            fmtRet.len = fmtArg.prec;

         // Write result.
         if(fmtRet.len < fmtArg.width)
         {
            // Full format will fit.
            if(ret + (int)fmtRet.len < (int)n)
            {
               ret += fmtArg.width;
               fmtArg.width -= fmtRet.len;

               WriteWidthPre(s, fmtArg.width);
               WriteStrEnt_s(fmtRet.len);
               WriteWidthPro(s, fmtArg.width);
            }

            // Partial format will fit.
            else if(ret < (int)n)
            {
               int max = n - ret;

               ret += fmtArg.width;
               fmtArg.width -= fmtRet.len;

               if(!(fmtArg.flags & __GDCC__FormatFlag_Left))
               {
                  if(max >= (int)fmtArg.width)
                  {
                     max -= fmtArg.width;
                     WriteWidth_s(fmtArg.width);
                  }
                  else
                     WriteWidth_s(max);
               }

               if(max >= (int)fmtRet.len)
               {
                  max -= fmtRet.len;
                  WriteStrEnt_s(fmtRet.len);
               }
               else if(max)
               {
                  WriteStrEnt_s(max);
                  max = 0;
               }

               if(fmtArg.flags & __GDCC__FormatFlag_Left)
               {
                  if(max)
                     WriteWidth_s(max);
               }
            }

         }
         else
         {
            // Full format will fit.
            if(ret + (int)fmtRet.len < (int)n)
               WriteStrEnt_s(fmtRet.len);

            // Partial format will fit.
            else if(ret < (int)n)
            {
               int max = n - ret;
               WriteStrEnt_s(max);
            }

            ret += fmtRet.len;
         }

         continue;

      default: return ~ret;
      }

      // Write result.
      if(fmtRet.len < fmtArg.width)
      {
         // Full format will fit.
         if(ret + (int)fmtRet.len < (int)n)
         {
            ret += fmtArg.width;
            fmtArg.width -= fmtRet.len;

            WriteWidthPre(s, fmtArg.width);
            while(fmtRet.len--) *s++ = *fmtRet.begin++;
            WriteWidthPro(s, fmtArg.width);
         }

         // Partial format will fit.
         else if(ret < (int)n)
         {
            int max = n - ret;

            ret += fmtArg.width;
            fmtArg.width -= fmtRet.len;

            if(!(fmtArg.flags & __GDCC__FormatFlag_Left))
            {
               if(max >= (int)fmtArg.width)
               {
                  max -= fmtArg.width;
                  WriteWidth_s(fmtArg.width);
               }
               else
                  WriteWidth_s(max);
            }

            if(max >= (int)fmtRet.len)
            {
               max -= fmtRet.len;
               while(fmtRet.len--) *s++ = *fmtRet.begin++;
            }
            else if(max)
               do *s++ = *fmtRet.begin++; while(--max);

            if(fmtArg.flags & __GDCC__FormatFlag_Left)
            {
               if(max)
                  WriteWidth_s(max);
            }
         }
      }
      else
      {
         // Full format will fit.
         if(ret + (int)fmtRet.len < (int)n)
            for(int max = fmtRet.len; max--;) *s++ = *fmtRet.begin++;

         // Partial format will fit.
         else if(ret < (int)n)
            for(int max = n - ret; max--;) *s++ = *fmtRet.begin++;

         ret += fmtRet.len;
      }
   }

   // Null terminate result.
   if(ret < (int)n)
      *s = '\0';
   else if(n)
      *(s - 1) = '\0';

   return ret;
}

//
// vsprintf
//
int vsprintf(char *restrict s, char const *restrict format, va_list arg)
{
   FormatDecl();

   while((fmtChr = *format++))
   {
      if(fmtChr != '%') {*s++ = fmtChr; ++ret; continue;}

      // Read format args.
      ParseFormat();

      // Perform format.
      switch(fmtChr)
      {
         FormatCases();

      case '%': *s++ = '%'; ++ret; continue;

      fmt_str:
         if(fmtRet.len > fmtArg.prec)
            fmtRet.len = fmtArg.prec;

         // Write result.
         if(fmtRet.len < fmtArg.width)
         {
            ret += fmtArg.width;
            fmtArg.width -= fmtRet.len;

            WriteWidthPre(s, fmtArg.width);
            WriteStrEnt_s(fmtRet.len);
            WriteWidthPro(s, fmtArg.width);
         }
         else
         {
            ret += fmtRet.len;
            WriteStrEnt_s(fmtRet.len);
         }

         continue;

      default: return ~ret;
      }

      // Write result.
      if(fmtRet.len < fmtArg.width)
      {
         ret += fmtArg.width;
         fmtArg.width -= fmtRet.len;

         WriteWidthPre(s, fmtArg.width);
         while(fmtRet.len--) *s++ = *fmtRet.begin++;
         WriteWidthPro(s, fmtArg.width);
      }
      else
      {
         ret += fmtRet.len;
         while(fmtRet.len--) *s++ = *fmtRet.begin++;
      }
   }

   // Null terminate result.
   *s++ = '\0';

   return ret;
}

//=========================================================
// Implementation extensions.
//

//
// __fprintf_str
//
int __fprintf_str(FILE *restrict stream,
   char const __str_ars *restrict format, ...)
{
   va_list arg;
   int     ret;

   va_start(arg, format);
   ret = __vfprintf_str(stream, format, arg);
   va_end(arg);

   return ret;
}

//
// __nprintf
//
int __nprintf(char const *restrict format, ...)
{
   va_list arg;
   int     ret;

   va_start(arg, format);
   ret = __vnprintf(format, arg);
   va_end(arg);

   return ret;
}

//
// __nprintf_str
//
int __nprintf_str(char const __str_ars *restrict format, ...)
{
   va_list arg;
   int     ret;

   va_start(arg, format);
   ret = __vnprintf_str(format, arg);
   va_end(arg);

   return ret;
}

//
// __printf_str
//
int __printf_str(char const __str_ars *restrict format, ...)
{
   va_list arg;
   int     ret;

   va_start(arg, format);
   ret = __vfprintf_str(stdout, format, arg);
   va_end(arg);

   return ret;
}

//
// __vfprintf_str
//
int __vfprintf_str(FILE *restrict stream,
   char const __str_ars *restrict format, __va_list arg)
{
   FormatDecl();

   while((fmtChr = *format++))
   {
      if(fmtChr != '%')
      {
         if(fputc(fmtChr, stream) == EOF)
            return ~ret;
         ++ret;
         continue;
      }

      // Read format args.
      ParseFormat();

      // Perform format.
      FormatSwitch_f();
   }

   return ret;
}

//
// __vnprintf
//
int __vnprintf(char const *restrict format, va_list arg)
{
   #if __GDCC_Target__ZDoom__
   FormatDecl();

   while((fmtChr = *format++))
   {
      if(fmtChr != '%') {ACS_PrintChar(fmtChr); ++ret; continue;}

      // Read format args.
      ParseFormat();

      // Perform format.
      FormatSwitch_n();
   }

   return ret;
   #else
   return vfprintf(stdout, format, arg);
   #endif
}

//
// __vnprintf_str
//
int __vnprintf_str(char const __str_ars *restrict format, va_list arg)
{
   #if __GDCC_Target__ZDoom__
   FormatDecl();

   while((fmtChr = *format++))
   {
      if(fmtChr != '%') {ACS_PrintChar(fmtChr); ++ret; continue;}

      // Read format args.
      ParseFormat();

      // Perform format.
      FormatSwitch_n();
   }

   return ret;
   #else
   return vfprintf(stdout, format, arg);
   #endif
}

//
// __vprintf_str
//
int __vprintf_str(char const __str_ars *restrict format, __va_list arg)
{
   return __vfprintf_str(stdout, format, arg);
}

// EOF

