//-----------------------------------------------------------------------------
//
// Copyright (C) 2013-2015 David Hill
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
      // PutExpPart Array
      //
      static void PutExpPart(std::ostream &out, IR::Exp_Array const *exp)
      {
         out << "Array ";

         PutType(out, exp->elemT);

         out << " (";

         if(!exp->elemV.empty())
         {
            auto itr = exp->elemV.begin(), end = exp->elemV.end();
            PutExp(out, *itr++);
            while(itr != end)
               PutExp(out << ", ", *itr++);
         }

         out << ')';
      }

      //
      // PutExpPart Assoc
      //
      static void PutExpPart(std::ostream &out, IR::Exp_Assoc const *exp)
      {
         out << "Assoc ";

         PutType(out, exp->getType());

         out << " (";

         if(!exp->elemV.empty())
         {
            auto itr = exp->elemV.begin(), end = exp->elemV.end();
            PutExp(out, *itr++);
            while(itr != end)
               PutExp(out << ", ", *itr++);
         }

         out << ')';
      }

      //
      // PutExpPart Binary
      //
      static void PutExpPart(std::ostream &out, IR::Exp_Binary const *exp,
         bool named = false)
      {
         PutExp(out, exp->expL); out << (named ? ", " : " ");
         PutExp(out, exp->expR);
      }

      //
      // PutExpPart BraUna
      //
      static void PutExpPart(std::ostream &out, IR::Exp_BraUna const *exp,
         bool = false)
      {
         PutExp(out, exp->exp);
      }

      //
      // PutExpPart BraBin
      //
      static void PutExpPart(std::ostream &out, IR::Exp_BraBin const *exp,
         bool named = false)
      {
         PutExp(out, exp->expL); out << (named ? ", " : " ");
         PutExp(out, exp->expR);
      }

      //
      // PutExpPart BraTer
      //
      static void PutExpPart(std::ostream &out, IR::Exp_BraTer const *exp,
         bool named = false)
      {
         PutExp(out, exp->expC); out << (named ? ", " : " ");
         PutExp(out, exp->expL); out << (named ? ", " : " ");
         PutExp(out, exp->expR);
      }

      //
      // PutExpPart Unary
      //
      static void PutExpPart(std::ostream &out, IR::Exp_Unary const *exp,
         bool = false)
      {
         PutExp(out, exp->exp);
      }

      //
      // PutExpPart Cst
      //
      static void PutExpPart(std::ostream &out, IR::Exp_Cst const *exp,
         bool named = false)
      {
         PutType(out, exp->type); out << (named ? ", " : " ");
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
// Extern Functions                                                           |
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
            if(OptExpName) \
               out << #name << '('; \
            else \
               out << (sym) << ' '; \
            PutExpPart(out, static_cast<IR::Exp_##name const *>(exp), OptExpName); \
            if(OptExpName) \
               out << ')'; \
            break

         #define CaseParN(name, sym) case Core::STR_##name: \
            out << #name "("; \
            PutExpPart(out, static_cast<IR::Exp_##name const *>(exp), true); \
            out << ')'; \
            break

         switch(exp->getName())
         {
            CasePart(Add,       "+");
            CaseParN(AddPtrRaw, "AddPtrRaw");
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
            CaseParN(Cst,       "Cst");
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

         case Core::STR_Array:
            PutExpPart(out, static_cast<IR::Exp_Array const *>(exp));
            break;

         case Core::STR_Assoc:
            PutExpPart(out, static_cast<IR::Exp_Assoc const *>(exp));
            break;

         case Core::STR_Glyph:
            PutExpPart(out, static_cast<IR::Exp_Glyph const *>(exp));
            break;

         case Core::STR_Tuple:
            out << (OptExpName ? "Tuple(" : "{");
            PutExpPart(out, static_cast<IR::Exp_Tuple const *>(exp));
            out << (OptExpName ? ')' : '}');
            break;

         case Core::STR_Value:
            PutExpPart(out, static_cast<IR::Exp_Value const *>(exp));
            break;

         default:
            out << "\"UNKNOWN IR::Exp: " << exp->getName() << '"';
            break;
         }

         #undef CaseParN
         #undef CasePart
      }
   }
}

// EOF

