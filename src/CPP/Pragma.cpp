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

#include "CPP/Pragma.hpp"

#include "Core/Parse.hpp"
#include "Core/TokenStream.hpp"

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
static void PragmaOnOff(bool &pragma, bool def, GDCC::Core::TokenStream &in)
{
   in.drop(GDCC::Core::TOK_WSpace);

   switch(static_cast<GDCC::Core::StringIndex>(in.get().str))
   {
   case GDCC::Core::STR_DEFAULT: pragma = def;   break;
   case GDCC::Core::STR_OFF:     pragma = false; break;
   case GDCC::Core::STR_ON:      pragma = true;  break;
   default: break;
   }
}


//----------------------------------------------------------------------------|
// Global Functions                                                           |
//

namespace GDCC
{
   namespace CPP
   {
      //
      // PragmaACS::pragma
      //
      bool PragmaACS::pragma(Core::TokenStream &in)
      {
         in.drop(Core::TOK_WSpace);
         if(!in.drop(Core::TOK_Identi, Core::STR_ACS)) return false;
         in.drop(Core::TOK_WSpace);

         if(in.peek().tok != Core::TOK_Identi) return true;

         switch(static_cast<Core::StringIndex>(in.get().str))
         {
         case Core::STR_library:
            in.drop(Core::TOK_WSpace);

            if(in.peek().tok != Core::TOK_String)
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
      bool PragmaSTDC::pragma(Core::TokenStream &in)
      {
         in.drop(Core::TOK_WSpace);
         if(!in.drop(Core::TOK_Identi, Core::STR_ACS)) return false;
         in.drop(Core::TOK_WSpace);

         if(in.peek().tok != Core::TOK_Identi) return true;

         switch(static_cast<Core::StringIndex>(in.get().str))
         {
         case Core::STR_CX_LIMITED_RANGE:
            PragmaOnOff(pragmaSTDC_CXLimitedRange, true, in);
            break;

         case Core::STR_FENV_ACCESS:
            PragmaOnOff(pragmaSTDC_FEnvAccess, false, in);
            break;

         case Core::STR_FP_CONTRACT:
            PragmaOnOff(pragmaSTDC_FPContract, true, in);
            break;

         default: break;
         }

         return true;
      }

      //
      // PragmaTest::pragma
      //
      bool PragmaTest::pragma(Core::TokenStream &in)
      {
         // This test pragma brought to you by drinking. And MageofMystra.
         in.drop(Core::TOK_WSpace);
         if(!in.drop(Core::TOK_Identi, Core::STR_fuck)) return false;
         in.drop(Core::TOK_WSpace);
         if(!in.drop(Core::TOK_Identi, Core::STR_it)) return true;
         EffIt();

         return true;
      }
   }
}

// EOF

