//-----------------------------------------------------------------------------
//
// Copyright (C) 2013 David Hill
//
// See COPYING for license information.
//
//-----------------------------------------------------------------------------
//
// Source position/origin information.
//
//-----------------------------------------------------------------------------

#include "Core/Origin.hpp"

#include "IR/IArchive.hpp"
#include "IR/OArchive.hpp"


//----------------------------------------------------------------------------|
// Global Functions                                                           |
//

namespace GDCC
{
   namespace Core
   {
      //
      // operator IR::OArchive << Origin
      //
      IR::OArchive &operator << (IR::OArchive &out, Origin const &in)
      {
         return out << in.file << in.line;
      }

      //
      // operator std::ostream << Origin
      //
      std::ostream &operator << (std::ostream &out, Origin const &in)
      {
         return out << in.file << ':' << in.line;
      }

      //
      // operator IR::IArchive >> Origin
      //
      IR::IArchive &operator >> (IR::IArchive &in, Origin &out)
      {
         return in >> out.file >> out.line;
      }
   }
}

// EOF

