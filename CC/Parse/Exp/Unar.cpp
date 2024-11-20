//-----------------------------------------------------------------------------
//
// Copyright (C) 2014-2024 David Hill
//
// See COPYING for license information.
//
//-----------------------------------------------------------------------------
//
// C unary-expression parsing.
//
//-----------------------------------------------------------------------------

#include "CC/Parse.hpp"

#include "CC/Factory.hpp"
#include "CC/Scope/Local.hpp"

#include "Core/Exception.hpp"
#include "Core/TokenStream.hpp"

#include "IR/Program.hpp"

#include "SR/Exp.hpp"
#include "SR/Type.hpp"
#include "SR/Warning.hpp"


//----------------------------------------------------------------------------|
// Static Functions                                                           |
//

namespace GDCC::CC
{
   //
   // GetExp_Unar_alignof
   //
   static SR::Exp::CRef GetExp_Unar_alignof(Parser &ctx, Scope &scope)
   {
      auto pos = ctx.in.get().pos;
      ctx.expect(Core::TOK_ParenO);
      auto type = ctx.getType(scope);
      ctx.expect(Core::TOK_ParenC);

      return ctx.fact.expCreate_SizeAlign(type, pos);
   }

   //
   // GetExp_Unar_div
   //
   static SR::Exp::CRef GetExp_Unar_div(Parser &ctx, Scope &scope)
   {
      // div-expression:
      //    <__div> ( assignment-expression , assignment-expression )

      auto pos = ctx.in.get().pos;
      ctx.expect(Core::TOK_ParenO);
      auto l = ctx.getExp_Assi(scope);
      ctx.expect(Core::TOK_Comma);
      auto r = ctx.getExp_Assi(scope);
      ctx.expect(Core::TOK_ParenC);

      return ctx.fact.expCreate_DivEx(l, r, pos);
   }

   //
   // GetExp_Unar_glyph
   //
   static SR::Exp::CRef GetExp_Unar_glyph(Parser &ctx, Scope &scope)
   {
      // glyph-expression:
      //    <__glyph> ( type-name , string-literal )

      // <__glyph>
      auto pos = ctx.in.get().pos;

      // (
      ctx.expect(Core::TOK_ParenO);

      // type-name
      auto type = ctx.getType(scope);

      if(!type->isCTypeObject() || !type->isTypeComplete())
         Core::Error(pos, "expected complete object type");

      // ,
      ctx.expect(Core::TOK_Comma);

      // string-literal
      IR::Glyph glyph = {ctx.prog, ctx.expectString().str};

      // )
      ctx.expect(Core::TOK_ParenC);

      // If the glyph has no type yet, set it now.
      auto &glyphData = glyph.getData();
      glyphData.type = type->getIRType();

      return ctx.fact.expCreate_IRExp(IR::ExpCreate_Glyph(glyph, pos), type, pos);
   }

   //
   // GetExp_Unar_indexof
   //
   static SR::Exp::CRef GetExp_Unar_indexof(Parser &ctx, Scope &scope)
   {
      // indexof-expression:
      //    <__indexof> ( type-name )
      //    <__indexof> unary-expression

      auto pos = ctx.in.get().pos;

      // (
      if(ctx.in.drop(Core::TOK_ParenO))
      {
         // type-name
         if(ctx.isType(scope))
         {
            auto type = ctx.getType(scope);

            // )
            ctx.expect(Core::TOK_ParenC);

            return ctx.fact.expCreate_SizeIndex(ctx.prog, type, pos);
         }

         ctx.in.unget();
      }
      else
         SR::WarnParentheses(pos, "indexof without parentheses");

      // TODO 2024-11-20: Add warning if expression is not an lvalue.
      return ctx.fact.expCreate_SizeIndex(ctx.prog, ctx.getExp_Unar(scope), pos);
   }

   //
   // GetExp_Unar_longjmp
   //
   static SR::Exp::CRef GetExp_Unar_longjmp(Parser &ctx, Scope &scope)
   {
      // longjmp-expression:
      //    <__longjmp> ( assignment-expression , assignment-expression )

      auto pos = ctx.in.get().pos;
      ctx.expect(Core::TOK_ParenO);
      auto env = ctx.getExp_Assi(scope);
      ctx.expect(Core::TOK_Comma);
      auto val = ctx.getExp_Assi(scope);
      ctx.expect(Core::TOK_ParenC);

      return ctx.fact.expCreate_JmpLng(scope, env, val, pos);
   }

