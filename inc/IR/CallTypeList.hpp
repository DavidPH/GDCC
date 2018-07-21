//-----------------------------------------------------------------------------
//
// Copyright (C) 2014-2018 David Hill
//
// See COPYING for license information.
//
//-----------------------------------------------------------------------------
//
// List of all CallTypes.
//
//-----------------------------------------------------------------------------

// No calling convention.
GDCC_IR_CallTypeList(None)

// "Action Function". Purely theoretical at this time. Could be implemented in
// a distant version of ZDoom.
GDCC_IR_CallTypeList(Action)

// "Assembly Function". Used for ACS, which implements some native functions as
// special instructions.
GDCC_IR_CallTypeList(AsmFunc)

// Language calling conventions. Must be mapped to another call type for IR.
GDCC_IR_CallTypeList(LangACS) // extern "ACS"
GDCC_IR_CallTypeList(LangASM) // extern "asm"
GDCC_IR_CallTypeList(LangAXX) // extern "ACS++"
GDCC_IR_CallTypeList(LangC)   // extern "C"
GDCC_IR_CallTypeList(LangCXX) // extern "C++"
GDCC_IR_CallTypeList(LangDS)  // extern "DS"

// "Native Function". Used for targets with special native function calls.
GDCC_IR_CallTypeList(Native)

// "Synchronous Script". Used for ACS. Must be mapped for IR.
GDCC_IR_CallTypeList(SScript)

// "Numbered Synchronous Script". Used for ACS.
GDCC_IR_CallTypeList(SScriptI)

// "Named Synchronous Script". Used for ACS.
GDCC_IR_CallTypeList(SScriptS)

// "Script". Used for ACS. Must be mapped to another call type for IR.
GDCC_IR_CallTypeList(Script)

// "Numbered Script". Used for ACS.
GDCC_IR_CallTypeList(ScriptI)

// "Named Script". Used for ACS.
GDCC_IR_CallTypeList(ScriptS)

// "Line Special". Used for ACS.
GDCC_IR_CallTypeList(Special)

// "Standard Call". The normal calling convention which propagates the stack
// pointer.
GDCC_IR_CallTypeList(StdCall)

// "Stack Call". A minimal calling convention which does not propagate the
// stack pointer. Used for low-level libGDCC functions and LangACS mapping.
GDCC_IR_CallTypeList(StkCall)

#undef GDCC_IR_CallTypeList

// EOF

