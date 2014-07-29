//-----------------------------------------------------------------------------
//
// Copyright (C) 2013-2014 David Hill
//
// See COPYING for license information.
//
//-----------------------------------------------------------------------------
//
// Expression parsing utilities.
//
//-----------------------------------------------------------------------------

#include "AS/Parse.hpp"

#include "Core/Parse.hpp"
#include "Core/TokenStream.hpp"

#include "IR/Exp.hpp"
#include "IR/Glyph.hpp"

#include <iostream>
#include <vector>


//----------------------------------------------------------------------------|
// Static Functions                                                           |
//

//
// ParseSuffix
//
static std::tuple<char const */*end*/, bool /*fixed*/, bool /*bitsS*/,
   GDCC::Core::FastU /*bitsI*/, GDCC::Core::FastU /*bitsF*/, bool /*satur*/>
ParseSuffix(char const *s, GDCC::Core::Token const &tok)
{
   GDCC::Core::FastU bitsF;
   GDCC::Core::FastU bitsI;
   bool              bitsS;
   bool              fixed;
   bool              satur;

   fixed = *s == 'F' || *s == 'f' ? ++s, false : true;
   bitsS = *s == 'S' || *s == 's' ? ++s, true : false;
   std::tie(s, bitsI, std::ignore) = GDCC::Core::ParseNumberFastU(s, 10);

   if(*s != '.')
   {
      std::cerr << "ERROR: " << tok.pos << ": bad number: '" << tok.str << "'\n";
      throw EXIT_FAILURE;
   }

   std::tie(s, bitsF, std::ignore) = GDCC::Core::ParseNumberFastU(++s, 10);
   satur = *s == 'S' || *s == 's' ? ++s, true : false;

   return std::make_tuple(s, fixed, bitsS, bitsI, bitsF, satur);
}


//----------------------------------------------------------------------------|
// Global Functions                                                           |
//

namespace GDCC
{
   namespace AS
   {
      //
      // ParseExp
      //
      IR::Exp::CRef ParseExp(Core::TokenStream &in, IR::Program &prog)
      {
         #define doE1(name, token) case Core::token: \
            return IR::ExpCreate_##name(ParseExp(in, prog), tok.pos)

         #define doE2(name, token) case Core::token: \
            l = ParseExp(in, prog); r = ParseExp(in, prog); \
            return IR::ExpCreate_##name(l, r, tok.pos)

         #define doE3(name, token) case Core::token: \
            c = ParseExp(in, prog); l = ParseExp(in, prog); r = ParseExp(in, prog); \
            return IR::ExpCreate_##name(c, l, r, tok.pos)

         IR::Exp::CPtr c, l, r;
         IR::Type t;

         Core::Token tok;
         switch((in >> tok, tok).tok)
         {
         case Core::TOK_Identi:
            switch(static_cast<Core::StringIndex>(tok.str))
            {
            case Core::STR_cast:
               t = ParseType(in, prog);
               return IR::ExpCreate_Cst(t, ParseExp(in, prog), tok.pos);

            case Core::STR_string:
               return IR::ExpCreate_Value(ParseMultiString(in, prog), tok.pos);

            default:
               std::cerr << "ERROR: " << tok.pos << ": expected expression\n";
               throw EXIT_FAILURE;
            }

         case Core::TOK_Number:
            return IR::ExpCreate_Value(ParseNumber(tok), tok.pos);

         case Core::TOK_String:
            return IR::ExpCreate_Glyph(IR::Glyph(&prog, tok.str), tok.pos);

            doE2(Add,    TOK_Add);
            doE2(BitAnd, TOK_And);
            doE2(BitOrI, TOK_OrI);
            doE2(BitOrX, TOK_OrX);
            doE2(CmpEQ,  TOK_CmpEQ);
            doE2(CmpGE,  TOK_CmpGE);
            doE2(CmpGT,  TOK_CmpGT);
            doE2(CmpLE,  TOK_CmpLE);
            doE2(CmpLT,  TOK_CmpLT);
            doE2(CmpNE,  TOK_CmpNE);
            doE3(Cnd,    TOK_Query);
            doE2(Div,    TOK_Div);
            doE1(Inv,    TOK_Inv);
            doE2(LogAnd, TOK_And2);
            doE2(LogOrI, TOK_OrI2);
            doE2(LogOrX, TOK_OrX2);
            doE2(Mod,    TOK_Mod);
            doE2(Mul,    TOK_Mul);
            doE1(Neg,    TOK_Sub2);
            doE1(Not,    TOK_Not);
            doE2(ShL,    TOK_ShL);
            doE2(ShR,    TOK_ShR);
            doE2(Sub,    TOK_Sub);



         case Core::TOK_BraceO:
            in.unget();
            return ParseExpMulti(in, prog);

         case Core::TOK_BrackO:
            in.unget();
            return IR::ExpCreate_Value(ParseMulti(in, prog), tok.pos);

         default:
            std::cerr << "ERROR: " << tok.pos << ": expected expression\n";
            throw EXIT_FAILURE;
         }

         #undef doE3
         #undef doE2
         #undef doE1
      }

      //
      // ParseExpMulti
      //
      IR::Exp::CRef ParseExpMulti(Core::TokenStream &in, IR::Program &prog)
      {
         std::vector<IR::Exp::CRef> val;
         auto pos = in.peek().pos;

         SkipToken(in, Core::TOK_BraceO, "{");

         if(in.peek().tok != Core::TOK_BrackC) do
            val.emplace_back(ParseExp(in, prog));
         while(in.drop(Core::TOK_Comma));

         SkipToken(in, Core::TOK_BraceC, "}");

         return IR::ExpCreate_Multi(val.data(), val.size(), pos);
      }

