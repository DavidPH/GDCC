//-----------------------------------------------------------------------------
//
// Copyright (C) 2014-2019 David Hill
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
//    <__loc_arr>
//    <__mod_arr>
//    <__hub_arr>
//
//-----------------------------------------------------------------------------

#include "CC/Parse.hpp"

#include "CC/Scope/Global.hpp"
#include "CC/Scope/Local.hpp"

#include "Core/Exception.hpp"
#include "Core/TokenStream.hpp"

#include "IR/Exp.hpp"
#include "IR/Linkage.hpp"

#include "SR/Attribute.hpp"
#include "SR/Space.hpp"
#include "SR/Warning.hpp"

#include "Target/Addr.hpp"


//----------------------------------------------------------------------------|
// Static Functions                                                           |
//

namespace GDCC::CC
{
   //
   // ParseAddrDeclBase
   //
   template<typename ScopeT>
   static void ParseAddrDeclBase(Parser &ctx, ScopeT &scope,
      SR::Attribute &attr)
   {
      // <__addrdef>
      if(!ctx.in.drop(Core::TOK_Identi, Core::STR___addrdef))
         Core::ErrorExpect("address-space-declaration", ctx.in.peek());

      // storage-class-specifier(opt)
           if(ctx.in.drop(Core::TOK_KeyWrd, Core::STR_extern))
         attr.storeExt = true;
      else if(ctx.in.drop(Core::TOK_KeyWrd, Core::STR_static))
         attr.storeInt = true;

      // address-space-specifier
      switch(ctx.expectIdenti().str)
      {
      case Core::STR___gbl_arr: attr.space.base = IR::AddrBase::GblArr; break;
      case Core::STR___wld_arr:
         SR::WarnDeprecated(attr.namePos,
            "__wld_arr is deprecrated, use __hub_arr instead");
      case Core::STR___hub_arr: attr.space.base = IR::AddrBase::HubArr; break;
      case Core::STR___loc_arr: attr.space.base = IR::AddrBase::LocArr; break;
      case Core::STR___map_arr:
         SR::WarnDeprecated(attr.namePos,
            "__map_arr is deprecrated, use __mod_arr instead");
      case Core::STR___mod_arr: attr.space.base = IR::AddrBase::ModArr; break;
      default: Core::ErrorExpect("address-space-specifier", ctx.in.reget());
      }

      // identifier
      attr.setName(ctx.expectIdenti());

      // attribute-specifier-list(opt)
      if(ctx.isAttrSpec(scope))
         ctx.parseAttrSpecList(scope, attr);

      // ;
      ctx.expect(Core::TOK_Semico);

      // Determine linkage.
      if(attr.space.base == IR::AddrBase::LocArr)
         attr.linka = IR::Linkage::None;

      else if(attr.storeInt)
         attr.linka = IR::GetLinkageInt(attr.linka);

      auto space = scope.getSpace(attr);

      if(!attr.storeExt)
         space->defin = true;

      if(attr.addrI)
         space->value = attr.addrI;

      scope.add(attr.name, space);
   }
}


//----------------------------------------------------------------------------|
// Extern Functions                                                           |
//

namespace GDCC::CC
{
   //
   // Parser::isAddrDecl
   //
   bool Parser::isAddrDecl(Scope &)
   {
      return in.peek(Core::TOK_Identi, Core::STR___addrdef);
   }

   //
   // Parser::parseAddrDecl
   //
   void Parser::parseAddrDecl(Scope_Global &scope, SR::Attribute &attr)
   {
      ParseAddrDeclBase(*this, scope, attr);
   }

   //
   // Parser::parseAddrDecl
   //
   void Parser::parseAddrDecl(Scope_Local &scope, SR::Attribute &attr)
   {
      ParseAddrDeclBase(*this, scope, attr);
   }
}

// EOF

