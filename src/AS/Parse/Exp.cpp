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

#include "Core/Exception.hpp"
#include "Core/Parse.hpp"
#include "Core/TokenStream.hpp"

#include "IR/Exp.hpp"
#include "IR/Glyph.hpp"

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
   using namespace GDCC;

   Core::FastU bitsF;
   Core::FastU bitsI;
   bool        bitsS;
   bool        fixed;
   bool        satur;

   fixed = *s == 'F' || *s == 'f' ? ++s, false : true;
   bitsS = *s == 'S' || *s == 's' ? ++s, true : false;
   std::tie(s, bitsI, std::ignore) = Core::ParseNumberFastU(s, 10);

   if(*s != '.')
      throw Core::ParseExceptExpect(tok, "number", false);

   std::tie(s, bitsF, std::ignore) = Core::ParseNumberFastU(++s, 10);
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
      // GetExp
      //
      IR::Exp::CRef GetExp(ParserCtx const &ctx)
      {
         #define doE1(name, token) case Core::token: \
            return IR::ExpCreate_##name(GetExp(ctx), tok.pos)

         #define doE2(name, token) case Core::token: \
            l = GetExp(ctx); r = GetExp(ctx); \
            return IR::ExpCreate_##name(l, r, tok.pos)

         #define doE3(name, token) case Core::token: \
            c = GetExp(ctx); l = GetExp(ctx); r = GetExp(ctx); \
            return IR::ExpCreate_##name(c, l, r, tok.pos)

         std::vector<IR::Exp::CRef> v;

         IR::Exp::CPtr c, l, r;
         IR::Type t;

         Core::Token tok;
         switch((tok = ctx.in.get()).tok)
         {
         case Core::TOK_Identi:
            switch(tok.str)
            {
               doE2(Add,       STR_Add);
               doE2(AddPtrRaw, STR_AddPtrRaw);
               doE2(BitAnd,    STR_BitAnd);
               doE2(BitOrI,    STR_BitOrI);
               doE2(BitOrX,    STR_BitOrX);
               doE2(CmpEQ,     STR_CmpEQ);
               doE2(CmpGE,     STR_CmpGE);
               doE2(CmpGT,     STR_CmpGT);
               doE2(CmpLE,     STR_CmpLE);
               doE2(CmpLT,     STR_CmpLT);
               doE2(CmpNE,     STR_CmpNE);
               doE3(Cnd,       STR_Cnd);
               doE2(Div,       STR_Div);
               doE1(Inv,       STR_Inv);
               doE2(LogAnd,    STR_LogAnd);
               doE2(LogOrI,    STR_LogOrI);
               doE2(LogOrX,    STR_LogOrX);
               doE2(Mod,       STR_Mod);
               doE2(Mul,       STR_Mul);
               doE1(Neg,       STR_Neg);
               doE1(Not,       STR_Not);
               doE2(ShL,       STR_ShL);
               doE2(ShR,       STR_ShR);
               doE2(Sub,       STR_Sub);

            case Core::STR_Cst:
               t = GetType(ctx);
               return IR::ExpCreate_Cst(std::move(t), GetExp(ctx), tok.pos);

            case Core::STR_Multi:
               TokenDrop(ctx, Core::TOK_ParenO, "'('");

               if(!ctx.in.peek(Core::TOK_ParenC)) do
                  v.emplace_back(GetExp(ctx));
               while(ctx.in.drop(Core::TOK_Comma));

               TokenDrop(ctx, Core::TOK_ParenC, "')'");

               return IR::ExpCreate_Multi({Core::Move, v.begin(), v.end()}, tok.pos);

            case Core::STR_Value:
               ctx.in.unget();
               return IR::ExpCreate_Value(GetValue(ctx), tok.pos);

            case Core::STR_string:
               return IR::ExpCreate_Value(GetMultiString(ctx), tok.pos);

            default:
               throw Core::ParseExceptExpect(tok, "expression", false);
            }

         case Core::TOK_Number:
            return IR::ExpCreate_Value(GetNumber(tok), tok.pos);

         case Core::TOK_String:
            return IR::ExpCreate_Glyph(IR::Glyph(ctx.prog, tok.str), tok.pos);

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
            if(!ctx.in.peek(Core::TOK_BraceC)) do
               v.emplace_back(GetExp(ctx));
            while(ctx.in.drop(Core::TOK_Comma));

            TokenDrop(ctx, Core::TOK_BraceC, "'}'");

            return IR::ExpCreate_Multi({Core::Move, v.begin(), v.end()}, tok.pos);

         case Core::TOK_BrackO:
            ctx.in.unget();
            return IR::ExpCreate_Value(GetMulti(ctx), tok.pos);

         default:
            throw Core::ParseExceptExpect(tok, "expression", false);
         }

         #undef doE3
         #undef doE2
         #undef doE1
      }

      //
      // GetFastI
      //
      Core::FastI GetFastI(ParserCtx const &ctx)
      {
         auto i = GetInteg(ctx);

         return number_cast<Core::FastI>(i);
      }

      //
      // GetFastU
      //
      Core::FastU GetFastU(ParserCtx const &ctx)
      {
         auto i = GetInteg(ctx);

         return number_cast<Core::FastU>(i);
      }

      //
      // GetInteg
      //
      Core::Integ GetInteg(ParserCtx const &ctx)
      {
         auto exp = GetExp(ctx);
         auto val = exp->getValue();

         switch(val.v)
         {
         case IR::ValueBase::Fixed:
            return val.vFixed.value >> val.vFixed.vtype.bitsF;

         default:
            throw Core::ParseExceptExpect(exp->pos, "integer expression",
               "non-integer expression", false, false);
         }
      }

      //
      // GetMulti
      //
      IR::Value_Multi GetMulti(ParserCtx const &ctx)
      {
         std::vector<IR::Value> val;

         TokenDrop(ctx, Core::TOK_BrackO, "'['");

         if(!ctx.in.peek(Core::TOK_BrackC)) do
            val.emplace_back(GetExp(ctx)->getValue());
         while(ctx.in.drop(Core::TOK_Comma));

         TokenDrop(ctx, Core::TOK_BrackC, "']'");

         Core::Array<IR::Value> vals{Core::Move, val.begin(), val.end()};
         Core::Array<IR::Type> types{vals.size()};

         for(std::size_t i = 0, e = vals.size(); i != e; ++i)
            types[i] = vals[i].getType();

         return {std::move(vals), {std::move(types)}};
      }

      //
      // GetMultiString
      //
      IR::Value_Multi GetMultiString(ParserCtx const &ctx)
      {
         std::vector<IR::Value_Fixed> val;
         IR::Type_Fixed               type;

         // Optionally take a type for string elements.
         if(!ctx.in.peek(Core::TOK_String))
         {
            type = GetType_Fixed(ctx);
         }
         // Otherwise, use a target-dependent default.
         else
         {
            // FIXME/TODO: ACS targets have 32-bit byte!
            type = IR::Type_Fixed(8, 0, false, false);
         }

         // Read string token.
         auto str = TokenPeekString(ctx).in.get().str;

         // Convert to values.
         val.reserve(str.size());
         for(unsigned char c : str)
            val.emplace_back(c, type);

         return {{Core::Move, val.begin(), val.end()}, {{val.size(), type}}};
      }

      //
      // GetNumber
      //
      IR::Value GetNumber(Core::Token const &tok)
      {
         // Read prefix.
         char const *s;
         int         base;
         std::tie(s, base) = Core::ParseNumberBaseC(tok.str.data());

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
            throw Core::ParseExceptExpect(tok, "number", false);

         if(fixed)
         {
            auto val = Core::MergeNumberFixedC(valI <<= bitsF, valF <<= bitsF,
               valE, digF, base);

            return IR::Value_Fixed(std::move(val), {bitsI, bitsF, bitsS, satur});
         }
         else
         {
            throw Core::ParseExceptExpect(tok, "fixed-point number", false);
         }
      }
   }
}

// EOF

