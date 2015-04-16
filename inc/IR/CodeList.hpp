//-----------------------------------------------------------------------------
//
// Copyright (C) 2013-2015 David Hill
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
// AdX* dst lop rop carry
GDCC_IR_CodeList(AdXU_W)

// Add* dst lop rop
GDCC_IR_CodeList(AddF_W)
GDCC_IR_CodeList(AddI_W)
GDCC_IR_CodeList(AddU_W)

// And* dst lop rop
GDCC_IR_CodeList(AndU_W)

// Bclo dst src (Bit, Count Leading Ones)
GDCC_IR_CodeList(Bclo_W)

// Bclz dst src (Bit, Count Leading Zeros)
GDCC_IR_CodeList(Bclz_W)

// Call src ret arg...
GDCC_IR_CodeList(Call)

// Casm src arg...
GDCC_IR_CodeList(Casm)

// Cmp* dst lop rop
GDCC_IR_CodeList(CmpF_EQ_W)
GDCC_IR_CodeList(CmpF_GE_W)
GDCC_IR_CodeList(CmpF_GT_W)
GDCC_IR_CodeList(CmpF_LE_W)
GDCC_IR_CodeList(CmpF_LT_W)
GDCC_IR_CodeList(CmpF_NE_W)
GDCC_IR_CodeList(CmpI_EQ_W)
GDCC_IR_CodeList(CmpI_GE_W)
GDCC_IR_CodeList(CmpI_GT_W)
GDCC_IR_CodeList(CmpI_LE_W)
GDCC_IR_CodeList(CmpI_LT_W)
GDCC_IR_CodeList(CmpI_NE_W)
GDCC_IR_CodeList(CmpU_EQ_W)
GDCC_IR_CodeList(CmpU_GE_W)
GDCC_IR_CodeList(CmpU_GT_W)
GDCC_IR_CodeList(CmpU_LE_W)
GDCC_IR_CodeList(CmpU_LT_W)
GDCC_IR_CodeList(CmpU_NE_W)

// Cnat src ret arg...
GDCC_IR_CodeList(Cnat)

// Copy lop rop
GDCC_IR_CodeList(Copy_W)

// Cscr_* src ret arg...
// Integer/String Asynchronous/Synchronous
GDCC_IR_CodeList(Cscr_IA)
GDCC_IR_CodeList(Cscr_IS)
GDCC_IR_CodeList(Cscr_SA)
GDCC_IR_CodeList(Cscr_SS)

// Cspe src ret arg...
GDCC_IR_CodeList(Cspe)

// DiX* dst lop rop
GDCC_IR_CodeList(DiXI_W)
GDCC_IR_CodeList(DiXU_W)

// Div* dst lop rop
GDCC_IR_CodeList(DivF_W)
GDCC_IR_CodeList(DivI_W)
GDCC_IR_CodeList(DivK_W)
GDCC_IR_CodeList(DivU_W)
GDCC_IR_CodeList(DivX_W)

// Inv* dst src
GDCC_IR_CodeList(InvU_W)

// Jcnd src lab
GDCC_IR_CodeList(Jcnd_Nil)
GDCC_IR_CodeList(Jcnd_Tru)

// Jump src
GDCC_IR_CodeList(Jump)

// Mod* dst lop rop
GDCC_IR_CodeList(ModI_W)
GDCC_IR_CodeList(ModU_W)

// Move dst src
GDCC_IR_CodeList(Move_W)

// MuX* dst lop rop
GDCC_IR_CodeList(MuXU_W)

// Mul* dst lop rop
GDCC_IR_CodeList(MulF_W)
GDCC_IR_CodeList(MulI_W)
GDCC_IR_CodeList(MulK_W)
GDCC_IR_CodeList(MulU_W)
GDCC_IR_CodeList(MulX_W)

// Neg* dst src
GDCC_IR_CodeList(NegF_W)
GDCC_IR_CodeList(NegI_W)

// Not* dst src
GDCC_IR_CodeList(NotU_W)

// OrI* dst lop rop
GDCC_IR_CodeList(OrIU_W)

// OrX* dst lop rop
GDCC_IR_CodeList(OrXU_W)

// Plsa size (Pointer, Local Stack Allocate)
GDCC_IR_CodeList(Plsa)

// Plsd src (Pointer, Local Stack Decrement)
GDCC_IR_CodeList(Plsd)

// Plsf (Pointer, Local Stack Free)
GDCC_IR_CodeList(Plsf)

// Plsi src (Pointer, Local Stack Increment)
GDCC_IR_CodeList(Plsi)

// Plsr src (Pointer, Local Stack Restore)
GDCC_IR_CodeList(Plsr)

// Plss dst (Pointer, Local Stack Save)
GDCC_IR_CodeList(Plss)

// Pltn dst src (Pointer, Local to Near)
GDCC_IR_CodeList(Pltn)

// Retn src...
GDCC_IR_CodeList(Retn)

// ShL* dst lop rop
GDCC_IR_CodeList(ShLF_W)
GDCC_IR_CodeList(ShLU_W)

// ShR* dst lop rop
GDCC_IR_CodeList(ShRF_W)
GDCC_IR_CodeList(ShRI_W)
GDCC_IR_CodeList(ShRU_W)

// SuX* dst lop rop
// SuX* dst lop rop carry
GDCC_IR_CodeList(SuXU_W)

// Sub* dst lop rop
GDCC_IR_CodeList(SubF_W)
GDCC_IR_CodeList(SubI_W)
GDCC_IR_CodeList(SubU_W)

// Swap lop rop
GDCC_IR_CodeList(Swap_W)

// Xcod_SID (Static Initialization Delay)
GDCC_IR_CodeList(Xcod_SID)

#undef GDCC_IR_CodeList

// EOF

