//-----------------------------------------------------------------------------
//
// Copyright (C) 2013 David Hill
//
// See COPYING for license information.
//
//-----------------------------------------------------------------------------
//
// Abstract Syntax Tree statement base.
//
//-----------------------------------------------------------------------------

#include "AST/Statement.hpp"

#include "IR/Block.hpp"


//----------------------------------------------------------------------------|
// Global Functions                                                           |
//

namespace GDCC
{
   namespace AST
   {
      //
      // Statement::genLabel
      //
      void Statement::genLabel(IR::Block &block) const
      {
         for(auto const &s : labels) block.addLabel(s);
         v_genLabel(block);
      }

      //
      // Statement::genStmnt
      //
      void Statement::genStmnt(IR::Block &block, Function *fn) const
      {
         for(auto const &s : labels) block.addLabel(s);

         if(isTrivial())
            v_genLabel(block);
         else
            v_genStmnt(block, fn);
      }

      //
      // Statement::v_genLabel
      //
      void Statement::v_genLabel(IR::Block &) const
      {
      }
   }
}

// EOF

