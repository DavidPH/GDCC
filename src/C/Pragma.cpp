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


//----------------------------------------------------------------------------|
// Global Functions                                                           |
//

namespace C
{
   //
   // PragmaACS::pragma
   //
   bool PragmaACS::pragma(GDCC::TokenStream &in)
   {
      in.drop(GDCC::TOK_WSpace);
      if(!in.drop(GDCC::TOK_Identi, GDCC::STR_ACS)) return false;
      in.drop(GDCC::TOK_WSpace);

      if(in.peek().tok != GDCC::TOK_Identi) return true;

      switch(static_cast<GDCC::StringIndex>(in.get().str))
      {
      case GDCC::STR_library:
         in.drop(GDCC::TOK_WSpace);

         if(in.peek().tok != GDCC::TOK_String)
         {
            std::cerr << "ERROR: " << in.peek().pos << ": expected string-literal\n";
            throw EXIT_FAILURE;
         }

         pragmaACS_library.emplace_back(in.peek().str);

         break;

      default: break;
      }

      return true;
   }

   //
   // PragmaSTDC::pragma
   //
   bool PragmaSTDC::pragma(GDCC::TokenStream &in)
   {
      in.drop(GDCC::TOK_WSpace);
      if(!in.drop(GDCC::TOK_Identi, GDCC::STR_ACS)) return false;
      in.drop(GDCC::TOK_WSpace);

      if(in.peek().tok != GDCC::TOK_Identi) return true;

      switch(static_cast<GDCC::StringIndex>(in.get().str))
      {
      case GDCC::STR_CX_LIMITED_RANGE:
         PragmaOnOff(pragmaSTDC_CXLimitedRange, true, in);
         break;

      case GDCC::STR_FENV_ACCESS:
         PragmaOnOff(pragmaSTDC_FEnvAccess, false, in);
         break;

      case GDCC::STR_FP_CONTRACT:
         PragmaOnOff(pragmaSTDC_FPContract, true, in);
         break;

      default: break;
      }

      return true;
   }

   //
   // PragmaTest::pragma
   //
   bool PragmaTest::pragma(GDCC::TokenStream &in)
   {
      // This test pragma brought to you by drinking. And MageofMystra.
      in.drop(GDCC::TOK_WSpace);
      if(!in.drop(GDCC::TOK_Identi, GDCC::STR_fuck)) return false;
      in.drop(GDCC::TOK_WSpace);
      if(!in.drop(GDCC::TOK_Identi, GDCC::STR_it)) return true;
      EffIt();

      return true;
   }
}

// EOF

