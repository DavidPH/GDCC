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
      // Parser::isDeclarator
      //
      bool Parser::isDeclarator(CC::Scope &scope)
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
      void Parser::parseDeclarator(CC::Scope &scope, AST::Attribute &attr)
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
            }
            else if(isTypeQual(scope))
               parseTypeQual(scope, declQual);
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
         attr.namePos = in.peek().pos;

         // object-address identifier
         if(in.peek(Core::TOK_NumInt))
         {
            attr.addrI = getExp_Prim(scope)->getIRExp();

            if(!in.drop(Core::TOK_Colon))
               throw Core::ParseExceptExpect(in.peek(), ":", true);

            if(!in.peek(Core::TOK_Identi))
               throw Core::ParseExceptExpect(in.peek(), "identifier", false);

            attr.setName(in.get());
         }

         // identifier
         else if(in.peek().tok == Core::TOK_Identi)
         {
            attr.setName(in.get());
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
      void Parser::parseDeclaratorSuffix(CC::Scope &scope, AST::Attribute &attr)
      {
         // [ expression(opt) ]
         if(in.drop(Core::TOK_BrackO))
         {
            AST::TypeQual quals  = attr.type->getQualAddr();

            // ]
            if(in.drop(Core::TOK_BrackC))
            {
               // Parse the next declarator suffix before creating new type.
               parseDeclaratorSuffix(scope, attr);

               // Create type.
               attr.type = attr.type->getTypeArray();
            }

            // expression ]
            else
            {
               // expression
               auto exp = getExp(scope);

               // ]
               if(!in.drop(Core::TOK_BrackC))
                  throw Core::ParseExceptExpect(in.peek(), "]", true);

               // Parse the next declarator suffix before creating new type.
               parseDeclaratorSuffix(scope, attr);

               // Create type.
               attr.type = attr.type->getTypeArray(CC::ExpToFastU(exp));
            }

            // Set qualifiers.
            attr.type = attr.type->getTypeQual(quals);
         }

         // ( parameter-type-list )
         else if(in.drop(Core::TOK_ParenO))
         {
            Core::Array<AST::Attribute> params;
            AST::TypeSet::CPtr          types;

            if(attr.callt == IR::CallType::None)
               attr.callt = IR::CallType::LangACS;

            // )
            if(in.drop(Core::TOK_ParenC))
            {
               attr.funcNoParam = true;
               types = AST::TypeSet::Get(false);
            }

            // parameter-type-list )
            else
            {
               // paramter-type-list
               std::tie(types, params) = getTypeList(scope);

               // )
               if(!in.drop(Core::TOK_ParenC))
                  throw Core::ParseExceptExpect(in.peek(), ")", true);
            }

            auto attrFunc = attr;

            // Parse the next declarator suffix before creating new type.
            // The declarator "f(void)[5]" is a function returning an array.
            // TODO: Come up with an example of a function declarator suffix
            // followed by another declarator suffix that is legal in C.
            parseDeclaratorSuffix(scope, attr);

            // Create type.
            attr.type = attr.type->getTypeFunction(types, attrFunc.callt);
            attr.param = std::move(params);
         }
      }
   }
}

// EOF

