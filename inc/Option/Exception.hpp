//-----------------------------------------------------------------------------
//
// Copyright (C) 2014 David Hill
//
// See COPYING for license information.
//
//-----------------------------------------------------------------------------
//
// Option processing exception handling.
//
//-----------------------------------------------------------------------------

#ifndef GDCC__Option__Exception_H__
#define GDCC__Option__Exception_H__

#include <memory>
#include <stdexcept>


//----------------------------------------------------------------------------|
// Types                                                                      |
//

namespace GDCC
{
   namespace Option
   {
      class Args;

      //
      // Exception
      //
      class Exception : public std::exception
      {
      public:
         Exception(Exception const &e);
         Exception(Exception &&e) = default;

         virtual char const *what() const noexcept;


         [[noreturn]]
         static void Error(Args const &args, char const *format, ...);

      private:
         explicit Exception(std::unique_ptr<char const[]> &&s);

         std::unique_ptr<char const[]> const msg;
      };
   }
}

#endif//GDCC__Option__Exception_H__

