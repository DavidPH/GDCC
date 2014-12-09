//-----------------------------------------------------------------------------
//
// Copyright (C) 2014 David Hill
//
// See COPYING for license information.
//
//-----------------------------------------------------------------------------
//
// C primary-expression parsing.
//
//-----------------------------------------------------------------------------

#include "CC/Parse.hpp"

#include "CC/Exp.hpp"
#include "CC/Scope/Function.hpp"
#include "CC/Type.hpp"

#include "AST/Attribute.hpp"
#include "AST/Exp.hpp"
#include "AST/Function.hpp"
#include "AST/Object.hpp"
#include "AST/Type.hpp"

#include "CPP/Pragma.hpp"

#include "Core/Array.hpp"
#include "Core/Exception.hpp"
#include "Core/Parse.hpp"
#include "Core/TokenStream.hpp"
#include "Core/Warning.hpp"

#include "IR/Exp.hpp"
#include "IR/Glyph.hpp"


//----------------------------------------------------------------------------|
// Static Functions                                                           |
//

namespace GDCC
{
   namespace CC
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
      static AST::Exp::CRef GetExp_Prim_div(ParserCtx const &ctx, Scope &scope)
      {
         // div-expression:
         //    <__div> ( assignment-expression , assignment-expression )

         // <__div>
         auto pos = ctx.in.get().pos;

         // (
         if(!ctx.in.drop(Core::TOK_ParenO))
            throw Core::ExceptStr(ctx.in.peek().pos, "expected '('");

         // assignment-expression
         auto l = GetExp_Assi(ctx, scope);

         // ,
         if(!ctx.in.drop(Core::TOK_Comma))
            throw Core::ExceptStr(ctx.in.peek().pos, "expected ','");

         // assignment-expression
         auto r = GetExp_Assi(ctx, scope);

         // )
         if(!ctx.in.drop(Core::TOK_ParenC))
            throw Core::ExceptStr(ctx.in.peek().pos, "expected ')'");

         return ExpCreate_DivEx(l, r, pos);
      }

      //
      // GetExp_Prim_func
      //
      static AST::Exp::CRef GetExp_Prim_func(ParserCtx const &ctx, Scope &scope)
      {
         auto &scopeFn = static_cast<Scope_Local &>(scope).fn;

         // <__func__>
         auto tok = ctx.in.get();

         if(!scopeFn.nameObj)
            scopeFn.nameObj = ExpCreate_String(ctx.prog, scope, scopeFn.fn->name, tok.pos);

         return static_cast<AST::Exp::CRef>(scopeFn.nameObj);
      }

      //
      // GetExp_Prim_generic
      //
      static AST::Exp::CRef GetExp_Prim_generic(ParserCtx const &ctx, Scope &scope)
      {
         // generic-selection:
         //    <_Generic> ( assignment-expression , generic-assoc-list )

         // <_Generic>
         auto pos = ctx.in.get().pos;

         // (
         if(!ctx.in.drop(Core::TOK_ParenO))
            throw Core::ExceptStr(ctx.in.peek().pos, "expected '('");

         // assignment-expression
         auto exp = GetExp_Assi(ctx, scope);

         // generic-assoc-list:
         //    generic-association
         //    generic-assoc-list , generic-association
         AST::Exp::CPtr            def;
         std::vector<GenAssoc> vec;
         while(ctx.in.drop(Core::TOK_Comma))
         {
            // generic-association:
            //    type-name : assignment-expression
            //    <default> : assignment-expression

            // type-name
            if(IsType(ctx, scope))
            {
               auto type = GetType(ctx, scope);

               // :
               if(!ctx.in.drop(Core::TOK_Colon))
                  throw Core::ExceptStr(ctx.in.peek().pos, "expected ':'");

               // assignment-expression
               vec.emplace_back(type, GetExp_Assi(ctx, scope));
            }

            // <default>
            else if(ctx.in.drop(Core::TOK_KeyWrd, Core::STR_default))
            {
               if(def)
                  throw Core::ExceptStr(ctx.in.peek().pos, "multiple default");

               // :
               if(!ctx.in.drop(Core::TOK_Colon))
                  throw Core::ExceptStr(ctx.in.peek().pos, "expected ':'");

               // assignment-expression
               def = GetExp_Assi(ctx, scope);
            }

            else
               throw Core::ExceptStr(ctx.in.peek().pos, "expected generic-association");
         }

         // )
         if(!ctx.in.drop(Core::TOK_ParenC))
            throw Core::ExceptStr(ctx.in.peek().pos, "expected ')'");

         return ExpCreate_GenSel(exp, def, {Core::Move, vec.begin(), vec.end()}, pos);
      }

