//-----------------------------------------------------------------------------
//
// Copyright (C) 2015-2019 David Hill
//
// See COPYING for license information.
//
//-----------------------------------------------------------------------------
//
// ACS statement parsing.
//
//-----------------------------------------------------------------------------

#include "ACC/Parse.hpp"

#include "ACC/Factory.hpp"

#include "AS/LabelTBuf.hpp"
#include "AS/TStream.hpp"

#include "CC/AsmGlyphTBuf.hpp"
#include "CC/Exp.hpp"
#include "CC/Scope/Case.hpp"
#include "CC/Scope/Function.hpp"
#include "CC/Statement/Goto.hpp"
#include "CC/Type.hpp"

#include "Core/Exception.hpp"
#include "Core/StringBuf.hpp"
#include "Core/TokenStream.hpp"

#include "SR/Exp.hpp"
#include "SR/Function.hpp"
#include "SR/Statement.hpp"


//----------------------------------------------------------------------------|
// Extern Functions                                                           |
//

namespace GDCC::ACC
{
   //
   // Parser::getStatement
   //
   SR::Statement::CRef Parser::getStatement(CC::Scope_Local &scope, Labels &&labels)
   {
      // compound-statement
      if(in.peek().tok == Core::TOK_BraceO)
         return getStatementCompound(scope, std::move(labels));

      if(in.peek(Core::TOK_KeyWrd))
         switch(in.peek().str)
      {
         // selection-statement
      case Core::STR_if:     return getStatement_if    (scope, std::move(labels));
      case Core::STR_switch: return getStatement_switch(scope, std::move(labels));

         // iteration-statement
      case Core::STR_while: return getStatement_while(scope, std::move(labels));
      case Core::STR_until: return getStatement_while(scope, std::move(labels));
      case Core::STR_do:    return getStatement_do   (scope, std::move(labels));
      case Core::STR_for:   return getStatement_for  (scope, std::move(labels));

         // jump-statement
      case Core::STR_continue:  return getStatement_continue (scope, std::move(labels));
      case Core::STR_break:     return getStatement_break    (scope, std::move(labels));
      case Core::STR_return:    return getStatement_return   (scope, std::move(labels));
      case Core::STR_restart:   return getStatement_restart  (scope, std::move(labels));
      case Core::STR_terminate: return getStatement_terminate(scope, std::move(labels));

      default: break;
      }

      // expression-statement
      return getStatementExp(scope, std::move(labels));
   }

   //
   // Parser::getStatement_do
   //
   SR::Statement::CRef Parser::getStatement_do(CC::Scope_Local &scope, Labels &&labels)
   {
      auto &loopScope = scope.createScopeLoop();

      // <do> statement <while> ( expression ) ;
      // <do> statement <until> ( expression ) ;

      // <do>
      auto pos = in.get().pos;

      // statement
      auto body = getStatement(loopScope);

      // <while>
      // <until>
      bool invert;
      if(!(invert = in.drop(Core::TOK_KeyWrd, Core::STR_until)) &&
         !in.drop(Core::TOK_KeyWrd, Core::STR_while))
         Core::ErrorExpect("'while' or 'until'", in.peek());

      // ( expression )
      auto cond = getStatementCond(loopScope);
      if(invert)
         cond = fact.expCreate_Not(cond, cond->pos);

      // ;
      expect(Core::TOK_Semico);

      return fact.stCreate_Do(std::move(labels), pos, loopScope, body, cond);
   }

   //
   // Parser::getStatement_restart
   //
   SR::Statement::CRef Parser::getStatement_restart(CC::Scope_Local &scope, Labels &&labels)
   {
      // <restart> ;

      // <restart>
      auto pos = in.get().pos;

      // ;
      expect(Core::TOK_Semico);

      return CC::Statement_Goto::Create(std::move(labels), pos, scope.fn.fn->getLabelRes());
   }

   //
   // Parser::getStatement_while
   //
   SR::Statement::CRef Parser::getStatement_while(CC::Scope_Local &scope, Labels &&labels)
   {
      auto &loopScope = scope.createScopeLoop();

      // <while> ( expression ) statement
      // <until> ( expression ) statement

      // <while>
      // <until>
      bool invert = in.peek().str == Core::STR_until;
      auto pos    = in.get().pos;

      // ( expression )
      auto cond = getStatementCond(loopScope);
      if(invert)
         cond = fact.expCreate_Not(cond, cond->pos);

      // statement
      auto body = getStatement(loopScope);

      return fact.stCreate_While(std::move(labels), pos, loopScope, cond, body);
   }

   //
   // Parser::getStatement_terminate
   //
   SR::Statement::CRef Parser::getStatement_terminate(CC::Scope_Local &scope, Labels &&labels)
   {
      // <terminate> ;

      // <terminate>
      auto pos = in.get().pos;

      // ;
      expect(Core::TOK_Semico);

      return fact.stCreate_Return(std::move(labels), pos, scope.fn);
   }

   //
   // Parser::getStatementCompound
   //
   SR::Statement::CRef Parser::getStatementCompound(CC::Scope_Local &scope, Labels &&labels)
   {
      // compound-statement:
      //    { block-item-list(opt) }

      // {
      auto pos = in.get().pos;

      std::vector<SR::Statement::CRef> stmnts;
      auto &blockScope = scope.createScopeBlock();

      // block-item-list:
      //    block-item
      //    block-item-list block-item
      while(!in.drop(Core::TOK_BraceC))
      {
         // block-item:
         //    declaration
         //    statement

         auto labs = getLabels(blockScope);

         // declaration
         if(isDecl(blockScope))
            stmnts.emplace_back(getDecl(blockScope, std::move(labs)));

         // statement
         else
            stmnts.emplace_back(getStatement(blockScope, std::move(labs)));
      }

      // }

      return SR::StatementCreate_Multi(std::move(labels), pos,
         {stmnts.begin(), stmnts.end()});
   }
}

// EOF

