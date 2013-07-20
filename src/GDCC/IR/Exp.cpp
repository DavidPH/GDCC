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

#include "Exp.hpp"

#include "IArchive.hpp"
#include "OArchive.hpp"

#include <iostream>


//----------------------------------------------------------------------------|
// Global Functions                                                           |
//

namespace GDCC
{
   namespace IR
   {
      //
      // Exp constructor
      //
      Exp::Exp(IArchive &in) : pos{GetIR(in, pos)}
      {
      }

      //
      // Exp::v_writeIR
      //
      OArchive &Exp::v_writeIR(OArchive &out) const
      {
         return out << pos;
      }

      //
      // Exp::writeIR
      //
      OArchive &Exp::writeIR(OArchive &out) const
      {
         return v_writeIR(out << getName());
      }

      //
      // GetIR_T<Exp::Ptr>::GetIR_F
      //
      Exp::Ptr GetIR_T<Exp::Ptr>::GetIR_F(IArchive &in)
      {
         switch(GetIR<StringIndex>(in))
         {
         case STR_ValueGlyph: return ExpGetIR_ValueGlyph(in);
         case STR_ValueRoot:  return ExpGetIR_ValueRoot (in);

         default:
            std::cerr << "invalid Exp\n";
            throw EXIT_FAILURE;
         }
      }

      //
      // GetIR_T<Exp::Ref>::GetIR_F
      //
      Exp::Ref GetIR_T<Exp::Ref>::GetIR_F(IArchive &in)
      {
         if(auto exp = GetIR<Exp::Ptr>(in))
            return static_cast<Exp::Ref>(exp);

         std::cerr << "invalid Exp::Ref\n";
         throw EXIT_FAILURE;
      }

      //
      // operator OArchive << Exp const *
      //
      OArchive &operator << (OArchive &out, Exp const *in)
      {
         if(in)
            return in->writeIR(out);
         else
            return out << STR_None;
      }
   }
}

// EOF

