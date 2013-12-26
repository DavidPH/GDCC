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

#include "AS/Parse.hpp"

#include "Core/TokenStream.hpp"

#include "IR/Type.hpp"

#include <iostream>
#include <vector>


//----------------------------------------------------------------------------|
// Static Functions                                                           |
//

//
// ParseType_Empty
//
static GDCC::IR::Type_Empty ParseType_Empty(GDCC::Core::TokenStream &in,
   GDCC::IR::Program &)
{
   GDCC::AS::SkipToken(in, GDCC::Core::TOK_ParenO, "(");
   GDCC::AS::SkipToken(in, GDCC::Core::TOK_ParenC, ")");

   return GDCC::IR::Type_Empty();
}

//
// ParseType_Fixed
//
static GDCC::IR::Type_Fixed ParseType_Fixed(GDCC::Core::TokenStream &in,
   GDCC::IR::Program &prog)
{
   GDCC::AS::SkipToken(in, GDCC::Core::TOK_ParenO, "(");
   auto bitsI = GDCC::AS::ParseFastU(in, prog);
   GDCC::AS::SkipToken(in, GDCC::Core::TOK_Comma, ",");
   auto bitsF = GDCC::AS::ParseFastU(in, prog);
   GDCC::AS::SkipToken(in, GDCC::Core::TOK_Comma, ",");
   bool bitsS = GDCC::AS::ParseFastU(in, prog);
   GDCC::AS::SkipToken(in, GDCC::Core::TOK_Comma, ",");
   bool satur = GDCC::AS::ParseFastU(in, prog);
   GDCC::AS::SkipToken(in, GDCC::Core::TOK_ParenC, ")");

   return GDCC::IR::Type_Fixed(bitsI, bitsF, bitsS, satur);
}

//
// ParseType_Float
//
static GDCC::IR::Type_Float ParseType_Float(GDCC::Core::TokenStream &in,
   GDCC::IR::Program &prog)
{
   GDCC::AS::SkipToken(in, GDCC::Core::TOK_ParenO, "(");
   auto bitsI = GDCC::AS::ParseFastU(in, prog);
   GDCC::AS::SkipToken(in, GDCC::Core::TOK_Comma, ",");
   auto bitsF = GDCC::AS::ParseFastU(in, prog);
   GDCC::AS::SkipToken(in, GDCC::Core::TOK_Comma, ",");
   bool bitsS = GDCC::AS::ParseFastU(in, prog);
   GDCC::AS::SkipToken(in, GDCC::Core::TOK_Comma, ",");
   bool satur = GDCC::AS::ParseFastU(in, prog);
   GDCC::AS::SkipToken(in, GDCC::Core::TOK_ParenC, ")");

   return GDCC::IR::Type_Float(bitsI, bitsF, bitsS, satur);
}

//
// ParseType_Funct
//
static GDCC::IR::Type_Funct ParseType_Funct(GDCC::Core::TokenStream &in,
   GDCC::IR::Program &)
{
   GDCC::AS::SkipToken(in, GDCC::Core::TOK_ParenO, "(");
   auto callT = GDCC::AS::ParseCallType(in.get());
   GDCC::AS::SkipToken(in, GDCC::Core::TOK_ParenC, ")");

   return GDCC::IR::Type_Funct(callT);
}

//
// ParseType_Multi
//
static GDCC::IR::Type_Multi ParseType_Multi(GDCC::Core::TokenStream &in,
   GDCC::IR::Program &prog)
{
   std::vector<GDCC::IR::Type> typev;

   GDCC::AS::SkipToken(in, GDCC::Core::TOK_ParenO, "(");

   if(in.peek().tok != GDCC::Core::TOK_ParenC) do
      typev.emplace_back(GDCC::AS::ParseType(in, prog));
   while(in.drop(GDCC::Core::TOK_Comma));

   GDCC::AS::SkipToken(in, GDCC::Core::TOK_ParenC, ")");

   GDCC::Core::Array<GDCC::IR::Type> types
      {GDCC::Core::Move, typev.begin(), typev.end()};

   return GDCC::IR::Type_Multi(std::move(types));
}

//
// ParseType_Point
//
static GDCC::IR::Type_Point ParseType_Point(GDCC::Core::TokenStream &in,
   GDCC::IR::Program &prog)
{
   GDCC::AS::SkipToken(in, GDCC::Core::TOK_ParenO, "(");
   auto addrB = GDCC::AS::ParseAddrBase(in.get());
   GDCC::AS::SkipToken(in, GDCC::Core::TOK_Comma, ",");
   auto addrS = GDCC::AS::ParseFastU(in, prog);
   GDCC::AS::SkipToken(in, GDCC::Core::TOK_Comma, ",");
   auto addrW = GDCC::AS::ParseFastU(in, prog);
   GDCC::AS::SkipToken(in, GDCC::Core::TOK_ParenC, ")");

   return GDCC::IR::Type_Point(addrB, addrS, addrW);
}

//
// ParseType_StrEn
//
static GDCC::IR::Type_StrEn ParseType_StrEn(GDCC::Core::TokenStream &in,
   GDCC::IR::Program &)
{
   GDCC::AS::SkipToken(in, GDCC::Core::TOK_ParenO, "(");
   GDCC::AS::SkipToken(in, GDCC::Core::TOK_ParenC, ")");

   return GDCC::IR::Type_StrEn();
}


//----------------------------------------------------------------------------|
// Global Functions                                                           |
//

namespace GDCC
{
   namespace AS
   {
      //
      // ParseType
      //
      IR::Type ParseType(Core::TokenStream &in, IR::Program &prog)
      {
         switch(static_cast<Core::StringIndex>(
            ExpectToken(in, Core::TOK_Identi, "identifier").get().str))
         {
            #define GDCC_IR_TypeList(name) \
               case Core::STR_##name: return ParseType_##name(in, prog);
            #include "IR/TypeList.hpp"

         default:
            in.unget();
            std::cerr << "ERROR: " << in.peek().pos << ": bad type: '"
               << in.peek().str << "'\n";
            throw EXIT_FAILURE;
         }
      }
   }
}

// EOF

