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
#include "AST/Type.hpp"

#include "Core/Array.hpp"
#include "Core/Exception.hpp"
#include "Core/Parse.hpp"
#include "Core/TokenStream.hpp"


//----------------------------------------------------------------------------|
// Static Functions                                                           |
//

//
// GetExp_Prim_generic
//
static GDCC::AST::Exp::CRef GetExp_Prim_generic(GDCC::CC::ParserData &in,
   GDCC::CC::Scope *ctx)
{
   using namespace GDCC;

   // generic-selection:
   //    <_Generic> ( assignment-expression , generic-assoc-list )

   // <_Generic>
   auto pos = in.in.get().pos;

   // (
   if(!in.in.drop(Core::TOK_ParenO))
      throw Core::ExceptStr(in.in.peek().pos, "expected '('");

   // assignment-expression
   auto exp = CC::GetExp_Assi(in, ctx);

   // generic-assoc-list:
   //    generic-association
   //    generic-assoc-list , generic-association
   AST::Exp::CPtr            def;
   std::vector<CC::GenAssoc> vec;
   while(in.in.drop(Core::TOK_Comma))
   {
      // generic-association:
      //    type-name : assignment-expression
      //    <default> : assignment-expression

      // type-name
      if(CC::IsType(in, ctx))
      {
         auto type = CC::GetType(in, ctx);

         // :
         if(!in.in.drop(Core::TOK_Colon))
            throw Core::ExceptStr(in.in.peek().pos, "expected ':'");

         // assignment-expression
         vec.emplace_back(type, CC::GetExp_Assi(in, ctx));
      }

      // <default>
      else if(in.in.drop(Core::TOK_KeyWrd, Core::STR_default))
      {
         if(def)
            throw Core::ExceptStr(in.in.peek().pos, "multiple default");

         // :
         if(!in.in.drop(Core::TOK_Colon))
            throw Core::ExceptStr(in.in.peek().pos, "expected ':'");

         // assignment-expression
         def = CC::GetExp_Assi(in, ctx);
      }

      else
         throw Core::ExceptStr(in.in.peek().pos, "expected generic-association");
   }

   // )
   if(!in.in.drop(Core::TOK_ParenC))
      throw Core::ExceptStr(in.in.peek().pos, "expected ')'");

   return CC::ExpCreate_GenSel(exp, def,
      Core::Array<CC::GenAssoc>(Core::Move, vec.begin(), vec.end()), pos);
}

//
// GetExp_Prim_Charac
//
static GDCC::AST::Exp::CRef GetExp_Prim_Charac(GDCC::CC::ParserData &in,
   GDCC::CC::Scope *)
{
   using namespace GDCC;

   auto tok = in.in.get();

   auto type = tok.tok == Core::TOK_Charac ? CC::TypeIntegPrS : CC::TypeIntegPrU;

   return CC::ExpCreate_LitInt(type, tok.str[0], tok.pos);
}

//
// GetExp_Prim_Identi
//
static GDCC::AST::Exp::CRef GetExp_Prim_Identi(GDCC::CC::ParserData &in,
   GDCC::CC::Scope *ctx)
{
   using namespace GDCC;

   auto tok = in.in.get();

   if(auto lookup = ctx->lookup(tok.str)) switch(lookup.res)
   {
   case CC::Lookup::Func:
      return CC::ExpCreate_Func(in.prog, lookup.resFunc, tok.pos);

   case CC::Lookup::Obj:
      return CC::ExpCreate_Obj(in.prog, lookup.resObj, tok.pos);

   default:
      throw Core::ExceptStr(tok.pos, "expected primary-expression");
   }

   // TODO: implicit function declaration
   // It sucks, but it is traditional and the method will be needed for ACS.

   throw Core::ExceptStr(tok.pos, "undeclared identifier");
}

