//-----------------------------------------------------------------------------
//
// Copyright (C) 2013 David Hill
//
// See COPYING for license information.
//
//-----------------------------------------------------------------------------
//
// Pragma parsing and handling.
//
//-----------------------------------------------------------------------------

#include "Pragma.hpp"

#include "GDCC/Parse.hpp"
#include "GDCC/TokenStream.hpp"

#include "GDCC/IR/Import.hpp"

#include <climits>
#include <csignal>
#include <ctime>
#include <iostream>


//----------------------------------------------------------------------------|
// Global Variables                                                           |
//

namespace C
{
   bool Pragma_STDC_CXLimitedRange = true;
   bool Pragma_STDC_FEnvAccess     = false;
   bool Pragma_STDC_FPContract     = true;
}


//----------------------------------------------------------------------------|
// Static Functions                                                           |
//

//
// EffIt
//
// Sometimes, one just cannot be arsed to figure out how to write some code.
//
static void EffIt()
{
   // Seed RNG.
   {
      unsigned int seed = 0;

      union {std::time_t time; unsigned char data[sizeof(time)];};

      std::time(&time);

      for(unsigned char c : data) if(c)
         seed = (seed << (sizeof(seed) * CHAR_BIT / sizeof(time))) + c;

      std::srand(seed);
   }

   // Print warning.
   static char const *const msg[] =
   {
      "Program will now crash.",
      "This is all your fault!",
      "I trusted you!",
      "Instead of ignoring your pragma, I'ma segfault.",
      "I'm afraid I can't let you do that.",
   };

   std::cerr << "Warning: Unknown pragma. "
      << msg[std::rand() % (sizeof(msg) / sizeof(*msg))] << std::endl;

   // Fuck it.
   std::raise(SIGSEGV);
}

//
// PragmaACS
//
static void PragmaACS(GDCC::TokenStream &in)
{
   in.drop(GDCC::TOK_WSpace);

   switch(static_cast<GDCC::StringIndex>(in.get().str))
   {
   case GDCC::STR_library:
      in.drop(GDCC::TOK_WSpace);

      if(in.peek().tok != GDCC::TOK_String)
      {
         std::cerr << "ERROR: " << in.peek().pos << ": expected string-literal\n";
         throw EXIT_FAILURE;
      }

      GDCC::IR::Import::Get(GDCC::ParseStringC(in.get().str));
      break;

   default: break;
   }
}

//
// PragmaOnOff
//
static void PragmaOnOff(bool &pragma, bool def, GDCC::TokenStream &in)
{
   in.drop(GDCC::TOK_WSpace);

   switch(static_cast<GDCC::StringIndex>(in.get().str))
   {
   case GDCC::STR_DEFAULT: pragma = def;   break;
   case GDCC::STR_OFF:     pragma = false; break;
   case GDCC::STR_ON:      pragma = true;  break;
   default: break;
   }
}

//
// PragmaSTDC
//
static void PragmaSTDC(GDCC::TokenStream &in)
{
   in.drop(GDCC::TOK_WSpace);

   switch(static_cast<GDCC::StringIndex>(in.get().str))
   {
   case GDCC::STR_CX_LIMITED_RANGE:
      PragmaOnOff(C::Pragma_STDC_CXLimitedRange, true, in);
      break;

   case GDCC::STR_FENV_ACCESS:
      PragmaOnOff(C::Pragma_STDC_FEnvAccess, false, in);
      break;

   case GDCC::STR_FP_CONTRACT:
      PragmaOnOff(C::Pragma_STDC_FPContract, true, in);
      break;

   default: break;
   }
}


//----------------------------------------------------------------------------|
// Global Functions                                                           |
//

namespace C
{
   //
   // Pragma
   //
   void Pragma(GDCC::TokenStream &in)
   {
      if(in.peek().tok == GDCC::TOK_WSpace) in.get();

      if(in.peek().tok != GDCC::TOK_Identi) return;

      switch(static_cast<GDCC::StringIndex>(in.get().str))
      {
      case GDCC::STR_ACS:
         PragmaACS(in);
         break;

      case GDCC::STR_STDC:
         PragmaSTDC(in);
         break;

      case GDCC::STR_fuck:
         // This test pragma brought to you by drinking. And MageofMystra.
         if(in.peek().tok == GDCC::TOK_WSpace) in.get();
         if(in.peek().tok == GDCC::TOK_Identi && in.peek().str == GDCC::STR_it)
            EffIt();

         break;

      default:
         break;
      }
   }
}

// EOF

