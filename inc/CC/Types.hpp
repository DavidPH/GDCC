//-----------------------------------------------------------------------------
//
// Copyright (C) 2018 David Hill
//
// See COPYING for license information.
//
//-----------------------------------------------------------------------------
//
// Common typedefs and class forward declarations.
//
//-----------------------------------------------------------------------------

#ifndef GDCC__CC__Types_H__
#define GDCC__CC__Types_H__

#include "../AS/Types.hpp"
#include "../CPP/Types.hpp"
#include "../SR/Types.hpp"


//----------------------------------------------------------------------------|
// Types                                                                      |
//

namespace GDCC::CC
{
   class AsmGlyphTBuf;
   class Exp_AddPoint;
   class Exp_AddPointEq;
   class Exp_AddPtrRaw;
   class Exp_AddStrEntInt;
   class Exp_Assign;
   class Exp_Call;
   class Exp_CallLit;
   class Exp_CallStk;
   class Exp_ConvertBoolSoft_Fixed;
   class Exp_ConvertBoolSoft_Float;
   class Exp_ConvertBoolSoft_PtrInv;
   class Exp_ConvertPtr;
   class Exp_ConvertPtrInt;
   class Exp_ConvertPtrInv;
   class Exp_ConvertPtrLoc;
   class Exp_ConvertPtrSh;
   class Exp_ConvertVoid;
   class Exp_Deref;
   class Exp_Func;
   class Exp_GenSel;
   class Exp_Init;
   class Exp_JmpLng;
   class Exp_JmpSet;
   class Exp_Mem;
   class Exp_MemProp;
   class Exp_Not;
   class Exp_Obj;
   class Exp_ObjAut;
   class Exp_Refer;
   class Exp_SubPtrInt;
   class Exp_SubPtrIntEq;
   class Exp_SubPtrPtrW;
   class Exp_VaArg;
   class GenAssoc;
   class Init;
   class InitMem;
   class InitRaw;
   class InitRawDes;
   class Init_Aggregate;
   class Init_Array;
   class Init_Array0;
   class Init_Div;
   class Init_Struct;
   class Init_Union;
   class Init_Value;
   class Lookup;
   class Parser;
   class Scope;
   class Scope_Block;
   class Scope_Break;
   class Scope_Case;
   class Scope_Function;
   class Scope_Global;
   class Scope_Local;
   class Scope_Loop;
   class Statement_Asm;
   class Statement_FuncPre;
   class Statement_FuncPro;
   class Statement_Goto;
   class Statement_GotoDyn;
   class Statement_If;
   class Statement_Loop;
   class Statement_ReturnExp;
   class Statement_ReturnNul;
   class Statement_Switch;
   class StructProp;
   class StructPropArg;
   class TypeSpec;
   class Type_Bool;
   class Type_BoolSoft;
   class Type_Char;
   class Type_Div;
   class Type_Enum;
   class Type_Fixed;
   class Type_FixedBase;
   class Type_Fract;
   class Type_Float;
   class Type_Integ;
   class Type_Struct;
}

#endif//GDCC__CC__Types_H__

