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

static std::vector<std::pair<GDCC::String, std::size_t>> Lines;

static std::unordered_map<C::Macro::Name, C::Macro> Table;

static C::Macro Macro_DATE{C::Macro::List(1)};
static C::Macro Macro_FILE{C::Macro::List(1)};
static C::Macro Macro_LINE{C::Macro::List(1)};
static C::Macro Macro_TIME{C::Macro::List(1)};


//----------------------------------------------------------------------------|
// Static Functions                                                           |
//

//
// Expand
//
template<typename Itr>
static void Expand(std::list<GDCC::Token> &out,
   GDCC::Range<std::list<GDCC::Token>::iterator> outRng, GDCC::Origin pos,
   std::unordered_set<GDCC::String> &names, C::Macro const &src,
   GDCC::Range<Itr> argRng)
{
   GDCC::Array<GDCC::Range<Itr>> argv;

   //
   // findArg
   //
   auto findArg = [&](GDCC::Token const &tok) -> GDCC::Range<Itr> *
   {
      if(tok.tok != GDCC::TOK_Identi) return nullptr;

      if(tok.str == GDCC::STR___VA_ARGS__ && !src.args.empty() && !src.args.back())
         return &argv.back();

      for(auto itr = src.args.begin(), end = src.args.end(); itr != end; ++itr)
      {
         if(tok.str == *itr)
            return &argv[itr - src.args.begin()];
      }

      return nullptr;
   };

   //
   // isConcat
   //
   auto isConcat = [](C::Macro::List const &list, C::Macro::List::const_iterator itr)
   {
      // Check behind.
      if(itr != list.begin())
      {
         if((itr - 1)->tok == GDCC::TOK_Hash2) return true;

         if((itr - 1)->tok == GDCC::TOK_WSpace && itr - 1 != list.begin())
         {
            if((itr - 2)->tok == GDCC::TOK_Hash2) return true;
         }
      }

      // Check ahead.
      if(itr + 1 != list.end())
      {
         if(itr[1].tok == GDCC::TOK_Hash2) return true;

         if(itr[1].tok == GDCC::TOK_WSpace && itr + 2 != list.end())
         {
            if(itr[2].tok == GDCC::TOK_Hash2) return true;
         }
      }

      return false;
   };

   //
   // isEmpty
   //
   auto isEmpty = [](GDCC::Range<Itr> const &argr)
   {
      for(auto const &argi : argr)
         if(argi.tok != GDCC::TOK_WSpace && argi.tok != GDCC::TOK_LnEnd)
            return false;

      return true;
   };

   // Build argument token ranges, if any.

   if(src.args.empty() && !argRng.empty())
   {
      std::cerr << "ERROR: " << argRng.begin()->pos << ": args to no-arg macro\n";
      throw EXIT_FAILURE;
   }

   if(!src.args.empty())
   {
      argv = GDCC::Array<GDCC::Range<Itr>>(src.args.size());
      auto argi = argv.begin();

      argi->first = argRng.first;

      std::size_t depth = 0;
      for(auto itr = argRng.begin(), end = argRng.end(); itr != end; ++itr)
      {
              if(itr->tok == GDCC::TOK_ParenO) ++depth;
         else if(itr->tok == GDCC::TOK_ParenC) --depth;
         else if(itr->tok == GDCC::TOK_Comma && !depth)
         {
            if(argi + 1 == argv.end())
            {
               if(src.args.back())
               {
                  std::cerr << "ERROR: " << itr->pos << ": too many macro args\n";
                  throw EXIT_FAILURE;
               }
               else
                  break;
            }

            argi++->last = itr++;
            argi->first = itr;
         }
      }

      argi->last = argRng.last;

      if(argi + 1 != argv.end())
      {
         std::cerr << "ERROR: " << argRng.begin()->pos << ": not enough many macro args\n";
         throw EXIT_FAILURE;
      }
   }

   std::vector<std::list<GDCC::Token>::iterator> concats;

   // Add src's tokens.
   for(auto itr = src.list.begin(), end = src.list.end(); itr != end; ++itr)
   {
      GDCC::Range<Itr> const *arg;

      // # operator
      if(itr->tok == GDCC::TOK_Hash && src.func)
      {
         if(itr + 1 == end || !(arg = findArg(itr[1])))
         {
            std::cerr << "ERROR: " << itr->pos << ": # not followed by arg\n";
            throw EXIT_FAILURE;
         }

         std::string tmp;

         tmp += '"';

         auto argItr = arg->begin(), argEnd = arg->end();

         // Skip leading whitespace.
         while(argItr != argEnd &&
            (argItr->tok == GDCC::TOK_WSpace || argItr->tok == GDCC::TOK_LnEnd))
            ++argItr;

         // Stringize tokens.
         for(; argItr != argEnd; ++argItr)
            C::Macro::Stringize(tmp, *argItr);

         // Discard trailing whitespace.
         while(std::isspace(tmp.back())) tmp.pop_back();

         tmp += '"';

         auto hash = GDCC::HashString(tmp.data(), tmp.size());
         auto str = GDCC::AddString(tmp.data(), tmp.size(), hash);

         out.emplace(outRng.end(), itr->pos, str, GDCC::TOK_String)->pos = pos;

         ++itr;
      }

      // ## operator
      else if(itr->tok == GDCC::TOK_Hash2)
         concats.emplace_back(out.emplace(outRng.end(), *itr));

      // Argument substitution.
      else if((arg = findArg(*itr)))
      {
         if(isEmpty(*arg) && isConcat(src.list, itr))
            out.emplace(outRng.end(), pos, GDCC::STR_, GDCC::TOK_Marker);

         else for(auto const &argTok : *arg)
            out.emplace(outRng.end(), argTok)->pos = pos;
      }

      else
         out.emplace(outRng.end(), *itr)->pos = pos;
   }

   // ## operator
   for(auto const &c : concats)
   {
      auto l = c; --l;
      auto r = c; ++r;

      out.erase(c);

      while(l->tok == GDCC::TOK_WSpace || l->tok == GDCC::TOK_LnEnd) out.erase(l--);
      while(r->tok == GDCC::TOK_WSpace || r->tok == GDCC::TOK_LnEnd) out.erase(r++);

      *l = C::Macro::Concat(*l, *r);
      out.erase(r);
   }

   // Rescanning and further replacement.
   // Yes, skip the first token. It is only in the range in order to preserve
   // begin through all the prior insertions.
   for(auto itr = outRng.begin(), end = outRng.end(); ++itr != end;)
   {
      C::Macro const *macro;

      if(itr->tok == GDCC::TOK_Identi &&
         !names.count(itr->str) &&
         (macro = C::Macro::Get(*itr)))
      {
         // Function-like macro.
         if(macro->func)
         {
            GDCC::Range<std::list<GDCC::Token>::iterator> mArgRng, mOutRng;

            // Search for (.
            for(mArgRng.first = itr;;)
            {
               if(++mArgRng.first == end) goto next_rescan;
               if(mArgRng.first->tok == GDCC::TOK_WSpace) continue;
               if(mArgRng.first->tok == GDCC::TOK_LnEnd) continue;
               if(mArgRng.first->tok == GDCC::TOK_ParenO) break;

               goto next_rescan;
            }

            // Determine bounds of arguments.
            for(mArgRng.last = ++mArgRng.first;; ++mArgRng.last)
            {
               if(mArgRng.last == end)
               {
                  std::cerr << "ERROR: " << mArgRng.first->pos
                     << ": unterminated macro call\n";
                  throw EXIT_FAILURE;
               }

               if(mArgRng.last->tok == GDCC::TOK_ParenC) break;
            }

            ++(mOutRng.last = mOutRng.first = mArgRng.last);

            names.emplace(itr->str);
            Expand(out, mOutRng, pos, names, *macro, mArgRng);
            names.erase(itr->str);

            out.erase(itr, ++mOutRng.first);

            itr = mOutRng.end(); --itr;
         }

         // Object-like macro.
         else
         {
            GDCC::Range<GDCC::Token const *> mArgRng{nullptr, nullptr};
            auto mOutRng = GDCC::MakeRange(itr, itr); ++mOutRng.last;

            names.emplace(itr->str);
            Expand(out, mOutRng, pos, names, *macro, mArgRng);
            names.erase(itr->str);

            out.erase(itr);

            itr = mOutRng.end(); --itr;
         }
      }

   next_rescan:;
   }
}

//
// Expand
//
static void Expand(std::vector<GDCC::Token> &out, GDCC::Token const &tok,
   GDCC::Range<GDCC::Token const *> argRng, C::Macro const &src)
{
   std::list<GDCC::Token> tmp{{{GDCC::STRNULL, 0}, GDCC::STR_, GDCC::TOK_Marker}};
   std::unordered_set<GDCC::String> names{tok.str};

   Expand(tmp, GDCC::MakeRange(tmp.begin(), tmp.end()), tok.pos, names, src, argRng);

   out.reserve(out.size() + tmp.size());
   for(auto &t : tmp)
   {
      if(t.tok != GDCC::TOK_Marker)
         out.emplace_back(std::move(t));
   }
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
   // Macro::expand
   //
   void Macro::expand(std::vector<GDCC::Token> &out, GDCC::Token const &tok) const
   {
      GDCC::Range<GDCC::Token const *> argRng{nullptr, nullptr};
      Expand(out, tok, argRng, *this);
   }

   //
   // Macro::expand
   //
   void Macro::expand(std::vector<GDCC::Token> &out, GDCC::Token const &tok,
      GDCC::Range<GDCC::Token const *> argRng) const
   {
      Expand(out, tok, argRng, *this);
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

