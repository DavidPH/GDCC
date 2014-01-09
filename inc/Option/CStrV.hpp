//-----------------------------------------------------------------------------
//
// Copyright (C) 2014 David Hill
//
// See COPYING for license information.
//
//-----------------------------------------------------------------------------
//
// C-string vector option handling.
//
//-----------------------------------------------------------------------------

#ifndef GDCC__Option__CStrV_H__
#define GDCC__Option__CStrV_H__

#include "../Option/Base.hpp"


//----------------------------------------------------------------------------|
// Types                                                                      |
//

namespace GDCC
{
   namespace Option
   {
      //
      // CStrV
      //
      class CStrV : public Base
      {
      public:
         CStrV(Program *list, Info const &info, std::size_t argMax);
         virtual ~CStrV();

         char const *const *begin() const {return strV;}

         char const *const *end() const {return strV + strC;}

         // Passes last arg to opt (if any), then removes the arg from list.
         void pop(Base *opt = nullptr, Args args = Args());

         std::size_t  strC; // count
         char const **strV; // vector

      protected:
         virtual std::size_t v_process(Args const &args);

      private:
         bool       *strF; // free needed
         std::size_t strA; // alloc
         std::size_t argMax;
      };
   }
}

#endif//GDCC__Option__CStrV_H__