   //
   // GetExp_Unar_offsetof
   //
   static SR::Exp::CRef GetExp_Unar_offsetof(Parser &ctx, Scope &scope)
   {
      // offsetof-expression:
      //    <__offsetof> ( type-name , identifier )

      // <__offsetof>
      auto pos = ctx.in.get().pos;

      // (
      ctx.expect(Core::TOK_ParenO);

      // type-name
      auto type = ctx.getType(scope);

      if(!type->isCTypeObject() || !type->isTypeComplete())
         Core::Error(pos, "expected complete object type");

      // ,
      ctx.expect(Core::TOK_Comma);

      // identifier
      Core::String name = ctx.expectIdenti().str;

      // )
      ctx.expect(Core::TOK_ParenC);

      try
      {
         return ctx.fact.expCreate_LitInt(SR::Type::Size,
            Core::NumberCast<Core::Integ>(type->getMember(name).addr), pos);
      }
      catch(SR::TypeError const &)
      {
         Core::Error(pos, "invalid member");
      }
   }

   //
   // GetExp_Unar_setjmp
   //
   static SR::Exp::CRef GetExp_Unar_setjmp(Parser &ctx, Scope &scope)
   {
      // setjmp-expression:
      //    <__setjmp> ( assignment-expression )

      auto pos = ctx.in.get().pos;
      ctx.expect(Core::TOK_ParenO);
      auto env = ctx.getExp_Assi(scope);
      ctx.expect(Core::TOK_ParenC);

      return ctx.fact.expCreate_JmpSet(env, pos);
   }

   //
   // GetExp_Unar_sizeof
   //
   static SR::Exp::CRef GetExp_Unar_sizeof(Parser &ctx, Scope &scope)
   {
      auto pos = ctx.in.get().pos;

      // (
      if(ctx.in.drop(Core::TOK_ParenO))
      {
         // type-name
         if(ctx.isType(scope))
         {
            auto type = ctx.getType(scope);

            // )
            ctx.expect(Core::TOK_ParenC);

            return ctx.fact.expCreate_SizeBytes(type, pos);
         }

         ctx.in.unget();
      }
      else
         SR::WarnParentheses(pos, "sizeof without parentheses");

      return ctx.fact.expCreate_SizeBytes(ctx.getExp_Unar(scope), pos);
   }

   //
   // GetExp_Unar_va_arg
   //
   static SR::Exp::CRef GetExp_Unar_va_arg(Parser &ctx, Scope &scope)
   {
      // va_arg-expression:
      //    <__va_arg> ( assignment-expression , type-name )

      auto pos = ctx.in.get().pos;
      ctx.expect(Core::TOK_ParenO);
      auto exp = ctx.getExp_Assi(scope);
      ctx.expect(Core::TOK_Comma);
      auto type = ctx.getType(scope);
      ctx.expect(Core::TOK_ParenC);

      return ctx.fact.expCreate_VaArg(type, exp, pos);
   }

   //
   // GetExp_Unar_Add
   //
   static SR::Exp::CRef GetExp_Unar_Add(Parser &ctx, Scope &scope)
   {
      auto pos = ctx.in.get().pos;

      return ctx.fact.expCreate_Add(ctx.getExp_Cast(scope), pos);
   }

   //
   // GetExp_Unar_Add2
   //
   static SR::Exp::CRef GetExp_Unar_Add2(Parser &ctx, Scope &scope)
   {
      auto pos = ctx.in.get().pos;

      return ctx.fact.expCreate_IncPre(ctx.getExp_Unar(scope), pos);
   }

   //
   // GetExp_Unar_And
   //
   static SR::Exp::CRef GetExp_Unar_And(Parser &ctx, Scope &scope)
   {
      auto pos = ctx.in.get().pos;

      return ctx.fact.expCreate_Refer(ctx.getExp_Cast(scope), pos);
   }

   //
   // GetExp_Unar_And2
   //
   static SR::Exp::CRef GetExp_Unar_And2(Parser &ctx, Scope &scope)
   {
      auto pos = ctx.in.get().pos;

      auto scopeLocal = dynamic_cast<Scope_Local *>(&scope);
      if(!scopeLocal)
         Core::Error(pos, "invalid scope for unary &&");

      auto  label = scopeLocal->getLabel(ctx.expectIdenti().str);
      auto &djump = ctx.prog.getDJump(label + "$djump");

      djump.label = label;
      djump.alloc = true;
      djump.defin = true;

      ctx.prog.getGlyphData(djump.glyph).type = IR::Type_DJump();

      return ctx.fact.expCreate_IRExp(
         IR::ExpCreate_Glyph({ctx.prog, djump.glyph}, pos),
         SR::Type::Label->getTypePointer(), pos);
   }

