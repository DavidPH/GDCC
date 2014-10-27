//-----------------------------------------------------------------------------
//
// Copyright (C) 2013-2014 David Hill
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
GDCC_IR_CodeList(AddF_W)
GDCC_IR_CodeList(AddF_W2)
GDCC_IR_CodeList(AddI_W)
GDCC_IR_CodeList(AddI_W2)
GDCC_IR_CodeList(AddI_W3)
GDCC_IR_CodeList(AddU_W)
GDCC_IR_CodeList(AddU_W2)
GDCC_IR_CodeList(AddU_W3)

// And* dst lop rop
GDCC_IR_CodeList(AndU_W)
GDCC_IR_CodeList(AndU_W2)
GDCC_IR_CodeList(AndU_W3)

// Call src ret arg...
GDCC_IR_CodeList(Call)

// Casm src arg...
GDCC_IR_CodeList(Casm)

// Cjmp src lab
GDCC_IR_CodeList(Cjmp_Nil)
GDCC_IR_CodeList(Cjmp_Tru)

// Cmp* dst lop rop
GDCC_IR_CodeList(CmpF_EQ_W)
GDCC_IR_CodeList(CmpF_EQ_W2)
GDCC_IR_CodeList(CmpF_GE_W)
GDCC_IR_CodeList(CmpF_GE_W2)
GDCC_IR_CodeList(CmpF_GT_W)
GDCC_IR_CodeList(CmpF_GT_W2)
GDCC_IR_CodeList(CmpF_LE_W)
GDCC_IR_CodeList(CmpF_LE_W2)
GDCC_IR_CodeList(CmpF_LT_W)
GDCC_IR_CodeList(CmpF_LT_W2)
GDCC_IR_CodeList(CmpF_NE_W)
GDCC_IR_CodeList(CmpF_NE_W2)
GDCC_IR_CodeList(CmpI_EQ_W)
GDCC_IR_CodeList(CmpI_EQ_W2)
GDCC_IR_CodeList(CmpI_EQ_W3)
GDCC_IR_CodeList(CmpI_GE_W)
GDCC_IR_CodeList(CmpI_GE_W2)
GDCC_IR_CodeList(CmpI_GE_W3)
GDCC_IR_CodeList(CmpI_GT_W)
GDCC_IR_CodeList(CmpI_GT_W2)
GDCC_IR_CodeList(CmpI_GT_W3)
GDCC_IR_CodeList(CmpI_LE_W)
GDCC_IR_CodeList(CmpI_LE_W2)
GDCC_IR_CodeList(CmpI_LE_W3)
GDCC_IR_CodeList(CmpI_LT_W)
GDCC_IR_CodeList(CmpI_LT_W2)
GDCC_IR_CodeList(CmpI_LT_W3)
GDCC_IR_CodeList(CmpI_NE_W)
GDCC_IR_CodeList(CmpI_NE_W2)
GDCC_IR_CodeList(CmpI_NE_W3)
GDCC_IR_CodeList(CmpU_EQ_W)
GDCC_IR_CodeList(CmpU_EQ_W2)
GDCC_IR_CodeList(CmpU_EQ_W3)
GDCC_IR_CodeList(CmpU_GE_W)
GDCC_IR_CodeList(CmpU_GE_W2)
GDCC_IR_CodeList(CmpU_GE_W3)
GDCC_IR_CodeList(CmpU_GT_W)
GDCC_IR_CodeList(CmpU_GT_W2)
GDCC_IR_CodeList(CmpU_GT_W3)
GDCC_IR_CodeList(CmpU_LE_W)
GDCC_IR_CodeList(CmpU_LE_W2)
GDCC_IR_CodeList(CmpU_LE_W3)
GDCC_IR_CodeList(CmpU_LT_W)
GDCC_IR_CodeList(CmpU_LT_W2)
GDCC_IR_CodeList(CmpU_LT_W3)
GDCC_IR_CodeList(CmpU_NE_W)
GDCC_IR_CodeList(CmpU_NE_W2)
GDCC_IR_CodeList(CmpU_NE_W3)

// Cnat src ret arg...
GDCC_IR_CodeList(Cnat)

