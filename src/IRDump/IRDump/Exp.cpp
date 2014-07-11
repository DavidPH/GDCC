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

#include "IRDump/IRDump.hpp"

#include "IR/Addr.hpp"
#include "IR/CallType.hpp"
#include "IR/Exp/Binary.hpp"
#include "IR/Exp/Branch.hpp"
#include "IR/Exp/Glyph.hpp"
#include "IR/Exp/Multi.hpp"
#include "IR/Exp/Unary.hpp"
#include "IR/Exp/Value.hpp"


//----------------------------------------------------------------------------|
// Static Functions                                                           |
//

//
// IRDump_ExpPart Binary
//
static void IRDump_ExpPart(std::ostream &out, GDCC::IR::Exp_Binary const *exp)
{
   GDCC::IRDump::IRDump_Exp(out, exp->expL); out << ' ';
   GDCC::IRDump::IRDump_Exp(out, exp->expR);
}

//
// IRDump_ExpPart BraUna
//
static void IRDump_ExpPart(std::ostream &out, GDCC::IR::Exp_BraUna const *exp)
{
   GDCC::IRDump::IRDump_Exp(out, exp->exp);
}

//
// IRDump_ExpPart BraBin
//
static void IRDump_ExpPart(std::ostream &out, GDCC::IR::Exp_BraBin const *exp)
{
   GDCC::IRDump::IRDump_Exp(out, exp->expL); out << ' ';
   GDCC::IRDump::IRDump_Exp(out, exp->expR);
}

//
// IRDump_ExpPart BraTer
//
static void IRDump_ExpPart(std::ostream &out, GDCC::IR::Exp_BraTer const *exp)
{
   GDCC::IRDump::IRDump_Exp(out, exp->expC); out << ' ';
   GDCC::IRDump::IRDump_Exp(out, exp->expL); out << ' ';
   GDCC::IRDump::IRDump_Exp(out, exp->expR);
}

//
// IRDump_ExpPart Unary
//
static void IRDump_ExpPart(std::ostream &out, GDCC::IR::Exp_Unary const *exp)
{
   GDCC::IRDump::IRDump_Exp(out, exp->exp);
}

//
// IRDump_ExpPart UnaryCst
//
static void IRDump_ExpPart(std::ostream &out, GDCC::IR::Exp_Cst const *exp)
{
   GDCC::IRDump::IRDump_Type(out, exp->type);
   GDCC::IRDump::IRDump_Exp(out, exp->exp);
}

//
// IRDump_ExpPart ValueGlyph
//
static void IRDump_ExpPart(std::ostream &out, GDCC::IR::Exp_Glyph const *exp)
{
   GDCC::IRDump::IRDumpString(out, static_cast<GDCC::Core::String>(exp->glyph));
}

//
// IRDump_ExpPart ValueMulti
//
static void IRDump_ExpPart(std::ostream &out, GDCC::IR::Exp_Multi const *exp)
{
   out << '{';

   if(!exp->expv.empty())
   {
      auto itr = exp->expv.begin(), end = exp->expv.end();
      GDCC::IRDump::IRDump_Exp(out, *itr++);
      while(itr != end)
      {
         out << ", ";
         GDCC::IRDump::IRDump_Exp(out, *itr++);
      }
   }

   out << '}';
}

//
// IRDump_ExpPart ValueRoot
//
static void IRDump_ExpPart(std::ostream &out, GDCC::IR::Exp_Value const *exp)
{
   GDCC::IRDump::IRDump_Value(out, exp->value);
}


//----------------------------------------------------------------------------|
// Global Functions                                                           |
//

