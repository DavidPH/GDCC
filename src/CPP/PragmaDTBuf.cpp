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

#include "CPP/PragmaDTBuf.hpp"

#include "CPP/IStream.hpp"

#include "Core/StreamTBuf.hpp"
#include "Core/StringBuf.hpp"
#include "Core/TokenStream.hpp"

#include <iostream>
#include <vector>


//----------------------------------------------------------------------------|
// Global Functions                                                           |
//

namespace GDCC
{
   namespace CPP
   {
      //
      // PragmaDTBuf::directive
      //
      bool PragmaDTBuf::directive(Core::Token const &tok)
      {
         if(tok.tok != Core::TOK_Identi || tok.str != Core::STR_pragma)
            return false;

         // Clear directive name.
         src.get();

         // Read tokens.
         std::vector<Core::Token> toks;
         while(src.peek().tok != Core::TOK_LnEnd && src.peek().tok != Core::TOK_EOF)
            toks.emplace_back(src.get());

         // Build macro buffer.
         Core::ArrayTBuf abuf{toks.data(), toks.size()};
         Core::TokenStream in{&abuf};

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
            while(src.peek().tok == Core::TOK_WSpace || src.peek().tok == Core::TOK_LnEnd)
               src.get();

            return src.get();
         };

         while((buf[0] = src.get()).tok == Core::TOK_Identi &&
            buf[0].str == Core::STR__Pragma)
         {
            auto pos = buf[0].pos;

            // <_Pragma> ( string-literal )
            if(skipWS().tok != Core::TOK_ParenO ||
               (buf[0] = skipWS()).tok != Core::TOK_String ||
               skipWS().tok != Core::TOK_ParenC)
            {
               std::cerr << "ERROR: " << pos << ": bad _Pragma\n";
               throw EXIT_FAILURE;
            }

            // Limited string processing.
            std::string str; str.reserve(buf[0].str.size());
            for(auto i = buf[0].str.begin() + 1, e = buf[0].str.end() - 1; i != e; ++i)
            {
               if(*i == '\\')
               {
                  if(i[1] == '\\') {str += '\\'; ++i; continue;}
                  if(i[1] == '"') {str += '"'; ++i; continue;}
               }

               str += *i;
            }

            // Build token stream.
            Core::StringBuf sbuf{str.data(), str.size()};
            IStream istr{sbuf, pos.file, pos.line};
            Core::StreamTBuf<IStream> tbuf{istr};
            Core::TokenStream in{&tbuf};

            prag.pragma(in);
         }

         sett(buf, buf, buf + 1);
      }
   }
}

// EOF

