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

