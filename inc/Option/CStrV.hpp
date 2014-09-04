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
      /// C-string vector option handler.

      /// Processes options by taking argument strings as-is and appending them
      /// to an array.
      ///
      class CStrV : public Base
      {
      public:
         /// Insertion constructor.
         CStrV(Program *program, Info const &optInfo, std::size_t argMax);

         /// Destructor.
         virtual ~CStrV();

         /// Returns an iterator to the beginning of the array.
         char const *const *begin() const {return strV;}

         /// Returns a pointer to the beginning of the array.
         char const *const *data() const {return strV;}

         /// Returns an iterator to one past the end of the array.
         char const *const *end() const {return strV + strC;}

         /// Removes the last element of the array.
         void pop(Base *opt = nullptr, Args args = Args());

         /// Returns the number of contained strings.
         std::size_t size() const {return strC;}


      protected:
         /// Virtual implementation of option processing.
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

