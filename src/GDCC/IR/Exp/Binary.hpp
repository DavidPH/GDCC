//-----------------------------------------------------------------------------
//
// Copyright (C) 2013 David Hill
//
// See COPYING for license information.
//
//-----------------------------------------------------------------------------
//
// Intermediary Representation expression: binary base and utilities.
//
//-----------------------------------------------------------------------------

#ifndef GDCC__IR__Exp__Binary_H__
#define GDCC__IR__Exp__Binary_H__

#include "../Exp.hpp"


//----------------------------------------------------------------------------|
// Macros                                                                     |
//

//
// GDCC_IR_Exp_BinaryDeclBase
//
#define GDCC_IR_Exp_BinaryDeclBase(name) \
public: \
   virtual String getName() const {return STR_Binary##name;} \
   \
protected: \
   virtual Value v_getValue() const

//
// GDCC_IR_Exp_BinaryDeclClass
//
#define GDCC_IR_Exp_BinaryDeclClass(name) \
   class Exp_Binary##name : public Exp_Binary \
   { \
      GDCC_CounterPreamble(GDCC::IR::Exp_Binary##name, GDCC::IR::Exp_Binary); \
      GDCC_IR_Exp_BinaryDeclBase(name); \
      GDCC_IR_Exp_BinaryDeclCon(name); \
      GDCC_IR_Exp_BinaryDeclCreate(name); \
   }

//
// GDCC_IR_Exp_BinaryDeclCon
//
#define GDCC_IR_Exp_BinaryDeclCon(name) \
protected: \
   Exp_Binary##name(Exp *l, Exp *r, Origin pos_) : Super{l, r, pos_} {} \
   explicit Exp_Binary##name(IArchive &in) : Super{in} {} \
   Exp_Binary##name(Exp_Binary##name const &) = default

//
// GDCC_IR_Exp_BinaryDeclCreate
//
#define GDCC_IR_Exp_BinaryDeclCreate(name) \
public: \
   friend Exp::Ref ExpCreate_Binary##name(Exp *l, Exp *r); \
   friend Exp::Ref ExpCreate_Binary##name(Exp *l, Exp *r, Origin pos); \
   friend Exp::Ref ExpGetIR_Binary##name(IArchive &in)

//
// GDCC_IR_Exp_BinaryImplArith
//
#define GDCC_IR_Exp_BinaryImplArith(op) \
   if(l.v == r.v) switch(l.v) \
   { \
   case ValueBase::Fixed: return l.vFixed op r.vFixed; \
   default: return Value_Empty(); \
   }

//
// GDCC_IR_Exp_BinaryImplCreate
//
// Implements normal ExpCreate and ExpGetIR functions for binary expressions.
//
#define GDCC_IR_Exp_BinaryImplCreate(name) \
   Exp::Ref ExpCreate_Binary##name(Exp *l, Exp *r) \
      {return static_cast<Exp::Ref>(new Exp_Binary##name(l, r, l->pos));} \
   \
   Exp::Ref ExpCreate_Binary##name(Exp *l, Exp *r, Origin pos) \
      {return static_cast<Exp::Ref>(new Exp_Binary##name(l, r, pos));} \
   \
   Exp::Ref ExpGetIR_Binary##name(IArchive &in) \
      {return static_cast<Exp::Ref>(new Exp_Binary##name(in));}

//
// GDCC__IR_Exp_BinaryImplValue
//
#define GDCC_IR_Exp_BinaryImplValue(name, op) \
   Value Exp_Binary##name::v_getValue() const \
   { \
      auto l = expL->getValue(), r = expR->getValue(); \
      GDCC_IR_Exp_BinaryImplArith(op); \
      return Value_Empty(); \
   }


//----------------------------------------------------------------------------|
// Types                                                                      |
//

namespace GDCC
{
   namespace IR
   {
      //
      // Exp_Binary
      //
      class Exp_Binary : public Exp
      {
         GDCC_CounterPreambleAbstract(GDCC::IR::Exp_Binary, GDCC::IR::Exp);

      public:
         Exp::Ref const expL, expR;

      protected:
         Exp_Binary(Exp_Binary const &) = default;
         Exp_Binary(Exp *l, Exp *r, Origin pos_) : Super{pos_}, expL{l}, expR{r} {}
         explicit Exp_Binary(IArchive &in);

         virtual Type v_getType() const;
         virtual Type v_getTypeInequal(Type const &l, Type const &r) const;

         virtual OArchive &v_putIR(OArchive &out) const;
      };

      GDCC_IR_Exp_BinaryDeclClass(Add);
      GDCC_IR_Exp_BinaryDeclClass(And);
      GDCC_IR_Exp_BinaryDeclClass(Div);
      GDCC_IR_Exp_BinaryDeclClass(Mod);
      GDCC_IR_Exp_BinaryDeclClass(Mul);
      GDCC_IR_Exp_BinaryDeclClass(OrI);
      GDCC_IR_Exp_BinaryDeclClass(OrX);
      GDCC_IR_Exp_BinaryDeclClass(ShL);
      GDCC_IR_Exp_BinaryDeclClass(ShR);
      GDCC_IR_Exp_BinaryDeclClass(Sub);
   }
}

#endif//GDCC__IR__Exp__Binary_H__

