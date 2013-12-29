//-----------------------------------------------------------------------------
//
// Copyright (C) 2013 David Hill
//
// See COPYING for license information.
//
//-----------------------------------------------------------------------------
//
// Abstract Syntax Tree space handling.
//
//-----------------------------------------------------------------------------

#include "AST/Space.hpp"

#include "IR/Addr.hpp"
#include "IR/Exp.hpp"
#include "IR/Linkage.hpp"


//----------------------------------------------------------------------------|
// Global Functions                                                           |
//

namespace GDCC
{
   namespace AST
   {
      //
      // Space constructor
      //
      Space::Space(Core::String name_, Core::String glyph_) :
         glyph{glyph_},
         linka{IR::Linkage::None},
         name {name_},
         space{IR::AddrBase::Gen},
         value{nullptr},

         defin{false}
      {
      }

      //
      // Space destructor
      //
      Space::~Space()
      {
      }

      //
      // Space::Create
      //
      Space::Ref Space::Create(Core::String name, Core::String glyph)
      {
         return static_cast<Ref>(new Space(name, glyph));
      }
   }
}

// EOF

