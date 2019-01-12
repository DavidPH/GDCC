//-----------------------------------------------------------------------------
//
// Copyright (C) 2014-2019 David Hill
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
#include "CC/Factory.hpp"
#include "CC/Scope/Function.hpp"
#include "CC/Type.hpp"

#include "CPP/Pragma.hpp"

#include "Core/Array.hpp"
#include "Core/Exception.hpp"
#include "Core/Parse.hpp"
#include "Core/TokenStream.hpp"

#include "IR/Exp.hpp"

#include "SR/Attribute.hpp"
#include "SR/Exp.hpp"
#include "SR/Function.hpp"
#include "SR/Object.hpp"
#include "SR/Type.hpp"
#include "SR/Warning.hpp"

#include "Target/CallType.hpp"


//----------------------------------------------------------------------------|
// Static Functions                                                           |
//

namespace GDCC::CC
{
   //
   // CheckDeprecated
   //
   template<typename T>
   static void CheckDeprecated(Core::Token const &tok, T const &entity)
   {
      if(!entity->warnUse || entity->warnDone) return;

      if(entity->warnUse != Core::STR_)
         SR::WarnDeprecated(tok.pos, tok.str, " is deprecated: ", entity->warnUse);
      else
         SR::WarnDeprecated(tok.pos, tok.str, " is deprecated");

      entity->warnDone = true;
   }

   //
   // GetExp_Prim_func
   //
   static SR::Exp::CRef GetExp_Prim_func(Parser &ctx, Scope &scope)
   {
      auto &scopeFn = static_cast<Scope_Local &>(scope).fn;

      // <__func__>
      auto tok = ctx.in.get();

      if(!scopeFn.nameObj)
         scopeFn.nameObj = ctx.fact.expCreate_String(ctx.prog, scope, scopeFn.fn->name, tok.pos);

      return static_cast<SR::Exp::CRef>(scopeFn.nameObj);
   }

   //
   // GetExp_Prim_generic
   //
   static SR::Exp::CRef GetExp_Prim_generic(Parser &ctx, Scope &scope)
   {
      // generic-selection:
      //    <_Generic> ( assignment-expression , generic-assoc-list )

      // <_Generic>
      auto pos = ctx.in.get().pos;

      // (
      ctx.expect(Core::TOK_ParenO);

      // assignment-expression
      auto exp = ctx.getExp_Assi(scope);

      // generic-assoc-list:
      //    generic-association
      //    generic-assoc-list , generic-association
      SR::Exp::CPtr         def;
      std::vector<GenAssoc> vec;
      while(ctx.in.drop(Core::TOK_Comma))
      {
         // generic-association:
         //    type-name : assignment-expression
         //    <default> : assignment-expression

         // type-name
         if(ctx.isType(scope))
         {
            auto type = ctx.getType(scope);

            // :
            ctx.expect(Core::TOK_Colon);

            // assignment-expression
            vec.emplace_back(type, ctx.getExp_Assi(scope));
         }

         // <default>
         else if(ctx.in.drop(Core::TOK_KeyWrd, Core::STR_default))
         {
            if(def)
               Core::Error(ctx.in.peek().pos, "multiple default");

            // :
            ctx.expect(Core::TOK_Colon);

            // assignment-expression
            def = ctx.getExp_Assi(scope);
         }

         else
            Core::ErrorExpect("generic-association", ctx.in.peek());
      }

      // )
      ctx.expect(Core::TOK_ParenC);

      return ctx.fact.expCreate_GenSel(exp, def, {Core::Move, vec.begin(), vec.end()}, pos);
   }

   //
   // GetExp_Prim_va_start
   //
   static SR::Exp::CRef GetExp_Prim_va_start(Parser &ctx, Scope &scope)
   {
      // va_start-expression:
      //    <__va_start>

      // <__va_start>
      auto pos = ctx.in.get().pos;

      Scope_Local *scopeLocal;
      if(!(scopeLocal = dynamic_cast<Scope_Local *>(&scope)))
         Core::Error(pos, "invalid scope for va_start");

      auto base = SR::Type::Void->getTypeQual({{IR::AddrBase::Aut, Core::STR_}});
      auto type = base->getTypePointer();

      IR::Value_Point val;
      val.vtype = type->getIRType().tPoint;
      val.addrB = base->getQualAddr().base;
      val.addrN = base->getQualAddr().name;

      switch(Target::GetCallTypeIR(scopeLocal->fn.fn->ctype))
      {
      case IR::CallType::SScriptI:
      case IR::CallType::SScriptS:
         val.value = static_cast<Core::FastU>(-1);
         break;

      case IR::CallType::StdCall:
         val.value = 0;
         break;

      default:
         Core::Error(pos, "invalid call type for va_start");
      }

      auto exp = IR::ExpCreate_Value(std::move(val), pos);
      return SR::ExpCreate_IRExp(exp, type, pos);
   }

