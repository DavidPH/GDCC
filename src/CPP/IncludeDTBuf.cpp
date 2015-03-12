//-----------------------------------------------------------------------------
//
// Copyright (C) 2013-2015 David Hill
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

#include "Core/Exception.hpp"
#include "Core/Option.hpp"
#include "Core/Parse.hpp"
#include "Core/Path.hpp"

#include "Option/Bool.hpp"

#include <fstream>
#include <iostream>


//----------------------------------------------------------------------------|
// Options                                                                    |
//

namespace GDCC
{
   namespace CPP
   {
      //
      // -i, --include
      //
      static Option::CStrV IncludeUsr
      {
         &Core::GetOptionList(), Option::Base::Info()
            .setName("include").setName('i')
            .setGroup("preprocessor")
            .setDescS("Adds a user include directory."),

         1
      };

      //
      // --sys-include
      //
      static Option::CStrV IncludeSys
      {
         &Core::GetOptionList(), Option::Base::Info()
            .setName("sys-include")
            .setGroup("preprocessor")
            .setDescS("Adds a system include directory."),

         1
      };

      //
      // --lang-include
      //
      static bool IncludeLangEnable = true;
      static Option::Bool IncludeLangEnableOpt
      {
         &Core::GetOptionList(), Option::Base::Info()
            .setName("lang-include")
            .setGroup("preprocessor")
            .setDescS("Enables automatic detection of system include "
               "directories by language. On by default."),

         &IncludeLangEnable
      };
   }
}


//----------------------------------------------------------------------------|
// Static Variables                                                           |
//

namespace GDCC
{
   namespace CPP
   {
      static std::vector<std::string> IncludeLang;
   }
}


//----------------------------------------------------------------------------|
// Global Functions                                                           |
//

namespace GDCC
{
   namespace CPP
   {
      //
      // IncludeDTBuf constructor
      //
      IncludeDTBuf::IncludeDTBuf(Core::TokenBuf &src_, IStreamHeader &istr_,
         MacroMap &macros_, PragmaDataBase &pragd_, PragmaParserBase &pragp_,
         Core::String dir_) :
         DirectiveTBuf{src_},
         istr(istr_),
         macros(macros_),
         pragd(pragd_),
         pragp(pragp_),
         dir{dir_}
      {
      }

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

         readInc(tok);

         return true;
      }

      //
      // IncludeDTBuf::doInc
      //
      void IncludeDTBuf::doInc(Core::String name,
         std::unique_ptr<std::streambuf> &&newStr)
      {
         macros.linePush(Macro::Stringize(name));

         str = std::move(newStr);
         inc.reset(new IncStream(*str, macros, pragd, pragp, name,
            Core::PathDirname(name)));
      }

      //
      // IncludeDTBuf::doIncHdr
      //
      bool IncludeDTBuf::doIncHdr(Core::String name, Core::Origin pos)
      {
         if(tryIncSys(name))
            return true;

         std::cerr << "ERROR: " << pos << ": could not include <" << name << ">\n";
         throw EXIT_FAILURE;
      }

      //
      // IncludeDTBuf::doIncStr
      //
      bool IncludeDTBuf::doIncStr(Core::String name, Core::Origin pos)
      {
         if(tryIncUsr(name) || tryIncSys(name))
            return true;

         std::cerr << "ERROR: " << pos << ": could not include \"" << name << "\"\n";
         throw EXIT_FAILURE;
      }

      //
      // IncludeDTBuf::readInc
      //
      void IncludeDTBuf::readInc(Core::Token const &tok)
      {
         // Read header token(s).
         std::vector<Core::Token> toks;
         istr.setNeedHeader(); // Try to get header token.
         while(src.peek().tok != Core::TOK_LnEnd && src.peek().tok != Core::TOK_EOF)
            toks.emplace_back(src.get());

         // Build macro buffer.
         Core::ArrayTBuf abuf{toks.data(), toks.size()};
         MacroTBuf mbuf{abuf, macros};

         // Skip whitespace.
         while(mbuf.peek().tok == Core::TOK_WSpace) mbuf.get();

         // User header.
         if(mbuf.peek().tok == Core::TOK_HdrStr)
            doIncStr(mbuf.get().str, tok.pos);

         // System header.
         else if(mbuf.peek().tok == Core::TOK_Header)
            doIncHdr(mbuf.get().str, tok.pos);

         // Expanded user header.
         else if(mbuf.peek().tok == Core::TOK_String)
            doIncStr(Core::ParseStringC(mbuf.get().str, '"'), tok.pos);

         // Expanded system header.
         else if(mbuf.peek().tok == Core::TOK_CmpLT)
         {
            std::string tmp;

            for(mbuf.get(); mbuf.peek().tok != Core::TOK_CmpGT;)
            {
               auto s = mbuf.get().str;
               tmp.append(s.data(), s.size());
            }

            doIncStr({tmp.data(), tmp.size()}, tok.pos);
         }

         // Not a valid header token.
         else
            throw Core::ExceptStr(tok.pos, "invalid include syntax");

         // Skip whitespace.
         while(mbuf.peek().tok == Core::TOK_WSpace) mbuf.get();

         // Must be at end of line.
         if(mbuf.peek().tok != Core::TOK_EOF)
            throw Core::ExceptStr(tok.pos, "invalid include syntax");
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
            std::string tmp{sys};
            Core::PathAppend(tmp, name);
            if(fbuf->open(tmp.data(), std::ios_base::in))
               return doInc({tmp.data(), tmp.size()}, std::move(fbuf)), true;
         }

         // Try language directories.
         if(IncludeLangEnable) for(auto lang : IncludeLang)
         {
            Core::PathAppend(lang, name);
            if(fbuf->open(lang.data(), std::ios_base::in))
               return doInc({lang.data(), lang.size()}, std::move(fbuf)), true;
         }

         return false;
      }

      //
      // IncludeDTBuf::tryIncUsr
      //
      bool IncludeDTBuf::tryIncUsr(Core::String name)
      {
         std::unique_ptr<std::filebuf> fbuf{new std::filebuf()};

         // Try current directory.
         if(dir)
         {
            std::string tmp{dir.data(), dir.size()};
            Core::PathAppend(tmp, name);
            if(fbuf->open(tmp.data(), std::ios_base::in))
               return doInc({tmp.data(), tmp.size()}, std::move(fbuf)), true;
         }

         // Try specified directories.
         for(auto usr : IncludeUsr)
         {
            std::string tmp{usr};
            Core::PathAppend(tmp, name);
            if(fbuf->open(tmp.data(), std::ios_base::in))
               return doInc({tmp.data(), tmp.size()}, std::move(fbuf)), true;
         }

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

            macros.lineDrop();
            inc.reset();
            str.reset();
         }

         DirectiveTBuf::underflow();
      }

      //
      // IncludeDTBuf::AddIncludeLang
      //
      void IncludeDTBuf::AddIncludeLang(char const *lang)
      {
         auto path = Core::GetSystemPath();
         Core::PathAppend(path, "lib");
         Core::PathAppend(path, "inc");
         Core::PathAppend(path, lang);
         IncludeLang.emplace_back(std::move(path));
      }
   }
}

// EOF

