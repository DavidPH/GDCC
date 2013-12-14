//-----------------------------------------------------------------------------
//
// Copyright (C) 2013 David Hill
//
// See COPYING for license information.
//
//-----------------------------------------------------------------------------
//
// List of all AddressSpace bases.
//
//-----------------------------------------------------------------------------

// Generic address space.
GDCC_IR_AddrList(Gen)    //             Generic

// Special address spaces.
GDCC_IR_AddrList(Cpy)    // __adr_cpy - Copy
GDCC_IR_AddrList(Lit)    //             Value
GDCC_IR_AddrList(Nul)    //             Void
GDCC_IR_AddrList(Stk)    //             Stack

// Intrinsic address spaces.
GDCC_IR_AddrList(Far)    // __far     - Far
GDCC_IR_AddrList(GblArs) // __gbl_ars - Global Arrays
GDCC_IR_AddrList(GblReg) // __gbl_reg - Global Register
GDCC_IR_AddrList(Loc)    // __loc     - Local
GDCC_IR_AddrList(LocArs) // __loc_ars - Local Arrays
GDCC_IR_AddrList(LocReg) // __loc_reg - Local Register
GDCC_IR_AddrList(MapArs) // __map_ars - Map Arrays
GDCC_IR_AddrList(MapReg) // __map_reg - Map Register
GDCC_IR_AddrList(StrArs) // __str_ars - String-table Arrays
GDCC_IR_AddrList(Vaa)    // __va_addr - Variadic Argument
GDCC_IR_AddrList(WldArs) // __wld_ars - World Arrays
GDCC_IR_AddrList(WldReg) // __wld_reg - World Register

// Declarable address spaces.
GDCC_IR_AddrList(GblArr) // __gbl_arr - Global Array
GDCC_IR_AddrList(MapArr) // __map_arr - Map Array
GDCC_IR_AddrList(StrArr) // __str_arr - String-table Array
GDCC_IR_AddrList(WldArr) // __wld_arr - World Array

#undef GDCC_IR_AddrList

// EOF

