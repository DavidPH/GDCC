//-----------------------------------------------------------------------------
//
// Copyright (C) 2013-2016 David Hill
//
// See COPYING for license information.
//
//-----------------------------------------------------------------------------
//
// Source Representation primary/primitive expression types.
//
//-----------------------------------------------------------------------------

#include "SR/Type/Primary.hpp"

#include "IR/Type.hpp"

#include "Platform/Platform.hpp"


//----------------------------------------------------------------------------|
// Extern Functions                                                           |
//

namespace GDCC
{
   namespace SR
   {
      Type::CRef Type::GetLabel()  {static CRef t{new Type_Label }; return t;}
      Type::CRef Type::GetNone()   {static CRef t{new Type_None  }; return t;}
      Type::CRef Type::GetSize()   {static CRef t{new Type_Size  }; return t;}
      Type::CRef Type::GetStrEnt() {static CRef t{new Type_StrEnt}; return t;}
      Type::CRef Type::GetVoid()   {static CRef t{new Type_Void  }; return t;}

      //
      // Type_Size::getIRType
      //
      IR::Type Type_Size::getIRType() const
      {
         return IR::Type_Fixed(getSizeBitsI(), getSizeBitsF(), getSizeBitsS(),
            isTypeSaturate());
      }

      //
      // Type_Size::getSizeAlign
      //
      Core::FastU Type_Size::getSizeAlign() const
      {
         return Platform::GetWordAlign();
      }

      //
      // Type_Size::getSizeBitsI
      //
      Core::FastU Type_Size::getSizeBitsI() const
      {
         return 32;
      }

      //
      // Type_Size::getSizeBytes
      //
      Core::FastU Type_Size::getSizeBytes() const
      {
         return Platform::GetWordBytes();
      }

      //
      // Type_Size::getSizePoint
      //
      Core::FastU Type_Size::getSizePoint() const
      {
         return 1;
      }

      //
      // Type_Size::getSizeShift
      //
      Core::FastU Type_Size::getSizeShift() const
      {
         return Platform::GetWordShift();
      }

      //
      // Type_Size::getSizeWords
      //
      Core::FastU Type_Size::getSizeWords() const
      {
         return 1;
      }

      //
      // Type_Void::getIRType
      //
      IR::Type Type_Void::getIRType() const
      {
         return IR::Type_Empty();
      }
   }
}

// EOF

