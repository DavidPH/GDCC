//-----------------------------------------------------------------------------
//
// Copyright (C) 2013-2018 David Hill
//
// See COPYING for license information.
//
//-----------------------------------------------------------------------------
//
// Macro handling.
//
//-----------------------------------------------------------------------------

#include "CPP/Macro.hpp"

#include "CPP/IStream.hpp"
#include "CPP/TSource.hpp"

#include "Core/Option.hpp"
#include "Core/SourceTBuf.hpp"
#include "Core/StringBuf.hpp"
#include "Core/TokenStream.hpp"

#include "Option/Exception.hpp"
#include "Option/Function.hpp"

#include "Platform/Platform.hpp"

#include <climits>
#include <cstring>
#include <ctime>


//----------------------------------------------------------------------------|
// Static Objects                                                             |
//

namespace GDCC::CPP
{
   static std::vector<std::pair<Macro::Name, std::unique_ptr<Macro>>> Deltas;
}


//----------------------------------------------------------------------------|
// Options                                                                    |
//

namespace GDCC::CPP
{
   //
   // -D --define
   //
   static Option::Function Define
   {
      &Core::GetOptionList(), Option::Base::Info()
         .setName("define").setName('D')
         .setGroup("preprocessor")
         .setDescS("Adds a predefined macro."),

      [](Option::Base *, Option::Args const &oargs) -> std::size_t
      {
         if(!oargs.argC)
            Option::Exception::Error(oargs, "argument required");

         // Tokenize argument.
         char const         *arg {oargs.argV[0]};
         Core::StringBuf    sbuf{arg, std::strlen(arg)};
         IStream            istr{sbuf, Core::STR_};
         TSource            tsrc{istr, istr.getOriginSource()};
         Core::SourceTBuf<> tbuf{tsrc};
         Core::TokenStream  in  {&tbuf};

         if(in.peek().tok != Core::TOK_Identi)
            Option::Exception::Error(oargs, "expected identifier");

         auto        name = in.get().str;
         Macro::Args args;
         Macro::List list;
         bool        func = in.drop(Core::TOK_ParenO);

         // Read args.
         if(func && !in.drop(Core::TOK_ParenC))
         {
            std::vector<Core::String> argsVec;

            do
            {
               if(in.drop(Core::TOK_Dot3))
                  {argsVec.emplace_back(Core::STRNULL); break;}

               if(in.peek().tok != Core::TOK_Identi)
                  Option::Exception::Error(oargs, "expected arg name");

               argsVec.emplace_back(in.get().str);
            }
            while(in.drop(Core::TOK_Comma));

            if(in.peek().tok != Core::TOK_ParenC)
               Option::Exception::Error(oargs, "expected )");

            args = Macro::Args(Core::Move, argsVec.begin(), argsVec.end());
         }

         // Read list.
         if(in.drop(Core::TOK_Equal))
         {
            std::vector<Core::Token> listVec;

            while(in.peek().tok != Core::TOK_EOF)
               listVec.emplace_back(in.get());

            list = Macro::List(Core::Move, listVec.begin(), listVec.end());
         }

         // Must not be any tokens left.
         if(in.peek().tok != Core::TOK_EOF)
            Option::Exception::Error(oargs, "expected end of define");

         Deltas.emplace_back(name, std::unique_ptr<Macro>(func
            ? new Macro(std::move(args), std::move(list))
            : new Macro(std::move(list))));

         return 1;
      }
   };

   //
   // -U, --undef
   //
   static Option::Function Undef
   {
      &Core::GetOptionList(), Option::Base::Info()
         .setName("undef").setName('U')
         .setGroup("preprocessor")
         .setDescS("Removes a predefined macro."),

      [](Option::Base *, Option::Args const &args) -> std::size_t
      {
         if(!args.argC)
            Option::Exception::Error(args, "argument required");

         Deltas.emplace_back(args.argV[0], nullptr);

         return 1;
      }
   };
}


//----------------------------------------------------------------------------|
// Extern Functions                                                           |
//

namespace GDCC::CPP
{
   //
   // Macro::operator == Macro
   //
   bool Macro::operator == (Macro const &macro) const
   {
      if(func != macro.func) return false;
      if(args != macro.args) return false;

      if(list.size() != macro.list.size()) return false;

      for(auto l = list.begin(), r = macro.list.begin(), e = list.end(); l != e; ++l, ++r)
      {
         // All whitespace separation is considered equivalent.
         if(l->tok == Core::TOK_WSpace && r->tok == Core::TOK_WSpace)
            continue;

         if(l->tok != r->tok || l->str != r->str)
            return false;
      }

      return true;
   }

