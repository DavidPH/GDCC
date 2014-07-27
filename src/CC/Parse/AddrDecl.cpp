//-----------------------------------------------------------------------------
//
// Copyright (C) 2014 David Hill
//
// See COPYING for license information.
//
//-----------------------------------------------------------------------------
//
// C address-space-declaration parsing.
//
// address-space-declaration:
//    <__addrdef> storage-class-specifier(opt) address-space-specifier
//       identifier attribute-specifier-list(opt) ;
//
// address-space-specifier:
//    <__gbl_arr>
//    <__map_arr>
//    <__wld_arr>
//
//-----------------------------------------------------------------------------

#include "CC/Parse.hpp"

#include "CC/Scope/Global.hpp"

#include "AST/Attribute.hpp"
#include "AST/Space.hpp"

#include "Core/Exception.hpp"
#include "Core/TokenStream.hpp"

#include "IR/Addr.hpp"
#include "IR/Exp.hpp"
#include "IR/Linkage.hpp"


//----------------------------------------------------------------------------|
// Global Functions                                                           |
//

namespace GDCC
{
   namespace CC
   {
      //
      // IsAddrDecl
      //
      bool IsAddrDecl(ParserCtx const &ctx, Scope &)
      {
         return ctx.in.peek(Core::TOK_Identi, Core::STR___addrdef);
      }

      //
      // ParseAddrDecl
      //
      void ParseAddrDecl(ParserCtx const &ctx, Scope &scope)
      {
         AST::Attribute attr;
         attr.linka = IR::Linkage::ExtC;

         // <__addrdef>
         if(!ctx.in.drop(Core::TOK_Identi, Core::STR___addrdef))
            throw Core::ExceptStr(ctx.in.peek().pos,
               "expected address-space-declaration");

         // storage-class-specifier(opt)
              if(ctx.in.drop(Core::TOK_KeyWrd, Core::STR_extern))
            attr.storeExt = true;
         else if(ctx.in.drop(Core::TOK_KeyWrd, Core::STR_static))
            attr.storeInt = true;

         // address-space-specifier
         switch(ctx.in.get().str)
         {
         case Core::STR___gbl_arr: attr.space.base = IR::AddrBase::GblArr; break;
         case Core::STR___map_arr: attr.space.base = IR::AddrBase::MapArr; break;
         case Core::STR___wld_arr: attr.space.base = IR::AddrBase::WldArr; break;
         default: throw Core::ExceptStr(ctx.in.reget().pos,
            "expected address-space-specifier");
         }

         // identifier
         if(!ctx.in.peek(Core::TOK_Identi))
            throw Core::ExceptStr(ctx.in.peek().pos, "expected identifier");

         attr.setName(ctx.in.get());

         // attribute-specifier-list(opt)
         if(IsAttrSpec(ctx, scope))
            ParseAttrSpecList(ctx, scope, attr);

         // ;
         if(!ctx.in.drop(Core::TOK_Semico))
            throw Core::ExceptStr(ctx.in.peek().pos, "expected ';'");

         // Determine linkage.
         if(attr.storeInt)
            attr.linka = IR::GetLinkageInt(attr.linka);

         auto space = scope.global.getSpace(attr);

         if(!attr.storeExt)
            space->defin = true;

         if(attr.addrI)
            space->value = attr.addrI;

         scope.add(attr.name, space);
      }
   }
}

// EOF

