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

      /// Basic option-related exception class.

      /// Messages are generated using information from an Args object, and are
      /// meant to be directly presentable to the user.
      ///
      class Exception : public std::exception
      {
      public:
         Exception() = delete;

         /// Copy constructor.
         Exception(Exception const &e);

         /// Move constructor.
         Exception(Exception &&e) = default;

         /// Returns the exception message.
         virtual char const *what() const noexcept;


         /// Throws an exception with a formatted message.
         [[noreturn]]
         static void Error(Args const &args, char const *format, ...);

      private:
         explicit Exception(std::unique_ptr<char const[]> &&s);

         std::unique_ptr<char const[]> const msg;
      };
   }
}

#endif//GDCC__Option__Exception_H__

