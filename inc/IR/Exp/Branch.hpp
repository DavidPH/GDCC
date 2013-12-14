//-----------------------------------------------------------------------------
//
// Copyright (C) 2013 David Hill
//
// See COPYING for license information.
//
//-----------------------------------------------------------------------------
//
// Intermediary Representation expression: branch base and utilities.
//
//-----------------------------------------------------------------------------

#ifndef GDCC__IR__Exp__Branch_H__
#define GDCC__IR__Exp__Branch_H__

#include "../../IR/Exp.hpp"


//----------------------------------------------------------------------------|
// Macros                                                                     |
//

//
// GDCC_IR_Exp_BraBinDeclClass
//
#define GDCC_IR_Exp_BraBinDeclClass(name) \
   class Exp_Branch##name : public Exp_BraBin \
   { \
      GDCC_Core_CounterPreamble( \
         GDCC::IR::Exp_Branch##name, GDCC::IR::Exp_BraBin); \
      GDCC_IR_Exp_BraBinDeclCon(name); \
      GDCC_IR_Exp_BraBinDeclCreate(name); \
      GDCC_IR_Exp_BranchDeclBase(name); \
   }

//
// GDCC_IR_Exp_BraBinDeclCon
//
#define GDCC_IR_Exp_BraBinDeclCon(name) \
protected: \
   Exp_Branch##name(Exp const *l, Exp const *r, Core::Origin pos_) : \
      Super{l, r, pos_} {} \
   explicit Exp_Branch##name(IArchive &in) : Super{in} {} \
   Exp_Branch##name(Exp_Branch##name const &) = default

//
// GDCC_IR_Exp_BraBinDeclCreate
//
#define GDCC_IR_Exp_BraBinDeclCreate(name) \
public: \
   friend Exp::CRef ExpCreate_Branch##name(Exp const *l, Exp const *r); \
   friend Exp::CRef ExpCreate_Branch##name(Exp const *l, Exp const *r, \
      Core::Origin pos); \
   friend Exp::CRef ExpGetIR_Branch##name(IArchive &in)

