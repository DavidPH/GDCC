//-----------------------------------------------------------------------------
//
// Copyright (C) 2013 David Hill
//
// See COPYING for license information.
//
//-----------------------------------------------------------------------------
//
// Macro expanding token buffer.
//
//-----------------------------------------------------------------------------

#include "CPP/MacroTBuf.hpp"

#include "CPP/Macro.hpp"

#include <iostream>
#include <vector>


//----------------------------------------------------------------------------|
// Static Functions                                                           |
//

//
// FindArg
//
static GDCC::CPP::MacroTBuf::Rng const *FindArg(GDCC::Core::Token const &tok,
   GDCC::CPP::Macro const &macro,
   GDCC::Core::Array<GDCC::CPP::MacroTBuf::Rng> const &argv)
{
   if(tok.tok != GDCC::Core::TOK_Identi) return nullptr;

   if(tok.str == GDCC::Core::STR___VA_ARGS__ && !macro.args.empty() && !macro.args.back())
      return &argv.back();

   for(auto itr = macro.args.begin(), end = macro.args.end(); itr != end; ++itr)
   {
      if(tok.str == *itr)
         return &argv[itr - macro.args.begin()];
   }

   return nullptr;
}

//
// IsConcat
//
static bool IsConcat(GDCC::Core::Token const *itr, GDCC::CPP::Macro const &macro)
{
   auto end = macro.list.end();

   if(itr + 1 != end)
   {
      if(itr[1].tok == GDCC::Core::TOK_Hash2) return true;

      if(itr[1].tok == GDCC::Core::TOK_WSpace && itr + 2 != end)
      {
         if(itr[2].tok == GDCC::Core::TOK_Hash2) return true;
      }
   }

   return false;
}

//
// IsEmpty
//
static bool IsEmpty(GDCC::CPP::MacroTBuf::Rng const &arg)
{
   for(auto const &argi : arg)
      if(argi.tok != GDCC::Core::TOK_WSpace && argi.tok != GDCC::Core::TOK_LnEnd)
         return false;

   return true;
}

//
// MakeArgs
//
static GDCC::Core::Array<GDCC::CPP::MacroTBuf::Rng>
MakeArgs(GDCC::CPP::Macro const &macro, GDCC::CPP::MacroTBuf::Rng const &argRng)
{
   // Build argument token ranges, if any.

   if(macro.args.empty() && !argRng.empty())
   {
      std::cerr << "ERROR: " << argRng.begin()->pos << ": args to no-arg macro\n";
      throw EXIT_FAILURE;
   }

   if(!macro.args.empty())
   {
      GDCC::Core::Array<GDCC::CPP::MacroTBuf::Rng> argv{macro.args.size()};
      auto argi = argv.begin();

      argi->first = argRng.first;

      std::size_t depth = 0;
      for(auto itr = argRng.begin(), end = argRng.end(); itr != end;)
      {
              if(itr->tok == GDCC::Core::TOK_ParenO) ++depth;
         else if(itr->tok == GDCC::Core::TOK_ParenC) --depth;
         else if(itr->tok == GDCC::Core::TOK_Comma && !depth)
         {
            if(argi + 1 == argv.end())
            {
               if(macro.args.back())
               {
                  std::cerr << "ERROR: " << itr->pos << ": too many macro args\n";
                  throw EXIT_FAILURE;
               }
               else
                  break;
            }

            argi++->last = itr++;
            argi->first = itr;

            continue;
         }

         ++itr;
      }

      argi->last = argRng.last;

      if(argi + 1 != argv.end())
      {
         std::cerr << "ERROR: " << argRng.begin()->pos
            << ": not enough macro args: expected " << macro.args.size()
            << " got " << (argi - argv.begin() + 1) << '\n';
         throw EXIT_FAILURE;
      }

      return argv;
   }

   return GDCC::Core::Array<GDCC::CPP::MacroTBuf::Rng>();
}


