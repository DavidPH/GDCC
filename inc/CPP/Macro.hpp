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

#ifndef GDCC__CPP__Macro_H__
#define GDCC__CPP__Macro_H__

#include "../CPP/Types.hpp"

#include "../Core/Array.hpp"
#include "../Core/String.hpp"
#include "../Core/Token.hpp"

#include <unordered_map>
#include <vector>


//----------------------------------------------------------------------------|
// Types                                                                      |
//

namespace GDCC::CPP
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

      // Returns true if the arguments (or lack thereof) and replacement lists
      // are equal, as defined in C. That is, whitespace separation is
      // considered equal regardless of composition.
      bool operator == (Macro const &other) const;

      bool operator != (Macro const &other) const {return !(*this == other);}

      Args args;
      List list;

      bool func : 1;


      // Concatenates two tokens.
      static Core::Token Concat(Core::Token const &l, Core::Token const &r);

      static Core::String MakeString(std::size_t i);

      static Core::String Stringize(Core::String str);

      static void Stringize(std::string &tmp, Core::Token const &tok);
   };

   //
   // MacroMap
   //
   class MacroMap
   {
   public:
      explicit MacroMap(Core::String file, std::size_t line = 0);

      // Adds a macro.
      void add(Core::String name, Macro const &macro);
      void add(Core::String name, Macro &&macro);

      // Gets the macro by the specified name or null if not defined.
      Macro const *find(Core::Token const &tok);

      // Removes a __FILE__/__LINE__ tracker.
      void lineDrop();

      // Sets the __FILE__ string.
      void lineFile(Core::String file);

      // Sets the __LINE__ offset. This value is added to the token's.
      void lineLine(std::size_t line);

      // Adds a __FILE__/__LINE__ tracker.
      void linePush(Core::String file, std::size_t line = 0);

      // Removes a macro.
      void rem(Core::String name);

      void reset();

   private:
      std::vector<std::pair<Core::String, std::size_t>> lines;
      std::unordered_map<Core::String, Macro>           table;

      Macro macroDATE, macroFILE, macroLINE, macroTIME;
   };
}

#endif//GDCC__CPP__Macro_H__

