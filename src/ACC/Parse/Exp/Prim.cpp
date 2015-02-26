//-----------------------------------------------------------------------------
//
// Copyright (C) 2015 David Hill
//
// See COPYING for license information.
//
//-----------------------------------------------------------------------------
//
// ACS primary-expression parsing.
//
//-----------------------------------------------------------------------------

#include "ACC/Parse.hpp"

#include "ACC/Pragma.hpp"

#include "AST/Attribute.hpp"
#include "AST/Exp.hpp"
#include "AST/Function.hpp"
#include "AST/Object.hpp"
#include "AST/Type.hpp"
#include "AST/Warning.hpp"

#include "CC/Exp.hpp"
#include "CC/Scope/Function.hpp"
#include "CC/Type.hpp"

#include "Core/Array.hpp"
#include "Core/Exception.hpp"
#include "Core/Parse.hpp"
#include "Core/TokenStream.hpp"

#include "IR/CallType.hpp"
#include "IR/Exp.hpp"
#include "IR/Glyph.hpp"


//----------------------------------------------------------------------------|
// Static Functions                                                           |
//

namespace GDCC
{
   namespace ACC
   {
      //
      // CheckDeprecated
      //
      template<typename T>
      static void CheckDeprecated(Core::Token const &tok, T const &entity)
      {
         if(!entity->warnUse || entity->warnDone) return;

         if(entity->warnUse != Core::STR_)
            AST::WarnDeprecated(tok.pos, tok.str, " is deprecated: ", entity->warnUse);
         else
            AST::WarnDeprecated(tok.pos, tok.str, " is deprecated");

         entity->warnDone = true;
      }

      //
      // GetExp_Prim_div
      //
      static AST::Exp::CRef GetExp_Prim_div(ParserCtx const &ctx, CC::Scope &scope)
      {
         // div-expression:
         //    <__div> ( assignment-expression , assignment-expression )

         // <__div>
         auto pos = ctx.in.get().pos;

         // (
         if(!ctx.in.drop(Core::TOK_ParenO))
            throw Core::ParseExceptExpect(ctx.in.peek(), "(", true);

         // assignment-expression
         auto l = GetExp_Assi(ctx, scope);

         // ,
         if(!ctx.in.drop(Core::TOK_Comma))
            throw Core::ParseExceptExpect(ctx.in.peek(), ",", true);

         // assignment-expression
         auto r = GetExp_Assi(ctx, scope);

         // )
         if(!ctx.in.drop(Core::TOK_ParenC))
            throw Core::ParseExceptExpect(ctx.in.peek(), ")", true);

         return CC::ExpCreate_DivEx(l, r, pos);
      }

      //
      // GetExp_Prim_offsetof
      //
      static AST::Exp::CRef GetExp_Prim_offsetof(ParserCtx const &ctx, CC::Scope &scope)
      {
         // offsetof-expression:
         //    <__offsetof> ( type-name , identifier )

         // <__offsetof>
         auto pos = ctx.in.get().pos;

         // (
         if(!ctx.in.drop(Core::TOK_ParenO))
            throw Core::ParseExceptExpect(ctx.in.peek(), "(", true);

         // type-name
         auto type = GetType(ctx, scope);

         if(!type->isCTypeObject() || !type->isTypeComplete())
            throw Core::ExceptStr(pos, "expected complete object type");

         // ,
         if(!ctx.in.drop(Core::TOK_Comma))
            throw Core::ParseExceptExpect(ctx.in.peek(), ",", true);

         // identifier
         if(!ctx.in.peek(Core::TOK_Identi))
            throw Core::ParseExceptExpect(ctx.in.peek(), "identifier", false);

         Core::String name = ctx.in.get().str;

         // )
         if(!ctx.in.drop(Core::TOK_ParenC))
            throw Core::ParseExceptExpect(ctx.in.peek(), ")", false);

         try
         {
            return CC::ExpCreate_LitInt(AST::Type::Size, type->getMember(name).addr, pos);
         }
         catch(AST::TypeError const &)
         {
            throw Core::ExceptStr(pos, "invalid member");
         }
      }

      //
      // GetExp_Prim_Charac
      //
      static AST::Exp::CRef GetExp_Prim_Charac(ParserCtx const &ctx, CC::Scope &)
      {
         auto tok = ctx.in.get();

         auto type = tok.tok == Core::TOK_Charac ? CC::TypeIntegPrS : CC::TypeIntegPrU;

         return CC::ExpCreate_LitInt(type, tok.str[0], tok.pos);
      }

