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

#include "IStream.hpp"

#include "GDCC/IR/Type.hpp"

#include <iostream>


//----------------------------------------------------------------------------|
// Static Functions                                                           |
//

//
// SkipComma
//
static void SkipComma(Asm::IStream &in)
{
   GDCC::Token tok;
   if((in >> tok, tok).tok != GDCC::TOK_Comma)
   {
      std::cerr << "ERROR: " << tok.pos << ": expected ,\n";
      throw EXIT_FAILURE;
   }
}

//
// SkipParenC
//
static void SkipParenC(Asm::IStream &in)
{
   GDCC::Token tok;
   if((in >> tok, tok).tok != GDCC::TOK_ParenC)
   {
      std::cerr << "ERROR: " << tok.pos << ": expected )\n";
      throw EXIT_FAILURE;
   }
}

//
// SkipParenO
//
static void SkipParenO(Asm::IStream &in)
{
   GDCC::Token tok;
   if((in >> tok, tok).tok != GDCC::TOK_ParenO)
   {
      std::cerr << "ERROR: " << tok.pos << ": expected (\n";
      throw EXIT_FAILURE;
   }
}

//
// ParseType_Empty
//
static GDCC::IR::Type_Empty ParseType_Empty(Asm::IStream &in)
{
   SkipParenO(in);
   SkipParenC(in);

   return GDCC::IR::Type_Empty();
}

//
// ParseType_Fixed
//
static GDCC::IR::Type_Fixed ParseType_Fixed(Asm::IStream &in)
{
   SkipParenO(in);
   auto bitsI = Asm::ParseFastU(in); SkipComma(in);
   auto bitsF = Asm::ParseFastU(in); SkipComma(in);
   bool bitsS = Asm::ParseFastU(in); SkipComma(in);
   bool satur = Asm::ParseFastU(in);
   SkipParenC(in);

   return GDCC::IR::Type_Fixed(bitsI, bitsF, bitsS, satur);
}

//
// ParseType_Float
//
static GDCC::IR::Type_Float ParseType_Float(Asm::IStream &in)
{
   SkipParenO(in);
   auto bitsI = Asm::ParseFastU(in); SkipComma(in);
   auto bitsF = Asm::ParseFastU(in); SkipComma(in);
   bool bitsS = Asm::ParseFastU(in); SkipComma(in);
   bool satur = Asm::ParseFastU(in);
   SkipParenC(in);

   return GDCC::IR::Type_Float(bitsI, bitsF, bitsS, satur);
}

//
// ParseType_Funct
//
static GDCC::IR::Type_Funct ParseType_Funct(Asm::IStream &in)
{
   GDCC::Token tok;

   SkipParenO(in);
   auto callT = Asm::ParseCallType((in >> tok, tok));
   SkipParenC(in);

   return GDCC::IR::Type_Funct(callT);
}

//
// ParseType_Multi
//
static GDCC::IR::Type_Multi ParseType_Multi(Asm::IStream &in)
{
   std::vector<GDCC::IR::Type> typev;
   GDCC::Token tok;

   SkipParenO(in);

   do typev.emplace_back(Asm::ParseType(in));
   while((in >> tok, tok).tok == GDCC::TOK_Comma);

   if(tok.tok != GDCC::TOK_ParenC)
   {
      std::cerr << "ERROR: " << tok.pos << ": expected )\n";
      throw EXIT_FAILURE;
   }

   GDCC::Array<GDCC::IR::Type> types{GDCC::Move, typev.begin(), typev.end()};

   return GDCC::IR::Type_Multi(std::move(types));
}

//
// ParseType_Point
//
static GDCC::IR::Type_Point ParseType_Point(Asm::IStream &in)
{
   GDCC::Token tok;

   SkipParenO(in);
   auto addrB = Asm::ParseAddrBase((in >> tok, tok)); SkipComma(in);
   auto addrO = Asm::ParseFastU(in);
   SkipParenC(in);

   return GDCC::IR::Type_Point(addrB, addrO);
}

//
// ParseType_StrEn
//
static GDCC::IR::Type_StrEn ParseType_StrEn(Asm::IStream &in)
{
   SkipParenO(in);
   SkipParenC(in);

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
   GDCC::IR::Type ParseType(IStream &in)
   {
      GDCC::Token tok;
      switch(static_cast<GDCC::StringIndex>((in >> tok, tok).str))
      {
         #define GDCC_IR_TypeList(name) \
            case GDCC::STR_##name: return ParseType_##name(in);
         #include "GDCC/IR/TypeList.hpp"

      default:
         std::cerr << "ERROR: " << tok.pos << ": bad type: '" << tok.str << "'\n";
         throw EXIT_FAILURE;
      }
   }
}

// EOF

