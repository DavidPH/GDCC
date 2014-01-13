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
// Global Functions                                                           |
//

namespace GDCC
{
   namespace CC
   {
      //
      // GetTypeQual
      //
      AST::TypeQual GetTypeQual(ParserData &in, Scope *ctx)
      {
         auto tok = in.in.get();
         if(tok.tok != Core::TOK_Identi && tok.tok != Core::TOK_KeyWrd)
            throw Core::ExceptStr(tok.pos, "expected identifier");

         switch(static_cast<Core::StringIndex>(tok.str))
         {
         // Standard C qualifiers.
         case Core::STR__Atomic:  return AST::QualAtom;
         case Core::STR_const:    return AST::QualCons;
         case Core::STR_restrict: return AST::QualRest;
         case Core::STR_volatile: return AST::QualVola;

         // Builtin address space names.
         case Core::STR___adr_cpy: return IR::AddrBase::Cpy;
         case Core::STR___far:     return IR::AddrBase::Far;
         case Core::STR___gbl_ars: return IR::AddrBase::GblArs;
         case Core::STR___gbl_reg: return IR::AddrBase::GblReg;
         case Core::STR___loc:     return IR::AddrBase::Loc;
         case Core::STR___loc_ars: return IR::AddrBase::LocArs;
         case Core::STR___loc_reg: return IR::AddrBase::LocReg;
         case Core::STR___map_ars: return IR::AddrBase::MapArs;
         case Core::STR___map_reg: return IR::AddrBase::MapReg;
         case Core::STR___str_ars: return IR::AddrBase::StrArs;
         case Core::STR___va_addr: return IR::AddrBase::Vaa;
         case Core::STR___wld_ars: return IR::AddrBase::WldArs;
         case Core::STR___wld_reg: return IR::AddrBase::WldReg;

         // Try a scope lookup for a user-defined address space.
         default:
            auto lookup = ctx->lookup(tok.str);
            if(lookup.res != Lookup::Space)
               throw Core::ExceptStr(tok.pos, "expected qualifier");

            return IR::AddrSpace(lookup.resSpace->space, lookup.resSpace->glyph);
         }
      }

      //
      // IsTypeQual
      //
      bool IsTypeQual(ParserData &in, Scope *ctx)
      {
         auto tok = in.in.peek();
         if(tok.tok != Core::TOK_Identi && tok.tok != Core::TOK_KeyWrd)
            return false;

         switch(static_cast<Core::StringIndex>(tok.str))
         {
         // Standard C qualifiers.
         case Core::STR__Atomic:  return true;
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
            return ctx->lookup(tok.str).res == Lookup::Space;
         }
      }
   }
}

// EOF

