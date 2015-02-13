//-----------------------------------------------------------------------------
//
// Copyright (C) 2015 David Hill
//
// See COPYING for license information.
//
//-----------------------------------------------------------------------------
//
// ACS declarator parsing.
//
//-----------------------------------------------------------------------------

#include "ACC/Parse.hpp"

#include "AST/Attribute.hpp"
#include "AST/Exp.hpp"
#include "AST/Type.hpp"

#include "CC/Exp.hpp"

#include "Core/Exception.hpp"
#include "Core/TokenStream.hpp"

#include "IR/CallType.hpp"
#include "IR/Exp.hpp"

#include <vector>


//----------------------------------------------------------------------------|
// Global Functions                                                           |
//

namespace GDCC
{
   namespace ACC
   {
      //
      // IsDeclarator
      //
      bool IsDeclarator(ParserCtx const &ctx, CC::Scope &scope)
      {
         switch(ctx.in.peek().tok)
         {
         case Core::TOK_Identi: return !IsDeclSpec(ctx, scope);
         case Core::TOK_Mul:    return true;
         case Core::TOK_BrackO: return true;
         case Core::TOK_ParenO: return true;
         default:               return false;
         }
      }

      //
      // ParseDeclarator
      //
      void ParseDeclarator(ParserCtx const &ctx, CC::Scope &scope,
         AST::Attribute &attr)
      {
         std::vector<Core::Token> toks;
         auto declQual = attr.type->getQual();

         // declarator:
         //    pointer(opt) direct-declarator

         // pointer:
         //    * type-qualifier-list(opt)
         //    * type-qualifier-list(opt) pointer
         for(;;)
         {
            if(ctx.in.drop(Core::TOK_Mul))
            {
               attr.type = attr.type->getTypeQual(declQual)->getTypePointer();
               declQual  = AST::QualNone;
            }
            else if(IsTypeQual(ctx, scope))
               ParseTypeQual(ctx, scope, declQual);
            else
               break;
         }
         attr.type = attr.type->getTypeQual(declQual);

         // direct-declarator:
         //    object-address(opt) identifier
         //    ( declarator )
         //    direct-declarator [ expression(opt) ]
         //    direct-declarator ( parameter-type-list )

         // Take the next token as the name position, in case this is an
         // abstract declarator.
         attr.namePos = ctx.in.peek().pos;

         // object-address identifier
         if(ctx.in.peek(Core::TOK_NumInt))
         {
            attr.addrI = GetExp_Prim(ctx, scope)->getIRExp();

            if(!ctx.in.drop(Core::TOK_Colon))
               throw Core::ParseExceptExpect(ctx.in.peek(), ":", true);

            if(!ctx.in.peek(Core::TOK_Identi))
               throw Core::ParseExceptExpect(ctx.in.peek(), "identifier", false);

            attr.setName(ctx.in.get());
         }

         // identifier
         else if(ctx.in.peek().tok == Core::TOK_Identi)
         {
            attr.setName(ctx.in.get());
         }

         // ( declarator )
         else if(ctx.in.peek().tok == Core::TOK_ParenO)
         {
            ctx.in.get();
            if(IsDeclarator(ctx, scope))
            {
               // Store the tokens for processing later.
               for(unsigned depth = 1; depth;)
               {
                  auto const &tok = ctx.in.get();
                  toks.push_back(tok);

                       if(tok.tok == Core::TOK_ParenO) ++depth;
                  else if(tok.tok == Core::TOK_ParenC) --depth;
                  else if(tok.tok == Core::TOK_EOF)
                     throw Core::ExceptStr(tok.pos, "unexpected end of file");
               }
            }
            else
            {
               // Just kidding, this is actually a function abstract-declarator.
               ctx.in.unget();
            }
         }

         // Parse the remaining direct-declarator syntax.
         ParseDeclaratorSuffix(ctx, scope, attr);

         // ( declarator )
         if(!toks.empty())
         {
            Core::ArrayTBuf   buf{toks.data(), toks.size()};
            Core::TokenStream str{&buf};

            ParseDeclarator(ParserCtx(ctx, str), scope, attr);
         }
      }

