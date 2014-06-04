//-----------------------------------------------------------------------------
//
// Copyright (C) 2013 David Hill
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
         bool canGetValue() const {return v_canGetValue();}

         virtual Core::String getName() const = 0;

         Type getType() const;

         Value getValue() const;

         OArchive &putIR(OArchive &out) const;

         Core::Origin const pos;

      protected:
         Exp(Exp const &) = default;
         explicit Exp(Core::Origin pos_) : pos{pos_} {}
         explicit Exp(IArchive &in);

         virtual bool v_canGetValue() const = 0;

         virtual Type v_getType() const = 0;

         virtual Value v_getValue() const = 0;

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

      GDCC_IR_Exp_DeclCreateE2(BinaryAdd);
      GDCC_IR_Exp_DeclCreateE2(BinaryAddPtrRaw);
      GDCC_IR_Exp_DeclCreateE2(BinaryAnd);
      GDCC_IR_Exp_DeclCreateE2(BinaryDiv);
      GDCC_IR_Exp_DeclCreateE2(BinaryMod);
      GDCC_IR_Exp_DeclCreateE2(BinaryMul);
      GDCC_IR_Exp_DeclCreateE2(BinaryOrI);
      GDCC_IR_Exp_DeclCreateE2(BinaryOrX);
      GDCC_IR_Exp_DeclCreateE2(BinaryShL);
      GDCC_IR_Exp_DeclCreateE2(BinaryShR);
      GDCC_IR_Exp_DeclCreateE2(BinarySub);

      GDCC_IR_Exp_DeclCreateE2(BranchAnd);
      GDCC_IR_Exp_DeclCreateE2(BranchCmpEQ);
      GDCC_IR_Exp_DeclCreateE2(BranchCmpGE);
      GDCC_IR_Exp_DeclCreateE2(BranchCmpGT);
      GDCC_IR_Exp_DeclCreateE2(BranchCmpLE);
      GDCC_IR_Exp_DeclCreateE2(BranchCmpLT);
      GDCC_IR_Exp_DeclCreateE2(BranchCmpNE);
      GDCC_IR_Exp_DeclCreateE3(BranchCnd);
      GDCC_IR_Exp_DeclCreateE1(BranchNot);
      GDCC_IR_Exp_DeclCreateE2(BranchOrI);
      GDCC_IR_Exp_DeclCreateE2(BranchOrX);

      GDCC_IR_Exp_DeclCreateE1(UnaryAdd);
      GDCC_IR_Exp_DeclCreateTE(UnaryCst);
      GDCC_IR_Exp_DeclCreateE1(UnaryNot);
      GDCC_IR_Exp_DeclCreateE1(UnarySub);

      Exp::CRef ExpCreate_ValueGlyph(Glyph glyph, Core::Origin pos);

      Exp::CRef ExpCreate_ValueMulti(Exp::CRef const *expv, std::size_t expc,
         Core::Origin pos);

      Exp::CRef ExpCreate_ValueRoot(Value const &value, Core::Origin pos);
      Exp::CRef ExpCreate_ValueRoot(Value &&value, Core::Origin pos);

      Exp::CRef ExpGetIR_ValueGlyph(IArchive &in);
      Exp::CRef ExpGetIR_ValueMulti(IArchive &in);
      Exp::CRef ExpGetIR_ValueRoot (IArchive &in);
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

