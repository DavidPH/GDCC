//-----------------------------------------------------------------------------
//
// Copyright (C) 2015 David Hill
//
// See COPYING for license information.
//
//-----------------------------------------------------------------------------
//
// ACS print-expression parsing.
//
//-----------------------------------------------------------------------------

#include "ACC/Parse.hpp"

#include "ACC/Scope.hpp"

#include "AST/Exp.hpp"
#include "AST/Type.hpp"

#include "CC/Exp.hpp"
#include "CC/Type.hpp"

#include "Core/Array.hpp"
#include "Core/Exception.hpp"
#include "Core/TokenStream.hpp"

#include "IR/Exp.hpp"
#include "IR/Glyph.hpp"


//----------------------------------------------------------------------------|
// Static Functions                                                           |
//

namespace GDCC
{
   namespace ACC
   {
      //
      // GetPrintProp
      //
      static PrintProp const &GetPrintProp(PrintDecl const *print,
         Core::String name, Core::Origin pos)
      {
         auto propItr = print->props.find(name);
         if(propItr == print->props.end())
            throw Core::ParseExceptExpect(pos, "print-specifier", name, false);

         return propItr->second;
      }

      //
      // GetPrintSpec_Array
      //
      static AST::Exp::CPtr GetPrintSpec_Array(Parser &ctx, CC::Scope &scope,
         AST::Exp const *propArray, AST::Exp const *propRange,
         Core::Array<AST::Exp::CRef> &&args, Core::Origin pos)
      {
         AST::Type::CRef type = args[0]->getType()->getBaseType();

         auto space = type->getQualAddr();

         auto indexExp = CC::ExpCreate_Cst(CC::TypeIntegPrS, args[0]);
         auto spaceExp = AST::ExpCreate_IRExp(IR::ExpCreate_Glyph(
            {ctx.prog, space.name}, pos), CC::TypeIntegPrS, pos);

         if(args.size() == 1 && propArray)
            return CC::ExpCreate_Call(propArray, {indexExp, spaceExp}, scope, pos);

         if(args.size() == 2 && propRange)
            return CC::ExpCreate_Call(propRange, {indexExp, spaceExp, args[1],
               CC::ExpCreate_LitInt(CC::TypeIntegPrS, 0x7FFFFFFF, pos)}, scope, pos);

         if(args.size() == 3 && propRange)
            return CC::ExpCreate_Call(propRange, {indexExp, spaceExp, args[1],
               args[2]}, scope, pos);

         return nullptr;
      }

      //
      // GetPrintSpec
      //
      static AST::Exp::CRef GetPrintSpec(Parser &ctx, CC::Scope &scope,
         PrintProp const &prop, Core::Origin pos)
      {
         // print-specifier:
         //    identifier : ( expression-list )
         //    identifier : assignment-expression

         Core::Array<AST::Exp::CRef> args;

         // ( expression-list )
         if(ctx.in.drop(Core::TOK_ParenO))
         {
            args = ctx.getExpList(scope);

            if(!ctx.in.drop(Core::TOK_ParenC))
               throw Core::ParseExceptExpect(ctx.in.peek(), ")", true);
         }
         // assignment-expression
         else
            args = {Core::Pack, ctx.getExp_Assi(scope)};

         for(auto &arg : args)
            arg = CC::ExpPromo_LValue(arg);

         // Possibly array special print?
         if(args.size() >= 1 && args[0]->getType()->isTypePointer())
         {
            AST::Type::CRef type = args[0]->getType()->getBaseType();

            auto space = type->getQualAddr();

            if(type->isCTypeInteg()) switch(space.base)
            {
            case IR::AddrBase::GblArr:
               if(auto exp = GetPrintSpec_Array(ctx, scope, prop.propGlobalArray,
                  prop.propGlobalRange, std::move(args), pos))
                  return static_cast<AST::Exp::CRef>(exp);
               break;

            case IR::AddrBase::LocArr:
               if(auto exp = GetPrintSpec_Array(ctx, scope, prop.propLocalArray,
                  prop.propLocalRange, std::move(args), pos))
                  return static_cast<AST::Exp::CRef>(exp);
               break;

            case IR::AddrBase::MapArr:
               if(auto exp = GetPrintSpec_Array(ctx, scope, prop.propMapArray,
                  prop.propMapRange, std::move(args), pos))
                  return static_cast<AST::Exp::CRef>(exp);
               break;

            case IR::AddrBase::WldArr:
               if(auto exp = GetPrintSpec_Array(ctx, scope, prop.propWorldArray,
                  prop.propWorldRange, std::move(args), pos))
                  return static_cast<AST::Exp::CRef>(exp);
               break;

            default:
               break;
            }
         }

         if(prop.prop)
            return CC::ExpCreate_Call(prop.prop, std::move(args), scope, pos);

         throw Core::ExceptStr(pos, "no valid print property");
      }

