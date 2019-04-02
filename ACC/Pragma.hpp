//-----------------------------------------------------------------------------
//
// Copyright (C) 2015-2018 David Hill
//
// See COPYING for license information.
//
//-----------------------------------------------------------------------------
//
// Pragma parsing and handling.
//
//-----------------------------------------------------------------------------

#ifndef GDCC__ACC__Pragma_H__
#define GDCC__ACC__Pragma_H__

#include "../ACC/Types.hpp"

#include "../CPP/Pragma.hpp"


//----------------------------------------------------------------------------|
// Types                                                                      |
//

namespace GDCC::ACC
{
   //
   // PragmaData
   //
   class PragmaData : public CPP::PragmaData
   {
   public:
      PragmaData();

      virtual void drop();
      virtual void push();

      bool stateBlockScope;
      bool stateDefineRaw;
      bool stateFixedType;

   protected:
      std::vector<bool> stackBlockScope;
      std::vector<bool> stackDefineRaw;
      std::vector<bool> stackFixedType;
   };

   //
   // PragmaParserBare
   //
   // Handles #pragma ...
   //
   class PragmaParserBare
   {
   public:
      PragmaParserBare(PragmaData &data_) : data{data_} {}

      bool parse(Core::Token const *toks, std::size_t n);

   protected:
      PragmaData &data;
   };
}

#endif//GDCC__ACC__Pragma_H__