      //
      // GetExp_Prim_glyph
      //
      static AST::Exp::CRef GetExp_Prim_glyph(ParserCtx const &ctx, Scope &scope)
      {
         // glyph-expression:
         //    <__glyph> ( type-name , string-literal )

         // <__glyph>
         auto pos = ctx.in.get().pos;

         // (
         if(!ctx.in.drop(Core::TOK_ParenO))
            throw Core::ExceptStr(ctx.in.peek().pos, "expected '('");

         // type-name
         auto type = GetType(ctx, scope);

         if(!type->isCTypeObject() || !type->isTypeComplete())
            throw Core::ExceptStr(pos, "expected complete object type");

         // ,
         if(!ctx.in.drop(Core::TOK_Comma))
            throw Core::ExceptStr(ctx.in.peek().pos, "expected ','");

         // string-literal
         if(!ctx.in.peek().isTokString())
            throw Core::ExceptStr(ctx.in.peek().pos, "expected string-literal");

         IR::Glyph glyph = {ctx.prog, ctx.in.get().str};

         // )
         if(!ctx.in.drop(Core::TOK_ParenC))
            throw Core::ExceptStr(ctx.in.peek().pos, "expected ')'");

         // If the glyph has no type yet, set it now.
         auto &glyphData = glyph.getData();
         glyphData.type = type->getIRType();

         return AST::ExpCreate_IRExp(IR::ExpCreate_Glyph(glyph, pos), type, pos);
      }

      //
      // GetExp_Prim_offsetof
      //
      static AST::Exp::CRef GetExp_Prim_offsetof(ParserCtx const &ctx, Scope &scope)
      {
         // offsetof-expression:
         //    <__offsetof> ( type-name , identifier )

         // <__offsetof>
         auto pos = ctx.in.get().pos;

         // (
         if(!ctx.in.drop(Core::TOK_ParenO))
            throw Core::ExceptStr(ctx.in.peek().pos, "expected '('");

         // type-name
         auto type = GetType(ctx, scope);

         if(!type->isCTypeObject() || !type->isTypeComplete())
            throw Core::ExceptStr(pos, "expected complete object type");

         // ,
         if(!ctx.in.drop(Core::TOK_Comma))
            throw Core::ExceptStr(ctx.in.peek().pos, "expected ','");

         // identifier
         if(!ctx.in.peek(Core::TOK_Identi))
            throw Core::ExceptStr(ctx.in.peek().pos, "expected identifier");

         Core::String name = ctx.in.get().str;

         // )
         if(!ctx.in.drop(Core::TOK_ParenC))
            throw Core::ExceptStr(ctx.in.peek().pos, "expected ')'");

         try
         {
            return ExpCreate_LitInt(AST::Type::Size, type->getMember(name).addr, pos);
         }
         catch(AST::TypeError const &)
         {
            throw Core::ExceptStr(pos, "invalid member");
         }
      }