   //
   // Macro::Concat
   //
   Core::Token Macro::Concat(Core::Token const &l, Core::Token const &r)
   {
      if(r.tok == Core::TOK_Marker) return l;
      if(l.tok == Core::TOK_Marker) return r;

      Core::Token tok;
      tok.pos = l.pos;
      tok.str = l.str + r.str;

      switch(static_cast<Core::StringIndex>(tok.str))
      {
      case Core::STR_TOK_Add:    tok.tok = Core::TOK_Add;    break;
      case Core::STR_TOK_Add2:   tok.tok = Core::TOK_Add2;   break;
      case Core::STR_TOK_AddEq:  tok.tok = Core::TOK_AddEq;  break;
      case Core::STR_TOK_And:    tok.tok = Core::TOK_And;    break;
      case Core::STR_TOK_And2:   tok.tok = Core::TOK_And2;   break;
      case Core::STR_TOK_And2Eq: tok.tok = Core::TOK_And2Eq; break;
      case Core::STR_TOK_AndEq:  tok.tok = Core::TOK_AndEq;  break;
      case Core::STR_TOK_CmpEQ:  tok.tok = Core::TOK_CmpEQ;  break;
      case Core::STR_TOK_CmpGE:  tok.tok = Core::TOK_CmpGE;  break;
      case Core::STR_TOK_CmpGT:  tok.tok = Core::TOK_CmpGT;  break;
      case Core::STR_TOK_CmpLE:  tok.tok = Core::TOK_CmpLE;  break;
      case Core::STR_TOK_CmpLT:  tok.tok = Core::TOK_CmpLT;  break;
      case Core::STR_TOK_CmpNE:  tok.tok = Core::TOK_CmpNE;  break;
      case Core::STR_TOK_Colon:  tok.tok = Core::TOK_Colon;  break;
      case Core::STR_TOK_Colon2: tok.tok = Core::TOK_Colon2; break;
      case Core::STR_TOK_Comma:  tok.tok = Core::TOK_Comma;  break;
      case Core::STR_TOK_Div:    tok.tok = Core::TOK_Div;    break;
      case Core::STR_TOK_DivEq:  tok.tok = Core::TOK_DivEq;  break;
      case Core::STR_TOK_Dot:    tok.tok = Core::TOK_Dot;    break;
      case Core::STR_TOK_Dot2:   tok.tok = Core::TOK_Dot2;   break;
      case Core::STR_TOK_Dot3:   tok.tok = Core::TOK_Dot3;   break;
      case Core::STR_TOK_DotPtr: tok.tok = Core::TOK_DotPtr; break;
      case Core::STR_TOK_Equal:  tok.tok = Core::TOK_Equal;  break;
      case Core::STR_TOK_Hash:   tok.tok = Core::TOK_Hash;   break;
      case Core::STR_TOK_Hash2:  tok.tok = Core::TOK_Hash2;  break;
      case Core::STR_TOK_Inv:    tok.tok = Core::TOK_Inv;    break;
      case Core::STR_TOK_Mem:    tok.tok = Core::TOK_Mem;    break;
      case Core::STR_TOK_MemPtr: tok.tok = Core::TOK_MemPtr; break;
      case Core::STR_TOK_Mod:    tok.tok = Core::TOK_Mod;    break;
      case Core::STR_TOK_ModEq:  tok.tok = Core::TOK_ModEq;  break;
      case Core::STR_TOK_Mul:    tok.tok = Core::TOK_Mul;    break;
      case Core::STR_TOK_MulEq:  tok.tok = Core::TOK_MulEq;  break;
      case Core::STR_TOK_Not:    tok.tok = Core::TOK_Not;    break;
      case Core::STR_TOK_OrI:    tok.tok = Core::TOK_OrI;    break;
      case Core::STR_TOK_OrI2:   tok.tok = Core::TOK_OrI2;   break;
      case Core::STR_TOK_OrI2Eq: tok.tok = Core::TOK_OrI2Eq; break;
      case Core::STR_TOK_OrIEq:  tok.tok = Core::TOK_OrIEq;  break;
      case Core::STR_TOK_OrX:    tok.tok = Core::TOK_OrX;    break;
      case Core::STR_TOK_OrX2:   tok.tok = Core::TOK_OrX2;   break;
      case Core::STR_TOK_OrX2Eq: tok.tok = Core::TOK_OrX2Eq; break;
      case Core::STR_TOK_OrXEq:  tok.tok = Core::TOK_OrXEq;  break;
      case Core::STR_TOK_Query:  tok.tok = Core::TOK_Query;  break;
      case Core::STR_TOK_Semico: tok.tok = Core::TOK_Semico; break;
      case Core::STR_TOK_ShL:    tok.tok = Core::TOK_ShL;    break;
      case Core::STR_TOK_ShLEq:  tok.tok = Core::TOK_ShLEq;  break;
      case Core::STR_TOK_ShR:    tok.tok = Core::TOK_ShR;    break;
      case Core::STR_TOK_ShREq:  tok.tok = Core::TOK_ShREq;  break;
      case Core::STR_TOK_Sub:    tok.tok = Core::TOK_Sub;    break;
      case Core::STR_TOK_Sub2:   tok.tok = Core::TOK_Sub2;   break;
      case Core::STR_TOK_SubEq:  tok.tok = Core::TOK_SubEq;  break;

      case Core::STR_TOK_BraceO: tok.tok = Core::TOK_BraceO; break;
      case Core::STR_TOK_BraceC: tok.tok = Core::TOK_BraceC; break;
      case Core::STR_TOK_BrackO: tok.tok = Core::TOK_BrackO; break;
      case Core::STR_TOK_BrackC: tok.tok = Core::TOK_BrackC; break;
      case Core::STR_TOK_ParenO: tok.tok = Core::TOK_ParenO; break;
      case Core::STR_TOK_ParenC: tok.tok = Core::TOK_ParenC; break;

      case Core::STR_TOK_DG_BraceO: tok.tok = Core::TOK_DG_BraceO; break;
      case Core::STR_TOK_DG_BraceC: tok.tok = Core::TOK_DG_BraceC; break;
      case Core::STR_TOK_DG_BrackO: tok.tok = Core::TOK_DG_BrackO; break;
      case Core::STR_TOK_DG_BrackC: tok.tok = Core::TOK_DG_BrackC; break;
      case Core::STR_TOK_DG_Hash:   tok.tok = Core::TOK_DG_Hash;   break;
      case Core::STR_TOK_DG_Hash2:  tok.tok = Core::TOK_DG_Hash2;  break;

      default:
         if((l.tok == Core::TOK_Identi || l.tok == Core::TOK_Number) &&
            (r.tok == Core::TOK_Identi || r.tok == Core::TOK_Number))
            tok.tok = l.tok;
         else
            tok.tok = Core::TOK_ChrSeq;
         break;
      }

      return tok;
   }

