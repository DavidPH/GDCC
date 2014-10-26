//-----------------------------------------------------------------------------
//
// Copyright (C) 2013-2014 David Hill
//
// See COPYING for license information.
//
//-----------------------------------------------------------------------------
//
// Intermediary Representation expression: unary base and utilities.
//
//-----------------------------------------------------------------------------

#ifndef GDCC__IR__Exp__Unary_H__
#define GDCC__IR__Exp__Unary_H__

#include "../../IR/Exp.hpp"


//----------------------------------------------------------------------------|
// Macros                                                                     |
//

//
// GDCC_IR_Exp_UnaryDeclBase
//
#define GDCC_IR_Exp_UnaryDeclBase(name) \
public: \
   virtual Core::String getName() const {return Core::STR_##name;} \
   \
protected: \
   virtual Value v_getValue() const

//
// GDCC_IR_Exp_UnaryDeclClass
//
#define GDCC_IR_Exp_UnaryDeclClass(name) \
   class Exp_##name : public Exp_Unary \
   { \
      GDCC_Core_CounterPreamble(GDCC::IR::Exp_##name, GDCC::IR::Exp_Unary); \
      GDCC_IR_Exp_UnaryDeclBase(name); \
      GDCC_IR_Exp_UnaryDeclCon(name); \
      GDCC_IR_Exp_UnaryDeclCreate(name); \
   }

//
// GDCC_IR_Exp_UnaryDeclCon
//
#define GDCC_IR_Exp_UnaryDeclCon(name) \
protected: \
   Exp_##name(Exp const *e, Core::Origin pos_) : Super{e, pos_} {} \
   explicit Exp_##name(IArchive &in) : Super{in} {} \
   Exp_##name(Exp_##name const &) = default

//
// GDCC_IR_Exp_UnaryDeclCreate
//
#define GDCC_IR_Exp_UnaryDeclCreate(name) \
public: \
   friend Exp::CRef ExpCreate_##name(Exp const *e); \
   friend Exp::CRef ExpCreate_##name(Exp const *e, Core::Origin pos); \
   friend Exp::CRef ExpGetIR_##name(IArchive &in)

//
// GDCC_IR_Exp_UnaryImplCreate
//
#define GDCC_IR_Exp_UnaryImplCreate(name) \
   Exp::CRef ExpCreate_##name(Exp const *e) \
      {return static_cast<Exp::CRef>(new Exp_##name(e, e->pos));} \
   \
   Exp::CRef ExpCreate_##name(Exp const *e, Core::Origin pos) \
      {return static_cast<Exp::CRef>(new Exp_##name(e, pos));} \
   \
   Exp::CRef ExpGetIR_##name(IArchive &in) \
      {return static_cast<Exp::CRef>(new Exp_##name(in));}


//----------------------------------------------------------------------------|
// Types                                                                      |
//

namespace GDCC
{
   namespace IR
   {
      //
      // Exp_Unary
      //
      class Exp_Unary : public Exp
      {
         GDCC_Core_CounterPreambleAbstract(GDCC::IR::Exp_Unary, GDCC::IR::Exp);

      public:
         Exp::CRef const exp;

      protected:
         Exp_Unary(Exp_Unary const &) = default;
         Exp_Unary(Exp const *e, Core::Origin pos_) : Super{pos_}, exp{e} {}
         explicit Exp_Unary(IArchive &in);

         virtual Type v_getType() const {return exp->getType();}

         virtual bool v_isValue() const
            {return exp->isValue();}

         virtual OArchive &v_putIR(OArchive &out) const;
      };

      GDCC_IR_Exp_UnaryDeclClass(Inv);
      GDCC_IR_Exp_UnaryDeclClass(Neg);

      //
      // Exp_Cst
      //
      class Exp_Cst : public Exp_Unary
      {
         GDCC_Core_CounterPreamble(GDCC::IR::Exp_Cst, GDCC::IR::Exp_Unary);

      public:
         Type const type;

         virtual Core::String getName() const {return Core::STR_Cst;}


         friend Exp::CRef ExpCreate_Cst(Type const &t, Exp const *e);
         friend Exp::CRef ExpCreate_Cst(Type const &t, Exp const *e,
            Core::Origin pos);
         friend Exp::CRef ExpCreate_Cst(Type &&t, Exp const *e);
         friend Exp::CRef ExpCreate_Cst(Type &&t, Exp const *e,
            Core::Origin pos);

         friend Exp::CRef ExpGetIR_Cst(IArchive &in);

      protected:
         Exp_Cst(Exp_Cst const &) = default;
         Exp_Cst(Type const &t, Exp const *e, Core::Origin pos_) :
            Super{e, pos_} , type{t} {}
         Exp_Cst(Type &&t, Exp const *e, Core::Origin pos_) :
            Super{e, pos_} , type{std::move(t)} {}
         explicit Exp_Cst(IArchive &in);

         virtual Type v_getType() const {return type;}

         virtual Value v_getValue() const;

         virtual OArchive &v_putIR(OArchive &out) const;
      };
   }
}

#endif//GDCC__IR__Exp__Unary_H__

