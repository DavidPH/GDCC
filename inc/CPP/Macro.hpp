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

#ifndef GDCC__CPP__Macro_H__
#define GDCC__CPP__Macro_H__

#include "../Core/Array.hpp"
#include "../Core/String.hpp"
#include "../Core/Token.hpp"
#include "../Core/Utility.hpp"


//----------------------------------------------------------------------------|
// Types                                                                      |
//

namespace GDCC
{
   namespace CPP
   {
      //
      // Macro
      //
      class Macro
      {
      public:
         using Args = Core::Array<Core::String>;
         using List = Core::Array<Core::Token>;
         using Name = Core::String;


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

         // Returns true if the arguments (or lack thereof) and replacement
         // lists are equal, as defined in C. That is, whitespace separation is
         // considered equal regardless of composition.
         bool operator == (Macro const &other) const;

         bool operator != (Macro const &other) const {return !(*this == other);}

         Args args;
         List list;

         bool func : 1;


         // Adds a macro.
         static void Add(Name name, Macro const &macro);
         static void Add(Name name, Macro &&macro);

         // Concatenates two tokens.
         static Core::Token Concat(Core::Token const &l, Core::Token const &r);

         // Gets the macro by the specified name or null if not defined.
         static Macro const *Get(Core::Token const &tok);

         // Removes a __FILE__/__LINE__ tracker.
         static void LineDrop();

         // Sets the __FILE__ string.
         static void LineFile(Core::String file);

         // Sets the __LINE__ offset. This value is added to the token's.
         static void LineLine(std::size_t line);

         // Adds a __FILE__/__LINE__ tracker.
         static void LinePush(Core::String file, std::size_t line = 0);

         static Core::String MakeString(std::size_t i);

         // Removes a macro.
         static void Rem(Name name);

         // Clears all macros and (re-)applies command line definitions.
         static void Reset();

         static Core::String Stringize(Core::String str);

         static void Stringize(std::string &tmp, Core::Token const &tok);
      };
   }
}

#endif//GDCC__CPP__Macro_H__

