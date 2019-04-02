//-----------------------------------------------------------------------------
//
// Copyright (C) 2013-2019 David Hill
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

#include "IR/Exp/Binary.hpp"
#include "IR/Exp/Branch.hpp"
#include "IR/Exp/Glyph.hpp"
#include "IR/Exp/Multi.hpp"
#include "IR/Exp/Unary.hpp"
#include "IR/Exp/Value.hpp"

#include "Option/Bool.hpp"

#include "Target/Addr.hpp"
#include "Target/CallType.hpp"


//----------------------------------------------------------------------------|
// Options                                                                    |
//

namespace GDCC::IRDump
{
   //
   // --named-expressions
   //
   Option::Bool DumpExpName
   {
      &Core::GetOptionList(), Option::Base::Info()
         .setName("named-expressions")
         .setGroup("output")
         .setDescS("Use expression names instead of symbols."),

      false
   };
}


//----------------------------------------------------------------------------|
// Static Functions                                                           |
//

namespace GDCC::IRDump
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
            PutExp(out << ' ', *itr++);
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
            PutExp(out << ' ', *itr++);
      }

      out << ')';
   }

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
      if(!exp->elemV.empty())
      {
         auto itr = exp->elemV.begin(), end = exp->elemV.end();
         PutExp(out, *itr++);
         while(itr != end)
            PutExp(out << ' ', *itr++);
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


//----------------------------------------------------------------------------|
// Extern Functions                                                           |
//

namespace GDCC::IRDump
{
   //
   // PutExp
   //
   void PutExp(std::ostream &out, IR::Exp const *exp)
   {
      #define CasePart(name, sym) case Core::STR_##name: \
         if(DumpExpName) \
            out << #name << '('; \
         else \
            out << (sym) << ' '; \
         PutExpPart(out, static_cast<IR::Exp_##name const *>(exp)); \
         if(DumpExpName) \
            out << ')'; \
         break

      #define CaseParN(name) case Core::STR_##name: \
         out << #name "("; \
         PutExpPart(out, static_cast<IR::Exp_##name const *>(exp)); \
         out << ')'; \
         break

      switch(exp->getName())
      {
         CasePart(Add,    "+");
         CaseParN(AddPtrRaw);
         CasePart(BitAnd, "&");
         CasePart(BitOrI, "|");
         CasePart(BitOrX, "^");
         CasePart(CmpEQ,  "==");
         CasePart(CmpGE,  ">=");
         CasePart(CmpGT,  ">");
         CasePart(CmpLE,  "<=");
         CasePart(CmpLT,  "<");
         CasePart(CmpNE,  "!=");
         CasePart(Cnd,    "?");
         CaseParN(Cst);
         CasePart(Div,    "/");
         CasePart(Inv,    "~");
         CasePart(LogAnd, "&&");
         CasePart(LogOrI, "||");
         CasePart(LogOrX, "^^");
         CasePart(Mod,    "%");
         CasePart(Mul,    "*");
         CasePart(Neg,    "--");
         CasePart(Not,    "!");
         CasePart(ShL,    "<<");
         CasePart(ShR,    ">>");
         CasePart(Sub,    "-");

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
         out << (DumpExpName ? "Tuple(" : "{");
         PutExpPart(out, static_cast<IR::Exp_Tuple const *>(exp));
         out << (DumpExpName ? ')' : '}');
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

// EOF

