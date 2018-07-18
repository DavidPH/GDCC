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

#ifndef GDCC__SR__Types_H__
#define GDCC__SR__Types_H__

#include "../IR/Types.hpp"


//----------------------------------------------------------------------------|
// Types                                                                      |
//

namespace GDCC::SR
{
   enum class Storage;
   enum class TypeRankC;

   class Arg;
   class Attribute;
   class Exp;
   class Exp_Add;
   class Exp_Arg;
   template<typename Base>
   class Exp_Arith;
   template<typename Base>
   class Exp_ArithEq;
   class Exp_BitAnd;
   class Exp_BitOrI;
   class Exp_BitOrX;
   class Exp_Binary;
   class Exp_CmpEQ;
   class Exp_CmpGE;
   class Exp_CmpGT;
   class Exp_CmpLE;
   class Exp_CmpLT;
   class Exp_CmpNE;
   class Exp_Cnd;
   class Exp_Convert;
   class Exp_ConvertArith;
   class Exp_ConvertBitfield;
   class Exp_Defer;
   class Exp_Div;
   class Exp_DivEx;
   class Exp_IRExp;
   class Exp_Inv;
   class Exp_LogAnd;
   class Exp_LogOrI;
   class Exp_Mod;
   class Exp_Mul;
   class Exp_MulSize;
   class Exp_Neg;
   class Exp_Pair;
   class Exp_ShL;
   class Exp_ShR;
   class Exp_Sub;
   class Exp_Unary;
   template<typename Base>
   class Exp_UnaryCode;
   class Function;
   class GenStmntCtx;
   class Object;
   class Space;
   class Statement;
   class Statement_Empty;
   class Statement_Exp;
   class Statement_Multi;
   class Temporary;
   class Type;
   class TypeError;
   class TypeMember;
   class TypeQual;
   class TypeSet;
   class Type_ArrBase;
   class Type_ArrVM;
   class Type_ArrVM0;
   class Type_Array;
   class Type_Array0;
   class Type_Bitfield;
   class Type_Function;
   class Type_Label;
   class Type_None;
   class Type_Pointer;
   class Type_PtrBase;
   class Type_RefL;
   class Type_RefR;
   class Type_Size;
   class Type_StrEnt;
   class Type_Void;
   class Type_Wrapper;
}

#endif//GDCC__SR__Types_H__

