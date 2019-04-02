//-----------------------------------------------------------------------------
//
// Copyright (C) 2013-2018 David Hill
//
// See COPYING for license information.
//
//-----------------------------------------------------------------------------
//
// Linkage type handling.
//
//-----------------------------------------------------------------------------

#ifndef GDCC__IR__Linkage_H__
#define GDCC__IR__Linkage_H__

#include "../IR/Types.hpp"

#include <ostream>


//----------------------------------------------------------------------------|
// Types                                                                      |
//

namespace GDCC::IR
{
   //
   // Linkage
   //
   enum class Linkage
   {
      #define GDCC_IR_LinkageList(name) name,
      #include "../IR/LinkageList.hpp"
   };
}


//----------------------------------------------------------------------------|
// Extern Functions                                                           |
//

namespace GDCC::IR
{
   OArchive &operator << (OArchive &out, Linkage in);
   std::ostream &operator << (std::ostream &out, Linkage in);

   IArchive &operator >> (IArchive &in, Linkage &out);

   Linkage GetLinkageExt(Linkage linka);

   Linkage GetLinkageInt(Linkage linka);
}

#endif//GDCC__IR__Linkage_H__

