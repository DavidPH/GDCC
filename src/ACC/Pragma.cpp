//-----------------------------------------------------------------------------
//
// Copyright (C) 2015 David Hill
//
// See COPYING for license information.
//
//-----------------------------------------------------------------------------
//
// Pragma parsing and handling.
//
//-----------------------------------------------------------------------------

#include "ACC/Pragma.hpp"

#include "Core/Exception.hpp"
#include "Core/TokenStream.hpp"


//----------------------------------------------------------------------------|
// Static Functions                                                           |
//

namespace GDCC
{
   namespace ACC
   {
      //
      // PragmaOnOff
      //
      static bool PragmaOnOff(Core::TokenStream &in, bool def)
      {
         in.drop(Core::TOK_WSpace);

         if(in.peek(Core::TOK_Identi)) switch(in.get().str)
         {
         case Core::STR_default: return def;
         case Core::STR_off:     return false;
         case Core::STR_on:      return true;
         default: in.unget(); break;
         }

         throw Core::ParseExceptExpect(in.peek(), "on-off-switch", false);
      }
   }
}


//----------------------------------------------------------------------------|
// Global Functions                                                           |
//

namespace GDCC
{
   namespace ACC
   {
      //
      // PragmaData default constructor
      //
      PragmaData::PragmaData() :
         CPP::PragmaData{},
         stateFixedType{}
      {
      }

      //
      // PragmaData::drop
      //
      void PragmaData::drop()
      {
         CPP::PragmaData::drop();

         if(stackFixedType.empty()) return;

         stateFixedType = stackFixedType.back();

         stackFixedType.pop_back();
      }

      //
      // PragmaData::push
      //
      void PragmaData::push()
      {
         CPP::PragmaData::push();

         stackFixedType.emplace_back(stateFixedType);
      }

      //
      // PragmaParserBare::parse
      //
      bool PragmaParserBare::parse(Core::Token const *toks, std::size_t n)
      {
         Core::ArrayTStream in{toks, n};

         in.drop(Core::TOK_WSpace);
         if(in.peek().tok != Core::TOK_Identi) return true;
         switch(in.get().str)
         {
         case Core::STR_fixed:
            data.stateFixedLiteral = data.stateFixedType = PragmaOnOff(in, false);
            break;

         case Core::STR_fixed_literal:
            data.stateFixedLiteral = PragmaOnOff(in, false);
            break;

         case Core::STR_fixed_type:
            data.stateFixedType = PragmaOnOff(in, false);
            break;

         default: break;
         }

         return true;
      }
   }
}

// EOF

