//-----------------------------------------------------------------------------
//
// Copyright (C) 2015 David Hill
//
// See COPYING for license information.
//
//-----------------------------------------------------------------------------
//
// ACS property-based declarations.
//
//-----------------------------------------------------------------------------

#include "ACC/PropDecl.hpp"


//----------------------------------------------------------------------------|
// Extern Functions                                                           |
//

namespace GDCC
{
   namespace ACC
   {
      //
      // CreateTransPropName::operator < CreateTransPropName
      //
      bool CreateTransPropName::operator < (CreateTransPropName const &r) const
      {
         if(prefix.tok != r.prefix.tok) return prefix.tok < r.prefix.tok;
         if(prefix.str != r.prefix.str) return prefix.str < r.prefix.str;

         return std::lexicographical_compare(std::begin(argc), std::end(argc),
            std::begin(r.argc), std::end(r.argc));

         return false;
      }
   }
}

// EOF

