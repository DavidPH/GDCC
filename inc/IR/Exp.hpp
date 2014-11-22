//-----------------------------------------------------------------------------
//
// Copyright (C) 2013-2014 David Hill
//
// See COPYING for license information.
//
//-----------------------------------------------------------------------------
//
// Intermediary Representation expression base.
//
//-----------------------------------------------------------------------------

#ifndef GDCC__IR__Exp_H__
#define GDCC__IR__Exp_H__

#include "../IR/Value.hpp"

#include "../Core/Counter.hpp"
#include "../Core/Origin.hpp"


//----------------------------------------------------------------------------|
// Macros                                                                     |
//

//
// GDCC_IR_Exp_DeclCreateE1
//
#define GDCC_IR_Exp_DeclCreateE1(name) \
   Exp::CRef ExpCreate_##name(Exp const *e); \
   Exp::CRef ExpCreate_##name(Exp const *e, Core::Origin pos); \
   Exp::CRef ExpGetIR_##name(IArchive &in)

//
// GDCC_IR_Exp_DeclCreateE2
//
#define GDCC_IR_Exp_DeclCreateE2(name) \
   Exp::CRef ExpCreate_##name(Exp const *l, Exp const *r); \
   Exp::CRef ExpCreate_##name(Exp const *l, Exp const *r, Core::Origin pos); \
   Exp::CRef ExpGetIR_##name(IArchive &in)

//
// GDCC_IR_Exp_DeclCreateE3
//
#define GDCC_IR_Exp_DeclCreateE3(name) \
   Exp::CRef ExpCreate_##name(Exp const *c, Exp const *l, Exp const *r); \
   Exp::CRef ExpCreate_##name(Exp const *c, Exp const *l, Exp const *r, \
      Core::Origin pos); \
   Exp::CRef ExpGetIR_##name(IArchive &in)

//
// GDCC_IR_Exp_DeclCreateTE
//
#define GDCC_IR_Exp_DeclCreateTE(name) \
   Exp::CRef ExpCreate_##name(Type const &t, Exp const *e); \
   Exp::CRef ExpCreate_##name(Type const &t, Exp const *e, Core::Origin pos); \
   Exp::CRef ExpCreate_##name(Type &&t, Exp const *e); \
   Exp::CRef ExpCreate_##name(Type &&t, Exp const *e, Core::Origin pos); \
   Exp::CRef ExpGetIR_##name(IArchive &in)


//----------------------------------------------------------------------------|
// Types                                                                      |
//

namespace GDCC
{
   namespace IR
   {
      class Glyph;
      template<typename T> struct GetIR_T;

      //
      // Exp
      //
      class Exp : public Core::Counter
      {
         GDCC_Core_CounterPreambleAbstract(GDCC::IR::Exp, GDCC::Core::Counter);

      public:
         bool operator == (Exp const &e) const;

         virtual Core::String getName() const = 0;

         Type getType() const;

         Value getValue() const;

         bool isNonzero() const;

         bool isValue() const {return v_isValue();}

         bool isZero() const;

         OArchive &putIR(OArchive &out) const;

         Core::Origin const pos;

      protected:
         Exp(Exp const &) = default;
         explicit Exp(Core::Origin pos_) : pos{pos_} {}
         explicit Exp(IArchive &in);

         virtual Type v_getType() const = 0;

         virtual Value v_getValue() const = 0;

         virtual bool v_isEqual(Exp const *e) const;

         virtual bool v_isValue() const = 0;

         virtual OArchive &v_putIR(OArchive &out) const;
      };

      //
      // GetIR_T<Exp::CPtr>
      //
      template<> struct GetIR_T<Exp::CPtr>
      {
         static Exp::CPtr GetIR_F(IArchive &in);
      };

      //
      // GetIR_T<Exp::CRef>
      //
      template<> struct GetIR_T<Exp::CRef>
      {
         static Exp::CRef GetIR_F(IArchive &in);
      };
   }
}


//----------------------------------------------------------------------------|
// Global Functions                                                           |
//

namespace GDCC
{
   namespace IR
   {
      OArchive &operator << (OArchive &out, Exp const *in);

      IArchive &operator >> (IArchive &in, Exp::CPtr &out);
      IArchive &operator >> (IArchive &in, Exp::CRef &out);

