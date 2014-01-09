//-----------------------------------------------------------------------------
//
// Copyright (C) 2013-2014 David Hill
//
// See COPYING for license information.
//
//-----------------------------------------------------------------------------
//
// #include directive token buffer.
//
//-----------------------------------------------------------------------------

#include "CPP/IncludeDTBuf.hpp"

#include "CPP/Macro.hpp"
#include "CPP/TStream.hpp"

#include "Core/Option.hpp"
#include "Core/Path.hpp"

#include <fstream>
#include <iostream>


//----------------------------------------------------------------------------|
// Options                                                                    |
//

//
// -i, --include
//
static GDCC::Option::CStrV IncludeUsr
{
   &GDCC::Core::GetOptionList(), GDCC::Option::Base::Info()
      .setName("include").setName('i')
      .setGroup("preprocessor")
      .setDescS("Adds a user include directory."),

   1
};

//
// --sys-include
//
static GDCC::Option::CStrV IncludeSys
{
   &GDCC::Core::GetOptionList(), GDCC::Option::Base::Info()
      .setName("sys-include")
      .setGroup("preprocessor")
      .setDescS("Adds a system include directory."),

   1
};


//----------------------------------------------------------------------------|
// Global Functions                                                           |
//

namespace GDCC
{
   namespace CPP
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
      bool IncludeDTBuf::directive(Core::Token const &tok)
      {
         if(tok.tok != Core::TOK_Identi || tok.str != Core::STR_include)
            return false;

         // Clear directive name.
         src.get();

         // Read header token(s).
         std::vector<Core::Token> toks;
         istr.needHeader = true; // Try to get header token.
         while(src.peek().tok != Core::TOK_LnEnd && src.peek().tok != Core::TOK_EOF)
            toks.emplace_back(src.get());

         // Build macro buffer.
         Core::ArrayTBuf abuf{toks.data(), toks.size()};
         MacroTBuf mbuf{abuf};

         // Skip whitespace that probably shouldn't exist.
         while(mbuf.peek().tok == Core::TOK_WSpace) mbuf.get();

         // User header.
         if(mbuf.peek().tok == Core::TOK_HdrStr)
         {
            if(tryIncUsr(mbuf.peek().str) || tryIncSys(mbuf.peek().str))
               return true;

            std::cerr << "ERROR: " << tok.pos << ": could not include \""
               << mbuf.peek().str << "\"\n";
            throw EXIT_FAILURE;
         }

         // System header.
         else if(mbuf.peek().tok == Core::TOK_Header)
         {
            if(tryIncSys(mbuf.peek().str))
               return true;

            std::cerr << "ERROR: " << tok.pos << ": could not include <"
               << mbuf.peek().str << ">\n";
            throw EXIT_FAILURE;
         }

         // Expanded user header.
         else if(mbuf.peek().tok == Core::TOK_String)
         {
            auto hdr = Core::ParseStringC(mbuf.peek().str);
            if(tryIncUsr(hdr) || tryIncSys(hdr))
               return true;

            std::cerr << "ERROR: " << tok.pos << ": could not include \"" << hdr
               << "\"\n";
            throw EXIT_FAILURE;
         }

         // Expanded system header.
         else if(mbuf.peek().tok == Core::TOK_CmpLT)
         {
            std::string tmp;

            for(mbuf.get(); mbuf.peek().tok != Core::TOK_CmpGT;)
               tmp += mbuf.peek().str.getData().str;

            auto hash = Core::HashString(tmp.data(), tmp.size());
            auto hdr = Core::AddString(tmp.data(), tmp.size(), hash);
            if(tryIncSys(hdr))
               return true;

            std::cerr << "ERROR: " << tok.pos << ": could not include <" << hdr
               << ">\n";
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
      bool IncludeDTBuf::tryIncSys(Core::String name)
      {
         std::unique_ptr<std::filebuf> fbuf{new std::filebuf()};

         // Try specified directories.
         for(auto sys : IncludeSys)
         {
            auto path = Core::PathConcat(sys, name);
            if(fbuf->open(path.data(), std::ios_base::in))
            {
               Macro::LinePush(Macro::Stringize(path));

               str = std::move(fbuf);
               inc.reset(new IncStream(*str, pragma, path, Core::PathDirname(path)));

               return true;
            }
         }

         return false;
      }

      //
      // IncludeDTBuf::tryIncUsr
      //
      bool IncludeDTBuf::tryIncUsr(Core::String name)
      {
         Core::String path;
         std::unique_ptr<std::filebuf> fbuf{new std::filebuf()};

         // Try current directory.
         if(dir)
         {
            path = Core::PathConcat(dir, name);
            if(fbuf->open(path.data(), std::ios_base::in))
               goto inc;
         }

         // Try specified directories.
         for(auto usr : IncludeUsr)
         {
            path = Core::PathConcat(usr, name);
            if(fbuf->open(path.getData().str, std::ios_base::in))
               goto inc;
         }

         return false;

      inc:
         Macro::LinePush(Macro::Stringize(path));

         str = std::move(fbuf);
         inc.reset(new IncStream(*str, pragma, path, Core::PathDirname(path)));

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
}

// EOF

