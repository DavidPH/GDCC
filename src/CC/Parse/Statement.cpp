//-----------------------------------------------------------------------------
//
// Copyright (C) 2014 David Hill
//
// See COPYING for license information.
//
//-----------------------------------------------------------------------------
//
// C statement parsing.
//
//-----------------------------------------------------------------------------

#include "CC/Parse.hpp"

#include "CC/Exp.hpp"
#include "CC/Scope/Block.hpp"
#include "CC/Statement.hpp"

#include "AST/Exp.hpp"
#include "AST/Function.hpp"
#include "AST/Statement.hpp"

#include "Core/Exception.hpp"
#include "Core/TokenStream.hpp"


//----------------------------------------------------------------------------|
// Static Functions                                                           |
//

//
// GetStatement_Compound
//
static GDCC::AST::Statement::CRef GetStatement_Compound(GDCC::CC::ParserData &in,
   GDCC::CC::LocalScope *ctx, GDCC::Core::Array<GDCC::Core::String> &labels)
{
   using namespace GDCC;

   // compound-statement:
   //    { block-item-list(opt) }

   // {
   auto pos = in.in.get().pos;

   std::vector<AST::Statement::CRef> stmnts;
   auto blockCtx = ctx->createScopeBlock();

   // block-item-list:
   //    block-item
   //    block-item-list block-item
   while(!in.in.drop(Core::TOK_BraceC))
   {
      // block-item:
      //    declaration
      //    statement

      // declaration
      if(CC::IsDecl(in, ctx))
         stmnts.emplace_back(CC::GetDecl(in, blockCtx));

      // statement
      else
         stmnts.emplace_back(CC::GetStatement(in, blockCtx));
   }

   // }

   return AST::StatementCreate_Multi(std::move(labels), pos,
      Core::Array<AST::Statement::CRef>(stmnts.begin(), stmnts.end()));
}

//
// GetStatement_Exp
//
static GDCC::AST::Statement::CRef GetStatement_Exp(GDCC::CC::ParserData &in,
   GDCC::CC::LocalScope *ctx, GDCC::Core::Array<GDCC::Core::String> &labels)
{
   using namespace GDCC;

   // expression-statement:
   //    expression(opt) ;

   auto pos = in.in.peek().pos;

   // ;
   if(in.in.drop(Core::TOK_Semico))
      return AST::StatementCreate_Empty(std::move(labels), pos);

   // expression
   auto exp = CC::GetExp(in, ctx);

   // ;
   if(!in.in.drop(Core::TOK_Semico))
      throw Core::ExceptStr(in.in.peek().pos, "expected ';'");

   return AST::StatementCreate_Exp(std::move(labels), pos, exp);
}

//
// GetStatement_break
//
static GDCC::AST::Statement::CRef GetStatement_break(GDCC::CC::ParserData &in,
   GDCC::CC::LocalScope *ctx, GDCC::Core::Array<GDCC::Core::String> &labels)
{
   using namespace GDCC;

   // <break> ;

   // <break>
   auto pos = in.in.get().pos;

   // ;
   if(!in.in.drop(Core::TOK_Semico))
      throw Core::ExceptStr(in.in.peek().pos, "expected ';'");

   return CC::StatementCreate_Break(std::move(labels), pos, ctx);
}

//
// GetStatement_continue
//
static GDCC::AST::Statement::CRef GetStatement_continue(GDCC::CC::ParserData &in,
   GDCC::CC::LocalScope *ctx, GDCC::Core::Array<GDCC::Core::String> &labels)
{
   using namespace GDCC;

   // <continue> ;

   // <continue>
   auto pos = in.in.get().pos;

   // ;
   if(!in.in.drop(Core::TOK_Semico))
      throw Core::ExceptStr(in.in.peek().pos, "expected ';'");

   return CC::StatementCreate_Continue(std::move(labels), pos, ctx);
}

//
// GetStatement_do
//
static GDCC::AST::Statement::CRef GetStatement_do(GDCC::CC::ParserData &in,
   GDCC::CC::LocalScope *ctx, GDCC::Core::Array<GDCC::Core::String> &labels)
{
   using namespace GDCC;

   auto loopCtx = ctx->createScopeLoop();

   // <do> statement <while> ( expression ) ;

   // <do>
   auto pos = in.in.get().pos;

   // statement
   auto body = GetStatement(in, loopCtx);

   // (
   if(!in.in.drop(Core::TOK_ParenO))
      throw Core::ExceptStr(in.in.peek().pos, "expected '('");

   // expression
   auto cond = GetExp(in, loopCtx);

   // )
   if(!in.in.drop(Core::TOK_ParenC))
      throw Core::ExceptStr(in.in.peek().pos, "expected ')'");

   // ;
   if(!in.in.drop(Core::TOK_Semico))
      throw Core::ExceptStr(in.in.peek().pos, "expected ';'");

   return CC::StatementCreate_Do(std::move(labels), pos, loopCtx, body, cond);
}

