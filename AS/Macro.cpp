//-----------------------------------------------------------------------------
//
// Copyright (C) 2013-2018 David Hill
//
// See COPYING for license information.
//
//-----------------------------------------------------------------------------
//
// Assembly macros.
//
//-----------------------------------------------------------------------------

#include "AS/Macro.hpp"

#include "Core/Exception.hpp"
#include "Core/Token.hpp"


//----------------------------------------------------------------------------|
// Extern Functions                                                           |
//

namespace GDCC::AS
{
   //
   // Macro::expand
   //
   void Macro::expand(IR::Block &block, IR::Arg const *argv,
      std::size_t argc) const
   {
      for(auto const &stmnt : list)
      {
         auto args = stmnt.args;
         for(auto &arg : args) if(arg.a == IR::ArgBase::Cpy)
         {
            if(arg.aCpy.value >= argc)
               Core::Error(block.getOrigin(), "insufficient macro arguments");

            arg = argv[arg.aCpy.value];
         }

         block.addStmntArgs(stmnt.code, std::move(args));
      }
   }

   //
   // MacroMap::add
   //
   void MacroMap::add(Core::String name, Macro &&macro)
   {
      table.emplace(name, std::move(macro));
   }

   //
   // MacroMap::find
   //
   Macro const *MacroMap::find(Core::Token const &tok)
   {
      auto itr = table.find(tok.str);
      return itr == table.end() ? nullptr : &itr->second;
   }

   //
   // MacroMap::rem
   //
   void MacroMap::rem(Core::String name)
   {
      table.erase(name);
   }
}

// EOF