   //
   // GetExp_Prim_Charac
   //
   static SR::Exp::CRef GetExp_Prim_Charac(Parser &ctx, Scope &)
   {
      auto tok = ctx.in.get();

      auto type = tok.tok == Core::TOK_Charac ? TypeIntegPrS : TypeIntegPrU;

      return ctx.fact.expCreate_LitInt(type, tok.str[0], tok.pos);
   }

   //
   // GetExp_Prim_Identi
   //
   static SR::Exp::CRef GetExp_Prim_Identi(Parser &ctx, Scope &scope)
   {
      switch(ctx.in.peek().str)
      {
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
         return ctx.fact.expCreate_LitInt(TypeIntegPrS, *lookup.resEnum, tok.pos);

      case Lookup::Func:
         CheckDeprecated(tok, lookup.resFunc);
         return ctx.fact.expCreate_Func(ctx.prog, lookup.resFunc, tok.pos);

      case Lookup::Obj:
         CheckDeprecated(tok, lookup.resObj);
         return ctx.fact.expCreate_Obj(ctx.prog, lookup.resObj, tok.pos);

      default:
         Core::ErrorExpect("primary-expression", tok);
      }

      // TODO: implicit function declaration
      // It sucks, but it is traditional and the method will be needed for ACS.

      Core::ErrorExpect("declared identifier", tok);
   }

   //
   // GetExp_Prim_KeyWrd
   //
   static SR::Exp::CRef GetExp_Prim_KeyWrd(Parser &ctx, Scope &scope)
   {
      switch(ctx.in.peek().str)
      {
      case Core::STR__Generic: return GetExp_Prim_generic(ctx, scope);
      default:
         Core::ErrorExpect("primary-expression", ctx.in.peek());
      }
   }

   //
   // GetExp_Prim_NumFix
   //
   static SR::Exp::CRef GetExp_Prim_NumFix(Parser &ctx, Scope &)
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
         Core::Error(tok.pos, "malformed fixed-constant");