      GDCC_IR_Exp_DeclCreateE2(Add);
      GDCC_IR_Exp_DeclCreateE2(AddPtrRaw);
      GDCC_IR_Exp_DeclCreateE2(BitAnd);
      GDCC_IR_Exp_DeclCreateE2(BitOrI);
      GDCC_IR_Exp_DeclCreateE2(BitOrX);
      GDCC_IR_Exp_DeclCreateE2(CmpEQ);
      GDCC_IR_Exp_DeclCreateE2(CmpGE);
      GDCC_IR_Exp_DeclCreateE2(CmpGT);
      GDCC_IR_Exp_DeclCreateE2(CmpLE);
      GDCC_IR_Exp_DeclCreateE2(CmpLT);
      GDCC_IR_Exp_DeclCreateE2(CmpNE);
      GDCC_IR_Exp_DeclCreateE3(Cnd);
      GDCC_IR_Exp_DeclCreateTE(Cst);
      GDCC_IR_Exp_DeclCreateE2(Div);
      GDCC_IR_Exp_DeclCreateE1(Inv);
      GDCC_IR_Exp_DeclCreateE2(LogAnd);
      GDCC_IR_Exp_DeclCreateE2(LogOrI);
      GDCC_IR_Exp_DeclCreateE2(LogOrX);
      GDCC_IR_Exp_DeclCreateE2(Mod);
      GDCC_IR_Exp_DeclCreateE2(Mul);
      GDCC_IR_Exp_DeclCreateE1(Neg);
      GDCC_IR_Exp_DeclCreateE1(Not);
      GDCC_IR_Exp_DeclCreateE2(ShL);
      GDCC_IR_Exp_DeclCreateE2(ShR);
      GDCC_IR_Exp_DeclCreateE2(Sub);

      Exp::CRef ExpCreate_Array(Type const &elemT,
         Core::Array<Exp::CRef> const &elemV, Core::Origin pos);
      Exp::CRef ExpCreate_Array(Type const &elemT,
         Core::Array<Exp::CRef>      &&elemV, Core::Origin pos);
      Exp::CRef ExpCreate_Array(Type      &&elemT,
         Core::Array<Exp::CRef> const &elemV, Core::Origin pos);
      Exp::CRef ExpCreate_Array(Type      &&elemT,
         Core::Array<Exp::CRef>      &&elemV, Core::Origin pos);

      Exp::CRef ExpCreate_Assoc(Type_Assoc const &elemT,
         Core::Array<Exp::CRef> const &elemV, Core::Origin pos);
      Exp::CRef ExpCreate_Assoc(Type_Assoc const &elemT,
         Core::Array<Exp::CRef>      &&elemV, Core::Origin pos);
      Exp::CRef ExpCreate_Assoc(Type_Assoc      &&elemT,
         Core::Array<Exp::CRef> const &elemV, Core::Origin pos);
      Exp::CRef ExpCreate_Assoc(Type_Assoc      &&elemT,
         Core::Array<Exp::CRef>      &&elemV, Core::Origin pos);

      Exp::CRef ExpCreate_Glyph(Glyph glyph, Core::Origin pos);

      Exp::CRef ExpCreate_Tuple(
         Core::Array<Exp::CRef> const &elemV, Core::Origin pos);
      Exp::CRef ExpCreate_Tuple(
         Core::Array<Exp::CRef>      &&elemV, Core::Origin pos);

      Exp::CRef ExpCreate_Union(Type_Union const &elemT, Exp const *elemV,
         Core::Origin pos);
      Exp::CRef ExpCreate_Union(Type_Union &&elemT, Exp const *elemV,
         Core::Origin pos);

      Exp::CRef ExpCreate_Value(Value const &value, Core::Origin pos);
      Exp::CRef ExpCreate_Value(Value &&value, Core::Origin pos);

      Exp::CRef ExpCreate_Zero();

      Exp::CRef ExpGetIR_Array(IArchive &in);
      Exp::CRef ExpGetIR_Assoc(IArchive &in);
      Exp::CRef ExpGetIR_Glyph(IArchive &in);
      Exp::CRef ExpGetIR_Value(IArchive &in);
      Exp::CRef ExpGetIR_Tuple(IArchive &in);
      Exp::CRef ExpGetIR_Union(IArchive &in);
   }
}

namespace GDCC
{
   namespace IR
   {
      //
      // operator IArchive >> Exp::CPtr
      //
      inline IArchive &operator >> (IArchive &in, Exp::CPtr &out)
      {
         out = GetIR_T<Exp::CPtr>::GetIR_F(in);
         return in;
      }

      //
      // operator IArchive >> Exp::CRef
      //
      inline IArchive &operator >> (IArchive &in, Exp::CRef &out)
      {
         out = GetIR_T<Exp::CRef>::GetIR_F(in);
         return in;
      }
   }
}

#endif//GDCC__IR__Exp_H__

