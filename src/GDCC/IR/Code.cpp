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

#include "Code.hpp"

#include "../String.hpp"


//----------------------------------------------------------------------------|
// Global Functions                                                           |
//

namespace GDCC
{
   namespace IR
   {
      //
      // operator OArchive << Code
      //
      OArchive &operator << (OArchive &out, Code in)
      {
         switch(in)
         {
            #define GDCC_IR_CodeList(name) \
               case Code::name: return out << STR_##name;
            #include "CodeList.hpp"
         case Code::None: return out << STR_None;
         }
      }
   }
}

// EOF

