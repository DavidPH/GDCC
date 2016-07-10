//-----------------------------------------------------------------------------
//
// Copyright (C) 2014-2016 David Hill
//
// See COPYING for license information.
//
//-----------------------------------------------------------------------------
//
// IR statement generation utilities.
//
//-----------------------------------------------------------------------------

#ifndef GDCC__SR__GenStmnt_H__
#define GDCC__SR__GenStmnt_H__


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

   namespace SR
   {
      class Function;

      //
      // GenStmntCtx
      //
      // Stores the context for IR statement generation.
      //
      class GenStmntCtx
      {
      public:
         IR::Block   &block;
         Function    *fn;
         IR::Program &prog;
      };
   }
}

#endif//GDCC__SR__GenStmnt_H__

