//-----------------------------------------------------------------------------
//
// Copyright (C) 2014 David Hill
//
// See COPYING for license information.
//
//-----------------------------------------------------------------------------
//
// C declarator parsing.
//
//-----------------------------------------------------------------------------

#include "CC/Parse.hpp"

#include "CC/Exp.hpp"

#include "AST/Attribute.hpp"
#include "AST/Exp.hpp"
#include "AST/Type.hpp"

#include "Core/Exception.hpp"
#include "Core/TokenStream.hpp"

#include "IR/CallType.hpp"

#include <vector>


//----------------------------------------------------------------------------|
// Global Functions                                                           |
//

namespace GDCC
{
   namespace CC
   {
      //
      // IsDeclarator
      //
      bool IsDeclarator(ParserCtx const &ctx, Scope &scope)
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
      void ParseDeclarator(ParserCtx const &ctx, Scope &scope, AST::Attribute &attr)
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
               declQual = AST::QualNone;
            }
            else if(IsTypeQual(ctx, scope))
               ParseTypeQual(ctx, scope, declQual);
            else
               break;
         }
         attr.type = attr.type->getTypeQual(declQual);

         // direct-declarator:
         //    identifier
         //    ( declarator )
         //    direct-declarator [ type-qualifier-list(opt)
         //       assignment-expression(opt) ]
         //    direct-declarator [ <static> type-qualifier-list(opt)
         //       assignment-expression ]
         //    direct-declarator [ type-qualifier-list <static>
         //       assignment-expression ]
         //    direct-declarator [ type-qualifier-list(opt) * ]
         //    direct-declarator ( parameter-type-list )
         //    direct-declarator ( identifier-list(opt) )

         // Take the next token as the name position, in case this is an abstract
         // declarator.
         attr.namePos = ctx.in.peek().pos;

         // identifier
         if(ctx.in.peek().tok == Core::TOK_Identi)
            attr.name = ctx.in.get().str;

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

            ParseDeclarator({ctx, str}, scope, attr);
         }
      }

      //
      // ParseDeclaratorSuffix
      //
      void ParseDeclaratorSuffix(ParserCtx const &ctx, Scope &scope,
         AST::Attribute &attr)
      {
         // [ type-qualifier-list(opt) assignment-expression(opt) ]
         // [ <static> type-qualifier-list(opt) assignment-expression ]
         // [ type-qualifier-list <static> assignment-expression ]
         // [ type-qualifier-list(opt) * ]
         if(ctx.in.drop(Core::TOK_BrackO))
         {
            AST::TypeQual quals  = attr.type->getQualAddr();
            bool          isQual = false;
            bool          isStat = false;

            // type-qualifier-list(opt)
            if(IsTypeQual(ctx, scope))
            {
               isQual = true;
               do ParseTypeQual(ctx, scope, quals);
               while(IsTypeQual(ctx, scope));
            }

            // * ]
            if(ctx.in.drop(Core::TOK_Mul))
            {
               // ]
               if(!ctx.in.drop(Core::TOK_BrackC))
                  throw Core::ExceptStr(ctx.in.peek().pos, "expected ']'");

               // Parse the next declarator suffix before creating new type.
               ParseDeclaratorSuffix(ctx, scope, attr);

               attr.type = attr.type->getTypeArray(nullptr)->getTypeQual(quals);
               return;
            }

            // <static>
            if(ctx.in.drop(Core::TOK_KeyWrd, Core::STR_static))
            {
               isStat = true;

               // type-qualifier-list(opt)
               if(!isQual && IsTypeQual(ctx, scope))
               {
                  isQual = true;
                  do ParseTypeQual(ctx, scope, quals);
                  while(IsTypeQual(ctx, scope));
               }
            }

            // ]
            if(ctx.in.drop(Core::TOK_BrackC))
            {
               if(isStat)
               {
                  throw Core::ExceptStr(ctx.in.reget().pos,
                     "expected assignment-expression");
               }

               // Parse the next declarator suffix before creating new type.
               ParseDeclaratorSuffix(ctx, scope, attr);

               attr.type = attr.type->getTypeArray();
            }

            // assignment-expression ]
            else
            {
               // assignment-expression
               auto exp = GetExp_Assi(ctx, scope);

               // ]
               if(!ctx.in.drop(Core::TOK_BrackC))
                  throw Core::ExceptStr(ctx.in.peek().pos, "expected ']'");

               // Parse the next declarator suffix before creating new type.
               ParseDeclaratorSuffix(ctx, scope, attr);

               if(exp->isIRExp())
                  attr.type = attr.type->getTypeArray(ExpToFastU(exp));
               else
                  attr.type = attr.type->getTypeArray(exp);
            }

            attr.type = attr.type->getTypeQual(quals);
         }

         // ( parameter-type-list )
         // ( identifier-list(opt) )
         else if(ctx.in.drop(Core::TOK_ParenO))
         {
            std::vector<AST::Attribute> params;
            AST::TypeSet::CPtr types;

            attr.callt = IR::CallType::LangC;

            // )
            if(ctx.in.drop(Core::TOK_ParenC))
            {
               attr.funcNoParam = true;
               types = AST::TypeSet::Get(false);
            }

            // parameter-type-list )
            // identifier-list )
            else
            {
               bool varia = false;

               // Special case for (void).
               if(ctx.in.drop(Core::TOK_KeyWrd, Core::STR_void))
               {
                  // Just drop the token and move on.
               }

               // parameter-type-list
               else if(IsDeclSpec(ctx, scope)) do
               {
                  // ... )
                  if(ctx.in.drop(Core::TOK_Dot3))
                     {varia = true; break;}

                  params.emplace_back();
                  auto &param = params.back();

                  // declaration-specifiers
                  ParseDeclSpec(ctx, scope, param);

                  // Disallow extern, static, or typedef.
                  if(param.isTypedef || param.storeExt || param.storeInt)
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

               // identifier-list
               else do
               {
                  if(ctx.in.peek().tok != Core::TOK_Identi)
                     throw Core::ExceptStr(ctx.in.peek().pos, "expected identifier");

                  params.emplace_back();
                  params.back().name = ctx.in.get().str;
                  params.back().type = AST::Type::None;
               }
               while(ctx.in.drop(Core::TOK_Comma));

               // )
               if(!ctx.in.drop(Core::TOK_ParenC))
                  throw Core::ExceptStr(ctx.in.peek().pos, "expected ')'");

               // Generate TypeSet.
               std::vector<AST::Type::CRef> typev;
               typev.reserve(params.size());
               for(auto &param : params)
                  typev.emplace_back(param.type);

               types = AST::TypeSet::Get(typev.data(), typev.size(), varia);
            }

            // Check for function attributes.
            while(IsAttrSpec(ctx, scope))
               ParseAttrSpec(ctx, scope, attr);

            // Parse the next declarator suffix before creating new type.
            // The declarator "f(void)[5]" is a function returning an array.
            // TODO: Come up with an example of a function declarator suffix
            // followed by another declarator suffix that is legal in C.
            auto attrTmp = attr;
            ParseDeclaratorSuffix(ctx, scope, attrTmp);

            // Create type.
            attr.type = attrTmp.type->getTypeFunction(types, attr.callt);
            attr.param = Core::Array<AST::Attribute>(
               Core::Move, params.begin(), params.end());
         }
      }
   }
}

// EOF