      //
      // ParseFastI
      //
      Core::FastI ParseFastI(Core::TokenStream &in, IR::Program &prog)
      {
         auto i = ParseInteg(in, prog);

         return number_cast<Core::FastI>(i);
      }

      //
      // ParseFastU
      //
      Core::FastU ParseFastU(Core::TokenStream &in, IR::Program &prog)
      {
         auto i = ParseInteg(in, prog);

         return number_cast<Core::FastU>(i);
      }

      //
      // ParseInteg
      //
      Core::Integ ParseInteg(Core::TokenStream &in, IR::Program &prog)
      {
         auto exp = ParseExp(in, prog);
         auto val = exp->getValue();

         switch(val.v)
         {
         case IR::ValueBase::Fixed:
            return val.vFixed.value >> val.vFixed.vtype.bitsF;

         default:
            std::cerr << "stub\n";
            throw EXIT_FAILURE;
         }
      }

      //
      // ParseMulti
      //
      IR::Value_Multi ParseMulti(Core::TokenStream &in, IR::Program &prog)
      {
         std::vector<IR::Value> val;

         SkipToken(in, Core::TOK_BrackO, "[");

         if(in.peek().tok != Core::TOK_BrackC) do
            val.emplace_back(ParseExp(in, prog)->getValue());
         while(in.drop(Core::TOK_Comma));

         SkipToken(in, Core::TOK_BrackC, "]");

         Core::Array<IR::Value> vals{Core::Move, val.begin(), val.end()};
         Core::Array<IR::Type> types{vals.size()};

         for(std::size_t i = 0, e = vals.size(); i != e; ++i)
            types[i] = vals[i].getType();

         return IR::Value_Multi(std::move(vals),
            IR::Type_Multi(std::move(types)));
      }

      //
      // ParseMultiString
      //
      IR::Value_Multi ParseMultiString(Core::TokenStream &in, IR::Program &prog)
      {
         std::vector<IR::Value_Fixed> val;
         IR::Type_Fixed               type;

         // Optionally take a type for string elements.
         if(in.peek().tok != Core::TOK_String)
         {
            auto pos = in.peek().pos;
            auto t = ParseType(in, prog);
            if(t.t != IR::TypeBase::Fixed)
            {
               std::cerr << "ERROR: " << pos << ": expected Type_Fixed\n";
               throw EXIT_FAILURE;
            }

            type = t.tFixed;
         }

         // Otherwise, use a target-dependent default.
         else
         {
            // FIXME/TODO: ACS targets have 32-bit byte!
            type = IR::Type_Fixed(8, 0, false, false);
         }

         // Read string token.
         ExpectToken(in, Core::TOK_String, "string");
         auto str = in.get().str;

         // Convert to values.
         val.reserve(str.size());
         for(unsigned char c : str)
            val.emplace_back(c, type);

         return IR::Value_Multi(
            Core::Array<IR::Value>(Core::Move, val.begin(), val.end()),
            IR::Type_Multi(Core::Array<IR::Type>(val.size(), type)));
      }

      //
      // ParseNumber
      //
      IR::Value ParseNumber(Core::Token const &tok)
      {
         // Read prefix.
         char const *s;
         int         base;
         std::tie(s, base) = Core::ParseNumberBaseC(tok.str.getData().str);

         // Read integral part.
         Core::Integ valI;
         std::tie(s, valI, std::ignore) = Core::ParseNumberInteg(s, base);

         // Read fractional part.
         bool        hasF;
         std::size_t digF;
         Core::Integ valF;

         if((hasF = *s == '.'))
            std::tie(s, valF, digF) = Core::ParseNumberInteg(++s, base);
         else
            valF = 0, digF = 0;

         // Read exponent.
         Core::FastI valE;

         if(*s == 'E' || *s == 'e' || *s == 'P' || *s == 'p')
            std::tie(s, valE) = Core::ParseNumberExpC(s);
         else
            valE = 0;

         // Read suffix.
         Core::FastU bitsF;
         Core::FastU bitsI;
         bool        bitsS;
         bool        fixed;
         bool        satur;

         if(*s == '_')
         {
            std::tie(s, fixed, bitsS, bitsI, bitsF, satur) = ParseSuffix(++s, tok);
         }
         else
         {
            // TODO: Target-based determination of default type.
            if(hasF)
            {
               bitsF = 15;
               bitsI = 16;
               bitsS = true;
               fixed = true;
               satur = false;
            }
            else
            {
               bitsF = 0;
               bitsI = 32;
               bitsS = false;
               fixed = true;
               satur = false;
            }
         }

         // Check that end of string was reached.
         if(*s)
         {
            std::cerr << "ERROR: " << tok.pos << ": bad number: '" << tok.str << "'\n";
            throw EXIT_FAILURE;
         }

         if(fixed)
         {
            auto val = Core::MergeNumberFixedC(valI <<= bitsF, valF <<= bitsF,
               valE, digF, base);

            return IR::Value_Fixed(val, IR::Type_Fixed(bitsI, bitsF, bitsS, satur));
         }
         else
         {
            std::cerr << "STUB:" __FILE__ << ':' << __LINE__ << '\n';
            throw EXIT_FAILURE;
         }
      }
   }
}

// EOF

