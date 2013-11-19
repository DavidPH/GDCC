//-----------------------------------------------------------------------------
//
// Copyright (C) 2013 David Hill
//
// See COPYING for license information.
//
//-----------------------------------------------------------------------------
//
// Type parsing utilities.
//
//-----------------------------------------------------------------------------

#include "Parse.hpp"

#include "GDCC/TokenStream.hpp"

#include "GDCC/IR/Type.hpp"

#include <iostream>


//----------------------------------------------------------------------------|
// Static Functions                                                           |
//

//
// ParseType_Empty
//
static GDCC::IR::Type_Empty ParseType_Empty(GDCC::TokenStream &in,
   GDCC::IR::Program &)
{
   Asm::SkipToken(in, GDCC::TOK_ParenO, "(");
   Asm::SkipToken(in, GDCC::TOK_ParenC, ")");

   return GDCC::IR::Type_Empty();
}

//
// ParseType_Fixed
//
static GDCC::IR::Type_Fixed ParseType_Fixed(GDCC::TokenStream &in,
   GDCC::IR::Program &prog)
{
   Asm::SkipToken(in, GDCC::TOK_ParenO, "(");
   auto bitsI = Asm::ParseFastU(in, prog); Asm::SkipToken(in, GDCC::TOK_Comma, ",");
   auto bitsF = Asm::ParseFastU(in, prog); Asm::SkipToken(in, GDCC::TOK_Comma, ",");
   bool bitsS = Asm::ParseFastU(in, prog); Asm::SkipToken(in, GDCC::TOK_Comma, ",");
   bool satur = Asm::ParseFastU(in, prog);
   Asm::SkipToken(in, GDCC::TOK_ParenC, ")");

   return GDCC::IR::Type_Fixed(bitsI, bitsF, bitsS, satur);
}

//
// ParseType_Float
//
static GDCC::IR::Type_Float ParseType_Float(GDCC::TokenStream &in,
   GDCC::IR::Program &prog)
{
   Asm::SkipToken(in, GDCC::TOK_ParenO, "(");
   auto bitsI = Asm::ParseFastU(in, prog); Asm::SkipToken(in, GDCC::TOK_Comma, ",");
   auto bitsF = Asm::ParseFastU(in, prog); Asm::SkipToken(in, GDCC::TOK_Comma, ",");
   bool bitsS = Asm::ParseFastU(in, prog); Asm::SkipToken(in, GDCC::TOK_Comma, ",");
   bool satur = Asm::ParseFastU(in, prog);
   Asm::SkipToken(in, GDCC::TOK_ParenC, ")");

   return GDCC::IR::Type_Float(bitsI, bitsF, bitsS, satur);
}

//
// ParseType_Funct
//
static GDCC::IR::Type_Funct ParseType_Funct(GDCC::TokenStream &in,
   GDCC::IR::Program &)
{
   Asm::SkipToken(in, GDCC::TOK_ParenO, "(");
   auto callT = Asm::ParseCallType(in.get());
   Asm::SkipToken(in, GDCC::TOK_ParenC, ")");

   return GDCC::IR::Type_Funct(callT);
}

//
// ParseType_Multi
//
static GDCC::IR::Type_Multi ParseType_Multi(GDCC::TokenStream &in,
   GDCC::IR::Program &prog)
{
   std::vector<GDCC::IR::Type> typev;

   Asm::SkipToken(in, GDCC::TOK_ParenO, "(");

   if(in.peek().tok != GDCC::TOK_ParenC) do
      typev.emplace_back(Asm::ParseType(in, prog));
   while(in.drop(GDCC::TOK_Comma));

   Asm::SkipToken(in, GDCC::TOK_ParenC, ")");

   GDCC::Array<GDCC::IR::Type> types{GDCC::Move, typev.begin(), typev.end()};

   return GDCC::IR::Type_Multi(std::move(types));
}

//
// ParseType_Point
//
static GDCC::IR::Type_Point ParseType_Point(GDCC::TokenStream &in,
   GDCC::IR::Program &prog)
{
   Asm::SkipToken(in, GDCC::TOK_ParenO, "(");
   auto addrB = Asm::ParseAddrBase(in.get()); Asm::SkipToken(in, GDCC::TOK_Comma, ",");
   auto addrS = Asm::ParseFastU(in, prog);    Asm::SkipToken(in, GDCC::TOK_Comma, ",");
   auto addrW = Asm::ParseFastU(in, prog);
   Asm::SkipToken(in, GDCC::TOK_ParenC, ")");

   return GDCC::IR::Type_Point(addrB, addrS, addrW);
}

//
// ParseType_StrEn
//
static GDCC::IR::Type_StrEn ParseType_StrEn(GDCC::TokenStream &in,
   GDCC::IR::Program &)
{
   Asm::SkipToken(in, GDCC::TOK_ParenO, "(");
   Asm::SkipToken(in, GDCC::TOK_ParenC, ")");

   return GDCC::IR::Type_StrEn();
}


//----------------------------------------------------------------------------|
// Global Functions                                                           |
//

namespace Asm
{
   //
   // ParseType
   //
   GDCC::IR::Type ParseType(GDCC::TokenStream &in, GDCC::IR::Program &prog)
   {
      switch(static_cast<GDCC::StringIndex>(
         ExpectToken(in, GDCC::TOK_Identi, "identifier").get().str))
      {
         #define GDCC_IR_TypeList(name) \
            case GDCC::STR_##name: return ParseType_##name(in, prog);
         #include "GDCC/IR/TypeList.hpp"

      default:
         in.unget();
         std::cerr << "ERROR: " << in.peek().pos << ": bad type: '"
            << in.peek().str << "'\n";
         throw EXIT_FAILURE;
      }
   }
}

// EOF