      //
      // GetExp_Prim_va_arg
      //
      static AST::Exp::CRef GetExp_Prim_va_arg(ParserCtx const &ctx, Scope &scope)
      {
         // va_arg-expression:
         //    <__va_arg> ( assignment-expression , type-name )

         // <__va_arg>
         auto pos = ctx.in.get().pos;

         // (
         if(!ctx.in.drop(Core::TOK_ParenO))
            throw Core::ExceptStr(ctx.in.peek().pos, "expected '('");

         // assignment-expression
         auto exp = GetExp_Assi(ctx, scope);

         // ,
         if(!ctx.in.drop(Core::TOK_Comma))
            throw Core::ExceptStr(ctx.in.peek().pos, "expected ','");

         // type-name
         auto type = GetType(ctx, scope);

         // )
         if(!ctx.in.drop(Core::TOK_ParenC))
            throw Core::ExceptStr(ctx.in.peek().pos, "expected ')'");

         return ExpCreate_VaArg(type, exp, pos);
      }

      //
      // GetExp_Prim_va_start
      //
      static AST::Exp::CRef GetExp_Prim_va_start(ParserCtx const &ctx, Scope &)
      {
         // va_start-expression:
         //    <__va_start>

         // <__va_start>
         auto pos = ctx.in.get().pos;

         auto type = AST::Type::Void
            ->getTypeQual({{IR::AddrBase::Loc, Core::STR_}})
            ->getTypePointer();

         return ExpCreate_LitNul(type, pos);
      }

      //
      // GetExp_Prim_Charac
      //
      static AST::Exp::CRef GetExp_Prim_Charac(ParserCtx const &ctx, Scope &)
      {
         auto tok = ctx.in.get();

         auto type = tok.tok == Core::TOK_Charac ? TypeIntegPrS : TypeIntegPrU;

         return ExpCreate_LitInt(type, tok.str[0], tok.pos);
      }

      //
      // GetExp_Prim_Identi
      //
      static AST::Exp::CRef GetExp_Prim_Identi(ParserCtx const &ctx, Scope &scope)
      {
         switch(ctx.in.peek().str)
         {
         case Core::STR___div:      return GetExp_Prim_div(ctx, scope);
         case Core::STR___glyph:    return GetExp_Prim_glyph(ctx, scope);
         case Core::STR___offsetof: return GetExp_Prim_offsetof(ctx, scope);
         case Core::STR___va_arg:   return GetExp_Prim_va_arg(ctx, scope);
         case Core::STR___va_start: return GetExp_Prim_va_start(ctx, scope);

         case Core::STR___func__:
            if(dynamic_cast<Scope_Local *>(&scope))
               return GetExp_Prim_func(ctx, scope);
            break;

         default: break;
         }

         auto tok = ctx.in.get();

         if(auto lookup = scope.lookup(tok.str)) switch(lookup.res)
         {
         case Lookup::Enum:
            return ExpCreate_LitInt(TypeIntegPrS, *lookup.resEnum, tok.pos);

         case Lookup::Func:
            CheckDeprecated(tok, lookup.resFunc);
            return ExpCreate_Func(ctx.prog, lookup.resFunc, tok.pos);

         case Lookup::Obj:
            CheckDeprecated(tok, lookup.resObj);
            return ExpCreate_Obj(ctx.prog, lookup.resObj, tok.pos);

         default:
            throw Core::ExceptStr(tok.pos, "expected primary-expression");
         }

         // TODO: implicit function declaration
         // It sucks, but it is traditional and the method will be needed for ACS.

         throw Core::ParseExceptExpect(tok.pos, "declared identifier", tok.str, false);
      }

      //
      // GetExp_Prim_KeyWrd
      //
      static AST::Exp::CRef GetExp_Prim_KeyWrd(ParserCtx const &ctx, Scope &scope)
      {
         switch(ctx.in.peek().str)
         {
         case Core::STR__Generic: return GetExp_Prim_generic(ctx, scope);
         default:
            throw Core::ExceptStr(ctx.in.peek().pos, "expected primary-expression");
         }
      }

      //
      // GetExp_Prim_NumFix
      //
      static AST::Exp::CRef GetExp_Prim_NumFix(ParserCtx const &ctx, Scope &)
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
         else if(*itr == 'R' || *itr == 'r') k = false, ++itr;
         else                                k = true;

         // Must be end of string.
         if(itr != end)
            throw Core::ExceptStr(tok.pos, "malformed fixed-constant");

