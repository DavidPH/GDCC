//-----------------------------------------------------------------------------
//
// Copyright (C) 2014 David Hill
//
// See COPYING for license information.
//
//-----------------------------------------------------------------------------
//
// C unary-expression parsing.
//
//-----------------------------------------------------------------------------

#include "CC/Parse.hpp"

#include "CC/Exp.hpp"

#include "AST/Exp.hpp"
#include "AST/Type.hpp"

#include "Core/Exception.hpp"
#include "Core/TokenStream.hpp"


//----------------------------------------------------------------------------|
// Static Functions                                                           |
//

//
// GetExp_Unar_alignof
//
static GDCC::AST::Exp::CRef GetExp_Unar_alignof(GDCC::CC::ParserCtx const &ctx,
   GDCC::CC::Scope &scope)
{
   using namespace GDCC;

   auto pos = ctx.in.get().pos;

   if(!ctx.in.drop(Core::TOK_ParenO))
      throw Core::ExceptStr(ctx.in.peek().pos, "expected '('");

   auto type = CC::GetType(ctx, scope);

   if(!ctx.in.drop(Core::TOK_ParenC))
      throw Core::ExceptStr(ctx.in.peek().pos, "expected ')'");

   return CC::ExpCreate_SizeAlign(type, pos);
}

//
// GetExp_Unar_sizeof
//
static GDCC::AST::Exp::CRef GetExp_Unar_sizeof(GDCC::CC::ParserCtx const &ctx,
   GDCC::CC::Scope &scope)
{
   using namespace GDCC;

   auto pos = ctx.in.get().pos;

   // (
   if(ctx.in.drop(Core::TOK_ParenO))
   {
      // type-name
      if(IsType(ctx, scope))
      {
         auto type = GetType(ctx, scope);

         // )
         if(!ctx.in.drop(Core::TOK_ParenC))
            throw Core::ExceptStr(ctx.in.peek().pos, "expected ')'");

         return CC::ExpCreate_SizeBytes(type, pos);
      }

      ctx.in.unget();
   }

   return CC::ExpCreate_SizeBytes(CC::GetExp_Unar(ctx, scope), pos);
}

//
// GetExp_Unar_Add
//
static GDCC::AST::Exp::CRef GetExp_Unar_Add(GDCC::CC::ParserCtx const &ctx,
   GDCC::CC::Scope &scope)
{
   using namespace GDCC;

   auto pos = ctx.in.get().pos;

   return CC::ExpCreate_Add(CC::GetExp_Cast(ctx, scope), pos);
}

//
// GetExp_Unar_Add2
//
static GDCC::AST::Exp::CRef GetExp_Unar_Add2(GDCC::CC::ParserCtx const &ctx,
   GDCC::CC::Scope &scope)
{
   using namespace GDCC;

   auto pos = ctx.in.get().pos;

   return CC::ExpCreate_IncPre(CC::GetExp_Unar(ctx, scope), pos);
}

//
// GetExp_Unar_And
//
static GDCC::AST::Exp::CRef GetExp_Unar_And(GDCC::CC::ParserCtx const &ctx,
   GDCC::CC::Scope &scope)
{
   using namespace GDCC;

   auto pos = ctx.in.get().pos;

   return CC::ExpCreate_Refer(CC::GetExp_Cast(ctx, scope), pos);
}

//
// GetExp_Unar_Inv
//
static GDCC::AST::Exp::CRef GetExp_Unar_Inv(GDCC::CC::ParserCtx const &ctx,
   GDCC::CC::Scope &scope)
{
   using namespace GDCC;

   auto pos = ctx.in.get().pos;

   return CC::ExpCreate_Inv(CC::GetExp_Cast(ctx, scope), pos);
}

//
// GetExp_Unar_KeyWrd
//
static GDCC::AST::Exp::CRef GetExp_Unar_KeyWrd(GDCC::CC::ParserCtx const &ctx,
   GDCC::CC::Scope &scope)
{
   using namespace GDCC;

   switch(ctx.in.peek().str)
   {
   case Core::STR_sizeof:   return GetExp_Unar_sizeof(ctx, scope);
   case Core::STR__Alignof: return GetExp_Unar_alignof(ctx, scope);

   default:
      return CC::GetExp_Post(ctx, scope);
   }
}

//
// GetExp_Unar_Mul
//
static GDCC::AST::Exp::CRef GetExp_Unar_Mul(GDCC::CC::ParserCtx const &ctx,
   GDCC::CC::Scope &scope)
{
   using namespace GDCC;

   auto pos = ctx.in.get().pos;

   return CC::ExpCreate_Deref(CC::GetExp_Cast(ctx, scope), pos);
}

//
// GetExp_Unar_Not
//
static GDCC::AST::Exp::CRef GetExp_Unar_Not(GDCC::CC::ParserCtx const &ctx,
   GDCC::CC::Scope &scope)
{
   using namespace GDCC;

   auto pos = ctx.in.get().pos;

   return CC::ExpCreate_Not(CC::GetExp_Cast(ctx, scope), pos);
}

//
// GetExp_Unar_Sub
//
static GDCC::AST::Exp::CRef GetExp_Unar_Sub(GDCC::CC::ParserCtx const &ctx,
   GDCC::CC::Scope &scope)
{
   using namespace GDCC;

   auto pos = ctx.in.get().pos;

   return CC::ExpCreate_Sub(CC::GetExp_Cast(ctx, scope), pos);
}

//
// GetExp_Unar_Sub2
//
static GDCC::AST::Exp::CRef GetExp_Unar_Sub2(GDCC::CC::ParserCtx const &ctx,
   GDCC::CC::Scope &scope)
{
   using namespace GDCC;

   auto pos = ctx.in.get().pos;

   return CC::ExpCreate_DecPre(CC::GetExp_Unar(ctx, scope), pos);
}


//----------------------------------------------------------------------------|
// Global Functions                                                           |
//

namespace GDCC
{
   namespace CC
   {
      //
      // GetExp_Unar
      //
      AST::Exp::CRef GetExp_Unar(ParserCtx const &ctx, Scope &scope)
      {
         switch(ctx.in.peek().tok)
         {
         case Core::TOK_KeyWrd: return GetExp_Unar_KeyWrd(ctx, scope);
         case Core::TOK_Add:    return GetExp_Unar_Add   (ctx, scope);
         case Core::TOK_Add2:   return GetExp_Unar_Add2  (ctx, scope);
         case Core::TOK_And:    return GetExp_Unar_And   (ctx, scope);
         case Core::TOK_Inv:    return GetExp_Unar_Inv   (ctx, scope);
         case Core::TOK_Mul:    return GetExp_Unar_Mul   (ctx, scope);
         case Core::TOK_Not:    return GetExp_Unar_Not   (ctx, scope);
         case Core::TOK_Sub:    return GetExp_Unar_Sub   (ctx, scope);
         case Core::TOK_Sub2:   return GetExp_Unar_Sub2  (ctx, scope);

         default:
            return GetExp_Post(ctx, scope);
         }
      }
   }
}

// EOF

