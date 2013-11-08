//-----------------------------------------------------------------------------
//
// Copyright (C) 2013 David Hill
//
// See COPYING for license information.
//
//-----------------------------------------------------------------------------
//
// Assembly macros.
//
//-----------------------------------------------------------------------------

#include "Macro.hpp"

#include <iostream>
#include <unordered_map>


//----------------------------------------------------------------------------|
// Static Variables                                                           |
//

static std::unordered_map<GDCC::String, Asm::Macro> MacroMap;


//----------------------------------------------------------------------------|
// Global Functions                                                           |
//

namespace Asm
{
   //
   // Macro::expand
   //
   void Macro::expand(GDCC::IR::Block &block, GDCC::IR::Arg const *argv,
      std::size_t argc) const
   {
      for(auto const &stmnt : list)
      {
         auto args = stmnt.args;
         for(auto &arg : args) if(arg.a == GDCC::IR::ArgBase::Cpy)
         {
            if(arg.aCpy.value >= argc)
            {
               std::cerr << "ERROR: " << block.getOrigin()
                  << ": insufficient macro arguments\n";
               throw EXIT_FAILURE;
            }

            arg = argv[arg.aCpy.value];
         }

         block.addStatementArgs(stmnt.code, std::move(args));
      }
   }

   //
   // Macro::Add
   //
   void Macro::Add(GDCC::String name, GDCC::IR::Block &&list)
   {
      MacroMap.emplace(std::piecewise_construct,
         std::forward_as_tuple(name),
         std::forward_as_tuple(name, std::move(list)));
   }

   //
   // Macro::Find
   //
   Macro const *Macro::Find(GDCC::String name)
   {
      auto itr = MacroMap.find(name);
      return itr == MacroMap.end() ? nullptr : &itr->second;
   }
}

// EOF

