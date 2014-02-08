//-----------------------------------------------------------------------------
//
// Copyright (C) 2013-2014 David Hill
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

#include "IR/CallType.hpp"
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
         callt  {IR::CallType::None},
         linka  {IR::Linkage::None},
         name   {Core::STRNULL},
         namePos{Core::STRNULL, 0},
         param  {},
         type   {nullptr},

         funcInline  {false},
         funcNoParam {false},
         funcNoReturn{false},

         isTypedef{false},

         storeAuto  {false},
         storeExt   {false},
         storeInt   {false},
         storeReg   {false},
         storeThread{false}
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

