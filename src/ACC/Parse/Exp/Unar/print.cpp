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
      // GetPrintSpec_Array
      //
      static AST::Exp::CPtr GetPrintSpec_Array(Parser &ctx,
         AST::Exp const *propArray, AST::Exp const *propRange,
         Core::Array<AST::Exp::CRef> &&args, Core::Origin pos)
      {
         AST::Type::CRef type = args[0]->getType()->getBaseType();

         auto space = type->getQualAddr();

         auto indexExp = CC::ExpCreate_Cst(CC::TypeIntegPrS, args[0]);
         auto spaceExp = AST::ExpCreate_IRExp(IR::ExpCreate_Glyph(
            {ctx.prog, space.name}, pos), CC::TypeIntegPrS, pos);

         if(args.size() == 1 && propArray)
            return CC::ExpCreate_Call(propArray, {indexExp, spaceExp}, pos);

         if(args.size() == 2 && propRange)
            return CC::ExpCreate_Call(propRange, {indexExp, spaceExp, args[1],
               CC::ExpCreate_LitInt(CC::TypeIntegPrS, 0x7FFFFFFF, pos)}, pos);

         if(args.size() == 3 && propRange)
            return CC::ExpCreate_Call(propRange, {indexExp, spaceExp, args[1],
               args[2]}, pos);

         return nullptr;
      }

      //
      // GetPrintSpec
      //
      static AST::Exp::CRef GetPrintSpec(Parser &ctx, CC::Scope &scope,
         PrintDecl const *print)
      {
         // print-specifier:
         //    identifier : ( expression-list )
         //    identifier : assignment-expression

         // identifier
         if(!ctx.in.peek(Core::TOK_Identi))
            throw Core::ParseExceptExpect(ctx.in.peek(), "identifier", false);

         auto name = ctx.in.get();

         auto propItr = print->props.find(name.str);
         if(propItr == print->props.end())
            throw Core::ParseExceptExpect(name, "print-specifier", false);

         PrintProp const &prop = propItr->second;

         // :
         if(!ctx.in.drop(Core::TOK_Colon))
            throw Core::ParseExceptExpect(ctx.in.peek(), ":", true);

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
               if(auto exp = GetPrintSpec_Array(ctx, prop.propGlobalArray,
                  prop.propGlobalRange, std::move(args), name.pos))
                  return static_cast<AST::Exp::CRef>(exp);
               break;

            case IR::AddrBase::LocArr:
               if(auto exp = GetPrintSpec_Array(ctx, prop.propLocalArray,
                  prop.propLocalRange, std::move(args), name.pos))
                  return static_cast<AST::Exp::CRef>(exp);
               break;

            case IR::AddrBase::MapArr:
               if(auto exp = GetPrintSpec_Array(ctx, prop.propMapArray,
                  prop.propMapRange, std::move(args), name.pos))
                  return static_cast<AST::Exp::CRef>(exp);
               break;

            case IR::AddrBase::WldArr:
               if(auto exp = GetPrintSpec_Array(ctx, prop.propWorldArray,
                  prop.propWorldRange, std::move(args), name.pos))
                  return static_cast<AST::Exp::CRef>(exp);
               break;

            default:
               break;
            }
         }

         if(prop.prop)
            return CC::ExpCreate_Call(prop.prop, std::move(args), name.pos);

         throw Core::ExceptStr(name.pos, "no valid print property");
      }
   }
}


//----------------------------------------------------------------------------|
// Global Functions                                                           |
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
         // print-identifier ( print-specifier-list(opt)
         //    print-argument-list(opt) )

         // print-identifier
         auto pos = in.get().pos;

         // Start with a no-op expression.
         auto exp = AST::ExpCreate_Size(0);

         if(print->propBegin)
            exp = CC::ExpCreate_Comma(exp,
               CC::ExpCreate_Call(print->propBegin, {}, pos), pos);

         // (
         if(!in.drop(Core::TOK_ParenO))
            throw Core::ParseExceptExpect(in.peek(), "(", true);

         // print-specifier-list:
         //    print-specifier
         //    print-specifier-list , print-specifier
         if(in.peek(Core::TOK_Identi)) do
            exp = CC::ExpCreate_Comma(exp, GetPrintSpec(*this, scope, print), pos);
         while(in.drop(Core::TOK_Comma));

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
                  print->propMore, {argp, argp + paramc}, pos), pos);

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
                  print->propOpt, {argp, argp + paramc}, pos), pos);

               argp += paramc;
               argc -= paramc;
            }

            if(print->propEnd)
            {
               exp = CC::ExpCreate_Comma(exp, CC::ExpCreate_Call(
                 print->propEnd, {argp, argp + argc}, pos), pos);
            }
         }
         else
         {
            if(print->propEnd)
               exp = CC::ExpCreate_Comma(exp,
                  CC::ExpCreate_Call(print->propEnd, {}, pos), pos);
         }

         // )
         if(!in.drop(Core::TOK_ParenC))
            throw Core::ParseExceptExpect(in.peek(), ")", true);

         return exp;
      }
   }
}

// EOF

