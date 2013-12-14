//-----------------------------------------------------------------------------
//
// Copyright (C) 2013 David Hill
//
// See COPYING for license information.
//
//-----------------------------------------------------------------------------
//
// Abstract Syntax Tree expression wrapping type base.
//
//-----------------------------------------------------------------------------

#include "AST/Type/Wrapper.hpp"

#include "AST/Exp.hpp"

#include "IR/Type.hpp"


//----------------------------------------------------------------------------|
// Global Functions                                                           |
//

namespace GDCC
{
   namespace AST
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

