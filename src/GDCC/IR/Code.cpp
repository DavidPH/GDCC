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

#include "IArchive.hpp"

#include "../String.hpp"

#include <iostream>


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

      //
      // operator IArchive >> Code
      //
      IArchive &operator >> (IArchive &in, Code &out)
      {
         switch(GetIR<StringIndex>(in))
         {
            #define GDCC_IR_CodeList(name) \
               case STR_##name: out = Code::name; return in;
            #include "CodeList.hpp"
         case STR_None: out = Code::None; return in;

         default:
            std::cerr << "invalid Code\n";
            throw EXIT_FAILURE;
         }
      }
   }
}

// EOF

