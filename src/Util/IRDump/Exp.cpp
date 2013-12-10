//-----------------------------------------------------------------------------
//
// Copyright (C) 2013 David Hill
//
// See COPYING for license information.
//
//-----------------------------------------------------------------------------
//
// IR::Exp dump/disassembly functions.
//
//-----------------------------------------------------------------------------

#include "../IRDump.hpp"

#include "GDCC/IR/Exp/Binary.hpp"
#include "GDCC/IR/Exp/Branch.hpp"
#include "GDCC/IR/Exp/Unary.hpp"
#include "GDCC/IR/Exp/ValueGlyph.hpp"
#include "GDCC/IR/Exp/ValueMulti.hpp"
#include "GDCC/IR/Exp/ValueRoot.hpp"
#include "GDCC/IR/Function.hpp"


//----------------------------------------------------------------------------|
// Static Functions                                                           |
//

//
// IRDump_ExpPart Binary
//
static void IRDump_ExpPart(std::ostream &out, GDCC::IR::Exp_Binary const *exp)
{
   Util::IRDump_Exp(out, exp->expL); out << ' ';
   Util::IRDump_Exp(out, exp->expR);
}

//
// IRDump_ExpPart BraUna
//
static void IRDump_ExpPart(std::ostream &out, GDCC::IR::Exp_BraUna const *exp)
{
   Util::IRDump_Exp(out, exp->exp);
}

//
// IRDump_ExpPart BraBin
//
static void IRDump_ExpPart(std::ostream &out, GDCC::IR::Exp_BraBin const *exp)
{
   Util::IRDump_Exp(out, exp->expL); out << ' ';
   Util::IRDump_Exp(out, exp->expR);
}

//
// IRDump_ExpPart BraTer
//
static void IRDump_ExpPart(std::ostream &out, GDCC::IR::Exp_BraTer const *exp)
{
   Util::IRDump_Exp(out, exp->expC); out << ' ';
   Util::IRDump_Exp(out, exp->expL); out << ' ';
   Util::IRDump_Exp(out, exp->expR);
}

//
// IRDump_ExpPart Unary
//
static void IRDump_ExpPart(std::ostream &out, GDCC::IR::Exp_Unary const *exp)
{
   Util::IRDump_Exp(out, exp->exp);
}

//
// IRDump_ExpPart UnaryCst
//
static void IRDump_ExpPart(std::ostream &out, GDCC::IR::Exp_UnaryCst const *exp)
{
   Util::IRDump_Type(out, exp->type);
   Util::IRDump_Exp(out, exp->exp);
}

//
// IRDump_ExpPart ValueGlyph
//
static void IRDump_ExpPart(std::ostream &out, GDCC::IR::Exp_ValueGlyph const *exp)
{
   Util::IRDumpString(out, static_cast<GDCC::String>(exp->glyph));
}

//
// IRDump_ExpPart ValueMulti
//
static void IRDump_ExpPart(std::ostream &out, GDCC::IR::Exp_ValueMulti const *exp)
{
   out << '{';

   if(!exp->expv.empty())
   {
      auto itr = exp->expv.begin(), end = exp->expv.end();
      Util::IRDump_Exp(out, *itr++);
      while(itr != end)
      {
         out << ", ";
         Util::IRDump_Exp(out, *itr++);
      }
   }

   out << '}';
}

//
// IRDump_ExpPart ValueRoot
//
static void IRDump_ExpPart(std::ostream &out, GDCC::IR::Exp_ValueRoot const *exp)
{
   Util::IRDump_Value(out, exp->value);
}


//----------------------------------------------------------------------------|
// Global Functions                                                           |
//

