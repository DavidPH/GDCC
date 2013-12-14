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

#include <ostream>


//----------------------------------------------------------------------------|
// Types                                                                      |
//

namespace GDCC
{
   namespace IR
   {
      class IArchive;
      class OArchive;

      //
      // Code
      //
      enum class Code
      {
         #define GDCC_IR_CodeList(name) name,
         #include "../IR/CodeList.hpp"
         None
      };
   }
}


//----------------------------------------------------------------------------|
// Global Functions                                                           |
//

namespace GDCC
{
   namespace IR
   {
      OArchive &operator << (OArchive &out, Code in);
      std::ostream &operator << (std::ostream &out, Code in);

      IArchive &operator >> (IArchive &in, Code &out);
   }
}

#endif//GDCC__IR__Code_H__

