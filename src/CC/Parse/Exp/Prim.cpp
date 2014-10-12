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
#include "CC/Scope.hpp"
#include "CC/Type.hpp"

#include "AST/Exp.hpp"
#include "AST/Object.hpp"
#include "AST/Type.hpp"

#include "Core/Array.hpp"
#include "Core/Exception.hpp"
#include "Core/Parse.hpp"
#include "Core/TokenStream.hpp"

#include "IR/Exp.hpp"
#include "IR/Glyph.hpp"


//----------------------------------------------------------------------------|
// Static Functions                                                           |
//

//
// GetExp_Prim_generic
//
static GDCC::AST::Exp::CRef GetExp_Prim_generic(GDCC::CC::ParserCtx const &ctx,
   GDCC::CC::Scope &scope)
{
   using namespace GDCC;

   // generic-selection:
   //    <_Generic> ( assignment-expression , generic-assoc-list )

   // <_Generic>
   auto pos = ctx.in.get().pos;

   // (
   if(!ctx.in.drop(Core::TOK_ParenO))
      throw Core::ExceptStr(ctx.in.peek().pos, "expected '('");

   // assignment-expression
   auto exp = CC::GetExp_Assi(ctx, scope);

   // generic-assoc-list:
   //    generic-association
   //    generic-assoc-list , generic-association
   AST::Exp::CPtr            def;
   std::vector<CC::GenAssoc> vec;
   while(ctx.in.drop(Core::TOK_Comma))
   {
      // generic-association:
      //    type-name : assignment-expression
      //    <default> : assignment-expression

      // type-name
      if(CC::IsType(ctx, scope))
      {
         auto type = CC::GetType(ctx, scope);

         // :
         if(!ctx.in.drop(Core::TOK_Colon))
            throw Core::ExceptStr(ctx.in.peek().pos, "expected ':'");

         // assignment-expression
         vec.emplace_back(type, CC::GetExp_Assi(ctx, scope));
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
         def = CC::GetExp_Assi(ctx, scope);
      }

      else
         throw Core::ExceptStr(ctx.in.peek().pos, "expected generic-association");
   }

   // )
   if(!ctx.in.drop(Core::TOK_ParenC))
      throw Core::ExceptStr(ctx.in.peek().pos, "expected ')'");

   return CC::ExpCreate_GenSel(exp, def,
      Core::Array<CC::GenAssoc>(Core::Move, vec.begin(), vec.end()), pos);
}

