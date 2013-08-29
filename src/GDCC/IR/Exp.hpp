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

#include "Value.hpp"

#include "../Counter.hpp"
#include "../Origin.hpp"


//----------------------------------------------------------------------------|
// Macros                                                                     |
//

//
// GDCC_IR_Exp_DeclCreateE1
//
#define GDCC_IR_Exp_DeclCreateE1(name) \
   Exp::Ref ExpCreate_##name(Exp *e); \
   Exp::Ref ExpCreate_##name(Exp *e, Origin pos); \
   Exp::Ref ExpGetIR_##name(IArchive &in)

//
// GDCC_IR_Exp_DeclCreateE2
//
#define GDCC_IR_Exp_DeclCreateE2(name) \
   Exp::Ref ExpCreate_##name(Exp *l, Exp *r); \
   Exp::Ref ExpCreate_##name(Exp *l, Exp *r, Origin pos); \
   Exp::Ref ExpGetIR_##name(IArchive &in)

//
// GDCC_IR_Exp_DeclCreateE3
//
#define GDCC_IR_Exp_DeclCreateE3(name) \
   Exp::Ref ExpCreate_##name(Exp *c, Exp *l, Exp *r); \
   Exp::Ref ExpCreate_##name(Exp *c, Exp *l, Exp *r, Origin pos); \
   Exp::Ref ExpGetIR_##name(IArchive &in)

//
// GDCC_IR_Exp_DeclCreateTE
//
#define GDCC_IR_Exp_DeclCreateTE(name) \
   Exp::Ref ExpCreate_##name(Type const &t, Exp *e); \
   Exp::Ref ExpCreate_##name(Type const &t, Exp *e, Origin pos); \
   Exp::Ref ExpCreate_##name(Type &&t, Exp *e); \
   Exp::Ref ExpCreate_##name(Type &&t, Exp *e, Origin pos); \
   Exp::Ref ExpGetIR_##name(IArchive &in)


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
      class Exp : public Counter
      {
         GDCC_CounterPreambleAbstract(GDCC::IR::Exp, GDCC::Counter);

      public:
         virtual String getName() const = 0;

         Type getType() const {return v_getType();}

         Value getValue() const {return v_getValue();}

         OArchive &putIR(OArchive &out) const;

         Origin const pos;

      protected:
         Exp(Exp const &) = default;
         explicit Exp(Origin pos_) : pos{pos_} {}
         explicit Exp(IArchive &in);

         virtual Type v_getType() const = 0;

         virtual Value v_getValue() const = 0;

         virtual OArchive &v_putIR(OArchive &out) const;
      };

      //
      // GetIR_T<Exp::Ptr>
      //
      template<> struct GetIR_T<Exp::Ptr>
      {
         static Exp::Ptr GetIR_F(IArchive &in);
      };

      //
      // GetIR_T<Exp::Ref>
      //
      template<> struct GetIR_T<Exp::Ref>
      {
         static Exp::Ref GetIR_F(IArchive &in);
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

      IArchive &operator >> (IArchive &in, Exp::Ptr &out);
      IArchive &operator >> (IArchive &in, Exp::Ref &out);

      GDCC_IR_Exp_DeclCreateE2(BinaryAdd);
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

      Exp::Ref ExpCreate_ValueGlyph(Glyph glyph, Origin pos);

      Exp::Ref ExpCreate_ValueRoot(Value const &value, Origin pos);
      Exp::Ref ExpCreate_ValueRoot(Value &&value, Origin pos);

      Exp::Ref ExpGetIR_ValueGlyph(IArchive &in);
      Exp::Ref ExpGetIR_ValueRoot (IArchive &in);
   }
}

namespace GDCC
{
   namespace IR
   {
      //
      // operator IArchive >> Exp::Ptr
      //
      inline IArchive &operator >> (IArchive &in, Exp::Ptr &out)
      {
         out = GetIR_T<Exp::Ptr>::GetIR_F(in);
         return in;
      }

      //
      // operator IArchive >> Exp::Ref
      //
      inline IArchive &operator >> (IArchive &in, Exp::Ref &out)
      {
         out = GetIR_T<Exp::Ref>::GetIR_F(in);
         return in;
      }
   }
}

#endif//GDCC__IR__Exp_H__

