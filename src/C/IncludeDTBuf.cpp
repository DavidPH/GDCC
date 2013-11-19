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

#include "GDCC/Path.hpp"

#include "Option/Option.hpp"

#include <fstream>
#include <iostream>


//----------------------------------------------------------------------------|
// Global Variables                                                           |
//

namespace Option
{
   //
   // -i, --include
   //
   OptionStrV IncludeUsr{'i', "include", "preprocessor",
      "Adds a user include directory.", nullptr, 1};

   //
   // --sys-include
   //
   OptionStrV IncludeSys{'\0', "sys-include", "preprocessor",
      "Adds a system include directory.", nullptr, 1};
}


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

      // Read header token(s).
      std::vector<GDCC::Token> toks;
      IStream::NeedHeader = true; // Try to get header token.
      while(src.peek().tok != GDCC::TOK_LnEnd && src.peek().tok != GDCC::TOK_EOF)
         toks.emplace_back(src.get());

      // Build macro buffer.
      GDCC::ArrayTBuf abuf{toks.data(), toks.size()};
      MacroTBuf mbuf{abuf};

      // Skip whitespace that probably shouldn't exist.
      while(mbuf.peek().tok == GDCC::TOK_WSpace) mbuf.get();

      // User header.
      if(mbuf.peek().tok == GDCC::TOK_HdrStr)
      {
         if(tryIncUsr(mbuf.peek().str) || tryIncSys(mbuf.peek().str))
            return true;

         std::cerr << "ERROR: " << tok.pos << ": could not include \""
            << mbuf.peek().str << "\"\n";
         throw EXIT_FAILURE;
      }

      // System header.
      else if(mbuf.peek().tok == GDCC::TOK_Header)
      {
         if(tryIncSys(mbuf.peek().str))
            return true;

         std::cerr << "ERROR: " << tok.pos << ": could not include <"
            << mbuf.peek().str << ">\n";
         throw EXIT_FAILURE;
      }

      // Expanded user header.
      else if(mbuf.peek().tok == GDCC::TOK_String)
      {
         auto hdr = GDCC::ParseStringC(mbuf.peek().str);
         if(tryIncUsr(hdr) || tryIncSys(hdr))
            return true;

         std::cerr << "ERROR: " << tok.pos << ": could not include \"" << hdr << "\"\n";
         throw EXIT_FAILURE;
      }

      // Expanded system header.
      else if(mbuf.peek().tok == GDCC::TOK_CmpLT)
      {
         std::string tmp;

         for(mbuf.get(); mbuf.peek().tok != GDCC::TOK_CmpGT;)
            tmp += mbuf.peek().str.getData().str;

         auto hash = GDCC::HashString(tmp.data(), tmp.size());
         auto hdr = GDCC::AddString(tmp.data(), tmp.size(), hash);
         if(tryIncSys(hdr))
            return true;

         std::cerr << "ERROR: " << tok.pos << ": could not include <" << hdr << ">\n";
         throw EXIT_FAILURE;
      }

      // Not a valid header token.
      else
      {
         std::cerr << "ERROR: " << tok.pos << ": invalid include syntax\n";
         throw EXIT_FAILURE;
      }
   }

   //
   // IncludeDTBuf::tryIncSys
   //
   bool IncludeDTBuf::tryIncSys(GDCC::String name)
   {
      std::unique_ptr<std::filebuf> fbuf{new std::filebuf()};

      // Try specified directories.
      for(auto sys : Option::IncludeSys)
      {
         auto path = GDCC::PathConcat(sys, name);
         if(fbuf->open(path.data(), std::ios_base::in))
         {
            Macro::LinePush(Macro::Stringize(path));

            str = std::move(fbuf);
            inc.reset(new IncStream(*str, pragma, path, GDCC::PathDirname(path)));

            return true;
         }
      }

      return false;
   }

   //
   // IncludeDTBuf::tryIncUsr
   //
   bool IncludeDTBuf::tryIncUsr(GDCC::String name)
   {
      GDCC::String path;
      std::unique_ptr<std::filebuf> fbuf{new std::filebuf()};

      // Try current directory.
      if(dir)
      {
         path = GDCC::PathConcat(dir, name);
         if(fbuf->open(path.data(), std::ios_base::in))
            goto inc;
      }

      // Try specified directories.
      for(auto usr : Option::IncludeUsr)
      {
         path = GDCC::PathConcat(usr, name);
         if(fbuf->open(path.getData().str, std::ios_base::in))
            goto inc;
      }

      return false;

   inc:
      Macro::LinePush(Macro::Stringize(path));

      str = std::move(fbuf);
      inc.reset(new IncStream(*str, pragma, path, GDCC::PathDirname(path)));

      return true;
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

