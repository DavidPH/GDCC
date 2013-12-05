//-----------------------------------------------------------------------------
//
// Copyright (C) 2013 David Hill
//
// See COPYING for license information.
//
//-----------------------------------------------------------------------------
//
// Abstract Syntax Tree primary/primitive expression types.
//
//-----------------------------------------------------------------------------

#include "Primary.hpp"

#include "Bytecode/Platform.hpp"
#include "GDCC/IR/Type.hpp"


//----------------------------------------------------------------------------|
// Global Functions                                                           |
//

namespace GDCC
{
   namespace AST
   {
      //
      // Type::GetNone
      //
      Type::CRef Type::GetNone()
      {
         static CRef t{new Type_None};
         return t;
      }

      //
      // Type::GetSize
      //
      Type::CRef Type::GetSize()
      {
         static CRef t{new Type_Size};
         return t;
      }

      //
      // Type_Size::getIRType
      //
      IR::Type Type_Size::getIRType() const
      {
         return IR::Type_Fixed(getSizeBitsI(), getSizeBitsF(), getSizeBitsS(), isTypeSaturate());
      }

      //
      // Type_Size::getSizeAlign
      //
      FastU Type_Size::getSizeAlign() const
      {
         return getSizeBytes();
      }

      //
      // Type_Size::getSizeBitsI
      //
      FastU Type_Size::getSizeBitsI() const
      {
         return 32;
      }

      //
      // Type_Size::getSizeBytes
      //
      FastU Type_Size::getSizeBytes() const
      {
         switch(Bytecode::TargetCur)
         {
         case Bytecode::Target::None: throw TypeError();

         case Bytecode::Target::MageCraft: return 4;
         case Bytecode::Target::ZDoom:     return 1;
         }
      }

      //
      // Type_Size::getSizePoint
      //
      FastU Type_Size::getSizePoint() const
      {
         return getSizeWords();
      }

      //
      // Type_Size::getSizeShift
      //
      FastU Type_Size::getSizeShift() const
      {
         return getSizeAlign();
      }

      //
      // Type_Size::getSizeWords
      //
      FastU Type_Size::getSizeWords() const
      {
         return 1;
      }
   }
}

// EOF

