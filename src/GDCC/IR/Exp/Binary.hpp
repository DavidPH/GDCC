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
// GDCC_IR_Exp_BinaryDecl
//
#define GDCC_IR_Exp_BinaryDecl(name) \
   class Exp_Binary##name : public Exp_Binary \
   { \
      GDCC_CounterPreamble(GDCC::IR::Exp_Binary##name, GDCC::IR::Exp_Binary); \
      \
   public: \
      virtual String getName() const {return STR_Binary##name;} \
      \
      friend Exp::CRef ExpCreate_Binary##name(Exp const *l, Exp const *r); \
      friend Exp::CRef ExpCreate_Binary##name(Exp const *l, Exp const *r, Origin pos); \
      friend Exp::CRef ExpGetIR_Binary##name(IArchive &in); \
      \
   protected: \
      Exp_Binary##name(Exp_Binary##name const &) = default; \
      Exp_Binary##name(Exp const *l, Exp const *r, Origin pos_) : Super{l, r, pos_} {} \
      explicit Exp_Binary##name(IArchive &in) : Super{in} {} \
      \
      virtual Type v_getType() const; \
      virtual Value v_getValue() const; \
   }

//
// GDCC_IR_Exp_BinaryImpl
//
#define GDCC_IR_Exp_BinaryImpl(name, op) \
   Exp::CRef ExpCreate_Binary##name(Exp const *l, Exp const *r) \
      {return static_cast<Exp::CRef>(new Exp_Binary##name(l, r, l->pos));} \
   \
   Exp::CRef ExpCreate_Binary##name(Exp const *l, Exp const *r, Origin pos) \
      {return static_cast<Exp::CRef>(new Exp_Binary##name(l, r, pos));} \
   \
   Exp::CRef ExpGetIR_Binary##name(IArchive &in) \
      {return static_cast<Exp::CRef>(new Exp_Binary##name(in));} \
   \
   Type Exp_Binary##name::v_getType() const \
      {return Type::Promote##name(expL->getType(), expR->getType());} \
   \
   Value Exp_Binary##name::v_getValue() const \
      {return expL->getValue() op expR->getValue();}


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
         Exp::CRef const expL, expR;

      protected:
         Exp_Binary(Exp_Binary const &) = default;
         Exp_Binary(Exp const *l, Exp const *r, Origin pos_) :
            Super{pos_}, expL{l}, expR{r} {}
         explicit Exp_Binary(IArchive &in);

         virtual bool v_canGetValue() const
            {return expL->canGetValue() && expR->canGetValue();}

         virtual OArchive &v_putIR(OArchive &out) const;
      };

      GDCC_IR_Exp_BinaryDecl(Add);
      GDCC_IR_Exp_BinaryDecl(And);
      GDCC_IR_Exp_BinaryDecl(Div);
      GDCC_IR_Exp_BinaryDecl(Mod);
      GDCC_IR_Exp_BinaryDecl(Mul);
      GDCC_IR_Exp_BinaryDecl(OrI);
      GDCC_IR_Exp_BinaryDecl(OrX);
      GDCC_IR_Exp_BinaryDecl(ShL);
      GDCC_IR_Exp_BinaryDecl(ShR);
      GDCC_IR_Exp_BinaryDecl(Sub);
   }
}

#endif//GDCC__IR__Exp__Binary_H__

