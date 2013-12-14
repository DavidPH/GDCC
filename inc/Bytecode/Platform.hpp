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

#ifndef GDCC__Bytecode__Platform_H__
#define GDCC__Bytecode__Platform_H__


//----------------------------------------------------------------------------|
// Types                                                                      |
//

namespace GDCC
{
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
}


//----------------------------------------------------------------------------|
// Global Variables                                                           |
//

namespace GDCC
{
   namespace Bytecode
   {
      extern Format FormatCur;
      extern Target TargetCur;
   }
}


//----------------------------------------------------------------------------|
// Global Functions                                                           |
//

namespace GDCC
{
   namespace Bytecode
   {
      unsigned GetWordAlign();
      unsigned GetWordBytes();
      unsigned GetWordShift();
   }
}

#endif//GDCC__Bytecode__Platform_H__

