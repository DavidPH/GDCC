//-----------------------------------------------------------------------------
//
// Copyright (C) 2013 David Hill
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

// Add* dst lop rop
GDCC_IR_CodeList(AddI_W)
GDCC_IR_CodeList(AddU_W)

// And* dst lop rop
GDCC_IR_CodeList(AndU_W)

// Call src ret arg...
GDCC_IR_CodeList(Call)

// Casm src arg...
GDCC_IR_CodeList(Casm)

// Cjmp src lab
GDCC_IR_CodeList(Cjmp_Nil)
GDCC_IR_CodeList(Cjmp_Tru)

// Cmp* dst lop rop
GDCC_IR_CodeList(CmpI_EQ_W)
GDCC_IR_CodeList(CmpU_EQ_W)
GDCC_IR_CodeList(CmpI_GE_W)
GDCC_IR_CodeList(CmpU_GE_W)
GDCC_IR_CodeList(CmpI_GT_W)
GDCC_IR_CodeList(CmpU_GT_W)
GDCC_IR_CodeList(CmpI_LE_W)
GDCC_IR_CodeList(CmpU_LE_W)
GDCC_IR_CodeList(CmpI_LT_W)
GDCC_IR_CodeList(CmpU_LT_W)
GDCC_IR_CodeList(CmpI_NE_W)
GDCC_IR_CodeList(CmpU_NE_W)

// Cnat src ret arg...
GDCC_IR_CodeList(Cnat)

// Cspe src ret arg...
GDCC_IR_CodeList(Cspe)

// Div* dst lop rop
GDCC_IR_CodeList(DivI_W)
GDCC_IR_CodeList(DivX_W)

// Inv* dst src
GDCC_IR_CodeList(InvU_W)

// Jump src
GDCC_IR_CodeList(Jump)

// Mod* dst lop rop
GDCC_IR_CodeList(ModI_W)

// Move dst src
GDCC_IR_CodeList(Move_W)

// Mul* dst lop rop
GDCC_IR_CodeList(MulI_W)
GDCC_IR_CodeList(MulX_W)

// Neg* dst src
GDCC_IR_CodeList(NegI_W)

// Not* dst src
GDCC_IR_CodeList(NotU_W)

// OrI* dst lop rop
GDCC_IR_CodeList(OrIU_W)

// OrX* dst lop rop
GDCC_IR_CodeList(OrXU_W)

// Retn src...
GDCC_IR_CodeList(Retn)

// ShL* dst lop rop
GDCC_IR_CodeList(ShLU_W)

// ShR* dst lop rop
GDCC_IR_CodeList(ShRI_W)

// Sub* dst lop rop
GDCC_IR_CodeList(SubI_W)
GDCC_IR_CodeList(SubU_W)

// Swap lop rop
GDCC_IR_CodeList(Swap_W)

#undef GDCC_IR_CodeList

// EOF

