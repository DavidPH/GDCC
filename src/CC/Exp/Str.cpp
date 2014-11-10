//-----------------------------------------------------------------------------
//
// Copyright (C) 2014 David Hill
//
// See COPYING for license information.
//
//-----------------------------------------------------------------------------
//
// C string literal expressions.
//
//-----------------------------------------------------------------------------

#include "CC/Exp.hpp"

#include "CC/Parse.hpp"
#include "CC/Scope/Global.hpp"
#include "CC/Type.hpp"

#include "AST/Attribute.hpp"
#include "AST/Exp.hpp"
#include "AST/Object.hpp"
#include "AST/Type.hpp"

#include "IR/Exp.hpp"
#include "IR/Linkage.hpp"
#include "IR/Program.hpp"
#include "IR/Value.hpp"


//----------------------------------------------------------------------------|
// Static Functions                                                           |
//

//
// CreateStr
//
static GDCC::AST::Exp::CRef CreateStr(
   GDCC::Core::Array<GDCC::IR::Value> &&val,
   GDCC::IR::Program                   &prog,
   GDCC::CC::Scope                     &scope,
   GDCC::Core::Origin                   pos,
   GDCC::AST::Type               const *type)
{
   using namespace GDCC;

   // Set literal object's attributes.
   AST::Attribute attr;
   attr.namePos = pos;
   attr.type    = type->getTypeQual(AST::QualCons)->getTypeArray(val.size());

   // Create the object to store the string.
   auto obj = scope.global.getObject(attr);

   // String literals are allowed to alias.
   obj->alias = true;

   // String literals are definitions.
   obj->defin = true;

   // Set object's initializer.
   auto initType = type->getIRType().tFixed;

   IR::Value_Array initVal{std::move(val), {initType, val.size()}};
   auto initExp = IR::ExpCreate_Value(std::move(initVal), pos);
   obj->init = AST::ExpCreate_IRExp(initExp, attr.type, pos);

   // The expression's result is the newly created object.
   return CC::ExpCreate_Obj(prog, obj, pos);
}


//----------------------------------------------------------------------------|
// Global Functions                                                           |
//

namespace GDCC
{
   namespace CC
   {
      //
      // ExpCreate_StrIdx
      //
      AST::Exp::CRef ExpCreate_StrIdx(IR::Program &prog, Scope &scope,
         Core::String str, Core::Origin pos)
      {
         // Generate glyph.
         auto glyph = scope.global.genGlyphObj(Core::STRNULL, IR::Linkage::None);

         // Create StrEnt.
         // Is it correct to modify the IR::Program directly like this?
         // What would an AST::StrEnt do?
         auto &strent = prog.getStrEnt(glyph);

         // Configure StrEnt.
         strent.valueStr = str;
         strent.alias    = true;
         strent.alloc    = true;
         strent.defin    = true;

         // Prepare associated glyph.
         prog.getGlyphData(glyph).type = IR::Type_StrEn();

         // Convert glyph to AST::Exp.
         return AST::ExpCreate_IRExp(
            IR::ExpCreate_Glyph(IR::Glyph(&prog, glyph), pos),
            AST::Type::StrEnt->getTypePointer(), pos);
      }

      //
      // ExpCreate_StrU08
      //
      AST::Exp::CRef ExpCreate_StrU08(IR::Program &prog, Scope &scope,
         Core::String str, Core::Origin pos)
      {
         return CreateStr(GetStrU08(str), prog, scope, pos, TypeChar);
      }

      //
      // ExpCreate_StrU16
      //
      AST::Exp::CRef ExpCreate_StrU16(IR::Program &prog, Scope &scope,
         Core::String str, Core::Origin pos)
      {
         return CreateStr(GetStrU16(str), prog, scope, pos, TypeIntegPrUH);
      }

      //
      // ExpCreate_StrU32
      //
      AST::Exp::CRef ExpCreate_StrU32(IR::Program &prog, Scope &scope,
         Core::String str, Core::Origin pos)
      {
         return CreateStr(GetStrU32(str), prog, scope, pos, TypeIntegPrU);
      }

      //
      // ExpCreate_String
      //
      AST::Exp::CRef ExpCreate_String(IR::Program &prog, Scope &scope,
         Core::String str, Core::Origin pos)
      {
         return CreateStr(GetString(str), prog, scope, pos, TypeChar);
      }
   }
}

// EOF

