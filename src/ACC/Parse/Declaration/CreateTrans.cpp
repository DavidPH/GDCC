//-----------------------------------------------------------------------------
//
// Copyright (C) 2015-2018 David Hill
//
// See COPYING for license information.
//
//-----------------------------------------------------------------------------
//
// ACS create-translation-declaration parsing.
//
//-----------------------------------------------------------------------------

#include "ACC/Parse.hpp"

#include "ACC/Scope.hpp"

#include "CC/Exp.hpp"

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
   // ParseDecl_CreateTransProp
   //
   static void ParseDecl_CreateTransProp(Parser &ctx, CC::Scope &scope,
      CreateTransDecl &decl)
   {
      CreateTransPropName name;

      // create-translation-property:
      //    create-translation-property-prefix(opt) (
      //       create-translation-property-name ) : constant-expression

      // create-translation-property-prefix(opt)
      if(!ctx.in.peek(Core::TOK_ParenO))
         name.prefix = ctx.in.get();
      else
         name.prefix = Core::TokenEOF;

      // (
      ctx.expect(Core::TOK_ParenO);

      // create-translation-property-name:
      //    identifier
      //    constant-expression : constant-expression = constant-expression
      //       : constant-expression
      Core::String nameSpecial;
      Core::Origin namePos = ctx.in.peek().pos;
      if(ctx.in.peek(Core::TOK_Identi, Core::TOK_ParenC))
      {
         nameSpecial = ctx.in.get().str;
      }
      else
      {
         nameSpecial = nullptr;

         name.argc[0] = CC::ExpToFastU(ctx.getExp_Cond(scope));
         ctx.expect(Core::TOK_Colon);
         name.argc[1] = CC::ExpToFastU(ctx.getExp_Cond(scope));
         ctx.expect(Core::TOK_Equal);
         name.argc[2] = CC::ExpToFastU(ctx.getExp_Cond(scope));
         ctx.expect(Core::TOK_Colon);
         name.argc[3] = CC::ExpToFastU(ctx.getExp_Cond(scope));
      }

      // )
      ctx.expect(Core::TOK_ParenC);

      // :
      ctx.expect(Core::TOK_Colon);

      // constant-expression;
      auto exp = CC::ExpPromo_LValue(ctx.getExp_Assi(scope));

      auto type = exp->getType();

      if(!type->isTypePointer() || !(type = type->getBaseType())->isTypeFunction())
         Core::Error(exp->pos, "expected function type");

      if(nameSpecial) switch(nameSpecial)
      {
      case Core::STR_begin: decl.propBegin = exp; break;
      case Core::STR_end:   decl.propEnd   = exp; break;
      default:
         Core::Error(namePos, "unrecognized special name");
      }
      else
      {
         decl.props[name].prop = exp;
      }
   }
}


//----------------------------------------------------------------------------|
// Extern Functions                                                           |
//

namespace GDCC::ACC
{
   //
   // Parser::getDecl_CreateTrans
   //
   SR::Statement::CRef Parser::getDecl_CreateTrans(Scope_Global &scope)
   {
      // create-translation-declaration:
      //    <createtranslation> identifier ( create-translation-property-list ) ;

      // <createtranslation>
      auto pos = expect(Core::TOK_KeyWrd, Core::STR_createtranslation).pos;

      // identifier
      auto name = expectIdenti().str;

      // (
      expect(Core::TOK_ParenO);

      // create-translation-property-list:
      //    create-translation-property
      //    create-translation-property-list , create-translation-property
      CreateTransDecl decl;
      do
         ParseDecl_CreateTransProp(*this, scope, decl);
      while(in.drop(Core::TOK_Comma));

      // )
      expect(Core::TOK_ParenC);

      // ;
      expect(Core::TOK_Semico);

      scope.addCreateTrans(name, std::move(decl));

      return SR::StatementCreate_Empty(pos);
   }
}

// EOF

