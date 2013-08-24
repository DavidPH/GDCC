//-----------------------------------------------------------------------------
//
// Copyright (C) 2013 David Hill
//
// See COPYING for license information.
//
//-----------------------------------------------------------------------------
//
// Pragma parsing and handling.
//
//-----------------------------------------------------------------------------

#include "Pragma.hpp"

#include "IStream.hpp"

#include "GDCC/StreamTBuf.hpp"
#include "GDCC/StringBuf.hpp"
#include "GDCC/TokenStream.hpp"

#include <climits>
#include <csignal>
#include <ctime>
#include <iostream>


//----------------------------------------------------------------------------|
// Static Functions                                                           |
//

//
// EffIt
//
// Sometimes, one just cannot be arsed to figure out how to write some code.
//
static void EffIt()
{
   // Seed RNG.
   {
      unsigned int seed = 0;

      union {std::time_t time; unsigned char data[sizeof(time)];};

      std::time(&time);

      for(unsigned char c : data) if(c)
         seed = (seed << (sizeof(seed) * CHAR_BIT / sizeof(time))) + c;

      std::srand(seed);
   }

   // Print warning.
   static char const *const msg[] =
   {
      "Program will now crash.",
      "This is all your fault!",
      "I trusted you!",
      "Instead of ignoring your pragma, I'ma segfault.",
      "I'm afraid I can't let you do that.",
   };

   std::cerr << "Warning: Unknown pragma. "
      << msg[std::rand() % (sizeof(msg) / sizeof(*msg))] << std::endl;

   // Fuck it.
   std::raise(SIGSEGV);
}


//----------------------------------------------------------------------------|
// Global Functions                                                           |
//

namespace C
{
   //
   // PragmaDTBuf::directive
   //
   bool PragmaDTBuf::directive(GDCC::Token const &tok)
   {
      if(tok.tok != GDCC::TOK_Identi || tok.str != GDCC::STR_pragma)
         return false;

      auto pos = src.get().pos;
      GDCC::TokenStream in{&src};

      pfn(pos, in);

      return true;
   }

   //
   // PragmaTBuf::underflow
   //
   void PragmaTBuf::underflow()
   {
      if(tptr() != tend()) return;

      //
      // skipWS
      //
      auto skipWS = [&]()
      {
         while(src.peek().tok == GDCC::TOK_WSpace || src.peek().tok == GDCC::TOK_LnEnd)
            src.get();

         return src.get();
      };

      while((buf[0] = src.get()).tok == GDCC::TOK_Identi &&
         buf[0].str == GDCC::STR__Pragma)
      {
         auto pos = buf[0].pos;

         // <_Pragma> ( string-literal )
         if(skipWS().tok != GDCC::TOK_ParenO ||
            (buf[0] = skipWS()).tok != GDCC::TOK_String ||
            skipWS().tok != GDCC::TOK_ParenC)
         {
            std::cerr << "ERROR: " << pos << ": bad _Pragma\n";
            throw EXIT_FAILURE;
         }

         // Limited string processing.
         auto const &strData = buf[0].str.getData();
         std::string str; str.reserve(strData.len);
         for(auto i = strData.str + 1, e = i + (strData.len - 2); i != e; ++i)
         {
            if(*i == '\\')
            {
               if(i[1] == '\\') {str += '\\'; ++i; continue;}
               if(i[1] == '"') {str += '"'; ++i; continue;}
            }

            str += *i;
         }

         // Build token stream.
         GDCC::StringBuf sbuf{str.data(), str.size()};
         IStream istr{sbuf, pos.file, pos.line};
         GDCC::StreamTBuf<IStream> tbuf{istr};
         GDCC::TokenStream in{&tbuf};

         pfn(pos, in);
      }

      sett(buf, buf, buf + 1);
   }

   //
   // Pragma
   //
   void Pragma(GDCC::Origin, GDCC::TokenStream &src)
   {
      if(src.peek().tok == GDCC::TOK_WSpace) src.get();

      if(src.peek().tok != GDCC::TOK_Identi) return;

      switch(static_cast<GDCC::StringIndex>(src.get().str))
      {
      case GDCC::STR_fuck:
         // This test pragma brought to you by drinking. And MageofMystra.
         if(src.peek().tok == GDCC::TOK_WSpace) src.get();
         if(src.peek().tok == GDCC::TOK_Identi && src.peek().str == GDCC::STR_it)
            EffIt();

         break;

      default:
         break;
      }
   }
}

// EOF

