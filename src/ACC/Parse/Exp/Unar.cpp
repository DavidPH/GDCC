//-----------------------------------------------------------------------------
//
// Copyright (C) 2015 David Hill
//
// See COPYING for license information.
//
//-----------------------------------------------------------------------------
//
// ACS unary-expression parsing.
//
//-----------------------------------------------------------------------------

#include "ACC/Parse.hpp"

#include "ACC/Scope.hpp"

#include "AST/Exp.hpp"
#include "AST/Type.hpp"

#include "CC/Exp.hpp"

#include "Core/Exception.hpp"
#include "Core/TokenStream.hpp"


//----------------------------------------------------------------------------|
// Static Functions                                                           |
//

namespace GDCC
{
   namespace ACC
   {
      //
      // GetExp_Unar_alignof
      //
      static AST::Exp::CRef GetExp_Unar_alignof(ParserCtx const &ctx,
         CC::Scope &scope)
      {
         auto pos = ctx.in.get().pos;

         if(!ctx.in.drop(Core::TOK_ParenO))
            throw Core::ParseExceptExpect(ctx.in.peek(), "(", true);

         auto type = GetType(ctx, scope);

         if(!ctx.in.drop(Core::TOK_ParenC))
            throw Core::ParseExceptExpect(ctx.in.peek(), ")", true);

         return CC::ExpCreate_SizeAlign(type, pos);
      }

      //
      // GetExp_Unar_sizeof
      //
      static AST::Exp::CRef GetExp_Unar_sizeof(ParserCtx const &ctx,
         CC::Scope &scope)
      {
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
                  throw Core::ParseExceptExpect(ctx.in.peek(), ")", true);

               return CC::ExpCreate_SizeBytes(type, pos);
            }

            ctx.in.unget();
         }

         return CC::ExpCreate_SizeBytes(GetExp_Unar(ctx, scope), pos);
      }

      //
      // GetExp_Unar_Add
      //
      static AST::Exp::CRef GetExp_Unar_Add(ParserCtx const &ctx,
         CC::Scope &scope)
      {
         auto pos = ctx.in.get().pos;

         return CC::ExpCreate_Add(GetExp_Cast(ctx, scope), pos);
      }

      //
      // GetExp_Unar_Add2
      //
      static AST::Exp::CRef GetExp_Unar_Add2(ParserCtx const &ctx,
         CC::Scope &scope)
      {
         auto pos = ctx.in.get().pos;

         return CC::ExpCreate_IncPre(GetExp_Unar(ctx, scope), pos);
      }

      //
      // GetExp_Unar_And
      //
      static AST::Exp::CRef GetExp_Unar_And(ParserCtx const &ctx,
         CC::Scope &scope)
      {
         auto pos = ctx.in.get().pos;

         return CC::ExpCreate_Refer(GetExp_Cast(ctx, scope), pos);
      }

      //
      // GetExp_Unar_Identi
      //
      static AST::Exp::CRef GetExp_Unar_Identi(ParserCtx const &ctx,
         CC::Scope &scope)
      {
         if(auto global = dynamic_cast<Scope_Global *>(&scope.global))
         {
            if(auto print = global->findPrint(ctx.in.peek().str))
               return GetExp_Unar_print(ctx, scope, print);
         }

         return GetExp_Post(ctx, scope);
      }

      //
      // GetExp_Unar_Inv
      //
      static AST::Exp::CRef GetExp_Unar_Inv(ParserCtx const &ctx,
         CC::Scope &scope)
      {
         auto pos = ctx.in.get().pos;

         return CC::ExpCreate_Inv(GetExp_Cast(ctx, scope), pos);
      }

      //
      // GetExp_Unar_KeyWrd
      //
      static AST::Exp::CRef GetExp_Unar_KeyWrd(ParserCtx const &ctx,
         CC::Scope &scope)
      {
         switch(ctx.in.peek().str)
         {
         case Core::STR_sizeof:   return GetExp_Unar_sizeof(ctx, scope);
         case Core::STR__Alignof: return GetExp_Unar_alignof(ctx, scope);

         default:
            return GetExp_Post(ctx, scope);
         }
      }

      //
      // GetExp_Unar_Mul
      //
      static AST::Exp::CRef GetExp_Unar_Mul(ParserCtx const &ctx,
         CC::Scope &scope)
      {
         auto pos = ctx.in.get().pos;

         return CC::ExpCreate_Deref(GetExp_Cast(ctx, scope), pos);
      }

      //
      // GetExp_Unar_Not
      //
      static AST::Exp::CRef GetExp_Unar_Not(ParserCtx const &ctx,
         CC::Scope &scope)
      {
         auto pos = ctx.in.get().pos;

         return CC::ExpCreate_Not(GetExp_Cast(ctx, scope), pos);
      }

      //
      // GetExp_Unar_Sub
      //
      static AST::Exp::CRef GetExp_Unar_Sub(ParserCtx const &ctx,
         CC::Scope &scope)
      {
         auto pos = ctx.in.get().pos;

         return CC::ExpCreate_Sub(GetExp_Cast(ctx, scope), pos);
      }

      //
      // GetExp_Unar_Sub2
      //
      static AST::Exp::CRef GetExp_Unar_Sub2(ParserCtx const &ctx,
         CC::Scope &scope)
      {
         auto pos = ctx.in.get().pos;

         return CC::ExpCreate_DecPre(GetExp_Unar(ctx, scope), pos);
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
      // GetExp_Unar
      //
      AST::Exp::CRef GetExp_Unar(ParserCtx const &ctx, CC::Scope &scope)
      {
         switch(ctx.in.peek().tok)
         {
         case Core::TOK_Identi: return GetExp_Unar_Identi(ctx, scope);
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