      //
      // ParseDeclaratorSuffix
      //
      void ParseDeclaratorSuffix(ParserCtx const &ctx, CC::Scope &scope,
         AST::Attribute &attr)
      {
         // [ expression(opt) ]
         if(ctx.in.drop(Core::TOK_BrackO))
         {
            AST::TypeQual quals  = attr.type->getQualAddr();

            // ]
            if(ctx.in.drop(Core::TOK_BrackC))
            {
               // Parse the next declarator suffix before creating new type.
               ParseDeclaratorSuffix(ctx, scope, attr);

               // Create type.
               attr.type = attr.type->getTypeArray();
            }

            // expression ]
            else
            {
               // expression
               auto exp = GetExp(ctx, scope);

               // ]
               if(!ctx.in.drop(Core::TOK_BrackC))
                  throw Core::ParseExceptExpect(ctx.in.peek(), "]", true);

               // Parse the next declarator suffix before creating new type.
               ParseDeclaratorSuffix(ctx, scope, attr);

               // Create type.
               attr.type = attr.type->getTypeArray(CC::ExpToFastU(exp));
            }

            // Set qualifiers.
            attr.type = attr.type->getTypeQual(quals);
         }

         // ( parameter-type-list )
         else if(ctx.in.drop(Core::TOK_ParenO))
         {
            Core::Array<AST::Attribute> params;
            AST::TypeSet::CPtr types;

            if(attr.callt == IR::CallType::None)
               attr.callt = IR::CallType::LangACS;

            // )
            if(ctx.in.drop(Core::TOK_ParenC))
            {
               attr.funcNoParam = true;
               types = AST::TypeSet::Get(false);
            }

            // parameter-type-list )
            else
            {
               // paramter-type-list
               std::tie(types, params) = GetTypeList(ctx, scope);

               // )
               if(!ctx.in.drop(Core::TOK_ParenC))
                  throw Core::ParseExceptExpect(ctx.in.peek(), ")", true);
            }

            auto attrFunc = attr;

            // Parse the next declarator suffix before creating new type.
            // The declarator "f(void)[5]" is a function returning an array.
            // TODO: Come up with an example of a function declarator suffix
            // followed by another declarator suffix that is legal in C.
            ParseDeclaratorSuffix(ctx, scope, attr);

            // Create type.
            attr.type = attr.type->getTypeFunction(types, attrFunc.callt);
            attr.param = std::move(params);
         }
      }

      std::pair<AST::TypeSet::CRef, Core::Array<AST::Attribute>>
      GetTypeList(ParserCtx const &ctx, CC::Scope &scope)
      {
         std::vector<AST::Attribute> params;

         bool varia = false;

         // Special case for (void).
         if(ctx.in.peek(Core::TOK_KeyWrd, Core::STR_void, Core::TOK_ParenC))
         {
            // Just drop the token and move on.
            ctx.in.get();
         }

         // parameter-type-list
         else do
         {
            // ... )
            if(ctx.in.drop(Core::TOK_Dot3))
               {varia = true; break;}

            params.emplace_back();
            auto &param = params.back();

            // declaration-specifiers
            ParseDeclSpec(ctx, scope, param);

            // Disallow extern, static, or typedef.
            if(param.storeGbl || param.storeWld || param.storeInt)
               throw Core::ExceptStr(ctx.in.reget().pos,
                  "bad parameter storage class");

            // declarator
            // abstract-declarator(opt)
            if(IsDeclarator(ctx, scope))
               ParseDeclarator(ctx, scope, param);

            // Change function to pointer-to-function.
            if(param.type->isCTypeFunction())
               param.type = param.type->getTypePointer();

            // Change array-of-T to pointer-to-T.
            else if(param.type->isTypeArray())
            {
               param.type = param.type->getBaseType()
                  ->getTypePointer()
                  ->getTypeQual(param.type->getQual());
            }
         }
         while(ctx.in.drop(Core::TOK_Comma));

         // Generate TypeSet.
         std::vector<AST::Type::CRef> typev;
         typev.reserve(params.size());
         for(auto &param : params)
            typev.emplace_back(param.type);

         auto types = AST::TypeSet::Get(typev.data(), typev.size(), varia);

         return {types, {Core::Move, params.begin(), params.end()}};
      }
   }
}

// EOF