   //
   // GetExp_Unar_Inv
   //
   static SR::Exp::CRef GetExp_Unar_Inv(Parser &ctx, Scope &scope)
   {
      auto pos = ctx.in.get().pos;

      return ctx.fact.expCreate_Inv(ctx.getExp_Cast(scope), pos);
   }

   //
   // GetExp_Unar_Mul
   //
   static SR::Exp::CRef GetExp_Unar_Mul(Parser &ctx, Scope &scope)
   {
      auto pos = ctx.in.get().pos;

      return ctx.fact.expCreate_Deref(ctx.getExp_Cast(scope), pos);
   }

   //
   // GetExp_Unar_Not
   //
   static SR::Exp::CRef GetExp_Unar_Not(Parser &ctx, Scope &scope)
   {
      auto pos = ctx.in.get().pos;

      return ctx.fact.expCreate_Not(ctx.getExp_Cast(scope), pos);
   }

   //
   // GetExp_Unar_Sub
   //
   static SR::Exp::CRef GetExp_Unar_Sub(Parser &ctx, Scope &scope)
   {
      auto pos = ctx.in.get().pos;

      return ctx.fact.expCreate_Sub(ctx.getExp_Cast(scope), pos);
   }

   //
   // GetExp_Unar_Sub2
   //
   static SR::Exp::CRef GetExp_Unar_Sub2(Parser &ctx, Scope &scope)
   {
      auto pos = ctx.in.get().pos;

      return ctx.fact.expCreate_DecPre(ctx.getExp_Unar(scope), pos);
   }
}


//----------------------------------------------------------------------------|
// Extern Functions                                                           |
//

namespace GDCC::CC
{
   //
   // Parser::getExp_Unar_Identi
   //
   SR::Exp::CRef Parser::getExp_Unar_Identi(Scope &scope)
   {
      switch(in.peek().str)
      {
      case Core::STR___div:      return GetExp_Unar_div(*this, scope);
      case Core::STR___glyph:    return GetExp_Unar_glyph(*this, scope);
      case Core::STR___indexof:  return GetExp_Unar_indexof(*this, scope);
      case Core::STR___longjmp:  return GetExp_Unar_longjmp(*this, scope);
      case Core::STR___offsetof: return GetExp_Unar_offsetof(*this, scope);
      case Core::STR___setjmp:   return GetExp_Unar_setjmp(*this, scope);
      case Core::STR___va_arg:   return GetExp_Unar_va_arg(*this, scope);

      default: break;
      }

      return getExp_Post(scope);
   }

   //
   // Parser::getExp_Unar_KeyWrd
   //
   SR::Exp::CRef Parser::getExp_Unar_KeyWrd(Scope &scope)
   {
      switch(in.peek().str)
      {
      case Core::STR_sizeof:   return GetExp_Unar_sizeof(*this, scope);
      case Core::STR__Alignof: return GetExp_Unar_alignof(*this, scope);

      default:
         return getExp_Post(scope);
      }
   }

   //
   // Parser::getExp_Unar
   //
   SR::Exp::CRef Parser::getExp_Unar(Scope &scope)
   {
      switch(in.peek().tok)
      {
      case Core::TOK_Identi: return getExp_Unar_Identi(       scope);
      case Core::TOK_KeyWrd: return getExp_Unar_KeyWrd(       scope);
      case Core::TOK_Add:    return GetExp_Unar_Add   (*this, scope);
      case Core::TOK_Add2:   return GetExp_Unar_Add2  (*this, scope);
      case Core::TOK_And:    return GetExp_Unar_And   (*this, scope);
      case Core::TOK_And2:   return GetExp_Unar_And2  (*this, scope);
      case Core::TOK_Inv:    return GetExp_Unar_Inv   (*this, scope);
      case Core::TOK_Mul:    return GetExp_Unar_Mul   (*this, scope);
      case Core::TOK_Not:    return GetExp_Unar_Not   (*this, scope);
      case Core::TOK_Sub:    return GetExp_Unar_Sub   (*this, scope);
      case Core::TOK_Sub2:   return GetExp_Unar_Sub2  (*this, scope);

      default:
         return getExp_Post(scope);
      }
   }
}

// EOF

