//-----------------------------------------------------------------------------
//
// Copyright (C) 2013-2014 David Hill
//
// See COPYING for license information.
//
//-----------------------------------------------------------------------------
//
// Linkage type handling.
//
//-----------------------------------------------------------------------------

#ifndef GDCC__IR__Types_H__
#define GDCC__IR__Types_H__

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
      // Linkage
      //
      enum class Linkage
      {
         #define GDCC_IR_LinkageList(name) name,
         #include "../IR/LinkageList.hpp"
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
      OArchive &operator << (OArchive &out, Linkage in);
      std::ostream &operator << (std::ostream &out, Linkage in);

      IArchive &operator >> (IArchive &in, Linkage &out);

      Linkage GetLinkageExt(Linkage linka);

      Linkage GetLinkageInt(Linkage linka);
   }
}

#endif//GDCC__IR__Types_H__