namespace Util
{
   //
   // IRDump_Exp
   //
   void IRDump_Exp(std::ostream &out, GDCC::IR::Exp const *exp)
   {
      #define CasePart(name, pre) case GDCC::STR_##name: \
         out << '(' << (pre) << ' '; \
         IRDump_ExpPart(out, static_cast<GDCC::IR::Exp_##name const *>(exp)); \
         out << ')'; \
         break

      switch(static_cast<GDCC::StringIndex>(exp->getName()))
      {
         CasePart(BinaryAdd, '+');
         CasePart(BinaryAnd, '&');
         CasePart(BinaryDiv, '/');
         CasePart(BinaryMod, '%');
         CasePart(BinaryMul, '*');
         CasePart(BinaryOrI, '|');
         CasePart(BinaryOrX, '^');
         CasePart(BinaryShL, "<<");
         CasePart(BinaryShR, ">>");
         CasePart(BinarySub, '-');

         CasePart(BranchAnd,   "&&");
         CasePart(BranchCmpEQ, "==");
         CasePart(BranchCmpGE, ">=");
         CasePart(BranchCmpGT, '>');
         CasePart(BranchCmpLE, "<=");
         CasePart(BranchCmpLT, '<');
         CasePart(BranchCmpNE, "!=");
         CasePart(BranchCnd,   '?');
         CasePart(BranchNot,   '!');
         CasePart(BranchOrI,   "||");
         CasePart(BranchOrX,   "^^");

         CasePart(UnaryAdd, "++");
         CasePart(UnaryCst, "cast");
         CasePart(UnaryNot, '~');
         CasePart(UnarySub, "--");

      case GDCC::STR_ValueGlyph:
         IRDump_ExpPart(out, static_cast<GDCC::IR::Exp_ValueGlyph const *>(exp));
         break;

      case GDCC::STR_ValueMulti:
         IRDump_ExpPart(out, static_cast<GDCC::IR::Exp_ValueMulti const *>(exp));
         break;

      case GDCC::STR_ValueRoot:
         IRDump_ExpPart(out, static_cast<GDCC::IR::Exp_ValueRoot const *>(exp));
         break;

      default:
         out << "\"UNKNOWN IR::Exp: " << exp->getName() << '"';
         break;
      }
   }

   //
   // IRDump_Type
   //
   void IRDump_Type(std::ostream &out, GDCC::IR::Type const &type)
   {
      switch(type.t)
      {
      case GDCC::IR::TypeBase::Empty:
         out << "Empty()";
         break;

      case GDCC::IR::TypeBase::Fixed:
         out << "Fixed("
            << type.tFixed.bitsI << ", "
            << type.tFixed.bitsF << ", "
            << type.tFixed.bitsS << ", "
            << type.tFixed.satur << ") ";
         break;

      case GDCC::IR::TypeBase::Float:
         out << "Float("
            << type.tFixed.bitsI << ", "
            << type.tFixed.bitsF << ", "
            << type.tFixed.bitsS << ", "
            << type.tFixed.satur << ") ";
         break;

      case GDCC::IR::TypeBase::Funct:
         out << "Funct(" << type.tFunct.callT << ") ";
         break;

      case GDCC::IR::TypeBase::Multi:
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

      case GDCC::IR::TypeBase::Point:
         out << "Point("
            << type.tPoint.reprB << ", "
            << type.tPoint.reprS << ", "
            << type.tPoint.reprW << ") ";
         break;

      case GDCC::IR::TypeBase::StrEn:
         out << "StrEn()";
         break;
      }
   }

   //
   // IRDump_Value
   //
   void IRDump_Value(std::ostream &out, GDCC::IR::Value const &val)
   {
      switch(val.v)
      {
      case GDCC::IR::ValueBase::Fixed:
         if(val.vFixed.vtype.bitsF)
            out << (static_cast<GDCC::Float>(val.vFixed.value) >> val.vFixed.vtype.bitsF);
         else
            out << val.vFixed.value;

         out << '_';
         if(val.vFixed.vtype.bitsS) out << 's';
         out << val.vFixed.vtype.bitsI << '.' << val.vFixed.vtype.bitsF;
         if(val.vFixed.vtype.satur) out << 's';

         break;

      case GDCC::IR::ValueBase::Float:
         out << val.vFloat.value;

         out << '_';
         if(val.vFloat.vtype.bitsS) out << 's';
         out << val.vFloat.vtype.bitsI << '.' << val.vFloat.vtype.bitsF;
         if(val.vFloat.vtype.satur) out << 's';

         break;

      case GDCC::IR::ValueBase::Multi:
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

// EOF

