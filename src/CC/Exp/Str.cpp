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

#include "CC/Scope/Global.hpp"
#include "CC/Scope/Local.hpp"
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
template<typename StrLen, typename StrVal>
static GDCC::AST::Exp::CRef CreateStr(GDCC::IR::Program &prog,
   GDCC::CC::Scope &scope, GDCC::Core::String str, GDCC::Core::Origin pos,
   GDCC::AST::Type const *type, StrLen const &strLen, StrVal const &strVal)
{
   using namespace GDCC;

   // Calculate string length.
   auto len = strLen(str);

   // Set literal object's attributes.
   AST::Attribute attr;
   attr.namePos = pos;
   attr.type    = type->getTypeQual(AST::QualCons)->getTypeArray(len);

   // Create the object to store the string.
   auto obj = scope.global.getObject(attr);

   // String literals are allowed to alias.
   obj->alias = true;

   // String literals are definitions.
   obj->defin = true;

   // Convert string to IR value.
   auto initType = type->getIRType().tFixed;

   std::vector<IR::Value_Fixed> val; val.reserve(len);
   strVal(str, val, initType);

   IR::Value_Multi initVal{
      Core::Array<IR::Value>(Core::Move, val.begin(), val.end()),
      IR::Type_Multi(Core::Array<IR::Type>(val.size(), initType))};

   // Set object's initializer.
   auto initExp = IR::ExpCreate_ValueRoot(std::move(initVal), pos);
   obj->init = AST::ExpCreate_IRExp(initExp, attr.type, pos);

   // The expression's result is the newly created object.
   return CC::ExpCreate_Obj(prog, obj, pos);
}

//
// StrLen_ChrU08
//
static std::size_t StrLen_ChrU08(GDCC::Core::String str)
{
   return str.size() + 1;
}

//
// StrLen_ChrU16
//
static std::size_t StrLen_ChrU16(GDCC::Core::String str)
{
   return str.size16() + 1;
}

//
// StrLen_ChrU32
//
static std::size_t StrLen_ChrU32(GDCC::Core::String str)
{
   return str.size32() + 1;
}

//
// StrVal_ChrU08
//
static void StrVal_ChrU08(GDCC::Core::String str,
   std::vector<GDCC::IR::Value_Fixed> &val, GDCC::IR::Type_Fixed const &type)
{
   for(unsigned char c : str)
      val.emplace_back(c, type);

   val.emplace_back(0, type);
}

//
// StrVal_ChrU16
//
static void StrVal_ChrU16(GDCC::Core::String str,
   std::vector<GDCC::IR::Value_Fixed> &val, GDCC::IR::Type_Fixed const &type)
{
   for(auto itr = str.begin(), end = str.end(); itr != end;)
   {
      char32_t c;
      std::tie(c, itr) = GDCC::Core::Str8To32(itr, end);

      if(c <= 0xFFFF)
         val.emplace_back(c, type);
      else
      {
         c -= 0x10000;
         val.emplace_back((c >> 10 & 0x3FF) | 0xD800, type);
         val.emplace_back((c       & 0x3FF) | 0xDC00, type);
      }
   }

   val.emplace_back(0, type);
}

//
// StrVal_ChrU32
//
static void StrVal_ChrU32(GDCC::Core::String str,
   std::vector<GDCC::IR::Value_Fixed> &val, GDCC::IR::Type_Fixed const &type)
{
   for(auto itr = str.begin(), end = str.end(); itr != end;)
   {
      char32_t c;
      std::tie(c, itr) = GDCC::Core::Str8To32(itr, end);

      val.emplace_back(c, type);
   }

   val.emplace_back(0, type);
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
         strent.defin    = true;

         // Prepare associated glyph.
         prog.getGlyphData(glyph).type = IR::Type_StrEn();

         // Convert glyph to AST::Exp.
         return AST::ExpCreate_IRExp(
            IR::ExpCreate_ValueGlyph(IR::Glyph(&prog, glyph), pos),
            AST::Type::StrEnt->getTypePointer(), pos);
      }

      //
      // ExpCreate_StrU08
      //
      AST::Exp::CRef ExpCreate_StrU08(IR::Program &prog, Scope &scope,
         Core::String str, Core::Origin pos)
      {
         return CreateStr(prog, scope, str, pos,
            TypeChar, StrLen_ChrU08, StrVal_ChrU08);
      }

      //
      // ExpCreate_StrU16
      //
      AST::Exp::CRef ExpCreate_StrU16(IR::Program &prog, Scope &scope,
         Core::String str, Core::Origin pos)
      {
         return CreateStr(prog, scope, str, pos,
            TypeIntegPrUH, StrLen_ChrU16, StrVal_ChrU16);
      }

      //
      // ExpCreate_StrU32
      //
      AST::Exp::CRef ExpCreate_StrU32(IR::Program &prog, Scope &scope,
         Core::String str, Core::Origin pos)
      {
         return CreateStr(prog, scope, str, pos,
            TypeIntegPrU, StrLen_ChrU32, StrVal_ChrU32);
      }

      //
      // ExpCreate_String
      //
      AST::Exp::CRef ExpCreate_String(IR::Program &prog, Scope &scope,
         Core::String str, Core::Origin pos)
      {
         return CreateStr(prog, scope, str, pos,
            TypeChar, StrLen_ChrU32, StrVal_ChrU32);
      }
   }
}

// EOF

