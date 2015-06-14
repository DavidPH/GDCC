//-----------------------------------------------------------------------------
//
// Copyright(C) 2014-2015 David Hill
//
// See COPYLIB for license information.
//
//-----------------------------------------------------------------------------
//
// Header for target library libc.
//
// Diagnostics.
//
//-----------------------------------------------------------------------------


//----------------------------------------------------------------------------|
// Macros                                                                     |
//

//
// assert
//
#ifdef assert
#undef assert
#endif

#ifdef NDEBUG
# define assert(ignore) ((void)0)
#else
# define assert(expression) \
   if(!(expression)) \
   { \
      extern struct __FILE __stderr; \
      extern void abort(void); \
      extern int fprintf(struct __FILE *restrict stream, \
         char const *restrict format, ...); \
      \
      fprintf(&__stderr, \
         "Assertion failed: %s, function %s, file %s, line %i.", \
         #expression, __func__, __FILE__, __LINE__); \
      abort(); \
   } \
   else ((void)0)
#endif

//
// static_assert
//
#if !defined(static_assert) && !defined(__cplusplus)
#define static_assert _Static_assert
#endif


//----------------------------------------------------------------------------|
// Types                                                                      |
//

struct __FILE;

// EOF

