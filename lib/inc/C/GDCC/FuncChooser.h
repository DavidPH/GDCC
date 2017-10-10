//-----------------------------------------------------------------------------
//
// Copyright(C) 2017 David Hill
//
// See COPYLIB for license information.
//
//-----------------------------------------------------------------------------
//
// Header for target library libc.
//
// Function chooser macros.
//
//-----------------------------------------------------------------------------

#ifndef __GDCC_Header__C__GDCC__FuncChooser_h__
#define __GDCC_Header__C__GDCC__FuncChooser_h__


//----------------------------------------------------------------------------|
// Macros                                                                     |
//

#define GDCC_ListChooser0(a0, ...) a0
#define GDCC_ListChooser1(a0, a1, ...) a1
#define GDCC_ListChooser2(a0, a1, a2, ...) a2
#define GDCC_ListChooser3(a0, a1, a2, a3, ...) a3
#define GDCC_ListChooser4(a0, a1, a2, a3, a4, ...) a4
#define GDCC_ListChooser5(a0, a1, a2, a3, a4, a5, ...) a5
#define GDCC_ListChooser6(a0, a1, a2, a3, a4, a5, a6, ...) a6
#define GDCC_ListChooser7(a0, a1, a2, a3, a4, a5, a6, a7, ...) a7
#define GDCC_ListChooser8(a0, a1, a2, a3, a4, a5, a6, a7, a8, ...) a8
#define GDCC_ListChooser9(a0, a1, a2, a3, a4, a5, a6, a7, a8, a9, ...) a9
#define GDCC_ListChooser(n, ...) GDCC_ListChooser##n(__VA_ARGS__)

#define GDCC_FuncChooserArgs0(f) f##0
#define GDCC_FuncChooserArgs1(f) f##1, GDCC_FuncChooserArgs0(f)
#define GDCC_FuncChooserArgs2(f) f##2, GDCC_FuncChooserArgs1(f)
#define GDCC_FuncChooserArgs3(f) f##3, GDCC_FuncChooserArgs2(f)
#define GDCC_FuncChooserArgs4(f) f##4, GDCC_FuncChooserArgs3(f)
#define GDCC_FuncChooserArgs5(f) f##5, GDCC_FuncChooserArgs4(f)
#define GDCC_FuncChooserArgs6(f) f##6, GDCC_FuncChooserArgs5(f)
#define GDCC_FuncChooserArgs7(f) f##7, GDCC_FuncChooserArgs6(f)
#define GDCC_FuncChooserArgs8(f) f##8, GDCC_FuncChooserArgs7(f)
#define GDCC_FuncChooserArgs9(f) f##9, GDCC_FuncChooserArgs8(f)

#define GDCC_FuncChooser(f, n, ...) \
   GDCC_ListChooser(n, GDCC_FuncChooserArgs##n __VA_ARGS__ (f), GDCC_FuncChooserArgs##n(f))

#define GDCC_FuncChooserCall(f, n, ...) GDCC_FuncChooser(f, n, __VA_ARGS__)(__VA_ARGS__)

#endif//__GDCC_Header__C__GDCC__FuncChooser_h__

