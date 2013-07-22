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
//-----------------------------------------------------------------------------

// Nop
GDCC_IR_CodeList(Nop)

// Add* dst lop rop
GDCC_IR_CodeList(AddU_W)

// Call src numArg numRet
GDCC_IR_CodeList(Call)

// Cjmp dat lab
GDCC_IR_CodeList(Cjmp_Nil)
GDCC_IR_CodeList(Cjmp_Tru)

// Cmp* dst lop rop
GDCC_IR_CodeList(CmpU_EQ_W)
GDCC_IR_CodeList(CmpU_GE_W)
GDCC_IR_CodeList(CmpU_GT_W)
GDCC_IR_CodeList(CmpU_LE_W)
GDCC_IR_CodeList(CmpU_LT_W)
GDCC_IR_CodeList(CmpU_NE_W)

// Jump src
GDCC_IR_CodeList(Jump)

// Move dst src
GDCC_IR_CodeList(Move_W)

// Not* dst src
GDCC_IR_CodeList(NotU_W)

// Retn src num
GDCC_IR_CodeList(Retn)

// Sub* dst lop rop
GDCC_IR_CodeList(SubU_W)

#undef GDCC_IR_CodeList

// EOF

