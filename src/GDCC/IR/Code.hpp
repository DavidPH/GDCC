//-----------------------------------------------------------------------------
//
// Copyright (C) 2013 David Hill
//
// See COPYING for license information.
//
//-----------------------------------------------------------------------------
//
// Intermediary Representation code handling.
//
//-----------------------------------------------------------------------------

#ifndef GDCC__IR__Code_H__
#define GDCC__IR__Code_H__


//----------------------------------------------------------------------------|
// Types                                                                      |
//

namespace GDCC
{
   namespace IR
   {
      //
      // Code
      //
      enum class Code
      {
         #define GDCC_IR_CodeList(name) name,
         #include "CodeList.hpp"
         None
      };
   }
}

#endif//GDCC__IR__Code_H__

