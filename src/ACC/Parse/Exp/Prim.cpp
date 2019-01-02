//-----------------------------------------------------------------------------
//
// Copyright (C) 2015-2019 David Hill
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

#include "CC/Exp.hpp"
#include "CC/Scope/Global.hpp"
#include "CC/Type.hpp"
#include "CC/Warning.hpp"

#include "Core/Exception.hpp"
#include "Core/Parse.hpp"
#include "Core/TokenStream.hpp"

#include "IR/Exp.hpp"
#include "IR/Linkage.hpp"

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

namespace GDCC::ACC
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
   // GetExp_ForwardFunc
   //
   static SR::Exp::CRef GetExp_ForwardFunc(Parser &ctx, CC::Scope &scope,
      Core::Token const &name)
   {
      CC::WarnForwardRef(name.pos, "implicitly declaring function ", name.str);

      // (
      auto pos = ctx.in.get().pos;

      // expression-list(opt)
      Core::Array<SR::Exp::CRef> args;
      if(!ctx.in.peek(Core::TOK_ParenC))
         args = ctx.getExpList(scope);

      // )
      ctx.expect(Core::TOK_ParenC);

      // Perform argument promotion before extracting types.
      // Yes, this will promote floats to double. No, I do not care.
      for(auto &arg : args)
         arg = CC::ExpPromo_Arg(arg, pos);

      Core::Array<SR::Type::CRef> types{args.begin(), args.end(),
         [](SR::Exp const *e) {return e->getType()->getTypeQual();}};

      // Generate attributes.
      SR::Attribute attr;

      attr.callt   = IR::CallType::LangACS;
      attr.linka   = IR::Linkage::ExtACS;
      attr.name    = name.str;
      attr.namePos = name.pos;
      attr.type    = CC::TypeIntegPrS->getTypeFunction(
         SR::TypeSet::Get(types.data(), types.size(), false), attr.callt);

      attr.declAuto = true;

      // Generate function.
      auto fn    = scope.global.getFunction(attr);
      auto fnExp = CC::ExpCreate_Func(ctx.prog, fn, name.pos);

      scope.add(name.str, fn);

      return CC::ExpCreate_Call(fnExp, std::move(args), scope, pos);
   }

   //
   // GetExp_Prim_Charac
   //
   static SR::Exp::CRef GetExp_Prim_Charac(Parser &ctx, CC::Scope &)
   {
      auto tok = ctx.in.get();

      auto type = tok.tok == Core::TOK_Charac ? CC::TypeIntegPrS : CC::TypeIntegPrU;

      return CC::ExpCreate_LitInt(type, tok.str[0], tok.pos);
   }

   //
   // GetExp_Prim_Identi
   //
   static SR::Exp::CRef GetExp_Prim_Identi(Parser &ctx, CC::Scope &scope)
   {
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
         Core::ErrorExpect("primary-expression", tok);
      }

      // Is this a forward function reference?
      if(ctx.in.peek(Core::TOK_ParenO))
         return GetExp_ForwardFunc(ctx, scope, tok);

      Core::ErrorExpect("declared identifier", tok);
   }

   //
   // GetExp_Prim_NumFix
   //
   static SR::Exp::CRef GetExp_Prim_NumFix(Parser &ctx, CC::Scope &)
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
         Core::Error(tok.pos, "malformed fixed-constant");

      // Dtermine type.
      SR::Type::CPtr type;

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
   static SR::Exp::CRef GetExp_Prim_NumFlt(Parser &ctx, CC::Scope &)
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
         Core::Error(tok.pos, "malformed floating-constant");

      // Dtermine type.
      SR::Type::CPtr type;

      switch(l)
      {
      case  0: type = CC::TypeFloatRS;   break;
      case +1: type = CC::TypeFloatRSL;  break;
      case +2: type = CC::TypeFloatRSLL; break;
      }

      // Create expression.
      auto valIR = IR::Value_Float(std::move(val), type->getIRType().tFloat);
      auto expIR = IR::ExpCreate_Value(std::move(valIR), tok.pos);
      return SR::ExpCreate_IRExp(expIR, type, tok.pos);
   }

   //
   // GetExp_Prim_NumInt
   //
   static SR::Exp::CRef GetExp_Prim_NumInt(Parser &ctx, CC::Scope &)
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
         Core::Error(tok.pos, "oversized integer-constant");

         #undef tryCreate
      }
   }

   //
   // GetExp_Prim_ParenO
   //
   static SR::Exp::CRef GetExp_Prim_ParenO(Parser &ctx, CC::Scope &scope)
   {
      ctx.expect(Core::TOK_ParenO);
      auto exp = ctx.getExp(scope);
      ctx.expect(Core::TOK_ParenC);

      return exp;
   }

   //
   // GetExp_Prim_String
   //
   static SR::Exp::CRef GetExp_Prim_String(Parser &ctx, CC::Scope &scope)
   {
      auto tok = ctx.in.get();

      return CC::ExpCreate_StrIdx(ctx.prog, scope, tok.str, tok.pos);
   }
}


//----------------------------------------------------------------------------|
// Extern Functions                                                           |
//

namespace GDCC::ACC
{
   //
   // Parser::getExp_Prim
   //
   SR::Exp::CRef Parser::getExp_Prim(CC::Scope &scope)
   {
      switch(in.peek().tok)
      {
      case Core::TOK_Charac: return GetExp_Prim_Charac(*this, scope);
      case Core::TOK_Identi: return GetExp_Prim_Identi(*this, scope);
      case Core::TOK_NumFix: return GetExp_Prim_NumFix(*this, scope);
      case Core::TOK_NumFlt: return GetExp_Prim_NumFlt(*this, scope);
      case Core::TOK_NumInt: return GetExp_Prim_NumInt(*this, scope);
      case Core::TOK_String: return GetExp_Prim_String(*this, scope);
      case Core::TOK_ParenO: return GetExp_Prim_ParenO(*this, scope);

      default:
         Core::ErrorExpect("primary-expression", in.peek());
      }
   }
}

// EOF

