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

#include "IR/Exp.hpp"

#include "IR/IArchive.hpp"
#include "IR/OArchive.hpp"

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
      // Exp::getType
      //
      Type Exp::getType() const
      {
         try {return v_getType();}
         catch(TypeError const &)
         {
            std::cerr << "ERROR: " << pos << ": type mismatch\n";
            throw EXIT_FAILURE;
         }
      }

      //
      // Exp::getValue
      //
      Value Exp::getValue() const
      {
         try {return v_getValue();}
         catch(TypeError const &)
         {
            std::cerr << "ERROR: " << pos << ": type mismatch\n";
            throw EXIT_FAILURE;
         }
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
      // GetIR_T<Exp::CPtr>::GetIR_F
      //
      Exp::CPtr GetIR_T<Exp::CPtr>::GetIR_F(IArchive &in)
      {
         switch(GetIR<Core::StringIndex>(in))
         {
         case Core::STR_None: return nullptr;

         case Core::STR_BinaryAdd:   return ExpGetIR_BinaryAdd  (in);
         case Core::STR_BinaryAnd:   return ExpGetIR_BinaryAnd  (in);
         case Core::STR_BinaryDiv:   return ExpGetIR_BinaryDiv  (in);
         case Core::STR_BinaryMod:   return ExpGetIR_BinaryMod  (in);
         case Core::STR_BinaryMul:   return ExpGetIR_BinaryMul  (in);
         case Core::STR_BinaryOrI:   return ExpGetIR_BinaryOrI  (in);
         case Core::STR_BinaryOrX:   return ExpGetIR_BinaryOrX  (in);
         case Core::STR_BinaryShL:   return ExpGetIR_BinaryShL  (in);
         case Core::STR_BinaryShR:   return ExpGetIR_BinaryShR  (in);
         case Core::STR_BinarySub:   return ExpGetIR_BinarySub  (in);
         case Core::STR_BranchAnd:   return ExpGetIR_BranchAnd  (in);
         case Core::STR_BranchCmpEQ: return ExpGetIR_BranchCmpEQ(in);
         case Core::STR_BranchCmpGE: return ExpGetIR_BranchCmpGE(in);
         case Core::STR_BranchCmpGT: return ExpGetIR_BranchCmpGT(in);
         case Core::STR_BranchCmpLE: return ExpGetIR_BranchCmpLE(in);
         case Core::STR_BranchCmpLT: return ExpGetIR_BranchCmpLT(in);
         case Core::STR_BranchCmpNE: return ExpGetIR_BranchCmpNE(in);
         case Core::STR_BranchCnd:   return ExpGetIR_BranchCnd  (in);
         case Core::STR_BranchNot:   return ExpGetIR_BranchNot  (in);
         case Core::STR_BranchOrI:   return ExpGetIR_BranchOrI  (in);
         case Core::STR_BranchOrX:   return ExpGetIR_BranchOrX  (in);
         case Core::STR_UnaryAdd:    return ExpGetIR_UnaryAdd   (in);
         case Core::STR_UnaryCst:    return ExpGetIR_UnaryCst   (in);
         case Core::STR_UnaryNot:    return ExpGetIR_UnaryNot   (in);
         case Core::STR_UnarySub:    return ExpGetIR_UnarySub   (in);
         case Core::STR_ValueGlyph:  return ExpGetIR_ValueGlyph (in);
         case Core::STR_ValueMulti:  return ExpGetIR_ValueMulti (in);
         case Core::STR_ValueRoot:   return ExpGetIR_ValueRoot  (in);

         case Core::STR_BinaryAddPtrRaw: return ExpGetIR_BinaryAddPtrRaw(in);

         default:
            std::cerr << "invalid Exp\n";
            throw EXIT_FAILURE;
         }
      }

      //
      // GetIR_T<Exp::CRef>::GetIR_F
      //
      Exp::CRef GetIR_T<Exp::CRef>::GetIR_F(IArchive &in)
      {
         if(auto exp = GetIR<Exp::CPtr>(in))
            return static_cast<Exp::CRef>(exp);

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
            return out << Core::STR_None;
      }
   }
}

// EOF

