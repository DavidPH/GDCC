//-----------------------------------------------------------------------------
//
// Copyright (C) 2013-2018 David Hill
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
// Extern Functions                                                           |
//

namespace GDCC::Core
{
   //
   // Exception copy constructor
   //
   Exception::Exception(Exception const &e) :
      pos{e.pos},
      msg{e.msg ? StrDup(e.msg.get()) : nullptr}
   {
   }

   //
   // Exception::genMsg
   //
   void Exception::genMsg() const
   {
      std::ostringstream oss;
      putOrigin(oss) << "unknown error";
      setMsg(oss.str());
   }

   //
   // Exception::putOrigin
   //
   std::ostream &Exception::putOrigin(std::ostream &out) const
   {
      if(pos.file) out << pos << ": ";
      return out;
   }

   //
   // Exception::setMsg
   //
   void Exception::setMsg(std::string const &str) const
   {
      msg = StrDup(str. data(), str.size());
   }

   //
   // ExceptFile::genMsg
   //
   void ExceptFile::genMsg() const
   {
      std::ostringstream oss;
      putOrigin(oss) << "could not open '" << filename << "' for " << mode;
      setMsg(oss.str());
   }

   //
   // ExceptStr::genMsg
   //
   void ExceptStr::genMsg() const
   {
      std::ostringstream oss;
      putOrigin(oss) << str;
      auto const &tmp = oss.str();
      msg = StrDup(tmp.data(), tmp.size());
   }

   //
   // ExceptUndef::genMsg
   //
   void ExceptUndef::genMsg() const
   {
      std::ostringstream oss;
      putOrigin(oss) << type << " undefined: '" << name << '\'';
      setMsg(oss.str());
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

      putOrigin(oss);

      oss << " expected ";
      if(expQ) oss << '\'';
      oss << exp;
      if(expQ) oss << '\'';

      oss << " got ";
      if(gotQ) oss << '\'';
      oss << got;
      if(gotQ) oss << '\'';

      setMsg(oss.str());
   }

   //
   // ParseExceptStr::genMsg
   //
   void ParseExceptStr::genMsg() const
   {
      std::ostringstream oss;
      putOrigin(oss) << str;
      setMsg(oss.str());
   }
}

// EOF

