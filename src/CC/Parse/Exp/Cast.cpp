//-----------------------------------------------------------------------------
//
// Copyright (C) 2014-2018 David Hill
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

#include "Core/Exception.hpp"
#include "Core/TokenStream.hpp"

#include "SR/Attribute.hpp"
#include "SR/Exp.hpp"
#include "SR/Object.hpp"
#include "SR/Type.hpp"


//----------------------------------------------------------------------------|
// Static Functions                                                           |
//

namespace GDCC::CC
{
   //
   // GetExp_CLit (global)
   //
   static SR::Exp::CRef GetExp_CLit(
      Parser        &ctx,
      Scope_Global  &scope,
      SR::Attribute &attr,
      SR::Exp const *init)
   {
      auto obj = scope.getObject(attr);

      obj->defin = true;
      obj->init  = init;

      return ExpCreate_Obj(ctx.prog, obj, attr.namePos);
   }

   //
   // GetExp_CLit (local)
   //
   static SR::Exp::CRef GetExp_CLit(
      Parser         &ctx,
      Scope_Local    &scope,
      SR::Attribute &attr,
      SR::Exp const *init)
   {
      auto pos = attr.namePos;
      auto obj = scope.getObject(attr);

      obj->defin = true;
      obj->init  = init;

      auto objExp = ExpCreate_Obj(ctx.prog, obj, pos);
      auto objSet = Exp_Assign::Create(objExp, init, pos);
      auto objRef = ExpCreate_Refer(objExp, pos);

      return ExpCreate_Deref(ExpCreate_Comma(objSet, objRef, pos), pos);
   }
}


//----------------------------------------------------------------------------|
// Extern Functions                                                           |
//

namespace GDCC::CC
{
   //
   // Parser::getExp_CLit
   //
   SR::Exp::CRef Parser::getExp_CLit(Scope &scope)
   {
      if(!in.drop(Core::TOK_ParenO))
         throw Core::ParseExceptExpect(in.peek(), "(", true);

      auto type = getType(scope);

      if(!in.drop(Core::TOK_ParenC))
         throw Core::ParseExceptExpect(in.peek(), ")", true);

      return getExp_CLit(scope, type);
   }

   //
   // Parser::getExp_CLit
   //
   SR::Exp::CRef Parser::getExp_CLit(Scope &scope, SR::Type const *type)
   {
      if(in.peek().tok != Core::TOK_BraceO)
         throw Core::ParseExceptExpect(in.peek(), "{", true);

      SR::Attribute attr;

      attr.namePos = in.peek().pos;

      auto init = getExp_Init(scope, type);
      attr.type = init->getType();

      if(auto s = dynamic_cast<Scope_Global *>(&scope))
         return GetExp_CLit(*this, *s, attr, init);

      if(auto s = dynamic_cast<Scope_Local *>(&scope))
         return GetExp_CLit(*this, *s, attr, init);

      throw Core::ExceptStr(attr.namePos, "invalid scope for compound literal");
   }

   //
   // Parser::getExp_Cast
   //
   SR::Exp::CRef Parser::getExp_Cast(Scope &scope)
   {
      if(isExp_Cast(scope))
      {
         // (
         auto pos = in.get().pos;

         // type-name
         auto type = getType(scope);

         // )
         if(!in.drop(Core::TOK_ParenC))
            throw Core::ParseExceptExpect(in.peek(), ")", true);

         // Compound literal.
         if(in.peek(Core::TOK_BraceO))
         {
            // Parse as though this is actually a postfix-expression.
            return getExp_Post(scope, getExp_CLit(scope, type));
         }

         return ExpCreate_Cst(type, getExp_Cast(scope), pos);
      }

      return getExp_Unar(scope);
   }

   //
   // Parser::isExp_Cast
   //
   bool Parser::isExp_Cast(Scope &scope)
   {
      if(in.drop(Core::TOK_ParenO))
      {
         bool res = isType(scope);
         in.unget();
         return res;
      }

      return false;
   }
}

// EOF

