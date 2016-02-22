//-----------------------------------------------------------------------------
//
// Copyright (C) 2013-2016 David Hill
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
      class Token;

      //
      // Exception
      //
      class Exception : public std::exception
      {
      public:
         Exception() : pos{nullptr, 0} {}
         Exception(Exception const &e);
         Exception(Exception &&) = default;
         explicit Exception(Origin pos_) noexcept : pos{pos_} {}

         virtual char const *what() const noexcept
            {if(!msg) genMsg(); return msg.get();}

         void setOrigin(Origin const &pos_) {pos = pos_;}

      protected:
         virtual void genMsg() const;

         Origin pos;

         mutable std::unique_ptr<char[]> msg;
      };

      //
      // ExceptFile
      //
      class ExceptFile : public Exception
      {
      public:
         ExceptFile(String filename_, String mode_) noexcept :
            Exception{{nullptr, 0}}, filename{filename_}, mode{mode_} {}

      private:
         virtual void genMsg() const;

         String const filename;
         String const mode;
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
            Exception{pos_}, str{str_} {}

      protected:
         virtual void genMsg() const;

         String const str;
      };

      //
      // ParseException
      //
      // Thrown by Parse and Get functions when encountering invalid syntax.
      //
      class ParseException : public Exception
      {
      public:
         using Exception::Exception;
      };

      //
      // ParseExceptExpect
      //
      class ParseExceptExpect : public ParseException
      {
      public:
         //
         // constructor
         //
         ParseExceptExpect(Origin pos_, String exp_, String got_, bool expQ_,
            bool gotQ_ = true) noexcept :
            ParseException{pos_},
            exp {exp_},
            got {got_},
            expQ{expQ_},
            gotQ{gotQ_}
         {
         }

         ParseExceptExpect(Token const &tok, String exp, bool expQ,
            bool gotQ = true) noexcept;

      protected:
         virtual void genMsg() const;

         Core::String const exp;
         Core::String const got;
         bool         const expQ : 1;
         bool         const gotQ : 1;
      };

      //
      // ParseExceptStr
      //
      // Carries a simple string message.
      //
      class ParseExceptStr : public ParseException
      {
      public:
         ParseExceptStr(Origin pos_, String str_) noexcept :
            ParseException{pos_}, str{str_} {}

      protected:
         virtual void genMsg() const;

         String const str;
      };
   }
}

#endif//GDCC__Core__Exception_H__

