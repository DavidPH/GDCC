//-----------------------------------------------------------------------------
//
// Copyright (C) 2013 David Hill
//
// See COPYING for license information.
//
//-----------------------------------------------------------------------------
//
// #include directive token buffer.
//
//-----------------------------------------------------------------------------

#include "IncludeDTBuf.hpp"

#include "Macro.hpp"
#include "TStream.hpp"

#include <fstream>
#include <iostream>


//----------------------------------------------------------------------------|
// Global Functions                                                           |
//

namespace C
{
   //
   // IncludeDTBuf destructor
   //
   IncludeDTBuf::~IncludeDTBuf()
   {
   }

   //
   // IncludeDTBuf::directive
   //
   bool IncludeDTBuf::directive(GDCC::Token const &tok)
   {
      if(tok.tok != GDCC::TOK_Identi || tok.str != GDCC::STR_include)
         return false;

      // Clear directive name.
      src.get();

      // Read header token.
      IStream::NeedHeader = true;
      auto hdr = src.get();

      // Process header.
      if(hdr.tok == GDCC::TOK_String)
      {
         if(tryIncUsr(hdr.str) || tryIncSys(hdr.str))
            return true;
      }
      else if(hdr.tok == GDCC::TOK_Header)
      {
         if(tryIncSys(hdr.str))
            return true;
      }

      // Try the third syntax.

      std::cerr << "STUB: " __FILE__ << ':' << __LINE__ << '\n';
      throw EXIT_FAILURE;
   }

   //
   // IncludeDTBuf::tryIncSys
   //
   bool IncludeDTBuf::tryIncSys(GDCC::String name)
   {
      std::unique_ptr<std::filebuf> fbuf{new std::filebuf()};

      if(!fbuf->open(name.getData().str, std::ios_base::in))
         return false;

      Macro::LinePush(Macro::Stringize(name));

      str = std::move(fbuf);
      inc.reset(new IncStream(*str, name));

      return true;
   }

   //
   // IncludeDTBuf::tryIncUsr
   //
   bool IncludeDTBuf::tryIncUsr(GDCC::String)
   {
      return false;
   }

   //
   // IncludeDTBuf::underflow
   //
   void IncludeDTBuf::underflow()
   {
      if(tptr() != tend()) return;

      if(inc)
      {
         if(*inc >> buf[0])
            return sett(buf, buf, buf + 1);

         Macro::LineDrop();
         inc.reset();
         str.reset();
      }

      DirectiveTBuf::underflow();
   }
}

// EOF

