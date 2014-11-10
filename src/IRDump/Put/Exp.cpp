//-----------------------------------------------------------------------------
//
// Copyright (C) 2013-2014 David Hill
//
// See COPYING for license information.
//
//-----------------------------------------------------------------------------
//
// IR::Exp dump/disassembly functions.
//
//-----------------------------------------------------------------------------

#include "IRDump/Put.hpp"

#include "Core/Option.hpp"

#include "IR/Addr.hpp"
#include "IR/CallType.hpp"
#include "IR/Exp/Binary.hpp"
#include "IR/Exp/Branch.hpp"
#include "IR/Exp/Glyph.hpp"
#include "IR/Exp/Multi.hpp"
#include "IR/Exp/Unary.hpp"
#include "IR/Exp/Value.hpp"

#include "Option/Bool.hpp"


//----------------------------------------------------------------------------|
// Options                                                                    |
//

//
// --named-expressions
//
static GDCC::Option::Bool NamedExpressions
{
   &GDCC::Core::GetOptionList(), GDCC::Option::Base::Info()
      .setName("named-expressions")
      .setGroup("output")
      .setDescS("Use expression names instead of symbols."),

   &GDCC::IRDump::OptExpName
};


//----------------------------------------------------------------------------|
// Static Functions                                                           |
//

namespace GDCC
{
   namespace IRDump
   {
      //
      // PutExpPart Binary
      //
      static void PutExpPart(std::ostream &out, IR::Exp_Binary const *exp)
      {
         PutExp(out, exp->expL); out << ' ';
         PutExp(out, exp->expR);
      }

      //
      // PutExpPart BraUna
      //
      static void PutExpPart(std::ostream &out, IR::Exp_BraUna const *exp)
      {
         PutExp(out, exp->exp);
      }

      //
      // PutExpPart BraBin
      //
      static void PutExpPart(std::ostream &out, IR::Exp_BraBin const *exp)
      {
         PutExp(out, exp->expL); out << ' ';
         PutExp(out, exp->expR);
      }

      //
      // PutExpPart BraTer
      //
      static void PutExpPart(std::ostream &out, IR::Exp_BraTer const *exp)
      {
         PutExp(out, exp->expC); out << ' ';
         PutExp(out, exp->expL); out << ' ';
         PutExp(out, exp->expR);
      }

      //
      // PutExpPart Unary
      //
      static void PutExpPart(std::ostream &out, IR::Exp_Unary const *exp)
      {
         PutExp(out, exp->exp);
      }

      //
      // PutExpPart Cst
      //
      static void PutExpPart(std::ostream &out, IR::Exp_Cst const *exp)
      {
         PutType(out, exp->type); out << ' ';
         PutExp(out, exp->exp);
      }

      //
      // PutExpPart Glyph
      //
      static void PutExpPart(std::ostream &out, IR::Exp_Glyph const *exp)
      {
         PutString(out, static_cast<Core::String>(exp->glyph));
      }

      //
      // PutExpPart Tuple
      //
      static void PutExpPart(std::ostream &out, IR::Exp_Tuple const *exp)
      {
         out << '{';

         if(!exp->elemV.empty())
         {
            auto itr = exp->elemV.begin(), end = exp->elemV.end();
            PutExp(out, *itr++);
            while(itr != end)
            {
               out << ", ";
               PutExp(out, *itr++);
            }
         }

         out << '}';
      }

      //
      // PutExpPart Value
      //
      static void PutExpPart(std::ostream &out, IR::Exp_Value const *exp)
      {
         PutValue(out, exp->value);
      }
   }
}


//----------------------------------------------------------------------------|
// Global Functions                                                           |
//

namespace GDCC
{
   namespace IRDump
   {
      //
      // PutExp
      //
      void PutExp(std::ostream &out, IR::Exp const *exp)
      {
         #define CasePart(name, sym) case Core::STR_##name: \
            out << (OptExpName ? #name : (sym)) << ' '; \
            PutExpPart(out, static_cast<IR::Exp_##name const *>(exp)); \
            break

         switch(static_cast<Core::StringIndex>(exp->getName()))
         {
            CasePart(Add,       "+");
            CasePart(AddPtrRaw, "AddPtrRaw");
            CasePart(BitAnd,    "&");
            CasePart(BitOrI,    "|");
            CasePart(BitOrX,    "^");
            CasePart(CmpEQ,     "==");
            CasePart(CmpGE,     ">=");
            CasePart(CmpGT,     ">");
            CasePart(CmpLE,     "<=");
            CasePart(CmpLT,     "<");
            CasePart(CmpNE,     "!=");
            CasePart(Cnd,       "?");
            CasePart(Cst,       "Cst");
            CasePart(Div,       "/");
            CasePart(Inv,       "~");
            CasePart(LogAnd,    "&&");
            CasePart(LogOrI,    "||");
            CasePart(LogOrX,    "^^");
            CasePart(Mod,       "%");
            CasePart(Mul,       "*");
            CasePart(Neg,       "--");
            CasePart(Not,       "!");
            CasePart(ShL,       "<<");
            CasePart(ShR,       ">>");
            CasePart(Sub,       "-");

         case Core::STR_Glyph:
            PutExpPart(out, static_cast<IR::Exp_Glyph const *>(exp));
            break;

         case Core::STR_Tuple:
            PutExpPart(out, static_cast<IR::Exp_Tuple const *>(exp));
            break;

         case Core::STR_Value:
            PutExpPart(out, static_cast<IR::Exp_Value const *>(exp));
            break;

         default:
            out << "\"UNKNOWN IR::Exp: " << exp->getName() << '"';
            break;
         }
      }
   }
}

// EOF

