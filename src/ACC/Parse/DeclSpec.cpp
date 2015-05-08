//-----------------------------------------------------------------------------
//
// Copyright (C) 2015 David Hill
//
// See COPYING for license information.
//
//-----------------------------------------------------------------------------
//
// ACS declaration specifier parsing.
//
//-----------------------------------------------------------------------------

#include "ACC/Parse.hpp"

#include "AST/Attribute.hpp"
#include "AST/Type.hpp"

#include "Core/Exception.hpp"
#include "Core/TokenStream.hpp"


//----------------------------------------------------------------------------|
// Global Functions                                                           |
//

namespace GDCC
{
   namespace ACC
   {
      //
      // Parser::isDeclSpec
      //
      bool Parser::isDeclSpec(CC::Scope &scope)
      {
         auto tok = in.peek();
         if(tok.tok != Core::TOK_Identi && tok.tok != Core::TOK_KeyWrd)
            return false;

         switch(tok.str)
         {
            // storage-class-specifier
         case Core::STR_global: return true;
         case Core::STR_static: return true;
         case Core::STR_world:  return true;

            // type-qualifier
            // type-specifier
         default:
            return isTypeQual(scope) || isTypeSpec(scope);
         }
      }

      //
      // Parser::parseDeclSpec
      //
      void Parser::parseDeclSpec(CC::Scope &scope, AST::Attribute &attr)
      {
         enum DeclStor
         {
            declStorNone,

            declStorGlob,
            declStorStat,
            declStorWorl,
         };

         auto pos = in.peek().pos;

         AST::TypeQual declQual = AST::QualNone;
         CC::TypeSpec  declSpec;
         DeclStor      declStor = declStorNone;

         //
         // setDeclStor
         //
         auto setDeclStor = [&](DeclStor stor)
         {
            if(declStor)
               throw Core::ExceptStr(pos, "multiple storage class specifier");

            declStor = stor;
         };

         // Read declaration-specifier tokens until there are no more.
         for(;;)
         {
            auto const &tok = in.peek();
            if(tok.tok != Core::TOK_Identi && tok.tok != Core::TOK_KeyWrd)
               break;

            switch(tok.str)
            {
               // storage-class-specifier
            case Core::STR_global: setDeclStor(declStorGlob); break;
            case Core::STR_static: setDeclStor(declStorStat); break;
            case Core::STR_world:  setDeclStor(declStorWorl); break;

            default:
               // type-specifier
               if(isTypeSpec(scope))
                  {parseTypeSpec(scope, attr, declSpec); continue;}

               // type-qualifier
               if(isTypeQual(scope))
                  {parseTypeQual(scope, declQual); continue;}

               goto parse_done;
            }

            in.get();
         }

         parse_done:;

         // Finalize the type specifiers.
         declSpec.finish(attr, declQual, pos);

         // Set attribute storage class.
         switch(declStor)
         {
         case declStorNone:                       break;
         case declStorGlob: attr.storeGbl = true; break;
         case declStorStat: attr.storeInt = true; break;
         case declStorWorl: attr.storeWld = true; break;
         }
      }
   }
}

// EOF