//
// GetStatement_for
//
static GDCC::AST::Statement::CRef GetStatement_for(GDCC::CC::ParserData &in,
   GDCC::CC::LocalScope *ctx, GDCC::Core::Array<GDCC::Core::String> &labels)
{
   using namespace GDCC;

   auto loopCtx = ctx->createScopeLoop();

   // <for> ( expression(opt) ; expression(opt) ; expression(opt) )
   // <for> ( declaration expression(opt) ; expression(opt) )

   // <for>
   auto pos = in.in.get().pos;

   // (
   if(!in.in.drop(Core::TOK_ParenO))
      throw Core::ExceptStr(in.in.peek().pos, "expected '('");

   AST::Statement::CPtr init;
   // declaration
   if(IsDecl(in, loopCtx))
   {
      init = GetDecl(in, loopCtx);
   }
   // expression(opt) ;
   else
   {
      // expression(opt)
      if(in.in.peek().tok != Core::TOK_Semico)
         init = AST::StatementCreate_Exp(GetExp(in, loopCtx));
      else
         init = AST::StatementCreate_Empty(pos);

      // ;
      if(!in.in.drop(Core::TOK_Semico))
         throw Core::ExceptStr(in.in.peek().pos, "expected ';'");
   }

   // expression(opt)
   AST::Exp::CPtr cond;
   if(in.in.peek().tok != Core::TOK_Semico)
      cond = GetExp(in, loopCtx);
   else
      throw Core::ExceptStr(pos, "empty cond stub");

   // ;
   if(!in.in.drop(Core::TOK_Semico))
      throw Core::ExceptStr(in.in.peek().pos, "expected ';'");

   // expression(opt)
   AST::Statement::CPtr iter;
   if(in.in.peek().tok != Core::TOK_ParenC)
      iter = AST::StatementCreate_Exp(GetExp(in, loopCtx));
   else
      iter = AST::StatementCreate_Empty(pos);

   // )
   if(!in.in.drop(Core::TOK_ParenC))
      throw Core::ExceptStr(in.in.peek().pos, "expected ')'");

   // statement
   auto body = GetStatement(in, loopCtx);

   return CC::StatementCreate_For(std::move(labels), pos, loopCtx, init, cond,
      iter, body);
}

//
// GetStatement_goto
//
static GDCC::AST::Statement::CRef GetStatement_goto(GDCC::CC::ParserData &in,
   GDCC::CC::LocalScope *ctx, GDCC::Core::Array<GDCC::Core::String> &labels)
{
   using namespace GDCC;

   // <goto> identifier ;

   // <goto>
   auto pos = in.in.get().pos;

   // identifier
   if(in.in.peek().tok != Core::TOK_Identi)
      throw Core::ExceptStr(in.in.peek().pos, "expected identifier");
   auto name = in.in.get().str;

   // ;
   if(!in.in.drop(Core::TOK_Semico))
      throw Core::ExceptStr(in.in.peek().pos, "expected ';'");

   return CC::StatementCreate_Goto(std::move(labels), pos, ctx, name);
}

//
// GetStatement_if
//
static GDCC::AST::Statement::CRef GetStatement_if(GDCC::CC::ParserData &in,
   GDCC::CC::LocalScope *ctx, GDCC::Core::Array<GDCC::Core::String> &labels)
{
   using namespace GDCC;

   // <if> ( expression ) statement
   // <if> ( expression ) statement <else> statement

   // <if>
   auto pos = in.in.get().pos;

   // (
   if(!in.in.drop(Core::TOK_ParenO))
      throw Core::ExceptStr(in.in.peek().pos, "expected '('");

   // expression
   auto cond = GetExp(in, ctx);

   // )
   if(!in.in.drop(Core::TOK_ParenC))
      throw Core::ExceptStr(in.in.peek().pos, "expected ')'");

   // statement
   auto bodyT = GetStatement(in, ctx);

   // <else> statement
   if(in.in.drop(Core::TOK_KeyWrd, Core::STR_else))
   {
      // statement
      auto bodyF = GetStatement(in, ctx);

      return CC::StatementCreate_If(std::move(labels), pos, cond, bodyT, bodyF);
   }

   return CC::StatementCreate_If(std::move(labels), pos, cond, bodyT);
}

//
// GetStatement_return
//
static GDCC::AST::Statement::CRef GetStatement_return(GDCC::CC::ParserData &in,
   GDCC::CC::LocalScope *ctx, GDCC::Core::Array<GDCC::Core::String> &labels)
{
   using namespace GDCC;

   auto fn = ctx->getFunction();

   // <return> expression(opt) ;

   // <return>
   auto pos = in.in.get().pos;

   // ;
   if(in.in.drop(Core::TOK_Semico))
      return CC::StatementCreate_Return(std::move(labels), pos, fn);

   // expression
   auto exp = CC::GetExp(in, ctx);

   // ;
   if(!in.in.drop(Core::TOK_Semico))
      throw Core::ExceptStr(in.in.peek().pos, "expected ';'");

   return CC::StatementCreate_Return(std::move(labels), pos, fn, exp);
}

