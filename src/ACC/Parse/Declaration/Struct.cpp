//-----------------------------------------------------------------------------
//
// Copyright (C) 2015-2019 David Hill
//
// See COPYING for license information.
//
//-----------------------------------------------------------------------------
//
// ACS structure-declaration parsing.
//
//-----------------------------------------------------------------------------

#include "ACC/Parse.hpp"

#include "ACC/Factory.hpp"
#include "ACC/Scope.hpp"

#include "CC/Type/Struct.hpp"

#include "Core/Exception.hpp"
#include "Core/TokenStream.hpp"

#include "SR/Attribute.hpp"
#include "SR/Statement.hpp"


//----------------------------------------------------------------------------|
// Static Functions                                                           |
//

namespace GDCC::ACC
{
   //
   // GetDeclStruct
   //
   static CC::Type_Struct::Ref GetDeclStruct(Core::Token name, Scope_Global &scope)
   {
      auto lookup = scope.lookupTypeTag(name.str);

      if(!lookup)
      {
         auto type = CC::Type_Struct::Create(name.str);
         scope.addTypeTag(name.str, type);
         return type;
      }

      if(!lookup->isCTypeStruct())
         Core::Error(name.pos, "expected structure type");

      return static_cast<CC::Type_Struct::Ref>(&*lookup);
   }
}


//----------------------------------------------------------------------------|
// Extern Functions                                                           |
//

namespace GDCC::ACC
{
   //
   // Parser::getDecl_Struct
   //
   SR::Statement::CRef Parser::getDecl_Struct(Scope_Global &scope)
   {
      // structure-declaration:
      //    <struct> identifier { structure-member-declaration-sequence }
      //    <struct> identifier ;

      // <struct>
      auto pos = expect(Core::TOK_KeyWrd, Core::STR_struct).pos;

      // identifier
      auto name = expectIdenti();

      auto type = GetDeclStruct(name, scope);

      scope.add(name.str, type);

      // ;
      if(in.drop(Core::TOK_Semico))
         return fact.stCreate_Empty({}, pos);

      // {
      expect(Core::TOK_BraceO);

      // List of members.
      std::vector<CC::Type_Struct::MemberData> memv;

      // Current address in structure.
      Core::FastU addr = 0;

      // Current size of structure.
      Core::FastU size = 0;

      //
      // addrAlign
      //
      auto addrAlign = [&](Core::FastU sizeAlign)
      {
         size = addr = (addr + sizeAlign - 1) / sizeAlign * sizeAlign;
      };

      //
      // addrBytes
      //
      auto addrBytes = [&](Core::FastU sizeBytes)
      {
         size = addr += sizeBytes;
      };

      //
      // findName
      //
      auto findName = [&memv](Core::String memName) -> bool
      {
         if(!memName) return false;

         for(auto const &mem : memv)
            if(mem.name == memName) return true;

         return false;
      };

      // structure-member-declaration-sequence:
      //    structure-member-declaration
      //    structure-member-declaration-sequence structure-member-declaration
      do
      {
         // struct-member-declaration:
         //    specifier-qualifier-sequence structure-member-declarator-list ;

         if(!isSpecQual(scope))
            Core::ErrorExpect("structure-member-declaration", in.peek());

         // specifier-qualifier-list
         SR::Attribute attrBase;
         parseSpecQual(scope, attrBase);

         // structure-member-declarator-list:
         //    structure-member-declarator
         //    structure-member-declarator-list , structure-member-declarator
         do
         {
            // structure-member-declarator:
            //    declarator

            SR::Attribute attr = attrBase;

            parseDeclarator(scope, attr);

            if(findName(attr.name))
               Core::Error(attr.namePos, "name reused");

            if(!attr.type->isTypeComplete())
               Core::Error(attr.namePos, "incomplete member");

            addrAlign(attr.type->getSizeAlign());
            memv.emplace_back(attr.name, attr.type, addr, false);
            addrBytes(attr.type->getSizeBytes());
         }
         while(in.drop(Core::TOK_Comma));

         // ;
         expect(Core::TOK_Semico);
      }
      while(!in.drop(Core::TOK_BraceC));

      if(memv.empty())
         Core::Error(pos, "empty structure");

      if(!type->isTypeComplete())
         type->setMembers(memv.data(), memv.size(), size);

      return fact.stCreate_Empty({}, pos);
   }
}

// EOF

