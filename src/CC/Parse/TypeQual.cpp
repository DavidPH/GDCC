//-----------------------------------------------------------------------------
//
// Copyright (C) 2013-2018 David Hill
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

#include "Core/Exception.hpp"
#include "Core/TokenStream.hpp"

#include "SR/Space.hpp"
#include "SR/Type.hpp"
#include "SR/Warning.hpp"


//----------------------------------------------------------------------------|
// Static Functions                                                           |
//

namespace GDCC::CC
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
   static void ParseTypeQual_Atomic(Parser &ctx, Scope &, SR::TypeQual &qual)
   {
      // If followed by a parenthesis, it is a type-specifier.
      if(ctx.in.peek().tok == Core::TOK_ParenO)
         Core::ErrorExpect("type-qualifier", ctx.in.reget());

      qual.aAtom = true;
   }
}


//----------------------------------------------------------------------------|
// Extern Functions                                                           |
//

namespace GDCC::CC
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
   void Parser::parseTypeQual(Scope &scope, SR::TypeQual &qual)
   {
      auto const &tok = expectIdenti(true);

      //
      // setSpace
      //
      auto setSpace = [&](IR::AddrBase space)
      {
         if(qual.space.base != IR::AddrBase::Gen)
            Core::Error(tok.pos, "multiple address-space-specifier");

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
         SR::WarnDeprecated(tok.pos,
            "__wld_ars is deprecrated, use __hub_ars instead");
      case Core::STR___hub_ars: setSpace(IR::AddrBase::HubArs); break;
      case Core::STR___wld_reg:
         SR::WarnDeprecated(tok.pos,
            "__wld_reg is deprecrated, use __hub_reg instead");
      case Core::STR___hub_reg: setSpace(IR::AddrBase::HubReg); break;
      case Core::STR___loc_reg: setSpace(IR::AddrBase::LocReg); break;
      case Core::STR___map_ars:
         SR::WarnDeprecated(tok.pos,
            "__map_ars is deprecrated, use __mod_ars instead");
      case Core::STR___mod_ars: setSpace(IR::AddrBase::ModArs); break;
      case Core::STR___map_reg:
         SR::WarnDeprecated(tok.pos,
            "__map_reg is deprecrated, use __mod_reg instead");
      case Core::STR___mod_reg: setSpace(IR::AddrBase::ModReg); break;
      case Core::STR___sta:     setSpace(IR::AddrBase::Sta);    break;
      case Core::STR___str_ars: setSpace(IR::AddrBase::StrArs); break;
      case Core::STR___va_addr: setSpace(IR::AddrBase::Vaa);    break;

         // Try a scope lookup for a user-defined address space.
      default:
         auto lookup = scope.lookup(tok.str);
         if(lookup.res != Lookup::Space)
            Core::ErrorExpect("type-qualifier", tok);

         if(qual.space.base != IR::AddrBase::Gen)
            Core::Error(tok.pos, "multiple address-space-specifier");

         qual.space.base = lookup.resSpace->space;
         qual.space.name = lookup.resSpace->glyph;
         break;
      }
   }
}

// EOF

