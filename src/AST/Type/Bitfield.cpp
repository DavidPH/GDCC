//-----------------------------------------------------------------------------
//
// Copyright (C) 2013 David Hill
//
// See COPYING for license information.
//
//-----------------------------------------------------------------------------
//
// Abstract Syntax Tree expression bitfield type handling.
//
//-----------------------------------------------------------------------------

#include "AST/Type/Bitfield.hpp"

#include "IR/Type.hpp"


//----------------------------------------------------------------------------|
// Global Functions                                                           |
//

namespace GDCC
{
   namespace AST
   {
      //
      // Type::getTypeBitfield
      //
      Type::CRef Type::getTypeBitfield(Core::FastU bitsF, Core::FastU bitsI,
         Core::FastU bitsO) const
      {
         // Search for existing bitfield type.
         if(auto type = bitType) do
         {
            if(type->bitsF == bitsF && type->bitsI == bitsI && type->bitsO == bitsO)
               return static_cast<CRef>(type);

            type = type->bitNext;
         }
         while(type != bitType);

         // Otherwise, create one and return it.
         return static_cast<CRef>(new Type_Bitfield(this, bitsF, bitsI, bitsO));
      }

      //
      // Type_Bitfield copy constructor
      //
      Type_Bitfield::Type_Bitfield(Type_Bitfield const &type) : Super{type},
         bitsF{type.bitsF}, bitsI{type.bitsI}, bitsO{type.bitsO}, bitNext{this},
         bitPrev{this}
      {
      }

      //
      // Type_Bitfield constructor
      //
      Type_Bitfield::Type_Bitfield(Type const *base_, Core::FastU bitsF_,
         Core::FastU bitsI_, Core::FastU bitsO_) :
         Super{base_}, bitsF{bitsF_}, bitsI{bitsI_}, bitsO{bitsO_}
      {
         GDCC_AST_Type_Insert(bit);
      }

      //
      // Type_Bitfield destructor
      //
      Type_Bitfield::~Type_Bitfield()
      {
         GDCC_AST_Type_Unlink(bit);
      }

      //
      // Type_Bitfield::getIRType
      //
      IR::Type Type_Bitfield::getIRType() const
      {
         return IR::Type_Fixed(bitsI, bitsF, getSizeBitsS(), isTypeSaturate());
      }
   }
}

// EOF