         // Dtermine type.
         AST::Type::CPtr type;

         if(k) switch(l)
         {
         case -1: type = u ? TypeFixedPrUH : TypeFixedPrSH; break;
         case  0: type = u ? TypeFixedPrU  : TypeFixedPrS;  break;
         case +1: type = u ? TypeFixedPrUL : TypeFixedPrSL; break;
         }
         else switch(l)
         {
         case -1: type = u ? TypeFractPrUH : TypeFractPrSH; break;
         case  0: type = u ? TypeFractPrU  : TypeFractPrS;  break;
         case +1: type = u ? TypeFractPrUL : TypeFractPrSL; break;
         }

         // Adjust value for fractional bits.
         // Special case where 1.0r translates to the maximum value.
         if(!k && val == 1)
            --(val <<= type->getSizeBitsF());
         else
            val <<= type->getSizeBitsF();

         // Create expression.
         return ExpCreate_LitInt(type, Core::NumberCast<Core::Integ>(val), tok.pos);
      }

      //
      // GetExp_Prim_NumFlt
      //
      static AST::Exp::CRef GetExp_Prim_NumFlt(ParserCtx const &ctx, Scope &scope)
      {
         // Check if this should be treated as a fixed-point literal.
         if(ctx.pragGDCC.pragmaGDCC_FixedLiteral) switch(ctx.in.peek().str.back())
         {
         case 'F': case 'f': break;
         default: return GetExp_Prim_NumFix(ctx, scope);
         }

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

         // If FIXED_LITERAL is in effect, allow alternative suffix syntax.
         if(ctx.pragGDCC.pragmaGDCC_FixedLiteral)
         {
            if(*itr == 'L' || *itr == 'l')
               l = itr[1] == itr[0] ? ++itr, 2 : 1, ++itr;
            else
               l = 0;

            if(*itr == 'F' || *itr == 'f') ++itr;
         }
         else
         {
                 if(*itr == 'F' || *itr == 'f') l =  0, ++itr;
            else if(*itr == 'L' || *itr == 'l') l = +2, ++itr;
            else                                l = +1;
         }

         // Must be end of string.
         if(itr != end)
            throw Core::ExceptStr(tok.pos, "malformed floating-constant");

         // Dtermine type.
         AST::Type::CPtr type;

         switch(l)
         {
         case  0: type = TypeFloatRS;   break;
         case +1: type = TypeFloatRSL;  break;
         case +2: type = TypeFloatRSLL; break;
         }

         // Create expression.
         auto valIR = IR::Value_Float(std::move(val), type->getIRType().tFloat);
         auto expIR = IR::ExpCreate_Value(std::move(valIR), tok.pos);
         return AST::ExpCreate_IRExp(expIR, type, tok.pos);
      }

      //
      // GetExp_Prim_NumInt
      //
      static AST::Exp::CRef GetExp_Prim_NumInt(ParserCtx const &ctx, Scope &)
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
                  return ExpCreate_LitInt((t), std::move(val), tok.pos)

         case 0:
            tryCreate(!u, TypeIntegPrS);
            tryCreate( x, TypeIntegPrU);
         case 1:
            tryCreate(!u, TypeIntegPrSL);
            tryCreate( x, TypeIntegPrUL);
         case 2:
            tryCreate(!u, TypeIntegPrSLL);
            tryCreate( x, TypeIntegPrULL);
         default:
            throw Core::ExceptStr(tok.pos, "oversized integer-constant");

            #undef tryCreate
         }
      }

      //
      // GetExp_Prim_ParenO
      //
      static AST::Exp::CRef GetExp_Prim_ParenO(ParserCtx const &ctx, Scope &scope)
      {
         // (
         ctx.in.get();

         // expression
         auto exp = GetExp(ctx, scope);

         // )
         if(!ctx.in.drop(Core::TOK_ParenC))
            throw Core::ExceptStr(ctx.in.peek().pos, "expected ')'");

         return exp;
      }

      //
      // GetExp_Prim_StrChr
      //
      static AST::Exp::CRef GetExp_Prim_StrChr(ParserCtx const &ctx, Scope &scope)
      {
         auto tok = ctx.in.get();

         return ExpCreate_String(ctx.prog, scope, tok.str, tok.pos);
      }

      //
      // GetExp_Prim_StrIdx
      //
      static AST::Exp::CRef GetExp_Prim_StrIdx(ParserCtx const &ctx, Scope &scope)
      {
         auto tok = ctx.in.get();

         return ExpCreate_StrIdx(ctx.prog, scope, tok.str, tok.pos);
      }

      //
      // GetExp_Prim_StrU08
      //
      static AST::Exp::CRef GetExp_Prim_StrU08(ParserCtx const &ctx, Scope &scope)
      {
         auto tok = ctx.in.get();

         return ExpCreate_StrU08(ctx.prog, scope, tok.str, tok.pos);
      }

      //
      // GetExp_Prim_StrU16
      //
      static AST::Exp::CRef GetExp_Prim_StrU16(ParserCtx const &ctx, Scope &scope)
      {
         auto tok = ctx.in.get();

         return ExpCreate_StrU16(ctx.prog, scope, tok.str, tok.pos);
      }

      //
      // GetExp_Prim_StrU32
      //
      static AST::Exp::CRef GetExp_Prim_StrU32(ParserCtx const &ctx, Scope &scope)
      {
         auto tok = ctx.in.get();

         return ExpCreate_StrU32(ctx.prog, scope, tok.str, tok.pos);
      }

      //
      // GetExp_Prim_String
      //
      static AST::Exp::CRef GetExp_Prim_String(ParserCtx const &ctx, Scope &scope)
      {
         auto tok = ctx.in.get();

         if(ctx.pragGDCC.pragmaGDCC_StrEntLiteral)
            return ExpCreate_StrIdx(ctx.prog, scope, tok.str, tok.pos);
         else
            return ExpCreate_String(ctx.prog, scope, tok.str, tok.pos);
      }
   }
}


