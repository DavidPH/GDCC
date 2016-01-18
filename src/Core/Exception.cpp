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

#include "Core/Exception.hpp"

#include "Core/Token.hpp"

#include <sstream>


//----------------------------------------------------------------------------|
// Global Functions                                                           |
//

namespace GDCC
{
   namespace Core
   {
      //
      // Exception::genMsg
      //
      void Exception::genMsg() const
      {
         std::ostringstream oss;
         oss << "ERROR: " << pos << ": unknown error";
         auto const &tmp = oss.str();
         msg = StrDup(tmp.data(), tmp.size());
      }

      //
      // ExceptFile::genMsg
      //
      void ExceptFile::genMsg() const
      {
         std::ostringstream oss;
         oss << "ERROR: ";
         if(pos.file) oss << pos << ": ";
         oss << "could not open '" << filename << "' for " << mode;
         auto const &tmp = oss.str();
         msg = StrDup(tmp.data(), tmp.size());
      }

      //
      // ExceptStr::genMsg
      //
      void ExceptStr::genMsg() const
      {
         std::ostringstream oss;
         oss << "ERROR: " << pos << ": " << str;
         auto const &tmp = oss.str();
         msg = StrDup(tmp.data(), tmp.size());
      }

      //
      // ParseExceptExpect constructor
      //
      ParseExceptExpect::ParseExceptExpect(Token const &tok, String exp_,
         bool expQ_, bool gotQ_) noexcept :
         ParseException{tok.pos},
         exp {exp_},
         got {tok.str},
         expQ{expQ_},
         gotQ{gotQ_}
      {
      }

      //
      // ParseExceptExpect::genMsg
      //
      void ParseExceptExpect::genMsg() const
      {
         std::ostringstream oss;

         oss << "ERROR: " << pos << ":";

         oss << " expected ";
         if(expQ) oss << '\'';
         oss << exp;
         if(expQ) oss << '\'';

         oss << " got ";
         if(gotQ) oss << '\'';
         oss << got;
         if(gotQ) oss << '\'';

         auto const &tmp = oss.str();
         msg = StrDup(tmp.data(), tmp.size());
      }

      //
      // ParseExceptStr::genMsg
      //
      void ParseExceptStr::genMsg() const
      {
         std::ostringstream oss;
         oss << "ERROR: " << pos << ": " << str;
         auto const &tmp = oss.str();
         msg = StrDup(tmp.data(), tmp.size());
      }
   }
}

// EOF

