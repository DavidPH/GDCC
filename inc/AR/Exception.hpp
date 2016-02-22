//-----------------------------------------------------------------------------
//
// Copyright (C) 2016 David Hill
//
// See COPYING for license information.
//
//-----------------------------------------------------------------------------
//
// Archive exceptions.
//
//-----------------------------------------------------------------------------

#ifndef GDCC__AR__Exception_H__
#define GDCC__AR__Exception_H__

#include "../Core/Exception.hpp"


//----------------------------------------------------------------------------|
// Types                                                                      |
//

namespace GDCC
{
   namespace AR
   {
      //
      // ReadError
      //
      class ReadError : public Core::Exception
      {
      public:
         ReadError(char const *err_) noexcept : err{err_} {}

         virtual void genMsg() const;

      private:
         char const *err;
      };
   }
}

#endif//GDCC__AR__Exception_H__

