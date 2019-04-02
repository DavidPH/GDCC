//-----------------------------------------------------------------------------
//
// Copyright (C) 2013-2019 David Hill
//
// See COPYING for license information.
//
//-----------------------------------------------------------------------------
//
// List of all AddressSpace bases.
//
//-----------------------------------------------------------------------------

// Generic address space.
GDCC_Target_AddrList(Gen)    //             Generic

// Special address spaces.
GDCC_Target_AddrList(Cpy)    // __adr_cpy - Copy
GDCC_Target_AddrList(Lit)    //             Value
GDCC_Target_AddrList(Nul)    //             Void
GDCC_Target_AddrList(Stk)    //             Stack

// Intrinsic address spaces.
GDCC_Target_AddrList(Aut)    // __aut     - Automatic
GDCC_Target_AddrList(Far)    // __far     - Far
GDCC_Target_AddrList(GblArs) // __gbl_ars - Global Arrays
GDCC_Target_AddrList(GblReg) // __gbl_reg - Global Register
GDCC_Target_AddrList(HubArs) // __hub_ars - Hub Arrays
GDCC_Target_AddrList(HubReg) // __hub_reg - Hub Register
GDCC_Target_AddrList(LocReg) // __loc_reg - Local Register
GDCC_Target_AddrList(ModArs) // __mod_ars - Module Arrays
GDCC_Target_AddrList(ModReg) // __mod_reg - Module Register
GDCC_Target_AddrList(Sta)    // __sta     - Static
GDCC_Target_AddrList(StrArs) // __str_ars - String-table Arrays
GDCC_Target_AddrList(Vaa)    // __va_addr - Variadic Argument

// Declarable address spaces.
GDCC_Target_AddrList(GblArr) // __gbl_arr - Global Array
GDCC_Target_AddrList(HubArr) // __hub_arr - Hub Array
GDCC_Target_AddrList(LocArr) // __loc_arr - Local Array
GDCC_Target_AddrList(ModArr) // __mod_arr - Module Array
GDCC_Target_AddrList(StrArr) // __str_arr - String-table Array

#undef GDCC_Target_AddrList

// EOF

