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
#include "IR/Program.hpp"


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
      // Object::getIRObject
      //
      IR::Object &Object::getIRObject(IR::Program &prog)
      {
         auto &obj = prog.getObject(glyph);

         obj.linka = linka;
         obj.space = type->getQualAddr();
         obj.words = type ? type->getSizeWords() : 0;

         obj.alias = alias;
         obj.defin = defin;

         if(value)
         {
            auto val = value->getValue();
            if(val.v == IR::ValueBase::Fixed)
               obj.value = number_cast<Core::FastU>(val.vFixed.value);
            else
               obj.alloc = true;
         }
         else
            obj.alloc = true;

         // Configure glyph's type, even if the glyph won't be backed.
         if(type)
            prog.getGlyphData(glyph).type = type->getTypePointer()->getIRType();

         return obj;
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

