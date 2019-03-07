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
//-----------------------------------------------------------------------------

// Nop
GDCC_IR_CodeList(Nop)

// Add dst lop rop
GDCC_IR_CodeList(Add)

// AddX dst lop rop
// AddX dst carry lop rop
GDCC_IR_CodeList(AddX)

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
GDCC_IR_CodeList(CmpEQ)
GDCC_IR_CodeList(CmpGE)
GDCC_IR_CodeList(CmpGT)
GDCC_IR_CodeList(CmpLE)
GDCC_IR_CodeList(CmpLT)
GDCC_IR_CodeList(CmpNE)

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

// Div dst lop rop
GDCC_IR_CodeList(Div)

// DivX dst lop rop
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

// Mod dst lop rop
GDCC_IR_CodeList(Mod)

// Move dst src
GDCC_IR_CodeList(Move)

// Mul dst lop rop
GDCC_IR_CodeList(Mul)

// MulX* dst lop rop
GDCC_IR_CodeList(MulX)

// Neg dst src
GDCC_IR_CodeList(Neg)

// Pltn dst src (Pointer, Local to Near)
GDCC_IR_CodeList(Pltn)

// Retn
// Retn src
GDCC_IR_CodeList(Retn)

// Rjnk
// Rjnk src
GDCC_IR_CodeList(Rjnk)

// ShL dst lop rop
GDCC_IR_CodeList(ShL)

// ShR dst lop rop
GDCC_IR_CodeList(ShR)

// Sub dst lop rop
GDCC_IR_CodeList(Sub)

// SubX dst lop rop
// SubX dst carry lop rop
GDCC_IR_CodeList(SubX)

// Swap lop rop
GDCC_IR_CodeList(Swap)

// Tr dst src
GDCC_IR_CodeList(Tr)

// Xcod_SID (Static Initialization Delay)
GDCC_IR_CodeList(Xcod_SID)

#undef GDCC_IR_CodeList

// EOF

