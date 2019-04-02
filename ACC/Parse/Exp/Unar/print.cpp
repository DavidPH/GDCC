//-----------------------------------------------------------------------------
//
// Copyright (C) 2015-2019 David Hill
//
// See COPYING for license information.
//
//-----------------------------------------------------------------------------
//
// ACS print-expression parsing.
//
//-----------------------------------------------------------------------------

#include "ACC/Parse.hpp"

#include "CC/Factory.hpp"
#include "ACC/Scope.hpp"

#include "CC/Type.hpp"

#include "Core/Array.hpp"
#include "Core/Exception.hpp"
#include "Core/TokenStream.hpp"

#include "IR/Exp.hpp"
#include "IR/Glyph.hpp"

#include "SR/Exp/IRExp.hpp"
#include "SR/Type.hpp"


//----------------------------------------------------------------------------|
// Static Functions                                                           |
//

namespace GDCC::ACC
{
   //
   // GetPrintProp
   //
   static PrintProp const &GetPrintProp(PrintDecl const *print,
      Core::String name, Core::Origin pos)
   {
      auto propItr = print->props.find(name);
      if(propItr == print->props.end())
         Core::ErrorExpect(pos, "print-specifier", name);

      return propItr->second;
   }

   //
   // GetPrintSpec_Array
   //
   static SR::Exp::CPtr GetPrintSpec_Array(Parser &ctx, CC::Scope &scope,
      SR::Exp const *propArray, SR::Exp const *propRange,
      Core::Array<SR::Exp::CRef> &&args, Core::Origin pos)
   {
      SR::Type::CRef type = args[0]->getType()->getBaseType();

      auto space = type->getQualAddr();

      auto indexExp = ctx.fact.expCreate_Cst(CC::TypeIntegPrS, args[0]);
      auto spaceExp = ctx.fact.expCreate_IRExp(IR::ExpCreate_Glyph(
         {ctx.prog, space.name}, pos), CC::TypeIntegPrS, pos);

      if(args.size() == 1 && propArray)
         return ctx.fact.expCreate_Call(propArray, {indexExp, spaceExp}, scope, pos);

      if(args.size() == 2 && propRange)
         return ctx.fact.expCreate_Call(propRange, {indexExp, spaceExp, args[1],
            ctx.fact.expCreate_LitInt(CC::TypeIntegPrS, 0x7FFFFFFF, pos)}, scope, pos);

      if(args.size() == 3 && propRange)
         return ctx.fact.expCreate_Call(propRange, {indexExp, spaceExp, args[1],
            args[2]}, scope, pos);

      return nullptr;
   }

   //
   // GetPrintSpec
   //
   static SR::Exp::CRef GetPrintSpec(Parser &ctx, CC::Scope &scope,
      PrintProp const &prop, Core::Origin pos)
   {
      // print-specifier:
      //    identifier : ( expression-list )
      //    identifier : assignment-expression

      Core::Array<SR::Exp::CRef> args;

      // ( expression-list )
      if(ctx.in.peek(Core::TOK_ParenO) && prop.isMultiArg())
      {
         ctx.in.get();

         args = ctx.getExpList(scope);

         ctx.expect(Core::TOK_ParenC);
      }
      // assignment-expression
      else
         args = {Core::Pack, ctx.getExp_Assi(scope)};

      for(auto &arg : args)
         arg = ctx.fact.expPromo_LValue(arg);

      // Possibly array special print?
      if(args.size() >= 1 && args[0]->getType()->isTypePointer())
      {
         SR::Type::CRef type = args[0]->getType()->getBaseType();

         auto space = type->getQualAddr();

         if(type->isCTypeInteg()) switch(space.base)
         {
         case IR::AddrBase::GblArr:
            if(auto exp = GetPrintSpec_Array(ctx, scope, prop.propGlobalArray,
               prop.propGlobalRange, std::move(args), pos))
               return static_cast<SR::Exp::CRef>(exp);
            break;

         case IR::AddrBase::HubArr:
            if(auto exp = GetPrintSpec_Array(ctx, scope, prop.propHubArray,
               prop.propHubRange, std::move(args), pos))
               return static_cast<SR::Exp::CRef>(exp);
            break;

         case IR::AddrBase::LocArr:
            if(auto exp = GetPrintSpec_Array(ctx, scope, prop.propLocalArray,
               prop.propLocalRange, std::move(args), pos))
               return static_cast<SR::Exp::CRef>(exp);
            break;

         case IR::AddrBase::ModArr:
            if(auto exp = GetPrintSpec_Array(ctx, scope, prop.propModuleArray,
               prop.propModuleRange, std::move(args), pos))
               return static_cast<SR::Exp::CRef>(exp);
            break;

         default:
            break;
         }
      }

      if(prop.prop)
         return ctx.fact.expCreate_Call(prop.prop, std::move(args), scope, pos);

      Core::Error(pos, "no valid print property");
   }

