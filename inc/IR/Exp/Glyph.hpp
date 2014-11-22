//-----------------------------------------------------------------------------
//
// Copyright (C) 2013-2014 David Hill
//
// See COPYING for license information.
//
//-----------------------------------------------------------------------------
//
// Intermediary Representation expression: glyph.
//
//-----------------------------------------------------------------------------

#ifndef GDCC__IR__Exp__Glyph_H__
#define GDCC__IR__Exp__Glyph_H__

#include "../../IR/Exp.hpp"

#include "../../IR/Glyph.hpp"


//----------------------------------------------------------------------------|
// Types                                                                      |
//

namespace GDCC
{
   namespace IR
   {
      //
      // Exp_Glyph
      //
      class Exp_Glyph final : public Exp
      {
         GDCC_Core_CounterPreamble(GDCC::IR::Exp_Glyph, GDCC::IR::Exp);

      public:
         virtual Core::String getName() const {return Core::STR_Glyph;}

         Glyph const glyph;


         friend Exp::CRef ExpCreate_Glyph(Glyph glyph, Core::Origin pos);

         friend Exp::CRef ExpGetIR_Glyph(IArchive &in);

      protected:
         Exp_Glyph(Exp_Glyph const &) = default;
         Exp_Glyph(Glyph glyph_, Core::Origin pos_) :
            Super{pos_}, glyph{glyph_} {}
         explicit Exp_Glyph(IArchive &in);

         virtual Type v_getType() const {return glyph.getData().type;}

         virtual Value v_getValue() const;

         virtual bool v_isEqual(Exp const *e) const;

         virtual bool v_isValue() const;

         virtual OArchive &v_putIR(OArchive &out) const;
      };
   }
}

#endif//GDCC__IR__Exp__Glyph_H__

