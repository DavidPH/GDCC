//-----------------------------------------------------------------------------
//
// Copyright (C) 2014-2019 David Hill
//
// See COPYING for license information.
//
//-----------------------------------------------------------------------------
//
// C string literal expressions.
//
//-----------------------------------------------------------------------------

#include "CC/Exp.hpp"

#include "CC/Factory.hpp"
#include "CC/Parse.hpp"
#include "CC/Scope/Global.hpp"
#include "CC/Type.hpp"

#include "IR/Exp.hpp"
#include "IR/Linkage.hpp"
#include "IR/Program.hpp"
#include "IR/Value.hpp"

#include "SR/Attribute.hpp"
#include "SR/Exp.hpp"
#include "SR/Object.hpp"
#include "SR/Type.hpp"


//----------------------------------------------------------------------------|
// Static Functions                                                           |
//

namespace GDCC::CC
{
   //
   // CreateStr
   //
   static SR::Exp::CRef CreateStr(Factory &fact, Core::Array<IR::Value> &&val,
      IR::Program &prog, Scope &scope, Core::Origin pos, SR::Type const *type)
   {
      // Set literal object's attributes.
      SR::Attribute attr;
      attr.namePos = pos;
      attr.type    = type->getTypeQual(SR::QualCons)->getTypeArray(val.size());

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
      obj->init = fact.expCreate_IRExp(initExp, attr.type, pos);

      // The expression's result is the newly created object.
      return fact.expCreate_Obj(prog, obj, pos);
   }
}


//----------------------------------------------------------------------------|
// Extern Functions                                                           |
//

namespace GDCC::CC
{
   //
   // Factory::expCreate_StrIdx
   //
   SR::Exp::CRef Factory::expCreate_StrIdx(IR::Program &prog, Scope &scope,
      Core::String str, Core::Origin pos)
   {
      // Generate glyph.
      auto glyph = scope.global.genGlyphObj(Core::STRNULL, IR::Linkage::None);

      // Create StrEnt.
      // Is it correct to modify the IR::Program directly like this?
      // What would an SR::StrEnt do?
      auto &strent = prog.getStrEnt(glyph);

      // Configure StrEnt.
      strent.valueStr = str;
      strent.alias    = true;
      strent.alloc    = true;
      strent.defin    = true;

      // Prepare associated glyph.
      prog.getGlyphData(glyph).type = IR::Type_StrEn();

      // Convert glyph to SR::Exp.
      return expCreate_IRExp(
         IR::ExpCreate_Glyph(IR::Glyph(&prog, glyph), pos),
         SR::Type::StrEnt->getTypePointer(), pos);
   }

   //
   // Factory::expCreate_StrU08
   //
   SR::Exp::CRef Factory::expCreate_StrU08(IR::Program &prog, Scope &scope,
      Core::String str, Core::Origin pos)
   {
      return CreateStr(*this, GetStrU08(str), prog, scope, pos, TypeChar);
   }

   //
   // Factory::expCreate_StrU16
   //
   SR::Exp::CRef Factory::expCreate_StrU16(IR::Program &prog, Scope &scope,
      Core::String str, Core::Origin pos)
   {
      return CreateStr(*this, GetStrU16(str), prog, scope, pos, TypeIntegPrUH);
   }

   //
   // Factory::expCreate_StrU32
   //
   SR::Exp::CRef Factory::expCreate_StrU32(IR::Program &prog, Scope &scope,
      Core::String str, Core::Origin pos)
   {
      return CreateStr(*this, GetStrU32(str), prog, scope, pos, TypeIntegPrU);
   }

   //
   // Factory::expCreate_String
   //
   SR::Exp::CRef Factory::expCreate_String(IR::Program &prog, Scope &scope,
      Core::String str, Core::Origin pos)
   {
      return CreateStr(*this, GetString(str), prog, scope, pos, TypeChar);
   }
}

// EOF

