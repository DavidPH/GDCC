//-----------------------------------------------------------------------------
//
// Copyright (C) 2013 David Hill
//
// See COPYING for license information.
//
//-----------------------------------------------------------------------------
//
// Intermediary Representation input.
//
//-----------------------------------------------------------------------------

#include "IArchive.hpp"

#include <iostream>
#include <cstring>


//----------------------------------------------------------------------------|
// Global Functions                                                           |
//

namespace GDCC
{
   namespace IR
   {
      //
      // IArchive constructor
      //
      IArchive::IArchive(std::istream &in)
      {
         std::size_t count = 0;
         for(int c; (c = in.get()), in; data.push_back(static_cast<char>(c)))
            if(!c) ++count;

         str = Array<char const *>(count);
         auto si = itr = str.begin();

         *si++ = data.data();
         for(auto ci = data.begin(), ce = data.end(); ci != ce; ++ci)
            if(!*ci) *si++ = &ci[1];
      }

      //
      // IArchive::getHeader
      //
      IArchive &IArchive::getHeader()
      {
         if(!std::strcmp(get(), "MgC_NTS")  ||
            !std::strcmp(get(), "GDCC::IR") ||
            !std::strcmp(get(), ""))
         {
            std::cerr << "not IR\n";
            throw EXIT_FAILURE;
         }

         return getTables();

         return *this;
      }
   }
}

// EOF

