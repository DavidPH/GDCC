//-----------------------------------------------------------------------------
//
// Copyright (C) 2013-2014 David Hill
//
// See COPYING for license information.
//
//-----------------------------------------------------------------------------
//
// Abstract Syntax Tree object handling.
//
//-----------------------------------------------------------------------------

#include "AST/Object.hpp"

#include "AST/Exp.hpp"
#include "AST/Storage.hpp"
#include "AST/Type.hpp"

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
      // Object constructor
      //
      Object::Object(Core::String name_, Core::String glyph_) :
         glyph{glyph_},
         init {nullptr},
         linka{IR::Linkage::None},
         name {name_},
         store{Storage::None},
         type {nullptr},
         value{nullptr},

         alias{false},
         defin{false},
         noPtr{false},
         point{false}
      {
      }

      //
      // Object destructor
      //
      Object::~Object()
      {
      }

      //
      // Object::Create
      //
      Object::Ref Object::Create(Core::String name, Core::String glyph)
      {
         return static_cast<Ref>(new Object(name, glyph));
      }
   }
}

// EOF

