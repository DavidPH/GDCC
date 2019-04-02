//-----------------------------------------------------------------------------
//
// Copyright (C) 2015-2019 David Hill
//
// See COPYING for license information.
//
//-----------------------------------------------------------------------------
//
// ACS expression parsing.
//
//-----------------------------------------------------------------------------

#include "ACC/Parse.hpp"

#include "ACC/Factory.hpp"
#include "ACC/Scope.hpp"

#include "CC/Exp.hpp"
#include "CC/Init.hpp"

#include "Core/TokenStream.hpp"

#include "SR/Attribute.hpp"
#include "SR/Exp.hpp"
#include "SR/Type.hpp"


//----------------------------------------------------------------------------|
// Extern Functions                                                           |
//

namespace GDCC::ACC
{
   //
   // Parser::getExp_Cast
   //
   SR::Exp::CRef Parser::getExp_Cast(CC::Scope &scope)
   {
      if(isExp_Cast(scope))
      {
         // (
         auto pos = in.get().pos;

         // type-name
         auto type = getType(scope);

         // )
         expect(Core::TOK_ParenC);

         return fact.expCreate_Cst(type, getExp_Cast(scope), pos);
      }

      return getExp_Unar(scope);
   }

   //
   // Parser::getExp_Post_ParenO
   //
   SR::Exp::CRef Parser::getExp_Post_ParenO(CC::Scope &scope, SR::Exp const *exp)
   {
      auto pos = in.get().pos;

      // const-call-specifier:
      //    <const> :
      in.drop(Core::TOK_KeyWrd, Core::STR_const, Core::TOK_Colon);

      Core::Array<SR::Exp::CRef> args;

      if(!in.peek(Core::TOK_ParenC))
         args = getExpList(scope);

      expect(Core::TOK_ParenC);

      return fact.expCreate_Call(exp, std::move(args), scope, pos);
   }

   //
   // Parser::getExp_Post
   //
   SR::Exp::CRef Parser::getExp_Post(CC::Scope &scope)
   {
      return getExp_Post(scope, getExp_Prim(scope));
   }

   //
   // Parser::getExp_Unar_Identi
   //
   SR::Exp::CRef Parser::getExp_Unar_Identi(CC::Scope &scope)
   {
      if(auto global = dynamic_cast<Scope_Global *>(&scope.global))
      {
         if(auto ctrans = global->findCreateTrans(in.peek().str))
            return getExp_Unar_createtranslation(scope, ctrans);

         if(auto print = global->findPrint(in.peek().str))
            return getExp_Unar_print(scope, print);
      }

      return CC::Parser::getExp_Unar_Identi(scope);
   }

   //
   // Parser::getExp_Unar_KeyWrd
   //
   SR::Exp::CRef Parser::getExp_Unar_KeyWrd(CC::Scope &scope)
   {
      switch(in.peek().str)
      {
      case Core::STR_createtranslation:
         if(auto global = dynamic_cast<Scope_Global *>(&scope.global))
         {
            if(auto ctrans = global->findCreateTrans(Core::STR___ + in.peek().str))
               return getExp_Unar_createtranslation(scope, ctrans);
         }
         break;

      case Core::STR_hudmessage:
      case Core::STR_hudmessagebold:
      case Core::STR_log:
      case Core::STR_print:
      case Core::STR_printbold:
      case Core::STR_strparam:
         if(auto global = dynamic_cast<Scope_Global *>(&scope.global))
         {
            if(auto print = global->findPrint(Core::STR___ + in.peek().str))
               return getExp_Unar_print(scope, print);
         }
         break;

      default:
         break;
      }

      return CC::Parser::getExp_Unar_KeyWrd(scope);
   }

   //
   // Parser::getInitRaw
   //
   CC::InitRaw Parser::getInitRaw(CC::Scope &scope)
   {
      CC::InitRaw init{in.peek().pos};

      // Bracketed initializer.
      if(in.drop(Core::TOK_BraceO))
      {
         std::vector<CC::InitRaw> sub;

         do
         {
            sub.emplace_back(getInitRaw(scope));
         }
         while(in.drop(Core::TOK_Comma) && !in.peek(Core::TOK_BraceC));

         expect(Core::TOK_BraceC);

         init.valueSub = {Core::Move, sub.begin(), sub.end()};
      }

      // Unbracketed initializer.
      else
      {
         init.valueExp = getExp_Assi(scope);
      }

      return init;
   }
}

// EOF

