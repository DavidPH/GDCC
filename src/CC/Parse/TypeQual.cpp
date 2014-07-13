//-----------------------------------------------------------------------------
//
// Copyright (C) 2013-2014 David Hill
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

#include "Core/Exception.hpp"
#include "Core/TokenStream.hpp"

#include <iostream>


//----------------------------------------------------------------------------|
// Static Functions                                                           |
//

//
// IsTypeQual_Atomic
//
static bool IsTypeQual_Atomic(GDCC::CC::ParserCtx const &ctx, GDCC::CC::Scope &)
{
   // If followed by a parenthesis, it is a type-specifier.
   ctx.in.get();
   bool res = ctx.in.peek().tok != GDCC::Core::TOK_ParenO;
   ctx.in.unget();
   return res;
}

//
// ParseTypeQual_Atomic
//
static void ParseTypeQual_Atomic(GDCC::CC::ParserCtx const &ctx,
   GDCC::CC::Scope &, GDCC::AST::TypeQual &qual)
{
   using namespace GDCC;

   // If followed by a parenthesis, it is a type-specifier.
   if(ctx.in.peek().tok == Core::TOK_ParenO)
      throw Core::ExceptStr(ctx.in.reget().pos, "expected type-qualifier");

   qual.aAtom = true;
}


//----------------------------------------------------------------------------|
// Global Functions                                                           |
//

namespace GDCC
{
   namespace CC
   {
      //
      // IsTypeQual
      //
      bool IsTypeQual(ParserCtx const &ctx, Scope &scope)
      {
         auto const &tok = ctx.in.peek();
         if(tok.tok != Core::TOK_Identi && tok.tok != Core::TOK_KeyWrd)
            return false;

         switch(tok.str)
         {
            // Standard C qualifiers.
         case Core::STR__Atomic:  return IsTypeQual_Atomic(ctx, scope);
         case Core::STR_const:    return true;
         case Core::STR_restrict: return true;
         case Core::STR_volatile: return true;

            // Builtin address space names.
         case Core::STR___adr_cpy: return true;
         case Core::STR___far:     return true;
         case Core::STR___gbl_ars: return true;
         case Core::STR___gbl_reg: return true;
         case Core::STR___loc:     return true;
         case Core::STR___loc_ars: return true;
         case Core::STR___loc_reg: return true;
         case Core::STR___map_ars: return true;
         case Core::STR___map_reg: return true;
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
      // ParseTypeQual
      //
      void ParseTypeQual(ParserCtx const &ctx, Scope &scope, AST::TypeQual &qual)
      {
         auto const &tok = ctx.in.get();
         if(tok.tok != Core::TOK_Identi && tok.tok != Core::TOK_KeyWrd)
            throw Core::ExceptStr(tok.pos, "expected identifier");

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
         case Core::STR__Atomic:  ParseTypeQual_Atomic(ctx, scope, qual); break;
         case Core::STR_const:    qual.aCons = true; break;
         case Core::STR_restrict: qual.aRest = true; break;
         case Core::STR_volatile: qual.aVola = true; break;

            // Builtin address space names.
         case Core::STR___adr_cpy: setSpace(IR::AddrBase::Cpy);    break;
         case Core::STR___far:     setSpace(IR::AddrBase::Far);    break;
         case Core::STR___gbl_ars: setSpace(IR::AddrBase::GblArs); break;
         case Core::STR___gbl_reg: setSpace(IR::AddrBase::GblReg); break;
         case Core::STR___loc:     setSpace(IR::AddrBase::Loc);    break;
         case Core::STR___loc_ars: setSpace(IR::AddrBase::LocArs); break;
         case Core::STR___loc_reg: setSpace(IR::AddrBase::LocReg); break;
         case Core::STR___map_ars: setSpace(IR::AddrBase::MapArs); break;
         case Core::STR___map_reg: setSpace(IR::AddrBase::MapReg); break;
         case Core::STR___str_ars: setSpace(IR::AddrBase::StrArs); break;
         case Core::STR___va_addr: setSpace(IR::AddrBase::Vaa);    break;
         case Core::STR___wld_ars: setSpace(IR::AddrBase::WldArs); break;
         case Core::STR___wld_reg: setSpace(IR::AddrBase::WldReg); break;

            // Try a scope lookup for a user-defined address space.
         default:
            auto lookup = scope.lookup(tok.str);
            if(lookup.res != Lookup::Space)
               throw Core::ExceptStr(tok.pos, "expected type-qualifier");

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