      //
      // GetExp_Prim_Identi
      //
      static AST::Exp::CRef GetExp_Prim_Identi(ParserCtx const &ctx, CC::Scope &scope)
      {
         switch(ctx.in.peek().str)
         {
         case Core::STR___div:      return GetExp_Prim_div(ctx, scope);
         case Core::STR___offsetof: return GetExp_Prim_offsetof(ctx, scope);

         default: break;
         }

         auto tok = ctx.in.get();

         if(auto lookup = scope.lookup(tok.str)) switch(lookup.res)
         {
         case CC::Lookup::Enum:
            return CC::ExpCreate_LitInt(CC::TypeIntegPrS, *lookup.resEnum, tok.pos);

         case CC::Lookup::Func:
            CheckDeprecated(tok, lookup.resFunc);
            return CC::ExpCreate_Func(ctx.prog, lookup.resFunc, tok.pos);

         case CC::Lookup::Obj:
            CheckDeprecated(tok, lookup.resObj);
            return CC::ExpCreate_Obj(ctx.prog, lookup.resObj, tok.pos);

         default:
            throw Core::ParseExceptExpect(tok, "primary-expression", false);
         }

         throw Core::ParseExceptExpect(tok, "declared identifier", false);
      }

      //
      // GetExp_Prim_KeyWrd
      //
      static AST::Exp::CRef GetExp_Prim_KeyWrd(ParserCtx const &ctx, CC::Scope &)
      {
         switch(ctx.in.peek().str)
         {
         default:
            throw Core::ParseExceptExpect(ctx.in.peek(), "primary-expression", false);
         }
      }

      //
      // GetExp_Prim_NumFix
      //
      static AST::Exp::CRef GetExp_Prim_NumFix(ParserCtx const &ctx, CC::Scope &)
      {
         auto tok = ctx.in.get();

         // Get prefix.
         auto     itr = tok.str.begin(), end = tok.str.end();
         unsigned base;
         std::tie(itr, base) = Core::ParseNumberBaseC(itr);

         // Get numeric component.
         Core::Ratio val;
         std::tie(itr, val, std::ignore) = Core::ParseNumberRatioC(itr, base);

         // Get suffix.
         bool k;
         int  l;
         bool u;

              if(*itr == 'U' || *itr == 'u') u = true, ++itr;
         else                                u = false;

              if(*itr == 'H' || *itr == 'h') l = -1, ++itr;
         else if(*itr == 'L' || *itr == 'l') l = +1, ++itr;
         else                                l =  0;

              if(*itr == 'K' || *itr == 'k') k = true,  ++itr;
         else                                k = false;

         // Must be end of string.
         if(itr != end)
            throw Core::ExceptStr(tok.pos, "malformed fixed-constant");

         // Dtermine type.
         AST::Type::CPtr type;

         switch(l)
         {
         case -1: type = u ? CC::TypeFixedPrUH : CC::TypeFixedPrSH; break;
         case  0: type = u ? CC::TypeFixedPrU  : CC::TypeFixedPrS;  break;
         case +1: type = u ? CC::TypeFixedPrUL : CC::TypeFixedPrSL; break;
         }

         val <<= type->getSizeBitsF();

         if(!k && !ctx.prag.stateFixedLiteral)
            type = u ? CC::TypeIntegPrU : CC::TypeIntegPrS;

         // Create expression.
         return CC::ExpCreate_LitInt(type, Core::NumberCast<Core::Integ>(val), tok.pos);
      }

      //
      // GetExp_Prim_NumFlt
      //
      static AST::Exp::CRef GetExp_Prim_NumFlt(ParserCtx const &ctx, CC::Scope &)
      {
         auto tok = ctx.in.get();

         // Get prefix.
         auto     itr = tok.str.begin(), end = tok.str.end();
         unsigned base;
         std::tie(itr, base) = Core::ParseNumberBaseC(itr);

         // Get numeric component.
         Core::Ratio val;
         std::tie(itr, val, std::ignore) = Core::ParseNumberRatioC(itr, base);

         // Get suffix.
         int l;

         if(*itr == 'L' || *itr == 'l')
            l = itr[1] == itr[0] ? ++itr, 2 : 1, ++itr;
         else
            l = 0;

         if(*itr == 'F' || *itr == 'f') ++itr;

         // Must be end of string.
         if(itr != end)
            throw Core::ExceptStr(tok.pos, "malformed floating-constant");

         // Dtermine type.
         AST::Type::CPtr type;

         switch(l)
         {
         case  0: type = CC::TypeFloatRS;   break;
         case +1: type = CC::TypeFloatRSL;  break;
         case +2: type = CC::TypeFloatRSLL; break;
         }

         // Create expression.
         auto valIR = IR::Value_Float(std::move(val), type->getIRType().tFloat);
         auto expIR = IR::ExpCreate_Value(std::move(valIR), tok.pos);
         return AST::ExpCreate_IRExp(expIR, type, tok.pos);
      }

