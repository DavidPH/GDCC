//-----------------------------------------------------------------------------
//
// Copyright (C) 2013 David Hill
//
// See COPYING for license information.
//
//-----------------------------------------------------------------------------
//
// General parsing utilities.
//
//-----------------------------------------------------------------------------

#ifndef GDCC__Parse_H__
#define GDCC__Parse_H__

#include "String.hpp"

#include <istream>
#include <stdexcept>
#include <string>
#include <utility>


//----------------------------------------------------------------------------|
// Types                                                                      |
//

namespace GDCC
{
   //
   // ParseError
   //
   class ParseError : public std::exception
   {
   public:
      explicit ParseError(String msg_) : msg{msg_} {}

      virtual char const *what() const noexcept {return msg.getData().str;}

      String msg;
   };
}


//----------------------------------------------------------------------------|
// Global Functions                                                           |
//

namespace GDCC
{
   char ParseEscapeC(std::istream &in);

   std::pair<int, char const *> ParseNumberPrefixC(char const *in);

   std::string ParseStringC(std::string const &in);

   void ReadEscapeC(std::istream &in, std::string &out);

   std::string ReadNumberC(std::istream &in);

   std::string ReadStringC(std::istream &in, char term);
}

#endif//GDCC__Parse_H__

