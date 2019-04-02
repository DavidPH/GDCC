//-----------------------------------------------------------------------------
//
// Copyright (C) 2014-2019 David Hill
//
// See COPYING for license information.
//
//-----------------------------------------------------------------------------
//
// List of all CallTypes.
//
//-----------------------------------------------------------------------------

// No calling convention.
GDCC_Target_CallTypeList(None)

// "Action Function". Purely theoretical at this time. Could be implemented in
// a distant version of ZDoom.
GDCC_Target_CallTypeList(Action)

// "Assembly Function". Used for ACS, which implements some native functions as
// special instructions.
GDCC_Target_CallTypeList(AsmFunc)

// Language calling conventions. Must be mapped to another call type for IR.
GDCC_Target_CallTypeList(LangACS) // extern "ACS"
GDCC_Target_CallTypeList(LangASM) // extern "asm"
GDCC_Target_CallTypeList(LangAXX) // extern "ACS++"
GDCC_Target_CallTypeList(LangC)   // extern "C"
GDCC_Target_CallTypeList(LangCXX) // extern "C++"
GDCC_Target_CallTypeList(LangDS)  // extern "DS"

// "Native Function". Used for targets with special native function calls.
GDCC_Target_CallTypeList(Native)

// "Synchronous Script". Used for ACS. Must be mapped for IR.
GDCC_Target_CallTypeList(SScript)

// "Numbered Synchronous Script". Used for ACS.
GDCC_Target_CallTypeList(SScriptI)

// "Named Synchronous Script". Used for ACS.
GDCC_Target_CallTypeList(SScriptS)

// "Script". Used for ACS. Must be mapped to another call type for IR.
GDCC_Target_CallTypeList(Script)

// "Numbered Script". Used for ACS.
GDCC_Target_CallTypeList(ScriptI)

// "Named Script". Used for ACS.
GDCC_Target_CallTypeList(ScriptS)

// "Line Special". Used for ACS.
GDCC_Target_CallTypeList(Special)

// "Standard Call". The normal calling convention which propagates the stack
// pointer.
GDCC_Target_CallTypeList(StdCall)

// "Stack Call". A minimal calling convention which does not propagate the
// stack pointer. Used for low-level libGDCC functions and LangACS mapping.
GDCC_Target_CallTypeList(StkCall)

#undef GDCC_Target_CallTypeList

// EOF

