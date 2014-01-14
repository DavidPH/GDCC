//-----------------------------------------------------------------------------
//
// Copyright (C) 2013-2014 David Hill
//
// See COPYING for license information.
//
//-----------------------------------------------------------------------------
//
// Abstract Syntax Tree function handling.
//
//-----------------------------------------------------------------------------

#include "AST/Function.hpp"

#include "AST/Statement.hpp"
#include "AST/Type.hpp"

#include "IR/CallType.hpp"
#include "IR/Exp.hpp"
#include "IR/Linkage.hpp"
#include "IR/ScriptType.hpp"


//----------------------------------------------------------------------------|
// Global Functions                                                           |
//

namespace GDCC
{
   namespace AST
   {
      //
      // Function constructor
      //
      Function::Function(Core::String name_, Core::String glyph_) :
         ctype   {IR::CallType::None},
         glyph   {glyph_},
         label   {Core::STRNULL},
         linka   {IR::Linkage::None},
         localArs{0},
         localReg{0},
         name    {name_},
         param   {0},
         retrn   {nullptr},
         stmnt   {nullptr},
         stype   {IR::ScriptType::None},
         valueInt{nullptr},
         valueStr{Core::STRNULL},

         defin   {false},
         sflagNet{false},
         sflagClS{false}
      {
      }

      //
      // Function destructor
      //
      Function::~Function()
      {
      }

      //
      // Function::Create
      //
      Function::Ref Function::Create(Core::String name, Core::String glyph)
      {
         return static_cast<Ref>(new Function(name, glyph));
      }
   }
}

// EOF