   //
   // Macro::MakeString
   //
   Core::String Macro::MakeString(std::size_t i)
   {
      char str[sizeof(i) * CHAR_BIT / 3 + 1];
      std::size_t len = std::sprintf(str, "%zu", i);
      return {str, len, Core::StrHash(str, len)};
   }

   //
   // Macro::Stringize
   //
   Core::String Macro::Stringize(Core::String str)
   {
      std::string tmp; tmp.reserve(str.size() + 2);

      tmp += '"';

      for(char c : str) switch(c)
      {
      case '\"': tmp += "\\\""; break;
      case '\\': tmp += "\\\\"; break;
      default: tmp += c; break;
      }

      tmp += '"';

      auto hash = Core::StrHash(tmp.data(), tmp.size());
      return {tmp.data(), tmp.size(), hash};
   }

   //
   // Macro::Stringize
   //
   void Macro::Stringize(std::string &tmp, Core::Token const &tok)
   {
      switch(tok.tok)
      {
      case Core::TOK_Charac:
      case Core::TOK_String:
         tmp.reserve(tmp.size() + tok.str.size());

         for(char c : tok.str) switch(c)
         {
         case '\"': tmp += "\\\""; break;
         case '\\': tmp += "\\\\"; break;
         default: tmp += c; break;
         }

         break;

      case Core::TOK_WSpace:
      case Core::TOK_LnEnd:
      case Core::TOK_Space:
      case Core::TOK_Tabul:
         if(!tmp.empty() && tmp.back() != ' ') tmp += ' ';
         break;

      default:
         tmp += tok.str.data();
         break;
      }
   }

   //
   // MacroMap constructor
   //
   MacroMap::MacroMap(Core::String file, std::size_t line) :
      macroDATE{Macro::List(1)},
      macroFILE{Macro::List(1)},
      macroLINE{Macro::List(1)},
      macroTIME{Macro::List(1)}
   {
      reset();
      linePush(file, line);
   }

   //
   // MacroMap::add
   //
   void MacroMap::add(Core::String name, Macro const &macro)
   {
      table.erase(name);

      table.emplace(name, macro);
   }

   //
   // MacroMap::add
   //
   void MacroMap::add(Core::String name, Macro &&macro)
   {
      table.erase(name);

      table.emplace(name, std::move(macro));
   }

