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

         case Core::STR_Add:       return ExpGetIR_Add      (in);
         case Core::STR_AddPtrRaw: return ExpGetIR_AddPtrRaw(in);
         case Core::STR_BitAnd:    return ExpGetIR_BitAnd   (in);
         case Core::STR_BitOrI:    return ExpGetIR_BitOrI   (in);
         case Core::STR_BitOrX:    return ExpGetIR_BitOrX   (in);
         case Core::STR_CmpEQ:     return ExpGetIR_CmpEQ    (in);
         case Core::STR_CmpGE:     return ExpGetIR_CmpGE    (in);
         case Core::STR_CmpGT:     return ExpGetIR_CmpGT    (in);
         case Core::STR_CmpLE:     return ExpGetIR_CmpLE    (in);
         case Core::STR_CmpLT:     return ExpGetIR_CmpLT    (in);
         case Core::STR_CmpNE:     return ExpGetIR_CmpNE    (in);
         case Core::STR_Cnd:       return ExpGetIR_Cnd      (in);
         case Core::STR_Cst:       return ExpGetIR_Cst      (in);
         case Core::STR_Div:       return ExpGetIR_Div      (in);
         case Core::STR_Glyph:     return ExpGetIR_Glyph    (in);
         case Core::STR_Inv:       return ExpGetIR_Inv      (in);
         case Core::STR_Mod:       return ExpGetIR_Mod      (in);
         case Core::STR_Mul:       return ExpGetIR_Mul      (in);
         case Core::STR_Multi:     return ExpGetIR_Multi    (in);
         case Core::STR_Neg:       return ExpGetIR_Neg      (in);
         case Core::STR_Not:       return ExpGetIR_Not      (in);
         case Core::STR_LogAnd:    return ExpGetIR_LogAnd   (in);
         case Core::STR_LogOrI:    return ExpGetIR_LogOrI   (in);
         case Core::STR_LogOrX:    return ExpGetIR_LogOrX   (in);
         case Core::STR_ShL:       return ExpGetIR_ShL      (in);
         case Core::STR_ShR:       return ExpGetIR_ShR      (in);
         case Core::STR_Sub:       return ExpGetIR_Sub      (in);
         case Core::STR_Value:     return ExpGetIR_Value    (in);

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

