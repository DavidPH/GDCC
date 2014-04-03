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
static GDCC::AST::Exp::CRef GetExp_Unar_alignof(GDCC::CC::ParserCtx const &in,
   GDCC::CC::Scope &ctx)
{
   using namespace GDCC;

   auto pos = in.in.get().pos;

   if(!in.in.drop(Core::TOK_ParenO))
      throw Core::ExceptStr(in.in.peek().pos, "expected '('");

   auto type = CC::GetType(in, ctx);

   if(!in.in.drop(Core::TOK_ParenC))
      throw Core::ExceptStr(in.in.peek().pos, "expected ')'");

   return CC::ExpCreate_SizeAlign(type, pos);
}

//
// GetExp_Unar_sizeof
//
static GDCC::AST::Exp::CRef GetExp_Unar_sizeof(GDCC::CC::ParserCtx const &in,
   GDCC::CC::Scope &ctx)
{
   using namespace GDCC;

   auto pos = in.in.get().pos;

   // (
   if(in.in.drop(Core::TOK_ParenO))
   {
      // type-name
      if(IsType(in, ctx))
      {
         auto type = GetType(in, ctx);

         // )
         if(!in.in.drop(Core::TOK_ParenC))
            throw Core::ExceptStr(in.in.peek().pos, "expected ')'");

         return CC::ExpCreate_SizeBytes(type, pos);
      }

      in.in.unget();
   }

   return CC::ExpCreate_SizeBytes(CC::GetExp_Unar(in, ctx), pos);
}

//
// GetExp_Unar_Add
//
static GDCC::AST::Exp::CRef GetExp_Unar_Add(GDCC::CC::ParserCtx const &in,
   GDCC::CC::Scope &ctx)
{
   using namespace GDCC;

   auto pos = in.in.get().pos;

   return CC::ExpCreate_Add(CC::GetExp_Cast(in, ctx), pos);
}

//
// GetExp_Unar_Add2
//
static GDCC::AST::Exp::CRef GetExp_Unar_Add2(GDCC::CC::ParserCtx const &in,
   GDCC::CC::Scope &ctx)
{
   using namespace GDCC;

   auto pos = in.in.get().pos;

   return CC::ExpCreate_IncPre(CC::GetExp_Unar(in, ctx), pos);
}

//
// GetExp_Unar_And
//
static GDCC::AST::Exp::CRef GetExp_Unar_And(GDCC::CC::ParserCtx const &in,
   GDCC::CC::Scope &ctx)
{
   using namespace GDCC;

   auto pos = in.in.get().pos;

   return CC::ExpCreate_Refer(CC::GetExp_Cast(in, ctx), pos);
}

//
// GetExp_Unar_Inv
//
static GDCC::AST::Exp::CRef GetExp_Unar_Inv(GDCC::CC::ParserCtx const &in,
   GDCC::CC::Scope &ctx)
{
   using namespace GDCC;

   auto pos = in.in.get().pos;

   return CC::ExpCreate_Inv(CC::GetExp_Cast(in, ctx), pos);
}

//
// GetExp_Unar_KeyWrd
//
static GDCC::AST::Exp::CRef GetExp_Unar_KeyWrd(GDCC::CC::ParserCtx const &in,
   GDCC::CC::Scope &ctx)
{
   using namespace GDCC;

   switch(in.in.peek().str)
   {
   case Core::STR_sizeof:   return GetExp_Unar_sizeof(in, ctx);
   case Core::STR__Alignof: return GetExp_Unar_alignof(in, ctx);

   default:
      return CC::GetExp_Post(in, ctx);
   }
}

//
// GetExp_Unar_Mul
//
static GDCC::AST::Exp::CRef GetExp_Unar_Mul(GDCC::CC::ParserCtx const &in,
   GDCC::CC::Scope &ctx)
{
   using namespace GDCC;

   auto pos = in.in.get().pos;

   return CC::ExpCreate_Deref(CC::GetExp_Cast(in, ctx), pos);
}

//
// GetExp_Unar_Not
//
static GDCC::AST::Exp::CRef GetExp_Unar_Not(GDCC::CC::ParserCtx const &in,
   GDCC::CC::Scope &ctx)
{
   using namespace GDCC;

   auto pos = in.in.get().pos;

   return CC::ExpCreate_Not(CC::GetExp_Cast(in, ctx), pos);
}

//
// GetExp_Unar_Sub
//
static GDCC::AST::Exp::CRef GetExp_Unar_Sub(GDCC::CC::ParserCtx const &in,
   GDCC::CC::Scope &ctx)
{
   using namespace GDCC;

   auto pos = in.in.get().pos;

   return CC::ExpCreate_Sub(CC::GetExp_Cast(in, ctx), pos);
}

//
// GetExp_Unar_Sub2
//
static GDCC::AST::Exp::CRef GetExp_Unar_Sub2(GDCC::CC::ParserCtx const &in,
   GDCC::CC::Scope &ctx)
{
   using namespace GDCC;

   auto pos = in.in.get().pos;

   return CC::ExpCreate_DecPre(CC::GetExp_Unar(in, ctx), pos);
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
      AST::Exp::CRef GetExp_Unar(ParserCtx const &in, Scope &ctx)
      {
         switch(in.in.peek().tok)
         {
         case Core::TOK_KeyWrd: return GetExp_Unar_KeyWrd(in, ctx);
         case Core::TOK_Add:    return GetExp_Unar_Add   (in, ctx);
         case Core::TOK_Add2:   return GetExp_Unar_Add2  (in, ctx);
         case Core::TOK_And:    return GetExp_Unar_And   (in, ctx);
         case Core::TOK_Inv:    return GetExp_Unar_Inv   (in, ctx);
         case Core::TOK_Mul:    return GetExp_Unar_Mul   (in, ctx);
         case Core::TOK_Not:    return GetExp_Unar_Not   (in, ctx);
         case Core::TOK_Sub:    return GetExp_Unar_Sub   (in, ctx);
         case Core::TOK_Sub2:   return GetExp_Unar_Sub2  (in, ctx);

         default:
            return GetExp_Post(in, ctx);
         }
      }
   }
}

// EOF