   //
   // MacroMap::find
   //
   Macro const *MacroMap::find(Core::Token const &tok)
   {
      if(tok.tok != Core::TOK_Identi) return nullptr;

      switch(tok.str)
      {
      case Core::STR___DATE__: return &macroDATE;
      case Core::STR___TIME__: return &macroTIME;

      case Core::STR___FILE__:
         if(lines.empty()) return nullptr;

         macroFILE.list[0].str = lines.back().first;
         return &macroFILE;

      case Core::STR___LINE__:
         if(lines.empty()) return nullptr;

         macroLINE.list[0].str = Macro::MakeString(tok.pos.line + lines.back().second);
         return &macroLINE;

      default:
         auto itr = table.find(tok.str);
         return itr == table.end() ? nullptr : &itr->second;
      }
   }

   //
   // MacroMap::lineDrop
   //
   void MacroMap::lineDrop()
   {
      if(!lines.empty())
         lines.pop_back();
   }

   //
   // MacroMap::lineFile
   //
   void MacroMap::lineFile(Core::String file)
   {
      if(!lines.empty())
         lines.back().first = file;
   }

   //
   // MacroMap::lineLine
   //
   void MacroMap::lineLine(std::size_t line)
   {
      if(!lines.empty())
         lines.back().second = line;
   }

   //
   // MacroMap::linePush
   //
   void MacroMap::linePush(Core::String file, std::size_t line)
   {
      lines.emplace_back(file, line);
   }

   //
   // MacroMap::rem
   //
   void MacroMap::rem(Core::String name)
   {
      table.erase(name);
   }

   //
   // MacroMap::reset
   //
   void MacroMap::reset()
   {
      lines.clear();
      table.clear();

      // Set up __DATE__ and __TIME__.
      {
         std::time_t t = std::time(nullptr);
         char str[21];

         // asctime:  'Ddd Mmm dd hh:mm:ss yyyy\n\0'
         // str:         ' Mmm dd hh:mm:ss yyyy'
         std::memcpy(str, std::asctime(std::gmtime(&t)) + 3, 21);

         // str:      ' Mmm dd"hh:mm:ss"yyyy'
         // __TIME__:        '"hh:mm:ss"'
         str[ 7] = '"';
         str[16] = '"';

         macroTIME.list[0] = {{nullptr, 0}, {str + 7, 10}, Core::TOK_String};

         // str:      '"Mmm dd yyyy":ss"yyyy'
         // __DATE__: '"Mmm dd yyyy"'
         str[ 0] = '"';
         str[ 7] = ' ';
         str[ 8] = str[17];
         str[ 9] = str[18];
         str[10] = str[19];
         str[11] = str[20];
         str[12] = '"';

         macroDATE.list[0] = {{nullptr, 0}, {str, 13}, Core::TOK_String};
      }

      // Set up __FILE__.
      macroFILE.list[0] = {{nullptr, 0}, nullptr, Core::TOK_String};

      // Set up __LINE__.
      macroLINE.list[0] = {{nullptr, 0}, nullptr, Core::TOK_Number};

      #define InsertN(name, n) \
         table.insert({name, {{{{}, n, Core::TOK_Number}}}})

      #define Insert1(name) \
         InsertN(name, Core::STR_1)

      #define FamilyCheck(e, n) \
         if(Platform::Is##e##_##n()) Insert1("__GDCC_" #e "__" #n "__")

      #define PlatformCase(e, n) \
         case Platform::e::n: Insert1("__GDCC_" #e "__" #n "__"); break

      // __GDCC__
      Insert1("__GDCC__");

      // Set up __GDCC_Family__*__.
      FamilyCheck(Family, ZDACS);

      // Set up __GDCC_Format__*__.
      switch(Platform::FormatCur)
      {
         PlatformCase(Format, None);
         PlatformCase(Format, ACSE);
         PlatformCase(Format, DGE_NTS);
      }

      // Set up __GDCC_Target__*__.
      switch(Platform::TargetCur)
      {
         PlatformCase(Target, None);
         PlatformCase(Target, Doominati);
         PlatformCase(Target, ZDoom);
         PlatformCase(Target, Zandronum);
      }

      // __STDC__
      Insert1("__STDC__");

      // Conditional feature macros.
      Insert1("__STDC_NO_ATOMICS__");
      Insert1("__STDC_NO_COMPLEX__");
      Insert1("__STDC_NO_THREADS__");
      Insert1("__STDC_NO_VLA__");

      // __STDC_VERSION__
      InsertN("__STDC_VERSION__", "201112L");

      #undef PlatformCase
      #undef FamilyCheck
      #undef Insert1
      #undef InsertN

      // Apply command line defines.
      for(auto const &delta : Deltas)
      {
         if(delta.second)
            table.emplace(delta.first, *delta.second);
         else
            table.erase(delta.first);
      }
   }
}

// EOF

