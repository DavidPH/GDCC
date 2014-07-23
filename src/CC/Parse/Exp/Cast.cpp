//-----------------------------------------------------------------------------
//
// Copyright (C) 2014 David Hill
//
// See COPYING for license information.
//
//-----------------------------------------------------------------------------
//
// C cast-expression parsing.
//
//-----------------------------------------------------------------------------

#include "CC/Parse.hpp"

#include "CC/Exp/Assign.hpp"
#include "CC/Exp/Init.hpp"
#include "CC/Init.hpp"
#include "CC/Scope/Global.hpp"
#include "CC/Scope/Local.hpp"

#include "AST/Attribute.hpp"
#include "AST/Exp.hpp"
#include "AST/Object.hpp"
#include "AST/Type.hpp"

#include "Core/Exception.hpp"
#include "Core/TokenStream.hpp"


//----------------------------------------------------------------------------|
// Static Functions                                                           |
//

//
// GetExp_CLit (global)
//
static GDCC::AST::Exp::CRef GetExp_CLit(
   GDCC::CC::ParserCtx const &,
   GDCC::CC::Scope_Global    &,
   GDCC::AST::Attribute      &attr,
   GDCC::AST::Exp      const *)
{
   throw GDCC::Core::ExceptStr(attr.namePos, "file-scope compound literal stub");
}

//
// GetExp_CLit (local)
//
static GDCC::AST::Exp::CRef GetExp_CLit(
   GDCC::CC::ParserCtx const &ctx,
   GDCC::CC::Scope_Local     &scope,
   GDCC::AST::Attribute      &attr,
   GDCC::AST::Exp      const *init)
{
   using namespace GDCC;

   auto pos = attr.namePos;
   auto obj = scope.getObject(attr);

   obj->defin = true;
   obj->init  = init;

   auto objExp = CC::ExpCreate_Obj(ctx.prog, obj, pos);
   auto objSet = CC::Exp_Assign::Create(objExp, init, pos);
   auto objRef = CC::ExpCreate_Refer(objExp, pos);

   return CC::ExpCreate_Deref(CC::ExpCreate_Comma(objSet, objRef, pos), pos);
}


//----------------------------------------------------------------------------|
// Global Functions                                                           |
//

namespace GDCC
{
   namespace CC
   {
      //
      // GetExp_CLit
      //
      AST::Exp::CRef GetExp_CLit(ParserCtx const &ctx, Scope &scope)
      {
         if(!ctx.in.drop(Core::TOK_ParenO))
            throw Core::ExceptStr(ctx.in.peek().pos, "expected '('");

         auto type = GetType(ctx, scope);

         if(!ctx.in.drop(Core::TOK_ParenC))
            throw Core::ExceptStr(ctx.in.peek().pos, "expected ')'");

         return GetExp_CLit(ctx, scope, type);
      }

      //
      // GetExp_CLit
      //
      AST::Exp::CRef GetExp_CLit(ParserCtx const &ctx, Scope &scope,
         AST::Type const *type)
      {
         if(ctx.in.peek().tok != Core::TOK_BraceO)
            throw Core::ExceptStr(ctx.in.peek().pos, "expected '{'");

         AST::Attribute attr;

         attr.namePos = ctx.in.peek().pos;
         attr.type    = type;

         auto init = Init::Get(ctx, scope, attr.type);

         // Infer array length.
         if(attr.type->isTypeArray() && !attr.type->isTypeComplete())
         {
            attr.type = attr.type->getBaseType()
               ->getTypeArray(init->width())
               ->getTypeQual(attr.type->getQual());
         }

         auto initExp = Exp_Init::Create(std::move(init), false);

         if(auto s = dynamic_cast<Scope_Global *>(&scope))
            return ::GetExp_CLit(ctx, *s, attr, initExp);

         if(auto s = dynamic_cast<Scope_Local *>(&scope))
            return ::GetExp_CLit(ctx, *s, attr, initExp);

         throw Core::ExceptStr(attr.namePos, "invalid scope for compound literal");
      }

      //
      // GetExp_Cast
      //
      AST::Exp::CRef GetExp_Cast(ParserCtx const &ctx, Scope &scope)
      {
         if(IsExp_Cast(ctx, scope))
         {
            // (
            auto pos = ctx.in.get().pos;

            // type-name
            auto type = GetType(ctx, scope);

            // )
            if(!ctx.in.drop(Core::TOK_ParenC))
               throw Core::ExceptStr(ctx.in.peek().pos, "expected ')'");

            // Compound literal.
            if(ctx.in.peek().tok == Core::TOK_BraceO)
            {
               // Parse as though this is actually a postfix-expression.
               return GetExp_Post(ctx, scope, GetExp_CLit(ctx, scope, type));
            }

            return ExpCreate_Cst(type, GetExp_Cast(ctx, scope), pos);
         }

         return GetExp_Unar(ctx, scope);
      }

      //
      // IsExp_Cast
      //
      bool IsExp_Cast(ParserCtx const &ctx, Scope &scope)
      {
         if(ctx.in.drop(Core::TOK_ParenO))
         {
            bool res = IsType(ctx, scope);
            ctx.in.unget();
            return res;
         }

         return false;
      }
   }
}

// EOF