// Copy lop rop
GDCC_IR_CodeList(Copy_W)

// Cspe src ret arg...
GDCC_IR_CodeList(Cspe)

// Div* dst lop rop
GDCC_IR_CodeList(DivF_W)
GDCC_IR_CodeList(DivF_W2)
GDCC_IR_CodeList(DivI_W)
GDCC_IR_CodeList(DivI_W2)
GDCC_IR_CodeList(DivI_W3)
GDCC_IR_CodeList(DivU_W)
GDCC_IR_CodeList(DivU_W2)
GDCC_IR_CodeList(DivU_W3)
GDCC_IR_CodeList(DivX_W)
GDCC_IR_CodeList(DivX_W2)

// Inv* dst src
GDCC_IR_CodeList(InvU_W)
GDCC_IR_CodeList(InvU_W2)
GDCC_IR_CodeList(InvU_W3)

// Jump src
GDCC_IR_CodeList(Jump)

// Mod* dst lop rop
GDCC_IR_CodeList(ModI_W)
GDCC_IR_CodeList(ModI_W2)
GDCC_IR_CodeList(ModI_W3)
GDCC_IR_CodeList(ModU_W)
GDCC_IR_CodeList(ModU_W2)
GDCC_IR_CodeList(ModU_W3)

// Move dst src
GDCC_IR_CodeList(Move_W)
GDCC_IR_CodeList(Move_W2)
GDCC_IR_CodeList(Move_W3)

// Mul* dst lop rop
GDCC_IR_CodeList(MulF_W)
GDCC_IR_CodeList(MulF_W2)
GDCC_IR_CodeList(MulI_W)
GDCC_IR_CodeList(MulI_W2)
GDCC_IR_CodeList(MulI_W3)
GDCC_IR_CodeList(MulU_W)
GDCC_IR_CodeList(MulU_W2)
GDCC_IR_CodeList(MulU_W3)
GDCC_IR_CodeList(MulX_W)
GDCC_IR_CodeList(MulX_W2)

// Neg* dst src
GDCC_IR_CodeList(NegF_W)
GDCC_IR_CodeList(NegF_W2)
GDCC_IR_CodeList(NegI_W)
GDCC_IR_CodeList(NegI_W2)
GDCC_IR_CodeList(NegI_W3)

// Not* dst src
GDCC_IR_CodeList(NotU_W)
GDCC_IR_CodeList(NotU_W2)
GDCC_IR_CodeList(NotU_W3)

// OrI* dst lop rop
GDCC_IR_CodeList(OrIU_W)
GDCC_IR_CodeList(OrIU_W2)
GDCC_IR_CodeList(OrIU_W3)

// OrX* dst lop rop
GDCC_IR_CodeList(OrXU_W)
GDCC_IR_CodeList(OrXU_W2)
GDCC_IR_CodeList(OrXU_W3)

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
GDCC_IR_CodeList(ShLU_W)
GDCC_IR_CodeList(ShLU_W2)
GDCC_IR_CodeList(ShLU_W3)

// ShR* dst lop rop
GDCC_IR_CodeList(ShRI_W)
GDCC_IR_CodeList(ShRI_W2)
GDCC_IR_CodeList(ShRI_W3)
GDCC_IR_CodeList(ShRU_W)
GDCC_IR_CodeList(ShRU_W2)
GDCC_IR_CodeList(ShRU_W3)

// Sub* dst lop rop
GDCC_IR_CodeList(SubF_W)
GDCC_IR_CodeList(SubF_W2)
GDCC_IR_CodeList(SubI_W)
GDCC_IR_CodeList(SubI_W2)
GDCC_IR_CodeList(SubI_W3)
GDCC_IR_CodeList(SubU_W)
GDCC_IR_CodeList(SubU_W2)
GDCC_IR_CodeList(SubU_W3)

// Swap lop rop
GDCC_IR_CodeList(Swap_W)
GDCC_IR_CodeList(Swap_W2)
GDCC_IR_CodeList(Swap_W3)

// Xcod_SID (Static Initialization Delay)
GDCC_IR_CodeList(Xcod_SID)

#undef GDCC_IR_CodeList

// EOF

