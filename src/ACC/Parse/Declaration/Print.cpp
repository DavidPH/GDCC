//-----------------------------------------------------------------------------
//
// Copyright (C) 2015 David Hill
//
// See COPYING for license information.
//
//-----------------------------------------------------------------------------
//
// ACS print-declaration parsing.
//
//-----------------------------------------------------------------------------

#include "ACC/Parse.hpp"

#include "ACC/Scope.hpp"

#include "AST/Statement.hpp"
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
      // ParseDecl_PrintProp
      //
      static void ParseDecl_PrintProp(Parser &ctx, CC::Scope &scope, PrintDecl &decl)
      {
         Core::String nameProp, nameSub;

         auto pos = ctx.in.peek().pos;

         if(ctx.in.peek(Core::TOK_Identi))
            nameProp = ctx.in.get().str;
         else
            nameProp = nullptr;


         if(ctx.in.drop(Core::TOK_ParenO))
         {
            if(!ctx.in.peek(Core::TOK_Identi) && !ctx.in.peek(Core::TOK_KeyWrd))
               throw Core::ParseExceptExpect(ctx.in.peek(), "identifier", false);

            nameSub = ctx.in.get().str;

            if(!ctx.in.drop(Core::TOK_ParenC))
               throw Core::ParseExceptExpect(ctx.in.peek(), ")", true);
         }
         else
            nameSub = nullptr;

         if(!ctx.in.drop(Core::TOK_Colon))
            throw Core::ParseExceptExpect(ctx.in.peek(), ":", true);

         auto exp = CC::ExpPromo_LValue(ctx.getExp_Assi(scope));

         auto type = exp->getType();

         if(!type->isTypePointer() || !(type = type->getBaseType())->isTypeFunction())
            throw Core::ExceptStr(exp->pos, "expected function type");

         if(nameProp)
         {
            PrintProp &prop = decl.props[nameProp];

            auto paramC = type->getParameters()->size();

            switch(nameSub)
            {
            case Core::STR_global:
                    if(paramC == 2) prop.propGlobalArray = exp;
               else if(paramC == 4) prop.propGlobalRange = exp;
               else throw Core::ExceptStr(exp->pos, "invalid argument count");
               break;

            case Core::STR_local:
                    if(paramC == 2) prop.propLocalArray = exp;
               else if(paramC == 4) prop.propLocalRange = exp;
               else throw Core::ExceptStr(exp->pos, "invalid argument count");
               break;

            case Core::STR_module:
                    if(paramC == 2) prop.propModuleArray = exp;
               else if(paramC == 4) prop.propModuleRange = exp;
               else throw Core::ExceptStr(exp->pos, "invalid argument count");
               break;

            case Core::STR_world:
                    if(paramC == 2) prop.propHubArray = exp;
               else if(paramC == 4) prop.propHubRange = exp;
               else throw Core::ExceptStr(exp->pos, "invalid argument count");
               break;

            case Core::STRNULL:
               prop.prop = exp;
               break;

            default:
               throw Core::ParseExceptExpect(pos, "print subproperty", nameSub, false);
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
               throw Core::ParseExceptStr(pos, "missing property name");

            default:
               throw Core::ParseExceptExpect(pos, "print subproperty", nameSub, false);
            }
         }
      }
   }
}


//----------------------------------------------------------------------------|
// Extern Functions                                                           |
//

namespace GDCC
{
   namespace ACC
   {
      //
      // Parser::getDecl_Print
      //
      AST::Statement::CRef Parser::getDecl_Print(Scope_Global &scope)
      {
         if(!in.peek(Core::TOK_KeyWrd, Core::STR_print))
            throw Core::ParseExceptExpect(in.peek(), "print-declaration", false);

         auto pos = in.get().pos;

         if(!in.peek(Core::TOK_Identi))
            throw Core::ParseExceptExpect(in.peek(), "identifier", false);

         auto name = in.get().str;

         if(!in.drop(Core::TOK_ParenO))
            throw Core::ParseExceptExpect(in.peek(), "(", true);

         PrintDecl decl;

         do
            ParseDecl_PrintProp(*this, scope, decl);
         while(in.drop(Core::TOK_Comma));

         if(!in.drop(Core::TOK_ParenC))
            throw Core::ParseExceptExpect(in.peek(), ")", true);

         if(!in.drop(Core::TOK_Semico))
            throw Core::ParseExceptExpect(in.peek(), ";", true);

         scope.addPrint(name, std::move(decl));

         return AST::StatementCreate_Empty(pos);
      }
   }
}

// EOF

