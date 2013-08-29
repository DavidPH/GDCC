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
      // Exp::v_putIR
      //
      OArchive &Exp::v_putIR(OArchive &out) const
      {
         return out << pos;
      }

      //
      // Exp::putIR
      //
      OArchive &Exp::putIR(OArchive &out) const
      {
         return v_putIR(out << getName());
      }

      //
      // GetIR_T<Exp::Ptr>::GetIR_F
      //
      Exp::Ptr GetIR_T<Exp::Ptr>::GetIR_F(IArchive &in)
      {
         switch(GetIR<StringIndex>(in))
         {
         case STR_None: return nullptr;

         case STR_BinaryAdd:   return ExpGetIR_BinaryAdd  (in);
         case STR_BinaryDiv:   return ExpGetIR_BinaryDiv  (in);
         case STR_BinaryMod:   return ExpGetIR_BinaryMod  (in);
         case STR_BinaryMul:   return ExpGetIR_BinaryMul  (in);
         case STR_BinaryOrI:   return ExpGetIR_BinaryOrI  (in);
         case STR_BinaryOrX:   return ExpGetIR_BinaryOrX  (in);
         case STR_BinaryShL:   return ExpGetIR_BinaryShL  (in);
         case STR_BinaryShR:   return ExpGetIR_BinaryShR  (in);
         case STR_BinarySub:   return ExpGetIR_BinarySub  (in);
         case STR_BranchAnd:   return ExpGetIR_BranchAnd  (in);
         case STR_BranchCmpEQ: return ExpGetIR_BranchCmpEQ(in);
         case STR_BranchCmpGE: return ExpGetIR_BranchCmpGE(in);
         case STR_BranchCmpGT: return ExpGetIR_BranchCmpGT(in);
         case STR_BranchCmpLE: return ExpGetIR_BranchCmpLE(in);
         case STR_BranchCmpLT: return ExpGetIR_BranchCmpLT(in);
         case STR_BranchCmpNE: return ExpGetIR_BranchCmpNE(in);
         case STR_BranchCnd:   return ExpGetIR_BranchCnd  (in);
         case STR_BranchNot:   return ExpGetIR_BranchNot  (in);
         case STR_BranchOrI:   return ExpGetIR_BranchOrI  (in);
         case STR_BranchOrX:   return ExpGetIR_BranchOrX  (in);
         case STR_UnaryAdd:    return ExpGetIR_UnaryAdd   (in);
         case STR_UnaryCst:    return ExpGetIR_UnaryCst   (in);
         case STR_UnaryNot:    return ExpGetIR_UnaryNot   (in);
         case STR_UnarySub:    return ExpGetIR_UnarySub   (in);
         case STR_ValueGlyph:  return ExpGetIR_ValueGlyph (in);
         case STR_ValueRoot:   return ExpGetIR_ValueRoot  (in);

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
            return in->putIR(out);
         else
            return out << STR_None;
      }
   }
}

// EOF

