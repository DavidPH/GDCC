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
         noRef{false},
         refer{false}
      {
      }

      //
      // Object destructor
      //
      Object::~Object()
      {
      }

      //
      // Object::genObject
      //
      void Object::genObject(IR::Program &prog)
      {
         IR::Object obj{glyph};

         obj.linka = linka;
         obj.space = type->getQualAddr();
         obj.words = type ? type->getSizeWords() : 0;

         obj.alias = alias;
         obj.defin = defin;

         // Map generic address space to implementation.
         if(obj.space.base == IR::AddrBase::Gen)
            obj.space = IR::GetAddrGen();

         if(init && init->isIRExp())
         {
            obj.initi = init->getIRExp();
            init      = nullptr;
         }

         if(value)
         {
            auto val = value->getValue();
            if(val.v == IR::ValueBase::Fixed)
               obj.value = Core::NumberCast<Core::FastU>(val.vFixed.value);
            else
               obj.alloc = true;
         }
         else
            obj.alloc = true;

         // Configure glyph's type, even if the glyph won't be backed.
         if(type)
            prog.getGlyphData(glyph).type = type->getTypePointer()->getIRType();

         // Merge into existing object (if any).
         prog.mergeObject(prog.getObject(glyph), std::move(obj));
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

