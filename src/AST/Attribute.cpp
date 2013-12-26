//-----------------------------------------------------------------------------
//
// Copyright (C) 2013 David Hill
//
// See COPYING for license information.
//
//-----------------------------------------------------------------------------
//
// Abstract Syntax Tree generalized attribute handling.
//
//-----------------------------------------------------------------------------

#include "AST/Attribute.hpp"

#include "AST/Type.hpp"

#include "IR/Linkage.hpp"


//----------------------------------------------------------------------------|
// Global Functions                                                           |
//

namespace GDCC
{
   namespace AST
   {
      //
      // Attribute constructor
      //
      Attribute::Attribute() :
         linka{IR::Linkage::None},
         name {Core::STRNULL},
         type {nullptr}
      {
      }

      //
      // Attribute destructor
      //
      Attribute::~Attribute()
      {
      }
   }
}

// EOF

