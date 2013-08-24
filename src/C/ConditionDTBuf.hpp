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

#ifndef C__ConditionDTBuf_H__
#define C__ConditionDTBuf_H__

#include "DirectiveTBuf.hpp"

#include <vector>


//----------------------------------------------------------------------------|
// Types                                                                      |
//

namespace C
{
   //
   // ConditionDTBuf
   //
   class ConditionDTBuf : public DirectiveTBuf
   {
   public:
      explicit ConditionDTBuf(GDCC::TokenBuf &src_) : DirectiveTBuf{src_} {}

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

      virtual bool directive(GDCC::Token const &tok);

      bool getSkip();

      bool isSkip() const {return !state.empty() && state.back().isSkip;}

      virtual void underflow();

      std::vector<CondState> state;
   };
}

#endif//C__ConditionDTBuf_H__