      //
      // GetExp_Prim_NumInt
      //
      static AST::Exp::CRef GetExp_Prim_NumInt(ParserCtx const &ctx, CC::Scope &)
      {
         auto tok = ctx.in.get();

         auto        itr = tok.str.begin(), end = tok.str.end();
         unsigned    base;
         Core::Integ val;
         unsigned    l = 0;
         bool        u = false;

         // Parse integer base.
         std::tie(itr, base)             = Core::ParseNumberBaseC(itr);
         std::tie(itr, val, std::ignore) = Core::ParseNumberInteg(itr, base);

         // Parse suffix.
         for(; itr != end; ++itr) switch(*itr)
         {
         case 'L': case 'l':
            if(l) throw Core::ExceptStr(tok.pos, "duplicate L");
            l = itr[1] == itr[0] ? ++itr, 2 : 1;
            break;

         case 'U': case 'u':
            if(u) throw Core::ExceptStr(tok.pos, "duplicate U");
            u = true;
            break;

         default:
            throw Core::ExceptStr(tok.pos, "malformed integer-constant");
         }

         // Octal/hex literals will promote to unsigned if necessary.
         bool x = u || base != 10;

         // Generate expression.
         switch(l)
         {
            #define tryCreate(c, t) \
               if((c) && (val >> (t)->getSizeBitsI()) == 0) \
                  return CC::ExpCreate_LitInt((t), std::move(val), tok.pos)

         case 0:
            tryCreate(!u, CC::TypeIntegPrS);
            tryCreate( x, CC::TypeIntegPrU);
         case 1:
            tryCreate(!u, CC::TypeIntegPrSL);
            tryCreate( x, CC::TypeIntegPrUL);
         case 2:
            tryCreate(!u, CC::TypeIntegPrSLL);
            tryCreate( x, CC::TypeIntegPrULL);
         default:
            throw Core::ExceptStr(tok.pos, "oversized integer-constant");

            #undef tryCreate
         }
      }

      //
      // GetExp_Prim_ParenO
      //
      static AST::Exp::CRef GetExp_Prim_ParenO(ParserCtx const &ctx, CC::Scope &scope)
      {
         // (
         ctx.in.get();

         // expression
         auto exp = GetExp(ctx, scope);

         // )
         if(!ctx.in.drop(Core::TOK_ParenC))
            throw Core::ParseExceptExpect(ctx.in.peek(), ")", true);

         return exp;
      }

      //
      // GetExp_Prim_String
      //
      static AST::Exp::CRef GetExp_Prim_String(ParserCtx const &ctx, CC::Scope &scope)
      {
         auto tok = ctx.in.get();

         return CC::ExpCreate_StrIdx(ctx.prog, scope, tok.str, tok.pos);
      }
   }
}


//----------------------------------------------------------------------------|
// Global Functions                                                           |
//

namespace GDCC
{
   namespace ACC
   {
      //
      // GetExp_Prim
      //
      AST::Exp::CRef GetExp_Prim(ParserCtx const &ctx, CC::Scope &scope)
      {
         switch(ctx.in.peek().tok)
         {
         case Core::TOK_Charac: return GetExp_Prim_Charac(ctx, scope);
         case Core::TOK_Identi: return GetExp_Prim_Identi(ctx, scope);
         case Core::TOK_KeyWrd: return GetExp_Prim_KeyWrd(ctx, scope);
         case Core::TOK_NumFix: return GetExp_Prim_NumFix(ctx, scope);
         case Core::TOK_NumFlt: return GetExp_Prim_NumFlt(ctx, scope);
         case Core::TOK_NumInt: return GetExp_Prim_NumInt(ctx, scope);
         case Core::TOK_String: return GetExp_Prim_String(ctx, scope);
         case Core::TOK_ParenO: return GetExp_Prim_ParenO(ctx, scope);

         default:
            throw Core::ParseExceptExpect(ctx.in.peek(), "primary-expression", false);
         }
      }
   }
}

// EOF