//----------------------------------------------------------------------------|
// Global Functions                                                           |
//

namespace GDCC
{
   namespace CPP
   {
      //
      // MacroTBuf::applyMarker
      //
      void MacroTBuf::applyMarker(Core::String str)
      {
         if(str == Core::STR_TOK_Add)
            ignoreAll = true;
         else if(str == Core::STR_TOK_Sub)
            ignoreAll = false;
         else if(str)
         {
            if(ignore.count(str))
               ignore.erase(str);
            else
               ignore.emplace(str);
         }
      }

      //
      // MacroTBuf::expand
      //
      bool MacroTBuf::expand(Itr itr)
      {
         if(itr == buf.end()) {buf.emplace(itr, src.get()); --itr;}

         // Marker handling.
         if(itr->tok == Core::TOK_Marker)
         {
            applyMarker(itr->str);

            buf.erase(itr);
            return true;
         }

         if(ignoreAll || itr->tok != Core::TOK_Identi || ignore.count(itr->str))
            return false;

         auto macro = macros.find(*itr);
         if(!macro) return false;

         // Function-like macro.
         if(macro->func)
         {
            auto first = itr++;

            // Search for opening parenthesis.
            for(;; ++itr)
            {
               if(itr == buf.end()) {buf.emplace(itr, src.get()); --itr;}

               if(itr->tok == Core::TOK_WSpace || itr->tok == Core::TOK_LnEnd) continue;
               if(itr->tok == Core::TOK_Marker) {applyMarker(itr->str); buf.erase(itr++); --itr; continue;}
               if(itr->tok == Core::TOK_ParenO) break;

               return false;
            }

            auto argFirst = itr++;

            // Search for closing parenthesis.
            for(std::size_t depth = 0;; ++itr)
            {
               if(itr == buf.end()) {buf.emplace(itr, src.get()); --itr;}

               if(itr->tok == Core::TOK_Marker) {applyMarker(itr->str); buf.erase(itr++); --itr; continue;}
               if(itr->tok == Core::TOK_ParenO) ++depth;
               if(itr->tok == Core::TOK_ParenC) {if(depth) --depth; else break;}

               if(itr->tok == Core::TOK_EOF)
               {
                  std::cerr << "ERROR: " << first->pos << ": unterminated macro call\n";
                  throw EXIT_FAILURE;
               }
            }

            auto argLast = itr++; ++argFirst;

            buf.emplace(first, first->pos, first->str, Core::TOK_Marker);

            expand(first, *macro, Core::MakeRange(argFirst, argLast));

            buf.emplace(first, first->pos, first->str, Core::TOK_Marker);

            buf.erase(first, itr);
         }

         // Object-like macro.
         else
         {
            buf.emplace(itr, itr->pos, itr->str, Core::TOK_Marker);

            expand(itr, *macro, Core::MakeRange(itr, itr));

            buf.emplace(itr, itr->pos, itr->str, Core::TOK_Marker);

            buf.erase(itr);
         }

         return true;
      }

