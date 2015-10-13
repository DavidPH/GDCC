//-----------------------------------------------------------------------------
//
// Copyright (C) 2013-2015 David Hill
//
// See COPYING for license information.
//
//-----------------------------------------------------------------------------
//
// C type qualifier parsing.
//
//-----------------------------------------------------------------------------

#include "CC/Parse.hpp"

#include "CC/Scope.hpp"

#include "AST/Space.hpp"
#include "AST/Type.hpp"
#include "AST/Warning.hpp"

#include "Core/Exception.hpp"
#include "Core/TokenStream.hpp"

#include <iostream>


//----------------------------------------------------------------------------|
// Static Functions                                                           |
//

namespace GDCC
{
   namespace CC
   {
      //
      // IsTypeQual_Atomic
      //
      static bool IsTypeQual_Atomic(Parser &ctx, Scope &)
      {
         // If followed by a parenthesis, it is a type-specifier.
         ctx.in.get();
         bool res = ctx.in.peek().tok != Core::TOK_ParenO;
         ctx.in.unget();
         return res;
      }

      //
      // ParseTypeQual_Atomic
      //
      static void ParseTypeQual_Atomic(Parser &ctx, Scope &, AST::TypeQual &qual)
      {
         // If followed by a parenthesis, it is a type-specifier.
         if(ctx.in.peek().tok == Core::TOK_ParenO)
            throw Core::ParseExceptExpect(ctx.in.reget(), "type-qualifier", false);

         qual.aAtom = true;
      }
   }
}


//----------------------------------------------------------------------------|
// Extern Functions                                                           |
//

namespace GDCC
{
   namespace CC
   {
      //
      // Parser::isTypeQual
      //
      bool Parser::isTypeQual(Scope &scope)
      {
         auto const &tok = in.peek();
         if(tok.tok != Core::TOK_Identi && tok.tok != Core::TOK_KeyWrd)
            return false;

         switch(tok.str)
         {
            // Standard C qualifiers.
         case Core::STR__Atomic:  return IsTypeQual_Atomic(*this, scope);
         case Core::STR_const:    return true;
         case Core::STR_restrict: return true;
         case Core::STR_volatile: return true;

            // Builtin address space names.
         case Core::STR___adr_cpy: return true;
         case Core::STR___aut:     return true;
         case Core::STR___far:     return true;
         case Core::STR___gbl_ars: return true;
         case Core::STR___gbl_reg: return true;
         case Core::STR___hub_ars: return true;
         case Core::STR___hub_reg: return true;
         case Core::STR___loc_reg: return true;
         case Core::STR___map_ars: return true;
         case Core::STR___map_reg: return true;
         case Core::STR___mod_ars: return true;
         case Core::STR___mod_reg: return true;
         case Core::STR___sta:     return true;
         case Core::STR___str_ars: return true;
         case Core::STR___va_addr: return true;
         case Core::STR___wld_ars: return true;
         case Core::STR___wld_reg: return true;

            // Try a scope lookup for a user-defined address space.
         default:
            return scope.lookup(tok.str).res == Lookup::Space;
         }
      }

      //
      // Parser::parseTypeQual
      //
      void Parser::parseTypeQual(Scope &scope, AST::TypeQual &qual)
      {
         auto const &tok = in.get();
         if(tok.tok != Core::TOK_Identi && tok.tok != Core::TOK_KeyWrd)
            throw Core::ParseExceptExpect(tok, "identifier", false);

         //
         // setSpace
         //
         auto setSpace = [&](IR::AddrBase space)
         {
            if(qual.space.base != IR::AddrBase::Gen)
               throw Core::ExceptStr(tok.pos, "multiple address-space-specifier");

            qual.space = space;
         };

         switch(tok.str)
         {
            // Standard C qualifiers.
         case Core::STR__Atomic:  ParseTypeQual_Atomic(*this, scope, qual); break;
         case Core::STR_const:    qual.aCons = true; break;
         case Core::STR_restrict: qual.aRest = true; break;
         case Core::STR_volatile: qual.aVola = true; break;

            // Builtin address space names.
         case Core::STR___adr_cpy: setSpace(IR::AddrBase::Cpy);    break;
         case Core::STR___aut:     setSpace(IR::AddrBase::Aut);    break;
         case Core::STR___far:     setSpace(IR::AddrBase::Far);    break;
         case Core::STR___gbl_ars: setSpace(IR::AddrBase::GblArs); break;
         case Core::STR___gbl_reg: setSpace(IR::AddrBase::GblReg); break;
         case Core::STR___wld_ars:
            AST::WarnDeprecated(tok.pos,
               "__wld_ars is deprecrated, use __hub_ars instead");
         case Core::STR___hub_ars: setSpace(IR::AddrBase::HubArs); break;
         case Core::STR___wld_reg:
            AST::WarnDeprecated(tok.pos,
               "__wld_reg is deprecrated, use __hub_reg instead");
         case Core::STR___hub_reg: setSpace(IR::AddrBase::HubReg); break;
         case Core::STR___loc_reg: setSpace(IR::AddrBase::LocReg); break;
         case Core::STR___map_ars:
            AST::WarnDeprecated(tok.pos,
               "__map_ars is deprecrated, use __mod_ars instead");
         case Core::STR___mod_ars: setSpace(IR::AddrBase::ModArs); break;
         case Core::STR___map_reg:
            AST::WarnDeprecated(tok.pos,
               "__map_reg is deprecrated, use __mod_reg instead");
         case Core::STR___mod_reg: setSpace(IR::AddrBase::ModReg); break;
         case Core::STR___sta:     setSpace(IR::AddrBase::Sta);    break;
         case Core::STR___str_ars: setSpace(IR::AddrBase::StrArs); break;
         case Core::STR___va_addr: setSpace(IR::AddrBase::Vaa);    break;

            // Try a scope lookup for a user-defined address space.
         default:
            auto lookup = scope.lookup(tok.str);
            if(lookup.res != Lookup::Space)
               throw Core::ParseExceptExpect(tok, "type-qualifier", false);

            if(qual.space.base != IR::AddrBase::Gen)
               throw Core::ExceptStr(tok.pos, "multiple address-space-specifier");

            qual.space.base = lookup.resSpace->space;
            qual.space.name = lookup.resSpace->glyph;
            break;
         }
      }
   }
}

// EOF