//
// GetExp_Prim_ParenO
//
static GDCC::AST::Exp::CRef GetExp_Prim_ParenO(GDCC::CC::ParserData &in,
   GDCC::CC::Scope *ctx)
{
   using namespace GDCC;

   // (
   in.in.get();

   // expression
   auto exp = CC::GetExp(in, ctx);

   // )
   if(!in.in.drop(Core::TOK_ParenC))
      throw Core::ExceptStr(in.in.peek().pos, "expected ')'");

   return exp;
}

//
// GetExp_Prim_KeyWrd
//
static GDCC::AST::Exp::CRef GetExp_Prim_KeyWrd(GDCC::CC::ParserData &in,
   GDCC::CC::Scope *ctx)
{
   using namespace GDCC;

   switch(in.in.peek().str)
   {
   case Core::STR__Generic: return GetExp_Prim_generic(in, ctx);
   default:
      throw Core::ExceptStr(in.in.peek().pos, "expected primary-expression");
   }
}

//
// GetExp_Prim_NumFix
//
static GDCC::AST::Exp::CRef GetExp_Prim_NumFix(GDCC::CC::ParserData &in,
   GDCC::CC::Scope *)
{
   using namespace GDCC;

   auto tok = in.in.get();

   throw Core::ExceptStr(tok.pos, "fixed-constant stub");
}

//
// GetExp_Prim_NumFlt
//
static GDCC::AST::Exp::CRef GetExp_Prim_NumFlt(GDCC::CC::ParserData &in,
   GDCC::CC::Scope *)
{
   using namespace GDCC;

   auto tok = in.in.get();

   throw Core::ExceptStr(tok.pos, "floating-constant stub");
}

//
// GetExp_Prim_NumInt
//
static GDCC::AST::Exp::CRef GetExp_Prim_NumInt(GDCC::CC::ParserData &in,
   GDCC::CC::Scope *)
{
   using namespace GDCC;

   auto tok = in.in.get();

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
// GetExp_Prim_String
//
static GDCC::AST::Exp::CRef GetExp_Prim_String(GDCC::CC::ParserData &in,
   GDCC::CC::Scope *)
{
   using namespace GDCC;

   auto tok = in.in.get();

   throw Core::ExceptStr(tok.pos, "string-literal stub");
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
      AST::Exp::CRef GetExp_Prim(ParserData &in, Scope *ctx)
      {
         switch(in.in.peek().tok)
         {
         case Core::TOK_Charac: return GetExp_Prim_Charac(in, ctx);
         case Core::TOK_ChrU16: return GetExp_Prim_Charac(in, ctx);
         case Core::TOK_ChrU32: return GetExp_Prim_Charac(in, ctx);
         case Core::TOK_ChrWid: return GetExp_Prim_Charac(in, ctx);
         case Core::TOK_Identi: return GetExp_Prim_Identi(in, ctx);
         case Core::TOK_KeyWrd: return GetExp_Prim_KeyWrd(in, ctx);
         case Core::TOK_NumFix: return GetExp_Prim_NumFix(in, ctx);
         case Core::TOK_NumFlt: return GetExp_Prim_NumFlt(in, ctx);
         case Core::TOK_NumInt: return GetExp_Prim_NumInt(in, ctx);
         case Core::TOK_StrIdx: return GetExp_Prim_String(in, ctx);
         case Core::TOK_StrU08: return GetExp_Prim_String(in, ctx);
         case Core::TOK_StrU16: return GetExp_Prim_String(in, ctx);
         case Core::TOK_StrU32: return GetExp_Prim_String(in, ctx);
         case Core::TOK_StrWid: return GetExp_Prim_String(in, ctx);
         case Core::TOK_String: return GetExp_Prim_String(in, ctx);
         case Core::TOK_ParenO: return GetExp_Prim_ParenO(in, ctx);

         default:
            throw Core::ExceptStr(in.in.peek().pos, "expected primary-expression");
         }
      }
   }
}

// EOF

