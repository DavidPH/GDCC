//-----------------------------------------------------------------------------
//
// Copyright (C) 2014-2018 David Hill
//
// See COPYING for license information.
//
//-----------------------------------------------------------------------------
//
// C parsing.
//
//-----------------------------------------------------------------------------

#include "CC/Parse.hpp"

#include "Core/Exception.hpp"
#include "Core/TokenStream.hpp"

#include "SR/Attribute.hpp"
#include "SR/Type.hpp"

#include <vector>


//----------------------------------------------------------------------------|
// Extern Functions                                                           |
//

namespace GDCC::CC
{
   //
   // Parser constructor
   //
   Parser::Parser(Core::TokenStream &in_, CPP::PragmaData &prag_,
      IR::Program &prog_) :
      in  (in_),
      prag(prag_),
      prog(prog_)
   {
   }

   Parser::Parser(Parser const &ctx, Core::TokenStream &in_) :
      in  (in_),
      prag(ctx.prag),
      prog(ctx.prog)
   {
   }

   //
   // Parser::getType
   //
   // type-name:
   //    specifier-qualifier-list abstract-declarator(opt)
   //
   SR::Type::CRef Parser::getType(Scope &scope)
   {
      SR::Attribute attr;

      parseSpecQual(scope, attr);
      if(isDeclarator(scope))
         parseDeclarator(scope, attr);

      return static_cast<SR::Type::CRef>(attr.type);
   }

   //
   // Parser::isType
   //
   bool Parser::isType(Scope &scope)
   {
      return isSpecQual(scope);
   }

   //
   // Parser::skipBalancedToken
   //
   void Parser::skipBalancedToken()
   {
      Core::TokenType tt;
      switch(in.get().tok)
      {
      case Core::TOK_BraceO: tt = Core::TOK_BraceC; break;
      case Core::TOK_BrackO: tt = Core::TOK_BrackC; break;
      case Core::TOK_ParenO: tt = Core::TOK_ParenC; break;
      default: return;
      }

      while(!in.drop(tt) && !in.drop(Core::TOK_EOF))
         skipBalancedToken();
   }

   //
   // Parser::getTypeList
   //
   std::pair<SR::TypeSet::CRef, Core::Array<SR::Attribute>>
   Parser::getTypeList(Scope &scope)
   {
      std::vector<SR::Attribute> params;

      bool varia = false;

      // Special case for (void).
      if(in.peek(Core::TOK_KeyWrd, Core::STR_void, Core::TOK_ParenC))
      {
         // Just drop the token and move on.
         in.get();
      }

      // parameter-type-list
      else do
      {
         // ... )
         if(in.drop(Core::TOK_Dot3))
            {varia = true; break;}

         params.emplace_back();
         auto &param = params.back();

         // declaration-specifiers
         parseDeclSpec(scope, param);

         // Disallow extern, static, or typedef. Also ACS global or world.
         if(param.isTypedef || param.storeExt || param.storeInt ||
            param.storeGbl || param.storeHub)
            throw Core::ExceptStr(in.reget().pos,
               "bad parameter storage class");

         // declarator
         // abstract-declarator(opt)
         if(isDeclarator(scope))
            parseDeclarator(scope, param);

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
      while(in.drop(Core::TOK_Comma));

      // Generate TypeSet.
      std::vector<SR::Type::CRef> typev;
      typev.reserve(params.size());
      for(auto &param : params)
         typev.emplace_back(param.type);

      auto types = SR::TypeSet::Get(typev.data(), typev.size(), varia);

      return {types, {Core::Move, params.begin(), params.end()}};
   }
}

// EOF

