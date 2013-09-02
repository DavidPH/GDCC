//-----------------------------------------------------------------------------
//
// Copyright (C) 2013 David Hill
//
// See COPYING for license information.
//
//-----------------------------------------------------------------------------
//
// Macro handling.
//
//-----------------------------------------------------------------------------

#include "Macro.hpp"

#include "IStream.hpp"

#include "GDCC/StreamTBuf.hpp"
#include "GDCC/StringBuf.hpp"
#include "GDCC/TokenStream.hpp"

#include "Option/Option.hpp"

#include <climits>
#include <cstring>
#include <ctime>
#include <iostream>
#include <list>
#include <unordered_map>
#include <unordered_set>


//----------------------------------------------------------------------------|
// Static Variables                                                           |
//

static std::vector<std::pair<C::Macro::Name, std::unique_ptr<C::Macro>>> Deltas;

static std::vector<std::pair<GDCC::String, std::size_t>> Lines;

static std::unordered_map<C::Macro::Name, C::Macro> Table;

static C::Macro Macro_DATE{C::Macro::List(1)};
static C::Macro Macro_FILE{C::Macro::List(1)};
static C::Macro Macro_LINE{C::Macro::List(1)};
static C::Macro Macro_TIME{C::Macro::List(1)};


//----------------------------------------------------------------------------|
// Global Variables                                                           |
//

namespace Option
{
   //
   // -D --define
   //
   OptionCall Define{'D', "define", "preprocessor", "Adds a predefined macro.",
      nullptr, [](strp opt, uint optf, uint argc, strv argv) -> uint
   {
      if(!argc)
         Exception::Error(opt, optf, "requires argument");

      // Tokenize argument.
      GDCC::StringBuf sbuf{argv[0], std::strlen(argv[0])};
      C::IStream istr{sbuf, GDCC::STR_};
      GDCC::StreamTBuf<C::IStream> tbuf{istr};
      GDCC::TokenStream in{&tbuf};

      if(in.peek().tok != GDCC::TOK_Identi)
         Exception::Error(opt, optf, "expected identifier");

      auto           name = in.get().str;
      C::Macro::Args args;
      C::Macro::List list;
      bool           func = in.drop(GDCC::TOK_ParenO);

      // Read args.
      if(func && !in.drop(GDCC::TOK_ParenC))
      {
         std::vector<GDCC::String> argsVec;

         do
         {
            if(in.drop(GDCC::TOK_Dot3))
               {argsVec.emplace_back(GDCC::STRNULL); break;}

            if(in.peek().tok != GDCC::TOK_Identi)
               Exception::Error(opt, optf, "expected arg name");

            argsVec.emplace_back(in.get().str);
         }
         while(in.drop(GDCC::TOK_Comma));

         if(in.peek().tok != GDCC::TOK_ParenC)
            Exception::Error(opt, optf, "expected )");

         args = C::Macro::Args(GDCC::Move, argsVec.begin(), argsVec.end());
      }

      // Read list.
      if(in.drop(GDCC::TOK_Equal))
      {
         std::vector<GDCC::Token> listVec;

         while(in.peek().tok != GDCC::TOK_EOF)
            listVec.emplace_back(in.get());

         list = C::Macro::List(GDCC::Move, listVec.begin(), listVec.end());
      }

      // Must not be any tokens left.
      if(in.peek().tok != GDCC::TOK_EOF)
         Exception::Error(opt, optf, "expected end of define");

      Deltas.emplace_back(name, std::unique_ptr<C::Macro>(func
         ? new C::Macro(std::move(args), std::move(list))
         : new C::Macro(std::move(list))));

      return 1;
   }};

   //
   // -U, --undef
   //
   OptionCall Undef{'U', "undef", "preprocessor", "Removes a predefined macro.",
      nullptr, [](strp opt, uint optf, uint argc, strv argv) -> uint
   {
      if(!argc)
         Exception::Error(opt, optf, "requires argument");

      Deltas.emplace_back(GDCC::AddString(argv[0]), nullptr);

      return 1;
   }};
}


//----------------------------------------------------------------------------|
// Global Functions                                                           |
//

