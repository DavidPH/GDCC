//-----------------------------------------------------------------------------
//
// Copyright (C) 2014-2016 David Hill
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
//    <__map_arr>
//    <__wld_arr>
//
//-----------------------------------------------------------------------------

#include "CC/Parse.hpp"

#include "CC/Scope/Global.hpp"
#include "CC/Scope/Local.hpp"

#include "Core/Exception.hpp"
#include "Core/TokenStream.hpp"

#include "IR/Addr.hpp"
#include "IR/Exp.hpp"
#include "IR/Linkage.hpp"

#include "SR/Attribute.hpp"
#include "SR/Space.hpp"
#include "SR/Warning.hpp"


//----------------------------------------------------------------------------|
// Static Functions                                                           |
//

namespace GDCC
{
   namespace CC
   {
      //
      // ParseAddrDeclBase
      //
      template<typename ScopeT>
      static void ParseAddrDeclBase(Parser &ctx, ScopeT &scope,
         SR::Attribute &attr)
      {
         attr.linka = IR::Linkage::ExtC;

         // <__addrdef>
         if(!ctx.in.drop(Core::TOK_Identi, Core::STR___addrdef))
            throw Core::ParseExceptExpect(ctx.in.peek(),
               "address-space-declaration", false);

         // storage-class-specifier(opt)
              if(ctx.in.drop(Core::TOK_KeyWrd, Core::STR_extern))
            attr.storeExt = true;
         else if(ctx.in.drop(Core::TOK_KeyWrd, Core::STR_static))
            attr.storeInt = true;

         // address-space-specifier
         if(!ctx.in.peek(Core::TOK_Identi))
            throw Core::ParseExceptExpect(ctx.in.peek(), "identifier", false);

         switch(ctx.in.get().str)
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
         default: throw Core::ParseExceptExpect(ctx.in.reget(),
            "address-space-specifier", false);
         }

         // identifier
         if(!ctx.in.peek(Core::TOK_Identi))
            throw Core::ParseExceptExpect(ctx.in.peek(), "identifier", false);

         attr.setName(ctx.in.get());

         // attribute-specifier-list(opt)
         if(ctx.isAttrSpec(scope))
            ctx.parseAttrSpecList(scope, attr);

         // ;
         if(!ctx.in.drop(Core::TOK_Semico))
            throw Core::ParseExceptExpect(ctx.in.peek(), ";", true);

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
}


//----------------------------------------------------------------------------|
// Extern Functions                                                           |
//

namespace GDCC
{
   namespace CC
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
}

// EOF

