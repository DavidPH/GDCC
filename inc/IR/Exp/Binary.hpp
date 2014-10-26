//-----------------------------------------------------------------------------
//
// Copyright (C) 2013-2014 David Hill
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

#include "../../IR/Exp.hpp"


//----------------------------------------------------------------------------|
// Macros                                                                     |
//

//
// GDCC_IR_Exp_BinaryDecl
//
#define GDCC_IR_Exp_BinaryDecl(name) \
   class Exp_##name : public Exp_Binary \
   { \
      GDCC_Core_CounterPreamble(GDCC::IR::Exp_##name, GDCC::IR::Exp_Binary); \
      \
   public: \
      virtual Core::String getName() const {return Core::STR_##name;} \
      \
      friend Exp::CRef ExpCreate_##name(Exp const *l, Exp const *r); \
      friend Exp::CRef ExpCreate_##name(Exp const *l, Exp const *r, \
         Core::Origin pos); \
      friend Exp::CRef ExpGetIR_##name(IArchive &in); \
      \
   protected: \
      Exp_##name(Exp_##name const &) = default; \
      Exp_##name(Exp const *l, Exp const *r, Core::Origin pos_) : \
         Super{l, r, pos_} {} \
      explicit Exp_##name(IArchive &in) : Super{in} {} \
      \
      virtual Type v_getType() const; \
      virtual Value v_getValue() const; \
   }

//
// GDCC_IR_Exp_BinaryImplCreate
//
#define GDCC_IR_Exp_BinaryImplCreate(name) \
   Exp::CRef ExpCreate_##name(Exp const *l, Exp const *r) \
      {return static_cast<Exp::CRef>(new Exp_##name(l, r, l->pos));} \
   \
   Exp::CRef ExpCreate_##name(Exp const *l, Exp const *r, Core::Origin pos) \
      {return static_cast<Exp::CRef>(new Exp_##name(l, r, pos));} \
   \
   Exp::CRef ExpGetIR_##name(IArchive &in) \
      {return static_cast<Exp::CRef>(new Exp_##name(in));}

//
// GDCC_IR_Exp_BinaryImpl
//
#define GDCC_IR_Exp_BinaryImpl(name, op) \
   GDCC_IR_Exp_BinaryImplCreate(name) \
   \
   Type Exp_##name::v_getType() const \
      {return Type::Promote##name(expL->getType(), expR->getType());} \
   \
   Value Exp_##name::v_getValue() const \
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
         GDCC_Core_CounterPreambleAbstract(
            GDCC::IR::Exp_Binary, GDCC::IR::Exp);

      public:
         Exp::CRef const expL, expR;

      protected:
         Exp_Binary(Exp_Binary const &) = default;
         Exp_Binary(Exp const *l, Exp const *r, Core::Origin pos_) :
            Super{pos_}, expL{l}, expR{r} {}
         explicit Exp_Binary(IArchive &in);

         virtual bool v_isValue() const
            {return expL->isValue() && expR->isValue();}

         virtual OArchive &v_putIR(OArchive &out) const;
      };

      GDCC_IR_Exp_BinaryDecl(Add);
      GDCC_IR_Exp_BinaryDecl(AddPtrRaw);
      GDCC_IR_Exp_BinaryDecl(BitAnd);
      GDCC_IR_Exp_BinaryDecl(BitOrI);
      GDCC_IR_Exp_BinaryDecl(BitOrX);
      GDCC_IR_Exp_BinaryDecl(Div);
      GDCC_IR_Exp_BinaryDecl(Mod);
      GDCC_IR_Exp_BinaryDecl(Mul);
      GDCC_IR_Exp_BinaryDecl(ShL);
      GDCC_IR_Exp_BinaryDecl(ShR);
      GDCC_IR_Exp_BinaryDecl(Sub);
   }
}

#endif//GDCC__IR__Exp__Binary_H__