//
// GetStatement_switch
//
static GDCC::AST::Statement::CRef GetStatement_switch(GDCC::CC::ParserData &in,
   GDCC::CC::LocalScope *ctx, GDCC::Core::Array<GDCC::Core::String> &labels)
{
   using namespace GDCC;

   auto switchCtx = ctx->createScopeSwitch();

   // <switch> ( expression ) statement

   // <switch>
   auto pos = in.in.get().pos;

   // (
   if(!in.in.drop(Core::TOK_ParenO))
      throw Core::ExceptStr(in.in.peek().pos, "expected '('");

   // expression
   auto cond = GetExp(in, switchCtx);

   // )
   if(!in.in.drop(Core::TOK_ParenC))
      throw Core::ExceptStr(in.in.peek().pos, "expected ')'");

   // statement
   auto body = GetStatement(in, switchCtx);

   return CC::StatementCreate_Switch(std::move(labels), pos, switchCtx, cond, body);
}

//
// GetStatement_while
//
static GDCC::AST::Statement::CRef GetStatement_while(GDCC::CC::ParserData &in,
   GDCC::CC::LocalScope *ctx, GDCC::Core::Array<GDCC::Core::String> &labels)
{
   using namespace GDCC;

   auto loopCtx = ctx->createScopeLoop();

   // <while> ( expression ) statement

   // <while>
   auto pos = in.in.get().pos;

   // (
   if(!in.in.drop(Core::TOK_ParenO))
      throw Core::ExceptStr(in.in.peek().pos, "expected '('");

   // expression
   auto cond = GetExp(in, loopCtx);

   // )
   if(!in.in.drop(Core::TOK_ParenC))
      throw Core::ExceptStr(in.in.peek().pos, "expected ')'");

   // statement
   auto body = GetStatement(in, loopCtx);

   return CC::StatementCreate_While(std::move(labels), pos, loopCtx, cond, body);
}

//
// GetStatementLabel
//
static GDCC::Core::Array<GDCC::Core::String> GetStatementLabel(
   GDCC::CC::ParserData &in, GDCC::CC::LocalScope *ctx)
{
   using namespace GDCC;

   std::vector<Core::String> labels;

   for(;;)
   {
      auto tok = in.in.peek();

      // Keyword label?
      if(tok.tok == Core::TOK_KeyWrd)
      {
         // <case> constant-expression :
         if(tok.str == Core::STR_case)
         {
            auto val = CC::ExpToInteg(CC::GetExp_Cond(in, ctx));

            if(!in.in.drop(Core::TOK_Colon))
               throw Core::ExceptStr(in.in.peek().pos, "expected ':'");

            throw Core::ExceptStr(tok.pos, "case stub");
         }

         // <default> :
         else if(tok.str == Core::STR_default)
         {
            if(!in.in.drop(Core::TOK_Colon))
               throw Core::ExceptStr(in.in.peek().pos, "expected ':'");

            throw Core::ExceptStr(tok.pos, "default stub");
         }

         else
            break;
      }

      // identifier :
      else if(tok.tok == Core::TOK_Identi)
      {
         in.in.get();
         if(in.in.drop(Core::TOK_Colon))
         {
            throw Core::ExceptStr(tok.pos, "label stub");
         }
         else
         {
            in.in.unget();
            break;
         }
      }

      else
         break;
   }

   return Core::Array<Core::String>(labels.begin(), labels.end());
}


//----------------------------------------------------------------------------|
// Global Functions                                                           |
//

namespace GDCC
{
   namespace CC
   {
      //
      // GetStatement
      //
      AST::Statement::CRef GetStatement(ParserData &in, LocalScope *ctx)
      {
         auto labels = GetStatementLabel(in, ctx);

         // compound-statement
         if(in.in.peek().tok == Core::TOK_BraceO)
            return GetStatement_Compound(in, ctx, labels);

         if(in.in.peek().tok == Core::TOK_KeyWrd) switch(in.in.peek().str)
         {
            // selection-statement
         case Core::STR_if:     return GetStatement_if    (in, ctx, labels);
         case Core::STR_switch: return GetStatement_switch(in, ctx, labels);

            // iteration-statement
         case Core::STR_while: return GetStatement_while(in, ctx, labels);
         case Core::STR_do:    return GetStatement_do   (in, ctx, labels);
         case Core::STR_for:   return GetStatement_for  (in, ctx, labels);

            // jump-statement:
         case Core::STR_goto:     return GetStatement_goto    (in, ctx, labels);
         case Core::STR_continue: return GetStatement_continue(in, ctx, labels);
         case Core::STR_break:    return GetStatement_break   (in, ctx, labels);
         case Core::STR_return:   return GetStatement_return  (in, ctx, labels);

         default: break;
         }

         // expression-statement
         return GetStatement_Exp(in, ctx, labels);
      }
   }
}

// EOF

