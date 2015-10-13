//-----------------------------------------------------------------------------
//
// Copyright (C) 2013-2015 David Hill
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
GDCC_IR_AddrList(Aut)    // __aut     - Automatic
GDCC_IR_AddrList(Far)    // __far     - Far
GDCC_IR_AddrList(GblArs) // __gbl_ars - Global Arrays
GDCC_IR_AddrList(GblReg) // __gbl_reg - Global Register
GDCC_IR_AddrList(HubArs) // __hub_ars - Hub Arrays
GDCC_IR_AddrList(HubReg) // __hub_reg - Hub Register
GDCC_IR_AddrList(LocReg) // __loc_reg - Local Register
GDCC_IR_AddrList(ModArs) // __mod_ars - Module Arrays
GDCC_IR_AddrList(ModReg) // __mod_reg - Module Register
GDCC_IR_AddrList(Sta)    // __sta     - Static
GDCC_IR_AddrList(StrArs) // __str_ars - String-table Arrays
GDCC_IR_AddrList(Vaa)    // __va_addr - Variadic Argument

// Declarable address spaces.
GDCC_IR_AddrList(GblArr) // __gbl_arr - Global Array
GDCC_IR_AddrList(HubArr) // __hub_arr - Hub Array
GDCC_IR_AddrList(LocArr) // __loc_arr - Local Array
GDCC_IR_AddrList(ModArr) // __mod_arr - Module Array
GDCC_IR_AddrList(StrArr) // __str_arr - String-table Array

#undef GDCC_IR_AddrList

// EOF