      //
      // GetPrintSpecList
      //
      static AST::Exp::CRef GetPrintSpecList(AST::Exp::CRef exp, Parser &ctx,
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
            if(!ctx.in.peek(Core::TOK_Identi))
               throw Core::ParseExceptExpect(ctx.in.peek(), "identifier", false);

            auto name = ctx.in.get();

            // :
            if(!ctx.in.drop(Core::TOK_Colon))
               throw Core::ParseExceptExpect(ctx.in.peek(), ":", true);

            PrintProp const &prop = GetPrintProp(print, name.str, name.pos);
            AST::Exp::CRef   spec = GetPrintSpec(ctx, scope, prop, name.pos);

            exp = CC::ExpCreate_Comma(exp, spec, name.pos);
         }
         while(ctx.in.drop(Core::TOK_Comma));

         return exp;
      }

      //
      // GetPrintSpecString
      //
      static AST::Exp::CRef GetPrintSpecString(AST::Exp::CRef exp, Parser &ctx,
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
                  throw Core::ExceptStr(pos, "no (str) property");

               auto strExp = CC::ExpCreate_StrIdx(ctx.prog, scope, {strStr, strItr}, pos);
               auto call   = CC::ExpCreate_Call(print->propStr, {strExp}, scope, pos);

               exp = CC::ExpCreate_Comma(exp, call, pos);
            }

            // If end-of-string, terminate.
            if(strItr == strEnd) break;

            // Skip specifier start.
            strStr = ++strItr;

            // Scan for end of print specifier.
            while(strItr != strEnd && *strItr != ':')
               ++strItr;

            // Handle specifier.
            if(!ctx.in.drop(Core::TOK_Comma))
               throw Core::ParseExceptExpect(ctx.in.peek(), ",", true);

            PrintProp const &prop = GetPrintProp(print, {strStr, strItr}, pos);
            AST::Exp::CRef   spec = GetPrintSpec(ctx, scope, prop, pos);

            exp = CC::ExpCreate_Comma(exp, spec, pos);

            // If end-of-string, terminate.
            if(strItr == strEnd) break;

            // Skip specifier end.
            ++strItr;
         }

         return exp;
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
      // Parser::getExp_Unar_print
      //
      AST::Exp::CRef Parser::getExp_Unar_print(CC::Scope &scope, PrintDecl const *print)
      {
         // print-expression:
         //    print-identifier ( print-specifier-list(opt) print-argument-list(opt) )
         //    print-identifier ( print-specifier-string print-argument-list(opt) )

         // print-identifier
         auto pos = in.get().pos;

         // Start with a no-op expression.
         auto exp = AST::ExpCreate_Size(0);

         if(print->propBegin)
            exp = CC::ExpCreate_Comma(exp,
               CC::ExpCreate_Call(print->propBegin, {}, scope, pos), pos);

         // (
         if(!in.drop(Core::TOK_ParenO))
            throw Core::ParseExceptExpect(in.peek(), "(", true);

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

            AST::Exp::CRef *argp = args.data();
            std::size_t     argc = args.size();

            if(print->propMore)
            {
               auto paramc = print->propMore->getType()->getBaseType()
                  ->getParameters()->size();

               if(paramc > argc)
                  throw Core::ExceptStr(pos, "insufficient arguments");

               exp = CC::ExpCreate_Comma(exp, CC::ExpCreate_Call(
                  print->propMore, {argp, argp + paramc}, scope, pos), pos);

               argp += paramc;
               argc -= paramc;
            }

            if(print->propOpt)
            {
               auto paramc = print->propOpt->getType()->getBaseType()
                  ->getParameters()->size();

               if(paramc > argc)
                  throw Core::ExceptStr(pos, "insufficient arguments");

               exp = CC::ExpCreate_Comma(exp, CC::ExpCreate_Call(
                  print->propOpt, {argp, argp + paramc}, scope, pos), pos);

               argp += paramc;
               argc -= paramc;
            }

            if(print->propEnd)
            {
               exp = CC::ExpCreate_Comma(exp, CC::ExpCreate_Call(
                 print->propEnd, {argp, argp + argc}, scope, pos), pos);
            }
         }
         else
         {
            if(print->propEnd)
               exp = CC::ExpCreate_Comma(exp,
                  CC::ExpCreate_Call(print->propEnd, {}, scope, pos), pos);
         }

         // )
         if(!in.drop(Core::TOK_ParenC))
            throw Core::ParseExceptExpect(in.peek(), ")", true);

         return exp;
      }
   }
}

// EOF

