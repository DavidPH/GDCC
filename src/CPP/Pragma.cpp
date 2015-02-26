//-----------------------------------------------------------------------------
//
// Copyright (C) 2013-2015 David Hill
//
// See COPYING for license information.
//
//-----------------------------------------------------------------------------
//
// Pragma parsing and handling.
//
//-----------------------------------------------------------------------------

#include "CPP/Pragma.hpp"

#include "Core/Exception.hpp"
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
      // PragmaDataBase::drop
      //
      void PragmaDataBase::drop()
      {
      }

      //
      // PragmaDataBase::push
      //
      void PragmaDataBase::push()
      {
      }

      //
      // PragmaData::drop
      //
      void PragmaData::drop()
      {
         if(stackFixedLiteral.empty()) return;

         stateCXLimitedRange = stackCXLimitedRange.back();
         stateFEnvAccess     = stackFEnvAccess    .back();
         stateFPContract     = stackFPContract    .back();
         stateFixedLiteral   = stackFixedLiteral  .back();
         stateStrEntLiteral  = stackStrEntLiteral .back();

         stackCXLimitedRange.pop_back();
         stackFEnvAccess    .pop_back();
         stackFPContract    .pop_back();
         stackFixedLiteral  .pop_back();
         stackStrEntLiteral .pop_back();
      }

      //
      // PragmaData::push
      //
      void PragmaData::push()
      {
         stackCXLimitedRange.emplace_back(stateCXLimitedRange);
         stackFEnvAccess    .emplace_back(stateFEnvAccess);
         stackFPContract    .emplace_back(stateFPContract);
         stackFixedLiteral  .emplace_back(stateFixedLiteral);
         stackStrEntLiteral .emplace_back(stateStrEntLiteral);
      }

      //
      // PragmaParserACS::parse
      //
      bool PragmaParserACS::parse(Core::Token const *toks, std::size_t n)
      {
         Core::ArrayTStream in{toks, n};

         in.drop(Core::TOK_WSpace);
         if(!in.drop(Core::TOK_Identi, Core::STR_ACS)) return false;
         in.drop(Core::TOK_WSpace);

         if(in.peek().tok != Core::TOK_Identi) return true;

         switch(in.get().str)
         {
         case Core::STR_library:
            in.drop(Core::TOK_WSpace);

            if(in.peek().tok != Core::TOK_String)
               throw Core::ParseExceptExpect(in.peek(), "string-literal", false);

            data.stateLibrary.emplace_back(in.peek().str);

            break;

         default: break;
         }

         return true;
      }

      //
      // PragmaParserGDCC::parse
      //
      bool PragmaParserGDCC::parse(Core::Token const *toks, std::size_t n)
      {
         Core::ArrayTStream in{toks, n};

         in.drop(Core::TOK_WSpace);
         if(!in.drop(Core::TOK_Identi, Core::STR_GDCC)) return false;
         in.drop(Core::TOK_WSpace);

         if(in.peek().tok != Core::TOK_Identi) return true;

         switch(in.get().str)
         {
         case Core::STR_FIXED_LITERAL:
            PragmaOnOff(data.stateFixedLiteral, false, in);
            break;

         case Core::STR_STRENT_LITERAL:
            PragmaOnOff(data.stateStrEntLiteral, false, in);
            break;

         default: break;
         }

         return true;
      }

      //
      // PragmaParserSTDC::parse
      //
      bool PragmaParserSTDC::parse(Core::Token const *toks, std::size_t n)
      {
         Core::ArrayTStream in{toks, n};

         in.drop(Core::TOK_WSpace);
         if(!in.drop(Core::TOK_Identi, Core::STR_STDC)) return false;
         in.drop(Core::TOK_WSpace);

         if(in.peek().tok != Core::TOK_Identi) return true;

         switch(in.get().str)
         {
         case Core::STR_CX_LIMITED_RANGE:
            PragmaOnOff(data.stateCXLimitedRange, true, in);
            break;

         case Core::STR_FENV_ACCESS:
            PragmaOnOff(data.stateFEnvAccess, false, in);
            break;

         case Core::STR_FP_CONTRACT:
            PragmaOnOff(data.stateFPContract, true, in);
            break;

         default: break;
         }

         return true;
      }
   }
}

// EOF

