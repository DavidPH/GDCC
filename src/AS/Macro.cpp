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

#include "AS/Macro.hpp"

#include "Core/Exception.hpp"
#include "Core/Token.hpp"


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
                  throw Core::ExceptStr(block.getOrigin(),
                     "insufficient macro arguments");

               arg = argv[arg.aCpy.value];
            }

            block.addStatementArgs(stmnt.code, std::move(args));
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
}

// EOF

