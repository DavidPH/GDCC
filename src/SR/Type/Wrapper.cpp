//-----------------------------------------------------------------------------
//
// Copyright (C) 2013-2016 David Hill
//
// See COPYING for license information.
//
//-----------------------------------------------------------------------------
//
// Source Representation expression wrapping type base.
//
//-----------------------------------------------------------------------------

#include "SR/Type/Wrapper.hpp"

#include "SR/Exp.hpp"

#include "IR/Type.hpp"


//----------------------------------------------------------------------------|
// Extern Functions                                                           |
//

namespace GDCC
{
   namespace SR
   {
      //
      // Type_Wrapper::getIRType
      //
      IR::Type Type_Wrapper::getIRType() const
      {
         return base->getIRType();
      }

      //
      // Type_Wrapper::getSizeBytesVM
      //
      Exp::CRef Type_Wrapper::getSizeBytesVM() const
      {
         return base->getSizeBytesVM();
      }

      //
      // Type_Wrapper::getSizePointVM
      //
      Exp::CRef Type_Wrapper::getSizePointVM() const
      {
         return base->getSizePointVM();
      }

      //
      // Type_Wrapper::getSizeWordsVM
      //
      Exp::CRef Type_Wrapper::getSizeWordsVM() const
      {
         return base->getSizeWordsVM();
      }
   }
}

// EOF