namespace C
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
         if(l->tok == GDCC::TOK_WSpace && r->tok == GDCC::TOK_WSpace)
            continue;

         if(l->tok != r->tok || l->str != r->str)
            return false;
      }

      return true;
   }

   //
   // Macro::Add
   //
   void Macro::Add(Name name, Macro const &macro)
   {
      Table.erase(name);

      Table.emplace(name, macro);
   }

   //
   // Macro::Add
   //
   void Macro::Add(Name name, Macro &&macro)
   {
      Table.erase(name);

      Table.emplace(name, std::move(macro));
   }

   //
   // Macro::Concat
   //
   GDCC::Token Macro::Concat(GDCC::Token const &l, GDCC::Token const &r)
   {
      if(r.tok == GDCC::TOK_Marker) return l;
      if(l.tok == GDCC::TOK_Marker) return r;

      GDCC::Token tok;
      tok.pos = l.pos;
      tok.str = l.str + r.str;

      switch(static_cast<GDCC::StringIndex>(tok.str))
      {
      case GDCC::STR_TOK_Add:    tok.tok = GDCC::TOK_Add;    break;
      case GDCC::STR_TOK_Add2:   tok.tok = GDCC::TOK_Add2;   break;
      case GDCC::STR_TOK_AddEq:  tok.tok = GDCC::TOK_AddEq;  break;
      case GDCC::STR_TOK_And:    tok.tok = GDCC::TOK_And;    break;
      case GDCC::STR_TOK_And2:   tok.tok = GDCC::TOK_And2;   break;
      case GDCC::STR_TOK_And2Eq: tok.tok = GDCC::TOK_And2Eq; break;
      case GDCC::STR_TOK_AndEq:  tok.tok = GDCC::TOK_AndEq;  break;
      case GDCC::STR_TOK_CmpEQ:  tok.tok = GDCC::TOK_CmpEQ;  break;
      case GDCC::STR_TOK_CmpGE:  tok.tok = GDCC::TOK_CmpGE;  break;
      case GDCC::STR_TOK_CmpGT:  tok.tok = GDCC::TOK_CmpGT;  break;
      case GDCC::STR_TOK_CmpLE:  tok.tok = GDCC::TOK_CmpLE;  break;
      case GDCC::STR_TOK_CmpLT:  tok.tok = GDCC::TOK_CmpLT;  break;
      case GDCC::STR_TOK_CmpNE:  tok.tok = GDCC::TOK_CmpNE;  break;
      case GDCC::STR_TOK_Colon:  tok.tok = GDCC::TOK_Colon;  break;
      case GDCC::STR_TOK_Colon2: tok.tok = GDCC::TOK_Colon2; break;
      case GDCC::STR_TOK_Comma:  tok.tok = GDCC::TOK_Comma;  break;
      case GDCC::STR_TOK_Div:    tok.tok = GDCC::TOK_Div;    break;
      case GDCC::STR_TOK_DivEq:  tok.tok = GDCC::TOK_DivEq;  break;
      case GDCC::STR_TOK_Dot:    tok.tok = GDCC::TOK_Dot;    break;
      case GDCC::STR_TOK_Dot2:   tok.tok = GDCC::TOK_Dot2;   break;
      case GDCC::STR_TOK_Dot3:   tok.tok = GDCC::TOK_Dot3;   break;
      case GDCC::STR_TOK_DotPtr: tok.tok = GDCC::TOK_DotPtr; break;
      case GDCC::STR_TOK_Equal:  tok.tok = GDCC::TOK_Equal;  break;
      case GDCC::STR_TOK_Hash:   tok.tok = GDCC::TOK_Hash;   break;
      case GDCC::STR_TOK_Hash2:  tok.tok = GDCC::TOK_Hash2;  break;
      case GDCC::STR_TOK_Inv:    tok.tok = GDCC::TOK_Inv;    break;
      case GDCC::STR_TOK_Mem:    tok.tok = GDCC::TOK_Mem;    break;
      case GDCC::STR_TOK_MemPtr: tok.tok = GDCC::TOK_MemPtr; break;
      case GDCC::STR_TOK_Mod:    tok.tok = GDCC::TOK_Mod;    break;
      case GDCC::STR_TOK_ModEq:  tok.tok = GDCC::TOK_ModEq;  break;
      case GDCC::STR_TOK_Mul:    tok.tok = GDCC::TOK_Mul;    break;
      case GDCC::STR_TOK_MulEq:  tok.tok = GDCC::TOK_MulEq;  break;
      case GDCC::STR_TOK_Not:    tok.tok = GDCC::TOK_Not;    break;
      case GDCC::STR_TOK_OrI:    tok.tok = GDCC::TOK_OrI;    break;
      case GDCC::STR_TOK_OrI2:   tok.tok = GDCC::TOK_OrI2;   break;
      case GDCC::STR_TOK_OrI2Eq: tok.tok = GDCC::TOK_OrI2Eq; break;
      case GDCC::STR_TOK_OrIEq:  tok.tok = GDCC::TOK_OrIEq;  break;
      case GDCC::STR_TOK_OrX:    tok.tok = GDCC::TOK_OrX;    break;
      case GDCC::STR_TOK_OrX2:   tok.tok = GDCC::TOK_OrX2;   break;
      case GDCC::STR_TOK_OrX2Eq: tok.tok = GDCC::TOK_OrX2Eq; break;
      case GDCC::STR_TOK_OrXEq:  tok.tok = GDCC::TOK_OrXEq;  break;
      case GDCC::STR_TOK_Query:  tok.tok = GDCC::TOK_Query;  break;
      case GDCC::STR_TOK_Semico: tok.tok = GDCC::TOK_Semico; break;
      case GDCC::STR_TOK_ShL:    tok.tok = GDCC::TOK_ShL;    break;
      case GDCC::STR_TOK_ShLEq:  tok.tok = GDCC::TOK_ShLEq;  break;
      case GDCC::STR_TOK_ShR:    tok.tok = GDCC::TOK_ShR;    break;
      case GDCC::STR_TOK_ShREq:  tok.tok = GDCC::TOK_ShREq;  break;
      case GDCC::STR_TOK_Sub:    tok.tok = GDCC::TOK_Sub;    break;
      case GDCC::STR_TOK_Sub2:   tok.tok = GDCC::TOK_Sub2;   break;
      case GDCC::STR_TOK_SubEq:  tok.tok = GDCC::TOK_SubEq;  break;

      case GDCC::STR_TOK_BraceO: tok.tok = GDCC::TOK_BraceO; break;
      case GDCC::STR_TOK_BraceC: tok.tok = GDCC::TOK_BraceC; break;
      case GDCC::STR_TOK_BrackO: tok.tok = GDCC::TOK_BrackO; break;
      case GDCC::STR_TOK_BrackC: tok.tok = GDCC::TOK_BrackC; break;
      case GDCC::STR_TOK_ParenO: tok.tok = GDCC::TOK_ParenO; break;
      case GDCC::STR_TOK_ParenC: tok.tok = GDCC::TOK_ParenC; break;

      case GDCC::STR_TOK_DG_BraceO: tok.tok = GDCC::TOK_DG_BraceO; break;
      case GDCC::STR_TOK_DG_BraceC: tok.tok = GDCC::TOK_DG_BraceC; break;
      case GDCC::STR_TOK_DG_BrackO: tok.tok = GDCC::TOK_DG_BrackO; break;
      case GDCC::STR_TOK_DG_BrackC: tok.tok = GDCC::TOK_DG_BrackC; break;
      case GDCC::STR_TOK_DG_Hash:   tok.tok = GDCC::TOK_DG_Hash;   break;
      case GDCC::STR_TOK_DG_Hash2:  tok.tok = GDCC::TOK_DG_Hash2;  break;

      default:
         if((l.tok == GDCC::TOK_Identi || l.tok == GDCC::TOK_Number) &&
            (r.tok == GDCC::TOK_Identi || r.tok == GDCC::TOK_Number))
            tok.tok = l.tok;
         else
            tok.tok = GDCC::TOK_ChrSeq;
         break;
      }

      return tok;
   }

   //
   // Macro::Get
   //
   Macro const *Macro::Get(GDCC::Token const &tok)
   {
      if(tok.tok != GDCC::TOK_Identi) return nullptr;

      switch(static_cast<GDCC::StringIndex>(tok.str))
      {
      case GDCC::STR___DATE__: return &Macro_DATE;
      case GDCC::STR___TIME__: return &Macro_TIME;

      case GDCC::STR___FILE__:
         if(Lines.empty()) return nullptr;

         Macro_FILE.list[0].pos = tok.pos;
         Macro_FILE.list[0].str = Lines.back().first;
         Macro_FILE.list[0].tok = GDCC::TOK_String;

         return &Macro_FILE;

      case GDCC::STR___LINE__:
         if(Lines.empty()) return nullptr;

         Macro_LINE.list[0].pos = tok.pos;
         Macro_LINE.list[0].str = MakeString(tok.pos.line + Lines.back().second);
         Macro_LINE.list[0].tok = GDCC::TOK_Number;

         return &Macro_LINE;

      default:
         auto itr = Table.find(tok.str);
         return itr == Table.end() ? nullptr : &itr->second;
      }
   }

   //
   // Macro::Rem
   //
   void Macro::Rem(Name name)
   {
      Table.erase(name);
   }

   //
   // Macro::LineDrop
   //
   void Macro::LineDrop()
   {
      if(!Lines.empty())
         Lines.pop_back();
   }

   //
   // Macro::LineFile
   //
   void Macro::LineFile(GDCC::String file)
   {
      if(!Lines.empty())
         Lines.back().first = file;
   }

   //
   // Macro::LineLine
   //
   void Macro::LineLine(std::size_t line)
   {
      if(!Lines.empty())
         Lines.back().second = line;
   }

   //
   // Macro::LinePush
   //
   void Macro::LinePush(GDCC::String file, std::size_t line)
   {
      Lines.emplace_back(file, line);
   }

   //
   // Macro::MakeString
   //
   GDCC::String Macro::MakeString(std::size_t i)
   {
      char str[sizeof(i) * CHAR_BIT / 8 + 1];
      auto len = std::sprintf(str, "%zu", i);
      return GDCC::AddString(str, len, GDCC::HashString(str, len));
   }

   //
   // Macro::Reset
   //
   void Macro::Reset()
   {
      // Set up __DATE__ and __TIME__.
      {
         std::time_t t = std::time(nullptr);
         std::size_t hash;
         char str[21];

         std::memcpy(str, std::asctime(std::gmtime(&t)) + 3, 21);

         str[ 7] = '"';
         str[16] = '"';

         hash = GDCC::HashString(str + 7, 10);
         Macro_TIME.list[0].pos = GDCC::Origin(GDCC::STRNULL, 0);
         Macro_TIME.list[0].str = GDCC::AddString(str + 7, 10, hash);
         Macro_TIME.list[0].tok = GDCC::TOK_String;

         str[ 0] = '"';
         str[ 7] = ' ';
         str[ 8] = str[17];
         str[ 9] = str[18];
         str[10] = str[19];
         str[11] = str[20];
         str[12] = '"';

         hash = GDCC::HashString(str, 13);
         Macro_DATE.list[0].pos = GDCC::Origin(GDCC::STRNULL, 0);
         Macro_DATE.list[0].str = GDCC::AddString(str, 13, hash);
         Macro_DATE.list[0].tok = GDCC::TOK_String;
      }

      Lines.clear();
      Table.clear();

      // Apply command line defines.
      for(auto const &delta : Deltas)
      {
         if(delta.second)
            Table.emplace(delta.first, *delta.second);
         else
            Table.erase(delta.first);
      }
   }

   //
   // Macro::Stringize
   //
   GDCC::String Macro::Stringize(GDCC::String str)
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

      auto hash = GDCC::HashString(tmp.data(), tmp.size());
      return GDCC::AddString(tmp.data(), tmp.size(), hash);
   }

   //
   // Macro::Stringize
   //
   void Macro::Stringize(std::string &tmp, GDCC::Token const &tok)
   {
      switch(tok.tok)
      {
      case GDCC::TOK_Charac:
      case GDCC::TOK_String:
         tmp.reserve(tmp.size() + tok.str.size());

         for(char c : tok.str) switch(c)
         {
         case '\"': tmp += "\\\""; break;
         case '\\': tmp += "\\\\"; break;
         default: tmp += c; break;
         }

         break;

      case GDCC::TOK_WSpace:
      case GDCC::TOK_LnEnd:
      case GDCC::TOK_Space:
      case GDCC::TOK_Tabul:
         if(!tmp.empty() && tmp.back() != ' ') tmp += ' ';
         break;

      default:
         tmp += tok.str.getData().str;
         break;
      }
   }
}

// EOF

