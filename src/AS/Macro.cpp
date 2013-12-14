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

#include "AS/Macro.hpp"

#include <iostream>
#include <unordered_map>


//----------------------------------------------------------------------------|
// Static Variables                                                           |
//

static std::unordered_map<GDCC::Core::String, GDCC::AS::Macro> MacroMap;


//----------------------------------------------------------------------------|
// Global Functions                                                           |
//

namespace GDCC
{
   namespace AS
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
      void Macro::Add(Core::String name, IR::Block &&list)
      {
         MacroMap.emplace(std::piecewise_construct,
            std::forward_as_tuple(name),
            std::forward_as_tuple(name, std::move(list)));
      }

      //
      // Macro::Find
      //
      Macro const *Macro::Find(Core::String name)
      {
         auto itr = MacroMap.find(name);
         return itr == MacroMap.end() ? nullptr : &itr->second;
      }
   }
}

// EOF

