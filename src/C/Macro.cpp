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

#include <unordered_map>


//----------------------------------------------------------------------------|
// Static Variables                                                           |
//

static std::vector<std::pair<GDCC::String, std::size_t>> Lines;

static std::unordered_map<C::Macro::Name, C::Macro> Table;


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
   // Macro::Get
   //
   Macro const *Macro::Get(GDCC::Token const &tok)
   {
      if(tok.tok != GDCC::TOK_Identi) return nullptr;

      switch(static_cast<GDCC::StringIndex>(tok.str))
      {
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
   // Macro::Reset
   //
   void Macro::Reset()
   {
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
}

// EOF

