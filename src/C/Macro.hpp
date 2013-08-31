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

#ifndef C__Macro_H__
#define C__Macro_H__

#include "GDCC/Array.hpp"
#include "GDCC/String.hpp"
#include "GDCC/Token.hpp"
#include "GDCC/Utility.hpp"


//----------------------------------------------------------------------------|
// Types                                                                      |
//

namespace C
{
   //
   // Macro
   //
   class Macro
   {
   public:
      using Args = GDCC::Array<GDCC::String>;
      using List = GDCC::Array<GDCC::Token>;
      using Name = GDCC::String;


      Macro() = default;

      Macro(Args const &args_, List const &list_) :
          args{args_}, list{list_}, func{true} {}
      Macro(Args const &args_, List &&list_) :
         args{args_}, list{std::move(list_)}, func{true} {}
      Macro(Args &&args_, List const &list_) :
         args{std::move(args_)}, list{list_}, func{true} {}
      Macro(Args &&args_, List &&list_) :
         args{std::move(args_)}, list{std::move(list_)}, func{true} {}

      Macro(List const &list_) : list{list_}, func{false} {}
      Macro(List &&list_) : list{std::move(list_)}, func{false} {}

      // Returns true if the arguments (or lack thereof) and replacement lists
      // are equal, as defined in C. That is, whitespace separation is equal.
      bool operator == (Macro const &other) const;

      bool operator != (Macro const &other) const {return !(*this == other);}

      Args args;
      List list;

      bool func : 1;


      // Adds a macro.
      static void Add(Name name, Macro const &macro);
      static void Add(Name name, Macro &&macro);

      // Concatenates two tokens.
      static GDCC::Token Concat(GDCC::Token const &l, GDCC::Token const &r);

      // Gets the macro by the specified name or null if not defined.
      static Macro const *Get(GDCC::Token const &tok);

      // Removes a __FILE__/__LINE__ tracker.
      static void LineDrop();

      // Sets the __FILE__ string.
      static void LineFile(GDCC::String file);

      // Sets the __LINE__ offset. This value is added to the token's.
      static void LineLine(std::size_t line);

      // Adds a __FILE__/__LINE__ tracker.
      static void LinePush(GDCC::String file, std::size_t line = 0);

      static GDCC::String MakeString(std::size_t i);

      // Removes a macro.
      static void Rem(Name name);

      // Clears all macros and (re-)applies command line definitions.
      static void Reset();

      static GDCC::String Stringize(GDCC::String str);

      static void Stringize(std::string &tmp, GDCC::Token const &tok);
   };
}

#endif//C__Macro_H__