namespace GDCC
{
   namespace IRDump
   {
      //
      // IRDump_Exp
      //
      void IRDump_Exp(std::ostream &out, IR::Exp const *exp)
      {
         #define CasePart(name, pre) case Core::STR_##name: \
            out << '(' << (pre) << ' '; \
            IRDump_ExpPart(out, static_cast<IR::Exp_##name const *>(exp)); \
            out << ')'; \
            break

         switch(static_cast<Core::StringIndex>(exp->getName()))
         {
            CasePart(AddPtrRaw, "AddPtrRaw");

            CasePart(Add,    '+');
            CasePart(BitAnd, '&');
            CasePart(BitOrI, '|');
            CasePart(BitOrX, '^');
            CasePart(CmpEQ,  "==");
            CasePart(CmpGE,  ">=");
            CasePart(CmpGT,  '>');
            CasePart(CmpLE,  "<=");
            CasePart(CmpLT,  '<');
            CasePart(CmpNE,  "!=");
            CasePart(Cnd,    '?');
            CasePart(Cst,    "cast");
            CasePart(Div,    '/');
            CasePart(Inv,    '~');
            CasePart(LogAnd, "&&");
            CasePart(LogOrI, "||");
            CasePart(LogOrX, "^^");
            CasePart(Mod,    '%');
            CasePart(Mul,    '*');
            CasePart(Neg,    "--");
            CasePart(Not,    '!');
            CasePart(ShL,    "<<");
            CasePart(ShR,    ">>");
            CasePart(Sub,    '-');

         case Core::STR_ValueGlyph:
            IRDump_ExpPart(out, static_cast<IR::Exp_Glyph const *>(exp));
            break;

         case Core::STR_ValueMulti:
            IRDump_ExpPart(out, static_cast<IR::Exp_Multi const *>(exp));
            break;

         case Core::STR_ValueRoot:
            IRDump_ExpPart(out, static_cast<IR::Exp_Value const *>(exp));
            break;

         default:
            out << "\"UNKNOWN IR::Exp: " << exp->getName() << '"';
            break;
         }
      }

      //
      // IRDump_Type
      //
      void IRDump_Type(std::ostream &out, IR::Type const &type)
      {
         switch(type.t)
         {
         case IR::TypeBase::Empty:
            out << "Empty()";
            break;

         case IR::TypeBase::Fixed:
            out << "Fixed("
               << type.tFixed.bitsI << ", "
               << type.tFixed.bitsF << ", "
               << type.tFixed.bitsS << ", "
               << type.tFixed.satur << ") ";
            break;

         case IR::TypeBase::Float:
            out << "Float("
               << type.tFixed.bitsI << ", "
               << type.tFixed.bitsF << ", "
               << type.tFixed.bitsS << ", "
               << type.tFixed.satur << ") ";
            break;

         case IR::TypeBase::Funct:
            out << "Funct(" << type.tFunct.callT << ") ";
            break;

         case IR::TypeBase::Multi:
            out << "Multi(";
            if(!type.tMulti.types.empty())
            {
               auto itr = type.tMulti.types.begin(), end = type.tMulti.types.end();
               IRDump_Type(out, *itr++);
               while(itr != end)
               {
                  out << ", ";
                  IRDump_Type(out, *itr++);
               }
            }
            out << ") ";
            break;

         case IR::TypeBase::Point:
            out << "Point("
               << type.tPoint.reprB << ", "
               << type.tPoint.reprS << ", "
               << type.tPoint.reprW << ") ";
            break;

         case IR::TypeBase::StrEn:
            out << "StrEn()";
            break;
         }
      }

      //
      // IRDump_Value
      //
      void IRDump_Value(std::ostream &out, IR::Value const &val)
      {
         switch(val.v)
         {
         case IR::ValueBase::Fixed:
            if(val.vFixed.vtype.bitsF)
               out << (static_cast<Core::Float>(val.vFixed.value) >> val.vFixed.vtype.bitsF);
            else
               out << val.vFixed.value;

            out << '_';
            if(val.vFixed.vtype.bitsS) out << 's';
            out << val.vFixed.vtype.bitsI << '.' << val.vFixed.vtype.bitsF;
            if(val.vFixed.vtype.satur) out << 's';

            break;

         case IR::ValueBase::Float:
            out << val.vFloat.value;

            out << '_';
            if(val.vFloat.vtype.bitsS) out << 's';
            out << val.vFloat.vtype.bitsI << '.' << val.vFloat.vtype.bitsF;
            if(val.vFloat.vtype.satur) out << 's';

            break;

         case IR::ValueBase::Multi:
            out << '[';
            if(!val.vMulti.value.empty())
            {
               auto itr = val.vMulti.value.begin(), end = val.vMulti.value.end();
               IRDump_Value(out, *itr++);
               while(itr != end)
               {
                  out << ", ";
                  IRDump_Value(out, *itr++);
               }
            }
            out << ']';
            break;

         default:
            out << "\"UNKNOWN IR::Value: " << val.v << '"';
            break;
         }
      }
   }
}

// EOF

