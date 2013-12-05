//-----------------------------------------------------------------------------
//
// Copyright (C) 2013 David Hill
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

#include "../Exp.hpp"


//----------------------------------------------------------------------------|
// Macros                                                                     |
//

//
// GDCC_IR_Exp_UnaryDeclBase
//
#define GDCC_IR_Exp_UnaryDeclBase(name) \
public: \
   virtual String getName() const {return STR_Unary##name;} \
   \
protected: \
   virtual Value v_getValue() const

//
// GDCC_IR_Exp_UnaryDeclClass
//
#define GDCC_IR_Exp_UnaryDeclClass(name) \
   class Exp_Unary##name : public Exp_Unary \
   { \
      GDCC_CounterPreamble(GDCC::IR::Exp_Unary##name, GDCC::IR::Exp_Unary); \
      GDCC_IR_Exp_UnaryDeclBase(name); \
      GDCC_IR_Exp_UnaryDeclCon(name); \
      GDCC_IR_Exp_UnaryDeclCreate(name); \
   }

//
// GDCC_IR_Exp_UnaryDeclCon
//
#define GDCC_IR_Exp_UnaryDeclCon(name) \
protected: \
   Exp_Unary##name(Exp const *e, Origin pos_) : Super{e, pos_} {} \
   explicit Exp_Unary##name(IArchive &in) : Super{in} {} \
   Exp_Unary##name(Exp_Unary##name const &) = default

//
// GDCC_IR_Exp_UnaryDeclCreate
//
#define GDCC_IR_Exp_UnaryDeclCreate(name) \
public: \
   friend Exp::CRef ExpCreate_Unary##name(Exp const *e); \
   friend Exp::CRef ExpCreate_Unary##name(Exp const *e, Origin pos); \
   friend Exp::CRef ExpGetIR_Unary##name(IArchive &in)

//
// GDCC_IR_Exp_UnaryImplCreate
//
#define GDCC_IR_Exp_UnaryImplCreate(name) \
   Exp::CRef ExpCreate_Unary##name(Exp const *e) \
      {return static_cast<Exp::CRef>(new Exp_Unary##name(e, e->pos));} \
   \
   Exp::CRef ExpCreate_Unary##name(Exp const *e, Origin pos) \
      {return static_cast<Exp::CRef>(new Exp_Unary##name(e, pos));} \
   \
   Exp::CRef ExpGetIR_Unary##name(IArchive &in) \
      {return static_cast<Exp::CRef>(new Exp_Unary##name(in));}


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
         GDCC_CounterPreambleAbstract(GDCC::IR::Exp_Unary, GDCC::IR::Exp);

      public:
         Exp::CRef const exp;

      protected:
         Exp_Unary(Exp_Unary const &) = default;
         Exp_Unary(Exp const *e, Origin pos_) : Super{pos_}, exp{e} {}
         explicit Exp_Unary(IArchive &in);

         virtual bool v_canGetValue() const
            {return exp->canGetValue();}

         virtual Type v_getType() const {return exp->getType();}

         virtual OArchive &v_putIR(OArchive &out) const;
      };

      GDCC_IR_Exp_UnaryDeclClass(Add);
      GDCC_IR_Exp_UnaryDeclClass(Not);
      GDCC_IR_Exp_UnaryDeclClass(Sub);

      //
      // Exp_UnaryCst
      //
      class Exp_UnaryCst : public Exp_Unary
      {
         GDCC_CounterPreamble(GDCC::IR::Exp_UnaryCst, GDCC::IR::Exp_Unary);

      public:
         Type const type;

         virtual String getName() const {return STR_UnaryCst;}


         friend Exp::CRef ExpCreate_UnaryCst(Type const &t, Exp const *e);
         friend Exp::CRef ExpCreate_UnaryCst(Type const &t, Exp const *e, Origin pos);
         friend Exp::CRef ExpCreate_UnaryCst(Type &&t, Exp const *e);
         friend Exp::CRef ExpCreate_UnaryCst(Type &&t, Exp const *e, Origin pos);

         friend Exp::CRef ExpGetIR_UnaryCst(IArchive &in);

      protected:
         Exp_UnaryCst(Exp_UnaryCst const &) = default;
         Exp_UnaryCst(Type const &t, Exp const *e, Origin pos_) :
            Super{e, pos_} , type{t} {}
         Exp_UnaryCst(Type &&t, Exp const *e, Origin pos_) :
            Super{e, pos_} , type{std::move(t)} {}
         explicit Exp_UnaryCst(IArchive &in);

         virtual Type v_getType() const {return type;}

         virtual Value v_getValue() const;

         virtual OArchive &v_putIR(OArchive &out) const;
      };
   }
}

#endif//GDCC__IR__Exp__Unary_H__

