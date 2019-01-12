//-----------------------------------------------------------------------------
//
// Copyright (C) 2015-2019 David Hill
//
// See COPYING for license information.
//
//-----------------------------------------------------------------------------
//
// ACS print-declaration parsing.
//
//-----------------------------------------------------------------------------

#include "ACC/Parse.hpp"

#include "ACC/Factory.hpp"
#include "ACC/Scope.hpp"

#include "Core/Exception.hpp"
#include "Core/TokenStream.hpp"

#include "SR/Statement.hpp"
#include "SR/Type.hpp"


//----------------------------------------------------------------------------|
// Static Functions                                                           |
//

namespace GDCC::ACC
{
   //
   // ParseDecl_PrintProp
   //
   static void ParseDecl_PrintProp(Parser &ctx, CC::Scope &scope, PrintDecl &decl)
   {
      // print-property:
      //    identifier : constant-expression
      //    identifier(opt) ( identifier ) : constant-expression

      Core::String nameProp, nameSub;

      auto pos = ctx.in.peek().pos;

      // identifier(opt)
      if(ctx.in.peek(Core::TOK_Identi))
         nameProp = ctx.in.get().str;
      else
         nameProp = nullptr;

      // ( identifier ) (opt)
      if(ctx.in.drop(Core::TOK_ParenO))
      {
         nameSub = ctx.expectIdenti(true).str;

         ctx.expect(Core::TOK_ParenC);
      }
      else
         nameSub = nullptr;

      // :
      ctx.expect(Core::TOK_Colon);

      // constant-expression
      auto exp = ctx.fact.expPromo_LValue(ctx.getExp_Assi(scope));

      auto type = exp->getType();

      if(!type->isTypePointer() || !(type = type->getBaseType())->isTypeFunction())
         Core::Error(exp->pos, "expected function type");

      if(nameProp)
      {
         PrintProp &prop = decl.props[nameProp];

         auto paramC = type->getParameters()->size();

         switch(nameSub)
         {
         case Core::STR_global:
                 if(paramC == 2) prop.propGlobalArray = exp;
            else if(paramC == 4) prop.propGlobalRange = exp;
            else Core::Error(exp->pos, "invalid argument count");
            break;

         case Core::STR_local:
                 if(paramC == 2) prop.propLocalArray = exp;
            else if(paramC == 4) prop.propLocalRange = exp;
            else Core::Error(exp->pos, "invalid argument count");
            break;

         case Core::STR_module:
                 if(paramC == 2) prop.propModuleArray = exp;
            else if(paramC == 4) prop.propModuleRange = exp;
            else Core::Error(exp->pos, "invalid argument count");
            break;

         case Core::STR_world:
                 if(paramC == 2) prop.propHubArray = exp;
            else if(paramC == 4) prop.propHubRange = exp;
            else Core::Error(exp->pos, "invalid argument count");
            break;

         case Core::STRNULL:
            prop.prop = exp;
            break;

         default:
            Core::ErrorExpect(pos, "print subproperty", nameSub);
         }
      }
      else
      {
         switch(nameSub)
         {
         case Core::STR_char:  decl.propChar  = exp; break;
         case Core::STR_begin: decl.propBegin = exp; break;
         case Core::STR_end:   decl.propEnd   = exp; break;
         case Core::STR_more:  decl.propMore  = exp; break;
         case Core::STR_opt:   decl.propOpt   = exp; break;
         case Core::STR_str:   decl.propStr   = exp; break;

         case Core::STRNULL:
            Core::Error(pos, "missing property name");

         default:
            Core::ErrorExpect(pos, "print subproperty", nameSub);
         }
      }
   }
}


//----------------------------------------------------------------------------|
// Extern Functions                                                           |
//

namespace GDCC::ACC
{
   //
   // Parser::getDecl_Print
   //
   SR::Statement::CRef Parser::getDecl_Print(Scope_Global &scope)
   {
      // print-declaration:
      //    <print> identifier ( print-property-list ) ;

      // <print>
      auto pos = expect(Core::TOK_KeyWrd, Core::STR_print).pos;

      // identifier
      auto name = expectIdenti().str;

      // (
      expect(Core::TOK_ParenO);

      // print-property-list:
      //    print-property
      //    print-property-list , print-property
      PrintDecl decl;
      do
         ParseDecl_PrintProp(*this, scope, decl);
      while(in.drop(Core::TOK_Comma));

      // )
      expect(Core::TOK_ParenC);

      // ;
      expect(Core::TOK_Semico);

      scope.addPrint(name, std::move(decl));

      return SR::StatementCreate_Empty(pos);
   }
}

// EOF

