//-----------------------------------------------------------------------------
//
// Copyright (C) 2014-2015 David Hill
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
// Extern Functions                                                           |
//

namespace GDCC
{
   namespace CC
   {
      //
      // Parser::isDeclarator
      //
      bool Parser::isDeclarator(Scope &scope)
      {
         switch(in.peek().tok)
         {
         case Core::TOK_Identi: return !isDeclSpec(scope);
         case Core::TOK_Mul:    return true;
         case Core::TOK_BrackO: return true;
         case Core::TOK_ParenO: return true;
         default:               return false;
         }
      }

      //
      // Parser::parseDeclarator
      //
      void Parser::parseDeclarator(Scope &scope, AST::Attribute &attr)
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
            if(in.drop(Core::TOK_Mul))
            {
               attr.type = attr.type->getTypeQual(declQual)->getTypePointer();
               declQual  = AST::QualNone;

               // Check for attributes.
               auto attrType = attr;
               if(isAttrSpec(scope))
                  parseAttrSpecList(scope, attrType);
            }
            else if(isTypeQual(scope))
               parseTypeQual(scope, declQual);
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
         attr.namePos = in.peek().pos;

         // identifier
         if(in.peek().tok == Core::TOK_Identi)
         {
            attr.name = in.get().str;

            if(isAttrSpec(scope))
               parseAttrSpecList(scope, attr);
         }

         // ( declarator )
         else if(in.peek().tok == Core::TOK_ParenO)
         {
            in.get();
            if(isDeclarator(scope))
            {
               // Store the tokens for processing later.
               for(unsigned depth = 1; depth;)
               {
                  auto const &tok = in.get();
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
               in.unget();
            }
         }

         // Parse the remaining direct-declarator syntax.
         parseDeclaratorSuffix(scope, attr);

         // ( declarator )
         if(!toks.empty())
         {
            Core::ArrayTBuf   buf{toks.data(), toks.size()};
            Core::TokenStream str{&buf};

            clone(str)->parseDeclarator(scope, attr);
         }
      }

      //
      // Parser::parseDeclaratorSuffix
      //
      void Parser::parseDeclaratorSuffix(Scope &scope, AST::Attribute &attr)
      {
         // [ type-qualifier-list(opt) assignment-expression(opt) ]
         // [ <static> type-qualifier-list(opt) assignment-expression ]
         // [ type-qualifier-list <static> assignment-expression ]
         // [ type-qualifier-list(opt) * ]
         if(in.drop(Core::TOK_BrackO))
         {
            AST::TypeQual quals  = attr.type->getQualAddr();
            bool          isQual = false;
            bool          isStat = false;

            // Element type must be complete.
            if(!attr.type->isTypeComplete())
               throw Core::ExceptStr(in.reget().pos, "incomplete element type");

            // type-qualifier-list(opt)
            if(isTypeQual(scope))
            {
               isQual = true;
               do parseTypeQual(scope, quals);
               while(isTypeQual(scope));
            }

            // * ]
            if(in.drop(Core::TOK_Mul))
            {
               // ]
               if(!in.drop(Core::TOK_BrackC))
                  throw Core::ParseExceptExpect(in.peek(), "]", true);

               // Check for array attributes.
               auto attrArray = attr;
               if(isAttrSpec(scope))
                  parseAttrSpecList(scope, attrArray);

               // Parse the next declarator suffix before creating new type.
               parseDeclaratorSuffix(scope, attr);

               // Create type.
               attr.type = attr.type->getTypeArray(nullptr)->getTypeQual(quals);
               return;
            }

            // <static>
            if(in.drop(Core::TOK_KeyWrd, Core::STR_static))
            {
               isStat = true;

               // type-qualifier-list(opt)
               if(!isQual && isTypeQual(scope))
               {
                  isQual = true;
                  do parseTypeQual(scope, quals);
                  while(isTypeQual(scope));
               }
            }

            // ]
            if(in.drop(Core::TOK_BrackC))
            {
               if(isStat)
               {
                  throw Core::ParseExceptExpect(in.reget(),
                     "assignment-expression", false);
               }

               // Check for array attributes.
               auto attrArray = attr;
               if(isAttrSpec(scope))
                  parseAttrSpecList(scope, attrArray);

               // Parse the next declarator suffix before creating new type.
               parseDeclaratorSuffix(scope, attr);

               // Create type.
               attr.type = attr.type->getTypeArray();
            }

            // assignment-expression ]
            else
            {
               // assignment-expression
               auto exp = getExp_Assi(scope);

               // ]
               if(!in.drop(Core::TOK_BrackC))
                  throw Core::ParseExceptExpect(in.peek(), "]", true);

               // Check for array attributes.
               auto attrArray = attr;
               if(isAttrSpec(scope))
                  parseAttrSpecList(scope, attrArray);

               // Parse the next declarator suffix before creating new type.
               parseDeclaratorSuffix(scope, attr);

               // Create type.
               if(exp->isIRExp())
                  attr.type = attr.type->getTypeArray(ExpToFastU(exp));
               else
                  attr.type = attr.type->getTypeArray(exp);
            }

            // Set qualifiers.
            attr.type = attr.type->getTypeQual(quals);
         }

         // ( parameter-type-list )
         // ( identifier-list(opt) )
         else if(in.drop(Core::TOK_ParenO))
         {
            Core::Array<AST::Attribute> params;
            AST::TypeSet::CPtr          types;

            if(attr.callt == IR::CallType::None)
               attr.callt = IR::CallType::LangC;

            // )
            if(in.drop(Core::TOK_ParenC))
            {
               attr.funcNoParam = true;
               types = AST::TypeSet::Get(false);
            }

            // parameter-type-list )
            // identifier-list )
            else
            {
               // parameter-type-list
               if(isDeclSpec(scope) || in.peek(Core::TOK_Dot3))
               {
                  std::tie(types, params) = getTypeList(scope);
               }

               // identifier-list
               else
               {
                  if(in.peek().tok != Core::TOK_Identi)
                     throw Core::ParseExceptExpect(in.peek(), "identifier", false);

                  std::vector<AST::Attribute> paramv;

                  do
                  {
                     paramv.emplace_back();
                     paramv.back().name = in.get().str;
                     paramv.back().type = AST::Type::None;
                  }
                  while(in.drop(Core::TOK_Comma));

                  // Generate TypeSet.
                  std::vector<AST::Type::CRef> typev;
                  typev.reserve(paramv.size());
                  for(auto &param : paramv)
                     typev.emplace_back(param.type);

                  types  = AST::TypeSet::Get(typev.data(), typev.size(), false);
                  params = {Core::Move, paramv.begin(), paramv.end()};
               }

               // )
               if(!in.drop(Core::TOK_ParenC))
                  throw Core::ParseExceptExpect(in.peek(), ")", true);
            }

            // Check for function attributes.
            auto attrFunc = attr;
            if(isAttrSpec(scope))
               parseAttrSpecList(scope, attrFunc);

            // Parse the next declarator suffix before creating new type.
            // The declarator "f(void)[5]" is a function returning an array.
            // TODO: Come up with an example of a function declarator suffix
            // followed by another declarator suffix that is legal in C.
            parseDeclaratorSuffix(scope, attr);

            // Create type.
            attr.type = attr.type->getTypeFunction(types, attrFunc.callt);
            attr.param = Core::Array<AST::Attribute>(
               Core::Move, params.begin(), params.end());
         }
      }
   }
}

// EOF