      // Dtermine type.
      SR::Type::CPtr type;

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
      return ctx.fact.expCreate_LitInt(type, Core::NumberCast<Core::Integ>(val), tok.pos);
   }

   //
   // GetExp_Prim_NumFlt
   //
   static SR::Exp::CRef GetExp_Prim_NumFlt(Parser &ctx, Scope &scope)
   {
      // Check if this should be treated as a fixed-point literal.
      if(ctx.prag.stateFixedLiteral) switch(ctx.in.peek().str.back())
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
      if(ctx.prag.stateFixedLiteral)
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
         Core::Error(tok.pos, "malformed floating-constant");

      // Dtermine type.
      SR::Type::CPtr type;

      switch(l)
      {
      case  0: type = TypeFloatRS;   break;
      case +1: type = TypeFloatRSL;  break;
      case +2: type = TypeFloatRSLL; break;
      }

      // Create expression.
      auto valIR = IR::Value_Float(std::move(val), type->getIRType().tFloat);
      auto expIR = IR::ExpCreate_Value(std::move(valIR), tok.pos);
      return SR::ExpCreate_IRExp(expIR, type, tok.pos);
   }

   //
   // GetExp_Prim_NumInt
   //
   static SR::Exp::CRef GetExp_Prim_NumInt(Parser &ctx, Scope &)
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
         if(l) Core::Error(tok.pos, "duplicate L");
         l = itr[1] == itr[0] ? ++itr, 2 : 1;
         break;

      case 'U': case 'u':
         if(u) Core::Error(tok.pos, "duplicate U");
         u = true;
         break;

      default:
         Core::Error(tok.pos, "malformed integer-constant");
      }

      // Octal/hex literals will promote to unsigned if necessary.
      bool x = u || base != 10;

      // Generate expression.
      switch(l)
      {
         #define tryCreate(c, t) \
            if((c) && (val >> (t)->getSizeBitsI()) == 0) \
               return ctx.fact.expCreate_LitInt((t), std::move(val), tok.pos)

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
         Core::Error(tok.pos, "oversized integer-constant");

         #undef tryCreate
      }
   }

   //
   // GetExp_Prim_ParenO
   //
   static SR::Exp::CRef GetExp_Prim_ParenO(Parser &ctx, Scope &scope)
   {
      ctx.in.get();
      auto exp = ctx.getExp(scope);
      ctx.expect(Core::TOK_ParenC);

      return exp;
   }

   //
   // GetExp_Prim_StrChr
   //
   static SR::Exp::CRef GetExp_Prim_StrChr(Parser &ctx, Scope &scope)
   {
      auto tok = ctx.in.get();

      return ctx.fact.expCreate_String(ctx.prog, scope, tok.str, tok.pos);
   }

   //
   // GetExp_Prim_StrIdx
   //
   static SR::Exp::CRef GetExp_Prim_StrIdx(Parser &ctx, Scope &scope)
   {
      auto tok = ctx.in.get();

      return ctx.fact.expCreate_StrIdx(ctx.prog, scope, tok.str, tok.pos);
   }

   //
   // GetExp_Prim_StrU08
   //
   static SR::Exp::CRef GetExp_Prim_StrU08(Parser &ctx, Scope &scope)
   {
      auto tok = ctx.in.get();

      return ctx.fact.expCreate_StrU08(ctx.prog, scope, tok.str, tok.pos);
   }

   //
   // GetExp_Prim_StrU16
   //
   static SR::Exp::CRef GetExp_Prim_StrU16(Parser &ctx, Scope &scope)
   {
      auto tok = ctx.in.get();

      return ctx.fact.expCreate_StrU16(ctx.prog, scope, tok.str, tok.pos);
   }

   //
   // GetExp_Prim_StrU32
   //
   static SR::Exp::CRef GetExp_Prim_StrU32(Parser &ctx, Scope &scope)
   {
      auto tok = ctx.in.get();

      return ctx.fact.expCreate_StrU32(ctx.prog, scope, tok.str, tok.pos);
   }

   //
   // GetExp_Prim_String
   //
   static SR::Exp::CRef GetExp_Prim_String(Parser &ctx, Scope &scope)
   {
      auto tok = ctx.in.get();

      if(ctx.prag.stateStrEntLiteral)
         return ctx.fact.expCreate_StrIdx(ctx.prog, scope, tok.str, tok.pos);
      else
         return ctx.fact.expCreate_String(ctx.prog, scope, tok.str, tok.pos);
   }
}


//----------------------------------------------------------------------------|
// Extern Functions                                                           |
//

namespace GDCC::CC
{
   //
   // Parser::getExp_Prim
   //
   SR::Exp::CRef Parser::getExp_Prim(Scope &scope)
   {
      switch(in.peek().tok)
      {
      case Core::TOK_Charac: return GetExp_Prim_Charac(*this, scope);
      case Core::TOK_ChrU16: return GetExp_Prim_Charac(*this, scope);
      case Core::TOK_ChrU32: return GetExp_Prim_Charac(*this, scope);
      case Core::TOK_ChrWid: return GetExp_Prim_Charac(*this, scope);
      case Core::TOK_Identi: return GetExp_Prim_Identi(*this, scope);
      case Core::TOK_KeyWrd: return GetExp_Prim_KeyWrd(*this, scope);
      case Core::TOK_NumFix: return GetExp_Prim_NumFix(*this, scope);
      case Core::TOK_NumFlt: return GetExp_Prim_NumFlt(*this, scope);
      case Core::TOK_NumInt: return GetExp_Prim_NumInt(*this, scope);
      case Core::TOK_StrChr: return GetExp_Prim_StrChr(*this, scope);
      case Core::TOK_StrIdx: return GetExp_Prim_StrIdx(*this, scope);
      case Core::TOK_StrU08: return GetExp_Prim_StrU08(*this, scope);
      case Core::TOK_StrU16: return GetExp_Prim_StrU16(*this, scope);
      case Core::TOK_StrU32: return GetExp_Prim_StrU32(*this, scope);
      case Core::TOK_StrWid: return GetExp_Prim_StrU32(*this, scope);
      case Core::TOK_String: return GetExp_Prim_String(*this, scope);
      case Core::TOK_ParenO: return GetExp_Prim_ParenO(*this, scope);

      default:
         Core::ErrorExpect("primary-expression", in.peek());
      }
   }
}

// EOF

