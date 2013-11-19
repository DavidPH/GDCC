//-----------------------------------------------------------------------------
//
// Copyright (C) 2013 David Hill
//
// See COPYING for license information.
//
//-----------------------------------------------------------------------------
//
// Pragma parsing.
//
//-----------------------------------------------------------------------------

#include "PragmaDTBuf.hpp"

#include "IStream.hpp"

#include "GDCC/StreamTBuf.hpp"
#include "GDCC/StringBuf.hpp"
#include "GDCC/TokenStream.hpp"

#include <iostream>
#include <vector>


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

      // Clear directive name.
      src.get();

      // Read tokens.
      std::vector<GDCC::Token> toks;
      while(src.peek().tok != GDCC::TOK_LnEnd && src.peek().tok != GDCC::TOK_EOF)
         toks.emplace_back(src.get());

      // Build macro buffer.
      GDCC::ArrayTBuf abuf{toks.data(), toks.size()};
      GDCC::TokenStream in{&abuf};

      prag.pragma(in);

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

         prag.pragma(in);
      }

      sett(buf, buf, buf + 1);
   }
}

// EOF

