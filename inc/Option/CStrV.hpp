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
         CStrV(Program *program, Info const &optInfo, std::size_t argMax);

         virtual ~CStrV();

         char const *const *begin() const {return strV;}

         char const *const *data() const {return strV;}

         char const *const *end() const {return strV + strC;}

         void pop(Base *opt = nullptr, Args args = Args());

         std::size_t size() const {return strC;}

      protected:
         virtual std::size_t v_process(Args const &args);

      private:
         bool        *strF; // free needed
         char const **strV; // vector
         std::size_t  strA; // vector size
         std::size_t  strC; // vector used
         std::size_t  argM; // max args consumed
      };
   }
}

#endif//GDCC__Option__CStrV_H__

