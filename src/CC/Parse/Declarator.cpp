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

#include "IR/Function.hpp"

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
      bool IsDeclarator(ParserData &in, Scope *ctx)
      {
         switch(in.in.peek().tok)
         {
         case Core::TOK_Identi: return !IsDeclSpec(in, ctx);
         case Core::TOK_Mul:    return true;
         case Core::TOK_BrackO: return true;
         case Core::TOK_ParenO: return true;
         default:               return false;
         }
      }

      //
      // ParseDeclarator
      //
      void ParseDeclarator(ParserData &in, Scope *ctx, AST::Attribute &attr)
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
            if(in.in.drop(Core::TOK_Mul))
            {
               attr.type = attr.type->getTypeQual(declQual)->getTypePointer();
               declQual = AST::QualNone;
            }
            else if(IsTypeQual(in, ctx))
               ParseTypeQual(in, ctx, declQual);
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

         // identifier
         if(in.in.peek().tok == Core::TOK_Identi)
            attr.name = in.in.get().str;

         // ( declarator )
         else if(in.in.peek().tok == Core::TOK_ParenO)
         {
            in.in.get();
            if(IsDeclarator(in, ctx))
            {
               // Store the tokens for processing later.
               for(unsigned depth = 1; depth;)
               {
                  auto const &tok = in.in.get();
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
               in.in.unget();
            }
         }

         // Parse the remaining direct-declarator syntax.
         ParseDeclaratorSuffix(in, ctx, attr);

         // ( declarator )
         if(!toks.empty())
         {
            Core::ArrayTBuf buf{toks.data(), toks.size()};
            Core::TokenStream str{&buf};
            ParserData tmp{str, in.prag};

            ParseDeclarator(tmp, ctx, attr);
         }
      }

      //
      // ParseDeclaratorSuffix
      //
      void ParseDeclaratorSuffix(ParserData &in, Scope *ctx, AST::Attribute &attr)
      {
         // [ type-qualifier-list(opt) assignment-expression(opt) ]
         // [ <static> type-qualifier-list(opt) assignment-expression ]
         // [ type-qualifier-list <static> assignment-expression ]
         // [ type-qualifier-list(opt) * ]
         if(in.in.drop(Core::TOK_BrackO))
         {
            AST::TypeQual quals  = AST::QualNone;
            bool          isQual = false;
            bool          isStat = false;

            // type-qualifier-list(opt)
            if(IsTypeQual(in, ctx))
            {
               isQual = true;
               do ParseTypeQual(in, ctx, quals);
               while(IsTypeQual(in, ctx));
            }

            // * ]
            if(in.in.drop(Core::TOK_Mul))
            {
               // ]
               if(!in.in.drop(Core::TOK_BrackC))
                  throw Core::ExceptStr(in.in.peek().pos, "expected ']'");

               // Parse the next declarator suffix before creating new type.
               ParseDeclaratorSuffix(in, ctx, attr);

               attr.type = attr.type->getTypeArray(nullptr)->getTypeQual(quals);
               return;
            }

            // <static>
            if(in.in.drop(Core::TOK_KeyWrd, Core::STR_static))
            {
               isStat = true;

               // type-qualifier-list(opt)
               if(!isQual && IsTypeQual(in, ctx))
               {
                  isQual = true;
                  do ParseTypeQual(in, ctx, quals);
                  while(IsTypeQual(in, ctx));
               }
            }

            // ]
            if(in.in.drop(Core::TOK_BrackC))
            {
               if(isStat)
               {
                  throw Core::ExceptStr(in.in.reget().pos,
                     "expected assignment-expression");
               }

               // Parse the next declarator suffix before creating new type.
               ParseDeclaratorSuffix(in, ctx, attr);

               attr.type = attr.type->getTypeArray();
            }

            // assignment-expression ]
            else
            {
               // assignment-expression
               auto exp = GetExp_Assi(in, ctx);

               // ]
               if(!in.in.drop(Core::TOK_BrackC))
                  throw Core::ExceptStr(in.in.peek().pos, "expected ']'");

               // Parse the next declarator suffix before creating new type.
               ParseDeclaratorSuffix(in, ctx, attr);

               if(exp->isIRExp())
                  attr.type = attr.type->getTypeArray(ExpToFastU(exp));
               else
                  attr.type = attr.type->getTypeArray(exp);
            }

            attr.type = attr.type->getTypeQual(quals);
         }

         // ( parameter-type-list )
         // ( identifier-list(opt) )
         else if(in.in.drop(Core::TOK_ParenO))
         {
            std::vector<AST::Attribute> params;
            AST::TypeSet::CPtr types;

            attr.callt = IR::CallType::LangC;

            // )
            if(in.in.drop(Core::TOK_ParenC))
            {
               attr.funcNoParam = true;
               types = AST::TypeSet::Get(false);
            }

            // parameter-type-list )
            // identifier-list )
            else
            {
               bool varia = false;

               // parameter-type-list
               if(IsDeclSpec(in, ctx)) do
               {
                  // ... )
                  if(in.in.drop(Core::TOK_Dot3))
                     {varia = true; break;}

                  params.emplace_back();
                  auto &param = params.back();

                  // declaration-specifiers
                  ParseDeclSpec(in, ctx, param);

                  // declarator
                  // abstract-declarator(opt)
                  if(IsDeclarator(in, ctx))
                     ParseDeclarator(in, ctx, param);

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

                  // void )
                  else if(param.type->isTypeVoid())
                  {
                     if(params.size() > 1 || param.name)
                        throw Core::ExceptStr(in.in.reget().pos, "void parameter");

                     params.pop_back();
                     break;
                  }
               }
               while(in.in.drop(Core::TOK_Comma));

               // identifier-list
               else do
               {
                  if(in.in.peek().tok != Core::TOK_Identi)
                     throw Core::ExceptStr(in.in.peek().pos, "expected identifier");

                  params.emplace_back();
                  params.back().name = in.in.get().str;
               }
               while(in.in.drop(Core::TOK_Comma));

               // )
               if(!in.in.drop(Core::TOK_ParenC))
                  throw Core::ExceptStr(in.in.peek().pos, "expected ')'");

               // Generate TypeSet.
               std::vector<AST::Type::CRef> typev;
               typev.reserve(params.size());
               for(auto &param : params)
                  typev.emplace_back(param.type);

               types = AST::TypeSet::Get(typev.data(), typev.size(), varia);
            }

            // Check for function attributes.
            while(IsAttrSpec(in, ctx))
               ParseAttrSpec(in, ctx, attr);

            // Parse the next declarator suffix before creating new type.
            // The declarator "f(void)[5]" is a function returning an array.
            // TODO: Come up with an example of a function declarator suffix
            // followed by another declarator suffix that is legal in C.
            auto attrTmp = attr;
            ParseDeclaratorSuffix(in, ctx, attrTmp);

            // Create type.
            attr.type = attrTmp.type->getTypeFunction(types, attr.callt);
            attr.param = Core::Array<AST::Attribute>(
               Core::Move, params.begin(), params.end());
         }
      }
   }
}

// EOF

