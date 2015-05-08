//-----------------------------------------------------------------------------
//
// Copyright (C) 2015 David Hill
//
// See COPYING for license information.
//
//-----------------------------------------------------------------------------
//
// ACS expression parsing.
//
//-----------------------------------------------------------------------------

#include "ACC/Parse.hpp"

#include "ACC/Scope.hpp"

#include "AST/Attribute.hpp"
#include "AST/Exp.hpp"
#include "AST/Type.hpp"

#include "CC/Exp.hpp"
#include "CC/Init.hpp"

#include "Core/Exception.hpp"
#include "Core/TokenStream.hpp"


//----------------------------------------------------------------------------|
// Global Functions                                                           |
//

namespace GDCC
{
   namespace ACC
   {
      //
      // Parser::getExp_Cast
      //
      AST::Exp::CRef Parser::getExp_Cast(CC::Scope &scope)
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

            return CC::ExpCreate_Cst(type, getExp_Cast(scope), pos);
         }

         return getExp_Unar(scope);
      }

      //
      // Parser::getExp_Post_ParenO
      //
      AST::Exp::CRef Parser::getExp_Post_ParenO(CC::Scope &scope, AST::Exp const *exp)
      {
         auto pos = in.get().pos;

         // const-call-specifier:
         //    <const> :
         in.drop(Core::TOK_KeyWrd, Core::STR_const, Core::TOK_Colon);

         Core::Array<AST::Exp::CRef> args;

         if(!in.peek(Core::TOK_ParenC))
            args = getExpList(scope);

         if(!in.drop(Core::TOK_ParenC))
            throw Core::ParseExceptExpect(in.peek(), ")", true);

         return CC::ExpCreate_Call(exp, std::move(args), pos);
      }

      //
      // Parser::getExp_Post
      //
      AST::Exp::CRef Parser::getExp_Post(CC::Scope &scope)
      {
         return getExp_Post(scope, getExp_Prim(scope));
      }

      //
      // Parser::getExp_Unar_Identi
      //
      AST::Exp::CRef Parser::getExp_Unar_Identi(CC::Scope &scope)
      {
         if(auto global = dynamic_cast<Scope_Global *>(&scope.global))
         {
            if(auto print = global->findPrint(in.peek().str))
               return getExp_Unar_print(scope, print);
         }

         return CC::Parser::getExp_Unar_Identi(scope);
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

            if(!in.drop(Core::TOK_BraceC))
               throw Core::ParseExceptExpect(in.peek(), "}", true);

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
}

// EOF

