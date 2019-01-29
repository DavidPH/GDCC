//-----------------------------------------------------------------------------
//
// Copyright (C) 2013-2019 David Hill
//
// See COPYING for license information.
//
//-----------------------------------------------------------------------------
//
// Intermediary Representation codes.
//
// Types:
//   A - unsigned fractional
//   F - signed floating-point
//   I - signed integral
//   K - unsigned fixed-point
//   R - signed fractional
//   U - unsigned integral
//   X - signed fixed-point
//
//-----------------------------------------------------------------------------

// Nop
GDCC_IR_CodeList(Nop)

// AdX* dst lop rop
// AdX* dst carry lop rop
GDCC_IR_CodeList(AdXU)

// Add* dst lop rop
GDCC_IR_CodeList(AddF)
GDCC_IR_CodeList(AddI)
GDCC_IR_CodeList(AddU)

// BAnd dst lop rop
GDCC_IR_CodeList(BAnd)

// BNot dst src
GDCC_IR_CodeList(BNot)

// BOrI dst lop rop
GDCC_IR_CodeList(BOrI)

// BOrX dst lop rop
GDCC_IR_CodeList(BOrX)

// Bclo dst src (Bit, Count Leading Ones)
GDCC_IR_CodeList(Bclo)

// Bclz dst src (Bit, Count Leading Zeros)
GDCC_IR_CodeList(Bclz)

// Bges dst src width off (Bitfield, Get Signed)
GDCC_IR_CodeList(Bges)

// Bget dst src width off (Bitfield, Get)
GDCC_IR_CodeList(Bget)

// Bset dst src width off (Bitfield, Set)
GDCC_IR_CodeList(Bset)

// Call dst src arg...
GDCC_IR_CodeList(Call)

// Casm dst src arg...
GDCC_IR_CodeList(Casm)

// Cmp* dst lop rop
GDCC_IR_CodeList(CmpF_EQ)
GDCC_IR_CodeList(CmpF_GE)
GDCC_IR_CodeList(CmpF_GT)
GDCC_IR_CodeList(CmpF_LE)
GDCC_IR_CodeList(CmpF_LT)
GDCC_IR_CodeList(CmpF_NE)
GDCC_IR_CodeList(CmpI_EQ)
GDCC_IR_CodeList(CmpI_GE)
GDCC_IR_CodeList(CmpI_GT)
GDCC_IR_CodeList(CmpI_LE)
GDCC_IR_CodeList(CmpI_LT)
GDCC_IR_CodeList(CmpI_NE)
GDCC_IR_CodeList(CmpU_EQ)
GDCC_IR_CodeList(CmpU_GE)
GDCC_IR_CodeList(CmpU_GT)
GDCC_IR_CodeList(CmpU_LE)
GDCC_IR_CodeList(CmpU_LT)
GDCC_IR_CodeList(CmpU_NE)

// Cnat dst src arg...
GDCC_IR_CodeList(Cnat)

// Copy lop rop
GDCC_IR_CodeList(Copy)

// Cscr_* dst src arg...
// Integer/String Asynchronous/Synchronous
GDCC_IR_CodeList(Cscr_IA)
GDCC_IR_CodeList(Cscr_IS)
GDCC_IR_CodeList(Cscr_SA)
GDCC_IR_CodeList(Cscr_SS)

// Cspe dst src arg...
GDCC_IR_CodeList(Cspe)

// DiX* dst lop rop
GDCC_IR_CodeList(DiXI)
GDCC_IR_CodeList(DiXU)

// Div* dst lop rop
GDCC_IR_CodeList(DivA)
GDCC_IR_CodeList(DivF)
GDCC_IR_CodeList(DivI)
GDCC_IR_CodeList(DivK)
GDCC_IR_CodeList(DivR)
GDCC_IR_CodeList(DivU)
GDCC_IR_CodeList(DivX)

// Jcnd_Nil src addr
GDCC_IR_CodeList(Jcnd_Nil)

// Jcnd_Tab src [value addr]...
GDCC_IR_CodeList(Jcnd_Tab)

// Jcnd_Tru src addr
GDCC_IR_CodeList(Jcnd_Tru)

// Jdyn src
GDCC_IR_CodeList(Jdyn)

// Jfar_Pro addr drop
GDCC_IR_CodeList(Jfar_Pro)

// Jfar_Set addr jmpbuf
GDCC_IR_CodeList(Jfar_Set)

// Jfar_Sta addr jmpbuf res
GDCC_IR_CodeList(Jfar_Sta)

// Jump src
GDCC_IR_CodeList(Jump)

// LAnd dst lop rop
GDCC_IR_CodeList(LAnd)

// LNot dst src
GDCC_IR_CodeList(LNot)

// LOrI dst lop rop
GDCC_IR_CodeList(LOrI)

// Mod* dst lop rop
GDCC_IR_CodeList(ModI)
GDCC_IR_CodeList(ModU)

// Move dst src
GDCC_IR_CodeList(Move)

// MuX* dst lop rop
GDCC_IR_CodeList(MuXU)

// Mul* dst lop rop
GDCC_IR_CodeList(MulA)
GDCC_IR_CodeList(MulF)
GDCC_IR_CodeList(MulI)
GDCC_IR_CodeList(MulK)
GDCC_IR_CodeList(MulR)
GDCC_IR_CodeList(MulU)
GDCC_IR_CodeList(MulX)

// Neg* dst src
GDCC_IR_CodeList(NegF)
GDCC_IR_CodeList(NegI)

// Pltn dst src (Pointer, Local to Near)
GDCC_IR_CodeList(Pltn)

// Retn
// Retn src
GDCC_IR_CodeList(Retn)

// Rjnk
// Rjnk src
GDCC_IR_CodeList(Rjnk)

// ShL* dst lop rop
GDCC_IR_CodeList(ShLF)
GDCC_IR_CodeList(ShLU)

// ShR* dst lop rop
GDCC_IR_CodeList(ShRF)
GDCC_IR_CodeList(ShRI)
GDCC_IR_CodeList(ShRU)

// SuX* dst lop rop
// SuX* dst lop rop carry
GDCC_IR_CodeList(SuXU)

// Sub* dst lop rop
GDCC_IR_CodeList(SubF)
GDCC_IR_CodeList(SubI)
GDCC_IR_CodeList(SubU)

// Swap lop rop
GDCC_IR_CodeList(Swap)

// Xcod_SID (Static Initialization Delay)
GDCC_IR_CodeList(Xcod_SID)

#undef GDCC_IR_CodeList

// EOF