//
// GetExp_Prim_glyph
//
static GDCC::AST::Exp::CRef GetExp_Prim_glyph(GDCC::CC::ParserCtx const &ctx,
   GDCC::CC::Scope &scope)
{
   using namespace GDCC;

   // glyph-expression:
   //    <__glyph> ( type-name , string-literal )

   // <__glyph>
   auto pos = ctx.in.get().pos;

   // (
   if(!ctx.in.drop(Core::TOK_ParenO))
      throw Core::ExceptStr(ctx.in.peek().pos, "expected '('");

   // type-name
   auto type = CC::GetType(ctx, scope);

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
// GetExp_Prim_va_arg
//
static GDCC::AST::Exp::CRef GetExp_Prim_va_arg(GDCC::CC::ParserCtx const &ctx,
   GDCC::CC::Scope &scope)
{
   using namespace GDCC;

   // va_arg-expression:
   //    <__va_arg> ( assignment-expression , type-name )

   // <__va_arg>
   auto pos = ctx.in.get().pos;

   // (
   if(!ctx.in.drop(Core::TOK_ParenO))
      throw Core::ExceptStr(ctx.in.peek().pos, "expected '('");

   // assignment-expression
   auto exp = CC::GetExp_Assi(ctx, scope);

   // ,
   if(!ctx.in.drop(Core::TOK_Comma))
      throw Core::ExceptStr(ctx.in.peek().pos, "expected ','");

   // type-name
   auto type = CC::GetType(ctx, scope);

   // )
   if(!ctx.in.drop(Core::TOK_ParenC))
      throw Core::ExceptStr(ctx.in.peek().pos, "expected ')'");

   return CC::ExpCreate_VaArg(type, exp, pos);
}

//
// GetExp_Prim_va_start
//
static GDCC::AST::Exp::CRef GetExp_Prim_va_start(GDCC::CC::ParserCtx const &ctx,
   GDCC::CC::Scope &)
{
   using namespace GDCC;

   // va_start-expression:
   //    <__va_start>

   // <__va_start>
   auto pos = ctx.in.get().pos;

   auto type = AST::Type::Void
      ->getTypeQual({{IR::AddrBase::Loc, Core::STR_}})
      ->getTypePointer();

   return CC::ExpCreate_LitNul(type, pos);
}

//
// GetExp_Prim_Charac
//
static GDCC::AST::Exp::CRef GetExp_Prim_Charac(GDCC::CC::ParserCtx const &ctx,
   GDCC::CC::Scope &)
{
   using namespace GDCC;

   auto tok = ctx.in.get();

   auto type = tok.tok == Core::TOK_Charac ? CC::TypeIntegPrS : CC::TypeIntegPrU;

   return CC::ExpCreate_LitInt(type, tok.str[0], tok.pos);
}

//
// GetExp_Prim_Identi
//
static GDCC::AST::Exp::CRef GetExp_Prim_Identi(GDCC::CC::ParserCtx const &ctx,
   GDCC::CC::Scope &scope)
{
   using namespace GDCC;

   switch(ctx.in.peek().str)
   {
   case Core::STR___glyph:    return GetExp_Prim_glyph(ctx, scope);
   case Core::STR___va_arg:   return GetExp_Prim_va_arg(ctx, scope);
   case Core::STR___va_start: return GetExp_Prim_va_start(ctx, scope);
   default: break;
   }

   auto tok = ctx.in.get();

   if(auto lookup = scope.lookup(tok.str)) switch(lookup.res)
   {
   case CC::Lookup::Func:
      return CC::ExpCreate_Func(ctx.prog, lookup.resFunc, tok.pos);

   case CC::Lookup::Obj:
      return CC::ExpCreate_Obj(ctx.prog, lookup.resObj, tok.pos);

   default:
      throw Core::ExceptStr(tok.pos, "expected primary-expression");
   }

   // TODO: implicit function declaration
   // It sucks, but it is traditional and the method will be needed for ACS.

   throw Core::ExceptStr(tok.pos, "undeclared identifier");
}

//
// GetExp_Prim_KeyWrd
//
static GDCC::AST::Exp::CRef GetExp_Prim_KeyWrd(GDCC::CC::ParserCtx const &ctx,
   GDCC::CC::Scope &scope)
{
   using namespace GDCC;

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
static GDCC::AST::Exp::CRef GetExp_Prim_NumFix(GDCC::CC::ParserCtx const &ctx,
   GDCC::CC::Scope &)
{
   using namespace GDCC;

   auto tok = ctx.in.get();

   throw Core::ExceptStr(tok.pos, "fixed-constant stub");
}

//
// GetExp_Prim_NumFlt
//
static GDCC::AST::Exp::CRef GetExp_Prim_NumFlt(GDCC::CC::ParserCtx const &ctx,
   GDCC::CC::Scope &)
{
   using namespace GDCC;

   auto tok = ctx.in.get();

   throw Core::ExceptStr(tok.pos, "floating-constant stub");
}

//
// GetExp_Prim_NumInt
//
static GDCC::AST::Exp::CRef GetExp_Prim_NumInt(GDCC::CC::ParserCtx const &ctx,
   GDCC::CC::Scope &)
{
   using namespace GDCC;

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
static GDCC::AST::Exp::CRef GetExp_Prim_ParenO(GDCC::CC::ParserCtx const &ctx,
   GDCC::CC::Scope &scope)
{
   using namespace GDCC;

   // (
   ctx.in.get();

   // expression
   auto exp = CC::GetExp(ctx, scope);

   // )
   if(!ctx.in.drop(Core::TOK_ParenC))
      throw Core::ExceptStr(ctx.in.peek().pos, "expected ')'");

   return exp;
}

//
// GetExp_Prim_StrIdx
//
static GDCC::AST::Exp::CRef GetExp_Prim_StrIdx(GDCC::CC::ParserCtx const &ctx,
   GDCC::CC::Scope &scope)
{
   auto tok = ctx.in.get();

   return GDCC::CC::ExpCreate_StrIdx(ctx.prog, scope, tok.str, tok.pos);
}

//
// GetExp_Prim_StrU08
//
static GDCC::AST::Exp::CRef GetExp_Prim_StrU08(GDCC::CC::ParserCtx const &ctx,
   GDCC::CC::Scope &scope)
{
   auto tok = ctx.in.get();

   return GDCC::CC::ExpCreate_StrU08(ctx.prog, scope, tok.str, tok.pos);
}

//
// GetExp_Prim_StrU16
//
static GDCC::AST::Exp::CRef GetExp_Prim_StrU16(GDCC::CC::ParserCtx const &ctx,
   GDCC::CC::Scope &scope)
{
   auto tok = ctx.in.get();

   return GDCC::CC::ExpCreate_StrU16(ctx.prog, scope, tok.str, tok.pos);
}

//
// GetExp_Prim_StrU32
//
static GDCC::AST::Exp::CRef GetExp_Prim_StrU32(GDCC::CC::ParserCtx const &ctx,
   GDCC::CC::Scope &scope)
{
   auto tok = ctx.in.get();

   return GDCC::CC::ExpCreate_StrU32(ctx.prog, scope, tok.str, tok.pos);
}

//
// GetExp_Prim_String
//
static GDCC::AST::Exp::CRef GetExp_Prim_String(GDCC::CC::ParserCtx const &ctx,
   GDCC::CC::Scope &scope)
{
   auto tok = ctx.in.get();

   return GDCC::CC::ExpCreate_String(ctx.prog, scope, tok.str, tok.pos);
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