   //
   // GetPrintSpecList
   //
   static SR::Exp::CRef GetPrintSpecList(SR::Exp::CRef exp, Parser &ctx,
      CC::Scope &scope, PrintDecl const *print)
   {
      // print-specifier-list:
      //    print-specifier
      //    print-specifier-list , print-specifier

      do
      {
         // print-specifier:
         //    identifier : print-specifier-argument

         // identifier
         auto name = ctx.expectIdenti();

         // :
         ctx.expect(Core::TOK_Colon);

         PrintProp const &prop = GetPrintProp(print, name.str, name.pos);
         SR::Exp::CRef   spec  = GetPrintSpec(ctx, scope, prop, name.pos);

         exp = ctx.fact.expCreate_Comma(exp, spec, name.pos);
      }
      while(ctx.in.drop(Core::TOK_Comma));

      return exp;
   }

   //
   // GetPrintSpecString
   //
   static SR::Exp::CRef GetPrintSpecString(SR::Exp::CRef exp, Parser &ctx,
      CC::Scope &scope, PrintDecl const *print)
   {
      // print-specifier-string:
      //    string-literal
      //    string-literal , print-specifier-argument-list

      // string-literal
      auto tok = ctx.in.get();
      auto &pos = tok.pos;

      for(auto strItr = tok.str.begin(), strEnd = tok.str.end();;)
      {
         auto strStr = strItr;

         // Scan for format specifier or end of string.
         while(strItr != strEnd && *strItr != '%') ++strItr;

         // If preceeding portion is non-empty, add string print.
         if(strStr != strItr)
         {
            if(!print->propStr)
               Core::Error(pos, "no (str) property");

            auto strExp = ctx.fact.expCreate_StrIdx(ctx.prog, scope, {strStr, strItr}, pos);
            auto call   = ctx.fact.expCreate_Call(print->propStr, {strExp}, scope, pos);

            exp = ctx.fact.expCreate_Comma(exp, call, pos);
         }

         // If end-of-string, terminate.
         if(strItr == strEnd) break;

         // Skip specifier start.
         strStr = ++strItr;

         // Scan for end of print specifier.
         while(strItr != strEnd && *strItr != ':')
            ++strItr;

         // Handle specifier.
         ctx.expect(Core::TOK_Comma);

         PrintProp const &prop = GetPrintProp(print, {strStr, strItr}, pos);
         SR::Exp::CRef   spec = GetPrintSpec(ctx, scope, prop, pos);

         exp = ctx.fact.expCreate_Comma(exp, spec, pos);

         // If end-of-string, terminate.
         if(strItr == strEnd) break;

         // Skip specifier end.
         ++strItr;
      }

      return exp;
   }
}


//----------------------------------------------------------------------------|
// Extern Functions                                                           |
//

namespace GDCC::ACC
{
   //
   // Parser::getExp_Unar_print
   //
   SR::Exp::CRef Parser::getExp_Unar_print(CC::Scope &scope, PrintDecl const *print)
   {
      // print-expression:
      //    print-identifier ( print-specifier-list(opt) print-argument-list(opt) )
      //    print-identifier ( print-specifier-string print-argument-list(opt) )

      // print-identifier
      auto pos = in.get().pos;

      // Start with a no-op expression.
      SR::Exp::CRef exp = SR::Exp_IRExp::Create_Size(0);

      if(print->propBegin)
         exp = fact.expCreate_Comma(exp,
            fact.expCreate_Call(print->propBegin, {}, scope, pos), pos);

      // (
      expect(Core::TOK_ParenO);

      // print-specifier-string
      if(in.peek().isTokString())
         exp = GetPrintSpecString(exp, *this, scope, print);

      // print-specifier-list(opt)
      else if(in.peek(Core::TOK_Identi))
         exp = GetPrintSpecList(exp, *this, scope, print);

      // print-argument-list:
      //    ; expression-list
      if(in.drop(Core::TOK_Semico))
      {
         auto args = getExpList(scope);

         SR::Exp::CRef *argp = args.data();
         std::size_t    argc = args.size();

         if(print->propMore)
         {
            auto paramc = print->propMore->getType()->getBaseType()
               ->getParameters()->size();

            if(paramc > argc)
               Core::Error(pos, "insufficient arguments");

            exp = fact.expCreate_Comma(exp, fact.expCreate_Call(
               print->propMore, {argp, argp + paramc}, scope, pos), pos);

            argp += paramc;
            argc -= paramc;
         }

         if(print->propOpt)
         {
            auto paramc = print->propOpt->getType()->getBaseType()
               ->getParameters()->size();

            if(paramc > argc)
               Core::Error(pos, "insufficient arguments");

            exp = fact.expCreate_Comma(exp, fact.expCreate_Call(
               print->propOpt, {argp, argp + paramc}, scope, pos), pos);

            argp += paramc;
            argc -= paramc;
         }

         if(print->propEnd)
         {
            exp = fact.expCreate_Comma(exp, fact.expCreate_Call(
               print->propEnd, {argp, argp + argc}, scope, pos), pos);
         }
      }
      else
      {
         if(print->propEnd)
            exp = fact.expCreate_Comma(exp,
               fact.expCreate_Call(print->propEnd, {}, scope, pos), pos);
      }

      // )
      expect(Core::TOK_ParenC);

      return exp;
   }
}

// EOF