      //
      // MacroTBuf::expand
      //
      void MacroTBuf::expand(Itr e, Macro const &macro, Rng const &argRng)
      {
         auto argv = MakeArgs(macro, argRng);

         for(auto itr = macro.list.begin(), end = macro.list.end(); itr != end; ++itr)
         {
            // # operator
            if(itr->tok == Core::TOK_Hash && macro.func)
            {
               Rng const *arg;

               while(itr + 1 != end &&
                  (itr[1].tok == Core::TOK_WSpace || itr[1].tok == Core::TOK_LnEnd))
                  ++itr;

               if(itr + 1 == end || !(arg = FindArg(itr[1], macro, argv)))
               {
                  std::cerr << "ERROR: " << e->pos << ": # not followed by arg\n";
                  throw EXIT_FAILURE;
               }

               buf.emplace(e, e->pos, stringize(*arg), Core::TOK_String);
               ++itr;
            }

            // ## operator
            else if(itr->tok == Core::TOK_Hash2)
            {
               if((++itr)->tok == Core::TOK_WSpace) ++itr;

               // Search backwards for token to concatenate to.
               auto tok = e; --tok;
               for(;; --tok)
               {
                  if(tok->tok == Core::TOK_WSpace || tok->tok == Core::TOK_LnEnd) continue;
                  if(tok->tok == Core::TOK_Marker && tok->str) continue;

                  break;
               }

               if(auto arg = FindArg(*itr, macro, argv))
               {
                  auto argItr = arg->begin(), argEnd = arg->end();

                  // Search for token to concatenate from.
                  for(;; ++argItr)
                  {
                     if(argItr->tok == Core::TOK_WSpace || argItr->tok == Core::TOK_LnEnd)
                        continue;
                     if(argItr->tok == Core::TOK_Marker && argItr->str)
                        {buf.emplace(e, *argItr); continue;}

                     break;
                  }

                  if(argItr != argEnd)
                  {
                     *tok = Macro::Concat(*tok, *argItr++);

                     for(; argItr != argEnd; ++argItr)
                        buf.emplace(e, e->pos, argItr->str, argItr->tok);
                  }
               }
               else
                  *tok = Macro::Concat(*tok, *itr);
            }

            // Argument substitution.
            else if(auto arg = FindArg(*itr, macro, argv))
            {
               if(IsConcat(itr, macro))
               {
                  if(IsEmpty(*arg))
                     buf.emplace(e, e->pos, Core::STRNULL, Core::TOK_Marker);

                  else for(auto const &argTok : *arg)
                     buf.emplace(e, e->pos, argTok.str, argTok.tok);
               }
               else
               {
                  // Mark start of fully expanded tokens.
                  auto eTmp = buf.emplace(e, e->pos, Core::STR_TOK_Add, Core::TOK_Marker);

                  // Use temporary buffer to fully expand argument.

                  std::vector<Core::Token> toks; toks.reserve(arg->size());

                  // Add tokens.
                  for(auto const &argTok : *arg)
                     toks.emplace_back(e->pos, argTok.str, argTok.tok);

                  // Build token buffer.
                  Core::ArrayTBuf abuf{toks.data(), toks.size()};
                  MacroTBuf mbuf{abuf, macros};

                  // Read from buffer.
                  while(mbuf.peek().tok != Core::TOK_EOF)
                     buf.emplace(e, mbuf.get());

                  // Mark end-1 of fully expanded tokens.
                  if(++eTmp != e) --(eTmp = e);
                  buf.emplace(eTmp, e->pos, Core::STR_TOK_Sub, Core::TOK_Marker);
               }
            }

            // Drop whitespace preceding a ##.
            else if(itr->tok != Core::TOK_WSpace || !IsConcat(itr, macro))
               buf.emplace(e, e->pos, itr->str, itr->tok);
         }
      }

      //
      // MacroTBuf::stringize
      //
      Core::String MacroTBuf::stringize(Rng const &arg)
      {
         std::string tmp;

         tmp += '"';

         auto itr = arg.begin(), end = arg.end();

         // Skip leading whitespace.
         while(itr != end && (itr->tok == Core::TOK_WSpace || itr->tok == Core::TOK_LnEnd))
            ++itr;

         // Stringize tokens.
         for(; itr != end; ++itr)
            Macro::Stringize(tmp, *itr);

         // Discard trailing whitespace.
         while(std::isspace(tmp.back())) tmp.pop_back();

         tmp += '"';

         return {tmp.data(), tmp.size()};
      }

      //
      // MacroTBuf::underflow
      //
      void MacroTBuf::underflow()
      {
         if(tptr() != tend()) return;

         if(!buf.empty()) buf.pop_front();

         while(expand(buf.begin())) {}

         if(buf.empty())
            sett(nullptr, nullptr, nullptr);
         else
            sett(&buf.front(), &buf.front(), &buf.front() + 1);
      }
   }
}

// EOF

