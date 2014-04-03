//-----------------------------------------------------------------------------
//
// Copyright (C) 2013 David Hill
//
// See COPYING for license information.
//
//-----------------------------------------------------------------------------
//
// Conditional inclusion directive token buffer.
//
//-----------------------------------------------------------------------------

#ifndef GDCC__CPP__ConditionDTBuf_H__
#define GDCC__CPP__ConditionDTBuf_H__

#include "../CPP/DirectiveTBuf.hpp"

#include <vector>


//----------------------------------------------------------------------------|
// Types                                                                      |
//

namespace GDCC
{
   namespace CPP
   {
      class MacroMap;

      //
      // ConditionDTBuf
      //
      class ConditionDTBuf : public DirectiveTBuf
      {
      public:
         ConditionDTBuf(Core::TokenBuf &src_, MacroMap &macros_) :
            DirectiveTBuf{src_}, macros(macros_) {}

      protected:
         //
         // CondState
         //
         struct CondState
         {
            explicit CondState(bool dead) :
               isDead{dead}, isElif{false}, isElse{false}, isSkip{dead} {}

            bool isDead : 1; // If true, parent state is skipping.
            bool isElif : 1; // If true, a block has been unskipped.
            bool isElse : 1; // If true, an #else has been encountered.
            bool isSkip : 1; // If true, skip tokens.
         };

         virtual bool directive(Core::Token const &tok);

         bool getSkip();

         bool isSkip() const {return !state.empty() && state.back().isSkip;}

         virtual void underflow();

         std::vector<CondState> state;
         MacroMap              &macros;
      };

      //
      // DefinedTBuf
      //
      // Handles the defined operator.
      //
      class DefinedTBuf : public Core::TokenBuf
      {
      public:
         DefinedTBuf(Core::TokenBuf &src_, MacroMap &macros_) :
            macros(macros_), src(src_) {}

      protected:
         virtual void underflow();

         Core::Token     buf[1];
         MacroMap       &macros;
         Core::TokenBuf &src;
      };

      //
      // IdentiTBuf
      //
      // Converts TOK_Identi tokens into TOK_Number "0" tokens.
      //
      class IdentiTBuf : public Core::TokenBuf
      {
      public:
         explicit IdentiTBuf(Core::TokenBuf &src_) : src(src_) {}

      protected:
         virtual void underflow();

         Core::TokenBuf &src;
         Core::Token buf[1];
      };
   }
}

#endif//GDCC__CPP__ConditionDTBuf_H__

