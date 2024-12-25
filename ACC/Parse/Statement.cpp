//-----------------------------------------------------------------------------
//
// Copyright (C) 2015-2024 David Hill
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
   // Parser::getSt
   //
   SR::Statement::CRef Parser::getSt(CC::Scope_Local &scope,
      SR::Attribute &&attr, Labels &&labels)
   {
      // compound-statement
      if(in.peek().tok == Core::TOK_BraceO)
         return getStCompound(scope, std::move(attr), std::move(labels));

      if(in.peek(Core::TOK_KeyWrd))
         switch(in.peek().str)
      {
         // selection-statement
      case Core::STR_if:     return getSt_if    (scope, std::move(attr), std::move(labels));
      case Core::STR_switch: return getSt_switch(scope, std::move(attr), std::move(labels));

         // iteration-statement
      case Core::STR_while: return getSt_while(scope, std::move(attr), std::move(labels));
      case Core::STR_until: return getSt_while(scope, std::move(attr), std::move(labels));
      case Core::STR_do:    return getSt_do   (scope, std::move(attr), std::move(labels));
      case Core::STR_for:   return getSt_for  (scope, std::move(attr), std::move(labels));

         // jump-statement
      case Core::STR_continue:  return getSt_continue (scope, std::move(attr), std::move(labels));
      case Core::STR_break:     return getSt_break    (scope, std::move(attr), std::move(labels));
      case Core::STR_goto:      return getSt_goto     (scope, std::move(attr), std::move(labels));
      case Core::STR_return:    return getSt_return   (scope, std::move(attr), std::move(labels));
      case Core::STR_restart:   return getSt_restart  (scope, std::move(attr), std::move(labels));
      case Core::STR_terminate: return getSt_terminate(scope, std::move(attr), std::move(labels));

      default: break;
      }

      // expression-statement
      return getStExp(scope, std::move(attr), std::move(labels));
   }

   //
   // Parser::getSt_do
   //
   SR::Statement::CRef Parser::getSt_do(CC::Scope_Local &scope,
      SR::Attribute &&, Labels &&labels)
   {
      auto &loopScope = scope.createScopeLoop();

      // <do> statement <while> ( expression ) ;
      // <do> statement <until> ( expression ) ;

      // <do>
      auto pos = in.get().pos;

      // statement
      auto body = getSt(loopScope);

      // <while>
      // <until>
      bool invert;
      if(!(invert = in.drop(Core::TOK_KeyWrd, Core::STR_until)) &&
         !in.drop(Core::TOK_KeyWrd, Core::STR_while))
         Core::ErrorExpect("'while' or 'until'", in.peek());

      // ( expression )
      auto cond = getStCond(loopScope);
      if(invert)
         cond = fact.expCreate_Not(cond, cond->pos);

      // ;
      expect(Core::TOK_Semico);

      return fact.stCreate_Do(std::move(labels), pos, loopScope, body, cond);
   }

   //
   // Parser::getSt_goto
   //
   SR::Statement::CRef Parser::getSt_goto(CC::Scope_Local &scope,
      SR::Attribute &&, Labels &&labels)
   {
      // <goto> identifier ;

      // <goto>
      auto pos = in.get().pos;

      // identifier
      auto name = expectIdenti().str;

      // ;
      expect(Core::TOK_Semico);

      return fact.stCreate_Goto(std::move(labels), pos, scope, name);
   }

   //
   // Parser::getSt_restart
   //
   SR::Statement::CRef Parser::getSt_restart(CC::Scope_Local &scope,
      SR::Attribute &&, Labels &&labels)
   {
      // <restart> ;

      // <restart>
      auto pos = in.get().pos;

      // ;
      expect(Core::TOK_Semico);

      return CC::Statement_Goto::Create(std::move(labels), pos, scope.fn.fn->getLabelRes());
   }

   //
   // Parser::getSt_while
   //
   SR::Statement::CRef Parser::getSt_while(CC::Scope_Local &scope,
      SR::Attribute &&, Labels &&labels)
   {
      auto &loopScope = scope.createScopeLoop();

      // <while> ( expression ) statement
      // <until> ( expression ) statement

      // <while>
      // <until>
      bool invert = in.peek().str == Core::STR_until;
      auto pos    = in.get().pos;

      // ( expression )
      auto cond = getStCond(loopScope);
      if(invert)
         cond = fact.expCreate_Not(cond, cond->pos);

      // statement
      auto body = getSt(loopScope);

      return fact.stCreate_While(std::move(labels), pos, loopScope, cond, body);
   }

   //
   // Parser::getSt_terminate
   //
   SR::Statement::CRef Parser::getSt_terminate(CC::Scope_Local &scope,
      SR::Attribute &&, Labels &&labels)
   {
      // <terminate> ;

      // <terminate>
      auto pos = in.get().pos;

      // ;
      expect(Core::TOK_Semico);

      return fact.stCreate_Return(std::move(labels), pos, scope.fn);
   }
}

// EOF