//
// GDCC_IR_Exp_BraBinImplCreate
//
// Implements normal ExpCreate and ExpGetIR functions for binary expressions.
//
#define GDCC_IR_Exp_BraBinImplCreate(name) \
   Exp::CRef ExpCreate_Branch##name(Exp const *l, Exp const *r) \
      {return static_cast<Exp::Ref>(new Exp_Branch##name(l, r, l->pos));} \
   \
   Exp::CRef ExpCreate_Branch##name(Exp const *l, Exp const *r, Core::Origin pos) \
      {return static_cast<Exp::Ref>(new Exp_Branch##name(l, r, pos));} \
   \
   Exp::CRef ExpGetIR_Branch##name(IArchive &in) \
      {return static_cast<Exp::Ref>(new Exp_Branch##name(in));}

//
// GDCC_IR_Exp_BraTerDeclClass
//
#define GDCC_IR_Exp_BraTerDeclClass(name) \
   class Exp_Branch##name : public Exp_BraTer \
   { \
      GDCC_Core_CounterPreamble( \
         GDCC::IR::Exp_Branch##name, GDCC::IR::Exp_BraTer); \
      GDCC_IR_Exp_BraTerDeclCon(name); \
      GDCC_IR_Exp_BraTerDeclCreate(name); \
      GDCC_IR_Exp_BranchDeclBase(name); \
      \
   protected: \
      virtual Type v_getType() const; \
   }

//
// GDCC_IR_Exp_BraTerDeclCon
//
#define GDCC_IR_Exp_BraTerDeclCon(name) \
protected: \
   Exp_Branch##name(Exp const *c, Exp const *l, Exp const *r, Core::Origin pos_) : \
      Super{c, l, r, pos_} {} \
   explicit Exp_Branch##name(IArchive &in) : Super{in} {} \
   Exp_Branch##name(Exp_Branch##name const &) = default

//
// GDCC_IR_Exp_BraTerDeclCreate
//
#define GDCC_IR_Exp_BraTerDeclCreate(name) \
public: \
   friend Exp::CRef ExpCreate_Branch##name(Exp const *c, Exp const *l, \
      Exp const *r); \
   friend Exp::CRef ExpCreate_Branch##name(Exp const *c, Exp const *l, \
      Exp const *r, Core::Origin pos); \
   friend Exp::CRef ExpGetIR_Branch##name(IArchive &in)

//
// GDCC_IR_Exp_BraTerImplCreate
//
// Implements normal ExpCreate and ExpGetIR functions for ternary expressions.
//
#define GDCC_IR_Exp_BraTerImplCreate(name) \
   Exp::CRef ExpCreate_Branch##name(Exp const *c, Exp const *l, Exp const *r) \
      {return static_cast<Exp::CRef>(new Exp_Branch##name(c, l, r, c->pos));} \
   \
   Exp::CRef ExpCreate_Branch##name(Exp const *c, Exp const *l, Exp const *r, \
      Core::Origin pos) \
      {return static_cast<Exp::CRef>(new Exp_Branch##name(c, l, r, pos));} \
   \
   Exp::CRef ExpGetIR_Branch##name(IArchive &in) \
      {return static_cast<Exp::CRef>(new Exp_Branch##name(in));}

//
// GDCC_IR_Exp_BraUnaDeclClass
//
#define GDCC_IR_Exp_BraUnaDeclClass(name) \
   class Exp_Branch##name : public Exp_BraUna \
   { \
      GDCC_Core_CounterPreamble( \
         GDCC::IR::Exp_Branch##name, GDCC::IR::Exp_BraUna); \
      GDCC_IR_Exp_BraUnaDeclCon(name); \
      GDCC_IR_Exp_BraUnaDeclCreate(name); \
      GDCC_IR_Exp_BranchDeclBase(name); \
   }

//
// GDCC_IR_Exp_BraUnaDeclCon
//
#define GDCC_IR_Exp_BraUnaDeclCon(name) \
protected: \
   Exp_Branch##name(Exp const *e, Core::Origin pos_) : Super{e, pos_} {} \
   explicit Exp_Branch##name(IArchive &in) : Super{in} {} \
   Exp_Branch##name(Exp_Branch##name const &) = default

//
// GDCC_IR_Exp_BraUnaDeclCreate
//
#define GDCC_IR_Exp_BraUnaDeclCreate(name) \
public: \
   friend Exp::CRef ExpCreate_Branch##name(Exp const *e); \
   friend Exp::CRef ExpCreate_Branch##name(Exp const *e, Core::Origin pos); \
   friend Exp::CRef ExpGetIR_Branch##name(IArchive &in)

//
// GDCC_IR_Exp_BraUnaImplCreate
//
// Implements normal ExpCreate and ExpGetIR functions for binary expressions.
//
#define GDCC_IR_Exp_BraUnaImplCreate(name) \
   Exp::CRef ExpCreate_Branch##name(Exp const *e) \
      {return static_cast<Exp::CRef>(new Exp_Branch##name(e, e->pos));} \
   \
   Exp::CRef ExpCreate_Branch##name(Exp const *e, Core::Origin pos) \
      {return static_cast<Exp::CRef>(new Exp_Branch##name(e, pos));} \
   \
   Exp::CRef ExpGetIR_Branch##name(IArchive &in) \
      {return static_cast<Exp::CRef>(new Exp_Branch##name(in));}

//
// GDCC_IR_Exp_BranchDeclBase
//
#define GDCC_IR_Exp_BranchDeclBase(name) \
public: \
   virtual Core::String getName() const {return Core::STR_Branch##name;} \
   \
protected: \
   virtual Value v_getValue() const

//
// GDCC_IR_Exp_BranchImplCmpValue
//
#define GDCC_IR_Exp_BranchImplCmpValue(name, op) \
   Value Exp_Branch##name::v_getValue() const \
   { \
      return Value_Fixed(expL->getValue() op expR->getValue(), BoolRaw); \
   }


//----------------------------------------------------------------------------|
// Types                                                                      |
//

namespace GDCC
{
   namespace IR
   {
      //
      // Exp_Branch
      //
      class Exp_Branch : public Exp
      {
         GDCC_Core_CounterPreambleAbstract(GDCC::IR::Exp_Branch, GDCC::IR::Exp);

      public:
         static Type_Fixed const BoolRaw;
         static Type const BoolType;

      protected:
         Exp_Branch(Exp_Branch const &) = default;
         Exp_Branch(Core::Origin pos_) : Super{pos_} {}
         explicit Exp_Branch(IArchive &in) : Super{in} {}

         virtual Type v_getType() const {return BoolRaw;}
      };

      //
      // Exp_BraBin
      //
      class Exp_BraBin : public Exp_Branch
      {
         GDCC_Core_CounterPreambleAbstract(
            GDCC::IR::Exp_BraBin, GDCC::IR::Exp_Branch);

      public:
         Exp::CRef const expL, expR;

      protected:
         Exp_BraBin(Exp_BraBin const &) = default;
         Exp_BraBin(Exp const *l, Exp const *r, Core::Origin pos_) :
            Super{pos_}, expL{l}, expR{r} {}
         explicit Exp_BraBin(IArchive &in);

         virtual bool v_canGetValue() const
            {return expL->canGetValue() && expR->canGetValue();}

         virtual OArchive &v_putIR(OArchive &out) const;
      };

      //
      // Exp_BraTer
      //
      class Exp_BraTer : public Exp_BraBin
      {
         GDCC_Core_CounterPreambleAbstract(
            GDCC::IR::Exp_BraTer, GDCC::IR::Exp_BraBin);

      public:
         Exp::CRef const expC;

      protected:
         Exp_BraTer(Exp_BraTer const &) = default;
         Exp_BraTer(Exp const *c, Exp const *l, Exp const *r, Core::Origin pos_) :
            Super{l, r, pos_}, expC{c} {}
         explicit Exp_BraTer(IArchive &in);

         virtual bool v_canGetValue() const
            {return Super::v_canGetValue() && expC->canGetValue();}

         virtual OArchive &v_putIR(OArchive &out) const;
      };

      //
      // Exp_BraUna
      //
      class Exp_BraUna : public Exp_Branch
      {
         GDCC_Core_CounterPreambleAbstract(
            GDCC::IR::Exp_BraUna, GDCC::IR::Exp_Branch);

      public:
         Exp::CRef const exp;

      protected:
         Exp_BraUna(Exp_BraUna const &) = default;
         Exp_BraUna(Exp const *e, Core::Origin pos_) : Super{pos_}, exp{e} {}
         explicit Exp_BraUna(IArchive &in);

         virtual bool v_canGetValue() const
            {return exp->canGetValue();}

         virtual OArchive &v_putIR(OArchive &out) const;
      };

      GDCC_IR_Exp_BraBinDeclClass(And);
      GDCC_IR_Exp_BraBinDeclClass(CmpEQ);
      GDCC_IR_Exp_BraBinDeclClass(CmpGE);
      GDCC_IR_Exp_BraBinDeclClass(CmpGT);
      GDCC_IR_Exp_BraBinDeclClass(CmpLE);
      GDCC_IR_Exp_BraBinDeclClass(CmpLT);
      GDCC_IR_Exp_BraBinDeclClass(CmpNE);
      GDCC_IR_Exp_BraTerDeclClass(Cnd);
      GDCC_IR_Exp_BraUnaDeclClass(Not);
      GDCC_IR_Exp_BraBinDeclClass(OrI);
      GDCC_IR_Exp_BraBinDeclClass(OrX);
   }
}

#endif//GDCC__IR__Exp__Branch_H__

