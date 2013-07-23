//-----------------------------------------------------------------------------
//
// Copyright (C) 2013 David Hill
//
// See COPYING for license information.
//
//-----------------------------------------------------------------------------
//
// Platform information/selection.
//
//-----------------------------------------------------------------------------

#ifndef Bytecode__Platform_H__
#define Bytecode__Platform_H__


//----------------------------------------------------------------------------|
// Types                                                                      |
//

namespace Bytecode
{
   //
   // Format
   //
   enum class Format
   {
      None,
      ACSE,
      MgC_NTS,
   };

   //
   // Target
   //
   enum class Target
   {
      None,
      MageCraft,
      ZDoom,
   };
}


//----------------------------------------------------------------------------|
// Global Variables                                                           |
//

namespace Bytecode
{
   extern Format FormatCur;
   extern Target TargetCur;
}

#endif//Bytecode__Platform_H__

