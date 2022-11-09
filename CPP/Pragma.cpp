//-----------------------------------------------------------------------------
//
// Copyright (C) 2013-2022 David Hill
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


//----------------------------------------------------------------------------|
// Static Functions                                                           |
//

namespace GDCC::CPP
{
   //
   // PragmaOnOff
   //
   static bool PragmaOnOff(bool &pragma, bool def, Core::TokenStream &in)
   {
      in.drop(Core::TOK_WSpace);

      switch(in.get().str)
      {
      case Core::STR_DEFAULT: pragma = def;   return true;
      case Core::STR_OFF:     pragma = false; return true;
      case Core::STR_ON:      pragma = true;  return true;
      default: return false;
      }
   }
}


//----------------------------------------------------------------------------|
// Extern Functions                                                           |
//

namespace GDCC::CPP
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
   // PragmaData default constructor
   //
   PragmaData::PragmaData() :
      stateCXLimitedRange{false},
      stateFEnvAccess    {false},
      stateFPContract    {false},
      stateFixedLiteral  {false},
      stateStrEntLiteral {false}
   {
   }

   //
   // PragmaData::drop
   //
   void PragmaData::drop()
   {
      PragmaDataBase::drop();

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
      PragmaDataBase::push();

      stackCXLimitedRange.push_back(stateCXLimitedRange);
      stackFEnvAccess    .push_back(stateFEnvAccess);
      stackFPContract    .push_back(stateFPContract);
      stackFixedLiteral  .push_back(stateFixedLiteral);
      stackStrEntLiteral .push_back(stateStrEntLiteral);
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
      if(!in.peek(Core::TOK_Identi)) return false;

      switch(in.get().str)
      {
      case Core::STR_library:
         in.drop(Core::TOK_WSpace);

         if(in.peek().tok != Core::TOK_String)
            return false;

         data.stateLibrary.emplace_back(Core::ParseStringC(in.get().str, 0, '"'));

         return true;

      default:
         return false;
      }
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
      if(!in.peek(Core::TOK_Identi)) return false;

      switch(in.get().str)
      {
      case Core::STR_FIXED_LITERAL:
         return PragmaOnOff(data.stateFixedLiteral, false, in);

      case Core::STR_STRENT_LITERAL:
         return PragmaOnOff(data.stateStrEntLiteral, false, in);

      case Core::STR_state:
         in.drop(Core::TOK_WSpace);
         if(!in.peek(Core::TOK_Identi)) return false;
         switch(in.get().str)
         {
         case Core::STR_restore: data.drop(); return true;
         case Core::STR_save:    data.push(); return true;
         default:                             return false;
         }

      default:
         return false;
      }
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
      if(!in.peek(Core::TOK_Identi)) return false;

      switch(in.get().str)
      {
      case Core::STR_CX_LIMITED_RANGE:
         return PragmaOnOff(data.stateCXLimitedRange, true, in);

      case Core::STR_FENV_ACCESS:
         return PragmaOnOff(data.stateFEnvAccess, false, in);

      case Core::STR_FP_CONTRACT:
         return PragmaOnOff(data.stateFPContract, true, in);

      default:
         return false;
      }
   }
}

// EOF

