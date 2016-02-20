//-----------------------------------------------------------------------------
//
// Copyright (C) 2014-2016 David Hill
//
// See COPYING for license information.
//
//-----------------------------------------------------------------------------
//
// C string literal parsing.
//
//-----------------------------------------------------------------------------

#include "CC/Parse.hpp"

#include "CC/Type.hpp"

#include "AST/Type.hpp"

#include "Core/Exception.hpp"
#include "Core/Token.hpp"

#include "IR/Value.hpp"

#include <tuple>
#include <vector>


//----------------------------------------------------------------------------|
// Static Functions                                                           |
//

//
// CreateStr
//
template<typename StrLen, typename StrVal>
static GDCC::Core::Array<GDCC::IR::Value> CreateStr(GDCC::Core::String str,
   GDCC::AST::Type const *type, StrLen const &strLen, StrVal const &strVal)
{
   using namespace GDCC;

   // Calculate string length.
   auto len = strLen(str);

   // Convert string to IR value.
   auto initType = type->getIRType().tFixed;

   std::vector<IR::Value_Fixed> val; val.reserve(len);
   strVal(str, val, initType);

   return Core::Array<IR::Value>(Core::Move, val.begin(), val.end());
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
      // GetStrU08
      //
      Core::Array<IR::Value> GetStrU08(Core::String str)
      {
         return CreateStr(str, TypeChar, StrLen_ChrU08, StrVal_ChrU08);
      }

      //
      // GetStrU16
      //
      Core::Array<IR::Value> GetStrU16(Core::String str)
      {
         return CreateStr(str, TypeIntegPrUH, StrLen_ChrU16, StrVal_ChrU16);
      }

      //
      // GetStrU32
      //
      Core::Array<IR::Value> GetStrU32(Core::String str)
      {
         return CreateStr(str, TypeIntegPrU, StrLen_ChrU32, StrVal_ChrU32);
      }

      //
      // GetString
      //
      Core::Array<IR::Value> GetString(Core::String str)
      {
         return CreateStr(str, TypeChar, StrLen_ChrU32, StrVal_ChrU32);
      }

      //
      // GetString
      //
      Core::Array<IR::Value> GetString(Core::Token const &tok)
      {
         switch(tok.tok)
         {
         case Core::TOK_StrU08: return GetStrU08(tok.str);
         case Core::TOK_StrU16: return GetStrU16(tok.str);
         case Core::TOK_StrU32: return GetStrU32(tok.str);
         case Core::TOK_String: return GetString(tok.str);

         default:
            throw Core::ExceptStr(tok.pos, "invalid string literal token");
         }
      }
   }
}

// EOF

