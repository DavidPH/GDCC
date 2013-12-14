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

#include "CPP/Macro.hpp"

#include "CPP/IStream.hpp"

#include "Core/StreamTBuf.hpp"
#include "Core/StringBuf.hpp"
#include "Core/TokenStream.hpp"

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

static std::vector<std::pair<GDCC::CPP::Macro::Name, std::unique_ptr<GDCC::CPP::Macro>>> Deltas;

static std::vector<std::pair<GDCC::Core::String, std::size_t>> Lines;

static std::unordered_map<GDCC::CPP::Macro::Name, GDCC::CPP::Macro> Table;

static GDCC::CPP::Macro Macro_DATE{GDCC::CPP::Macro::List(1)};
static GDCC::CPP::Macro Macro_FILE{GDCC::CPP::Macro::List(1)};
static GDCC::CPP::Macro Macro_LINE{GDCC::CPP::Macro::List(1)};
static GDCC::CPP::Macro Macro_TIME{GDCC::CPP::Macro::List(1)};


//----------------------------------------------------------------------------|
// Global Variables                                                           |
//

namespace GDCC
{
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
         Core::StringBuf sbuf{argv[0], std::strlen(argv[0])};
         CPP::IStream istr{sbuf, Core::STR_};
         Core::StreamTBuf<CPP::IStream> tbuf{istr};
         Core::TokenStream in{&tbuf};

         if(in.peek().tok != Core::TOK_Identi)
            Exception::Error(opt, optf, "expected identifier");

         auto             name = in.get().str;
         CPP::Macro::Args args;
         CPP::Macro::List list;
         bool             func = in.drop(Core::TOK_ParenO);

         // Read args.
         if(func && !in.drop(Core::TOK_ParenC))
         {
            std::vector<Core::String> argsVec;

            do
            {
               if(in.drop(Core::TOK_Dot3))
                  {argsVec.emplace_back(Core::STRNULL); break;}

               if(in.peek().tok != Core::TOK_Identi)
                  Exception::Error(opt, optf, "expected arg name");

               argsVec.emplace_back(in.get().str);
            }
            while(in.drop(Core::TOK_Comma));

            if(in.peek().tok != Core::TOK_ParenC)
               Exception::Error(opt, optf, "expected )");

            args = CPP::Macro::Args(Core::Move, argsVec.begin(), argsVec.end());
         }

         // Read list.
         if(in.drop(Core::TOK_Equal))
         {
            std::vector<Core::Token> listVec;

            while(in.peek().tok != Core::TOK_EOF)
               listVec.emplace_back(in.get());

            list = CPP::Macro::List(Core::Move, listVec.begin(), listVec.end());
         }

         // Must not be any tokens left.
         if(in.peek().tok != Core::TOK_EOF)
            Exception::Error(opt, optf, "expected end of define");

         Deltas.emplace_back(name, std::unique_ptr<CPP::Macro>(func
            ? new CPP::Macro(std::move(args), std::move(list))
            : new CPP::Macro(std::move(list))));

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

         Deltas.emplace_back(Core::AddString(argv[0]), nullptr);

         return 1;
      }};
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
      // Macro::Get
      //
      Macro const *Macro::Get(Core::Token const &tok)
      {
         if(tok.tok != Core::TOK_Identi) return nullptr;

         switch(static_cast<Core::StringIndex>(tok.str))
         {
         case Core::STR___DATE__: return &Macro_DATE;
         case Core::STR___TIME__: return &Macro_TIME;

         case Core::STR___FILE__:
            if(Lines.empty()) return nullptr;

            Macro_FILE.list[0].pos = tok.pos;
            Macro_FILE.list[0].str = Lines.back().first;
            Macro_FILE.list[0].tok = Core::TOK_String;

            return &Macro_FILE;

         case Core::STR___LINE__:
            if(Lines.empty()) return nullptr;

            Macro_LINE.list[0].pos = tok.pos;
            Macro_LINE.list[0].str = MakeString(tok.pos.line + Lines.back().second);
            Macro_LINE.list[0].tok = Core::TOK_Number;

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
      void Macro::LineFile(Core::String file)
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
      void Macro::LinePush(Core::String file, std::size_t line)
      {
         Lines.emplace_back(file, line);
      }

      //
      // Macro::MakeString
      //
      Core::String Macro::MakeString(std::size_t i)
      {
         char str[sizeof(i) * CHAR_BIT / 8 + 1];
         auto len = std::sprintf(str, "%zu", i);
         return Core::AddString(str, len, Core::HashString(str, len));
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

            hash = Core::HashString(str + 7, 10);
            Macro_TIME.list[0].pos = Core::Origin(Core::STRNULL, 0);
            Macro_TIME.list[0].str = Core::AddString(str + 7, 10, hash);
            Macro_TIME.list[0].tok = Core::TOK_String;

            str[ 0] = '"';
            str[ 7] = ' ';
            str[ 8] = str[17];
            str[ 9] = str[18];
            str[10] = str[19];
            str[11] = str[20];
            str[12] = '"';

            hash = Core::HashString(str, 13);
            Macro_DATE.list[0].pos = Core::Origin(Core::STRNULL, 0);
            Macro_DATE.list[0].str = Core::AddString(str, 13, hash);
            Macro_DATE.list[0].tok = Core::TOK_String;
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

         auto hash = Core::HashString(tmp.data(), tmp.size());
         return Core::AddString(tmp.data(), tmp.size(), hash);
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
            tmp += tok.str.getData().str;
            break;
         }
      }
   }
}

// EOF