//----------------------------------------------------------------------------|
// Global Functions                                                           |
//

namespace GDCC
{
   namespace CC
   {
      //
      // GetExp_Prim
      //
      AST::Exp::CRef GetExp_Prim(ParserCtx const &ctx, Scope &scope)
      {
         switch(ctx.in.peek().tok)
         {
         case Core::TOK_Charac: return GetExp_Prim_Charac(ctx, scope);
         case Core::TOK_ChrU16: return GetExp_Prim_Charac(ctx, scope);
         case Core::TOK_ChrU32: return GetExp_Prim_Charac(ctx, scope);
         case Core::TOK_ChrWid: return GetExp_Prim_Charac(ctx, scope);
         case Core::TOK_Identi: return GetExp_Prim_Identi(ctx, scope);
         case Core::TOK_KeyWrd: return GetExp_Prim_KeyWrd(ctx, scope);
         case Core::TOK_NumFix: return GetExp_Prim_NumFix(ctx, scope);
         case Core::TOK_NumFlt: return GetExp_Prim_NumFlt(ctx, scope);
         case Core::TOK_NumInt: return GetExp_Prim_NumInt(ctx, scope);
         case Core::TOK_StrChr: return GetExp_Prim_StrChr(ctx, scope);
         case Core::TOK_StrIdx: return GetExp_Prim_StrIdx(ctx, scope);
         case Core::TOK_StrU08: return GetExp_Prim_StrU08(ctx, scope);
         case Core::TOK_StrU16: return GetExp_Prim_StrU16(ctx, scope);
         case Core::TOK_StrU32: return GetExp_Prim_StrU32(ctx, scope);
         case Core::TOK_StrWid: return GetExp_Prim_StrU32(ctx, scope);
         case Core::TOK_String: return GetExp_Prim_String(ctx, scope);
         case Core::TOK_ParenO: return GetExp_Prim_ParenO(ctx, scope);

         default:
            throw Core::ExceptStr(ctx.in.peek().pos, "expected primary-expression");
         }
      }
   }
}

// EOF

