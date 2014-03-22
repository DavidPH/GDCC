//-----------------------------------------------------------------------------
//
// Copyright (C) 2014 David Hill
//
// See COPYING for license information.
//
//-----------------------------------------------------------------------------
//
// IR statement generation utilities.
//
//-----------------------------------------------------------------------------

#ifndef GDCC__AST__GenStmnt_H__
#define GDCC__AST__GenStmnt_H__


//----------------------------------------------------------------------------|
// Types                                                                      |
//

namespace GDCC
{
   namespace IR
   {
      class Block;
      class Program;
   }

   namespace AST
   {
      class Function;

      //
      // GenStmntCtx
      //
      // Stores the context for IR statement generation.
      //
      struct GenStmntCtx
      {
         IR::Block   &block;
         Function    *fn;
         IR::Program &prog;
      };
   }
}

#endif//GDCC__AST__GenStmnt_H__

