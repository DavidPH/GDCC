//-----------------------------------------------------------------------------
//
// Copyright (C) 2014-2018 David Hill
//
// See COPYING for license information.
//
//-----------------------------------------------------------------------------
//
// Option processing exception handling.
//
//-----------------------------------------------------------------------------

#include "Option/Exception.hpp"

#include "Option/Base.hpp"
#include "Option/StrUtil.hpp"

#include <cstdarg>
#include <cstdio>


//----------------------------------------------------------------------------|
// Extern Functions                                                           |
//

namespace GDCC::Option
{
   //
   // Exception constructor
   //
   Exception::Exception(std::unique_ptr<char const[]> &&s) :
      msg{std::move(s)}
   {
   }

   //
   // Exception copy constructor
   //
   Exception::Exception(Exception const &e) :
      msg{StrDup(e.msg.get())}
   {
   }

   //
   // Exception::what
   //
   char const *Exception::what() const noexcept
   {
      return msg.get();
   }

   //
   // Exception::Error
   //
   [[noreturn]]
   void Exception::Error(Args const &args, char const *format, ...)
   {
      va_list varg;
      std::size_t len, pos;

      if(args.nameL)
         len = std::snprintf(nullptr, 0, "'--%s': ", args.nameL);
      else if(args.nameS)
         len = std::snprintf(nullptr, 0, "'-%c': ", args.nameS);
      else
         len = std::snprintf(nullptr, 0, "--: ");

      va_start(varg, format);
      len += std::vsnprintf(nullptr, 0, format, varg);
      va_end(varg);

      std::unique_ptr<char[]> msg{new char[++len]};

      if(args.nameL)
         pos = std::snprintf(&msg[0], len, "'--%s': ", args.nameL);
      else if(args.nameS)
         pos = std::snprintf(&msg[0], len, "'-%c': ", args.nameS);
      else
         pos = std::snprintf(&msg[0], len, "--: ");

      va_start(varg, format);
      std::vsnprintf(&msg[pos], len - pos, format, varg);
      va_end(varg);

      throw Exception(std::move(msg));
   }
}

// EOF

