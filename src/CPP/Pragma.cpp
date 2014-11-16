//-----------------------------------------------------------------------------
//
// Copyright (C) 2013-2014 David Hill
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

namespace GDCC
{
   namespace CPP
   {
      //
      // PragmaOnOff
      //
      static void PragmaOnOff(bool &pragma, bool def, Core::TokenStream &in)
      {
         in.drop(Core::TOK_WSpace);

         switch(in.get().str)
         {
         case Core::STR_DEFAULT: pragma = def;   break;
         case Core::STR_OFF:     pragma = false; break;
         case Core::STR_ON:      pragma = true;  break;
         default: break;
         }
      }
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

         switch(in.get().str)
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
      // PragmaGDCC::pragma
      //
      bool PragmaGDCC::pragma(Core::TokenStream &in)
      {
         in.drop(Core::TOK_WSpace);
         if(!in.drop(Core::TOK_Identi, Core::STR_GDCC)) return false;
         in.drop(Core::TOK_WSpace);

         if(in.peek().tok != Core::TOK_Identi) return true;

         switch(in.get().str)
         {
         case Core::STR_FIXED_LITERAL:
            PragmaOnOff(pragmaGDCC_FixedLiteral, false, in);
            break;

         case Core::STR_STRENT_LITERAL:
            PragmaOnOff(pragmaGDCC_StrEntLiteral, false, in);
            break;

         default: break;
         }

         return true;
      }

      //
      // PragmaGDCC::pragmaDrop
      //
      void PragmaGDCC::pragmaDrop()
      {
         if(pragmaGDCC_FixedLiteral_Stack.empty()) return;

         pragmaGDCC_FixedLiteral  = pragmaGDCC_FixedLiteral_Stack.back();
         pragmaGDCC_StrEntLiteral = pragmaGDCC_StrEntLiteral_Stack.back();

         pragmaGDCC_FixedLiteral_Stack.pop_back();
         pragmaGDCC_StrEntLiteral_Stack.pop_back();
      }

      //
      // PragmaGDCC::pragmaPush
      //
      void PragmaGDCC::pragmaPush()
      {
         pragmaGDCC_FixedLiteral_Stack.emplace_back(pragmaGDCC_FixedLiteral);
         pragmaGDCC_StrEntLiteral_Stack.emplace_back(pragmaGDCC_StrEntLiteral);
      }

      //
      // PragmaSTDC::pragma
      //
      bool PragmaSTDC::pragma(Core::TokenStream &in)
      {
         in.drop(Core::TOK_WSpace);
         if(!in.drop(Core::TOK_Identi, Core::STR_STDC)) return false;
         in.drop(Core::TOK_WSpace);

         if(in.peek().tok != Core::TOK_Identi) return true;

         switch(in.get().str)
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
      // PragmaSTDC::pragmaDrop
      //
      void PragmaSTDC::pragmaDrop()
      {
         if(pragmaSTDC_CXLimitedRange_Stack.empty()) return;

         pragmaSTDC_CXLimitedRange = pragmaSTDC_CXLimitedRange_Stack.back();
         pragmaSTDC_FEnvAccess     = pragmaSTDC_FEnvAccess_Stack.back();
         pragmaSTDC_FPContract     = pragmaSTDC_FPContract_Stack.back();

         pragmaSTDC_CXLimitedRange_Stack.pop_back();
         pragmaSTDC_FEnvAccess_Stack.pop_back();
         pragmaSTDC_FPContract_Stack.pop_back();
      }

      //
      // PragmaSTDC::pragmaPush
      //
      void PragmaSTDC::pragmaPush()
      {
         pragmaSTDC_CXLimitedRange_Stack.emplace_back(pragmaSTDC_CXLimitedRange);
         pragmaSTDC_FEnvAccess_Stack.emplace_back(pragmaSTDC_FEnvAccess);
         pragmaSTDC_FPContract_Stack.emplace_back(pragmaSTDC_FPContract);
      }

      //
      // PragmaTest::pragma
      //
      bool PragmaTest::pragma(Core::TokenStream &in)
      {
         // This test pragma brought to you by drinking. And MageofMystra.
         // Please do not drink and MageofMystra.
         in.drop(Core::TOK_WSpace);
         if(!in.drop(Core::TOK_Identi, Core::STR_fuck)) return false;
         in.drop(Core::TOK_WSpace);
         if(!in.drop(Core::TOK_Identi, Core::STR_it)) return true;

         std::cerr << "Warning: Unknown pragma." << std::endl;
         // Fuck it.
         std::raise(SIGSEGV);

         return true;
      }
   }
}

// EOF

