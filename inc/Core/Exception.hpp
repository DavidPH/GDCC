//-----------------------------------------------------------------------------
//
// Copyright (C) 2013 David Hill
//
// See COPYING for license information.
//
//-----------------------------------------------------------------------------
//
// Base GDCC exception classes.
//
//-----------------------------------------------------------------------------

#ifndef GDCC__Core__Exception_H__
#define GDCC__Core__Exception_H__

#include "../Core/Origin.hpp"


//----------------------------------------------------------------------------|
// Types                                                                      |
//

namespace GDCC
{
   namespace Core
   {
      //
      // Exception
      //
      class Exception : public std::exception
      {
      public:
         Exception(Exception const &e);
         Exception(Exception &&) = default;
         explicit Exception(Origin pos_) noexcept : pos{pos_} {}

         virtual char const *what() const noexcept
            {if(!msg) genMsg(); return msg.get();}

      protected:
         virtual void genMsg() const;

         Origin pos;

         mutable std::unique_ptr<char[]> msg;
      };

      //
      // ExceptStr
      //
      // Carries a simple string message.
      //
      class ExceptStr : public Exception
      {
      public:
         ExceptStr(Origin pos_, String str_) noexcept :
            Exception{pos_}, str{str_.data()} {}
         ExceptStr(Origin pos_, char const *str_) noexcept :
            Exception{pos_}, str{str_} {}

      protected:
         virtual void genMsg() const;

         char const *str;
      };
   }
}

#endif//GDCC__Core__Exception_H__

