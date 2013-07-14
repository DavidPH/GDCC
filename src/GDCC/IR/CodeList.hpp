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

// Call src numArg numRet
GDCC_IR_CodeList(Call)

// Move dst src
GDCC_IR_CodeList(Move_W)

// Retn src num
GDCC_IR_CodeList(Retn)

#undef GDCC_IR_CodeList

// EOF

