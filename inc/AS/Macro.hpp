//-----------------------------------------------------------------------------
//
// Copyright (C) 2013-2014 David Hill
//
// See COPYING for license information.
//
//-----------------------------------------------------------------------------
//
// Assembly macros.
//
//-----------------------------------------------------------------------------

#ifndef GDCC__AS__Macro_H__
#define GDCC__AS__Macro_H__

#include "../IR/Block.hpp"

#include <unordered_map>


//----------------------------------------------------------------------------|
// Types                                                                      |
//

namespace GDCC
{
   namespace Core
   {
      class Token;
   }

   namespace AS
   {
      //
      // Macro
      //
      class Macro
      {
      public:
         explicit Macro(IR::Block &&list_) : list{std::move(list_)} {}

         void expand(IR::Block &block, IR::Arg const *argv,
            std::size_t argc) const;

      private:
         IR::Block list;
      };

      //
      // MacroMap
      //
      class MacroMap
      {
      public:
         // Adds a macro.
         void add(Core::String name, Macro &&macro);

         // Gets the macro by the specified name or null if not defined.
         Macro const *find(Core::Token const &tok);

         // Removes a macro.
         void rem(Core::String name);

      private:
         std::unordered_map<Core::String, Macro> table;
      };
   }
}

#endif//GDCC__AS__Macro_H__

