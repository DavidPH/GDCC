//-----------------------------------------------------------------------------
//
// Copyright (C) 2013 David Hill
//
// See COPYING for license information.
//
//-----------------------------------------------------------------------------
//
// Abstract Syntax Tree expression pointer/reference type handling.
//
//-----------------------------------------------------------------------------

#include "AST/Type/Pointer.hpp"

#include "Bytecode/Platform.hpp"

#include "IR/Type.hpp"


//----------------------------------------------------------------------------|
// Global Functions                                                           |
//

namespace GDCC
{
   namespace AST
   {
      //
      // Type::getTypePointer
      //
      Type::CRef Type::getTypePointer() const
      {
         if(!ptrType) new Type_Pointer(this);
         return static_cast<CRef>(ptrType);
      }

      //
      // Type::getTypeRefL
      //
      Type::CRef Type::getTypeRefL() const
      {
         if(!lvrType) new Type_RefL(this);
         return static_cast<CRef>(lvrType);
      }

      //
      // Type::getTypeRefR
      //
      Type::CRef Type::getTypeRefR() const
      {
         if(!rvrType) new Type_RefR(this);
         return static_cast<CRef>(rvrType);
      }

      //
      // Type_PtrBase::getIRType
      //
      IR::Type Type_PtrBase::getIRType() const
      {
         if(base->isTypeFunction())
            return IR::Type_Funct(base->getCallType());

         return IR::Type_Point(base->getQualAddr().base, base->getSizePoint(),
            base->getSizeShift());
      }

      //
      // Type_PtrBase::getSizeAlign
      //
      Core::FastU Type_PtrBase::getSizeAlign() const
      {
         return Bytecode::GetWordAlign();
      }

      //
      // Type_PtrBase::getSizeBytes
      //
      Core::FastU Type_PtrBase::getSizeBytes() const
      {
         return getSizeWords() * Bytecode::GetWordBytes();
      }

      //
      // Type_PtrBase::getSizePoint
      //
      Core::FastU Type_PtrBase::getSizePoint() const
      {
         // Pointers to pointers are word-oriented pointers.
         return getSizeWords();
      }

      //
      // Type_PtrBase::getSizeShift
      //
      Core::FastU Type_PtrBase::getSizeShift() const
      {
         return Bytecode::GetWordShift();
      }

      //
      // Type_PtrBase::getSizeWords
      //
      Core::FastU Type_PtrBase::getSizeWords() const
      {
         if(base->isTypeFunction()) switch(base->getCallType())
         {
         default:
            return 1;
         }

         if(base->isTypeLabel())
            return 1;

         if(base->isCTypeObject()) switch(base->getQualAddr().base)
         {
         case IR::AddrBase::Far:
         case IR::AddrBase::GblArs:
         case IR::AddrBase::MapArs:
         case IR::AddrBase::StrArs:
         case IR::AddrBase::WldArs:
            return 2;

         default:
            return 1;
         }

         return 1;
      }

      //
      // Type_PtrBase::isTypeVM
      //
      bool Type_PtrBase::isTypeVM() const
      {
         // Pointers are a variably-modified type if they refer to a VLA.
         return base->isTypeArray() && base->isTypeSizeVM();
      }

      //
      // Type_Pointer constructor
      //
      Type_Pointer::Type_Pointer(Type const *base_) : Super{base_}
      {
         base->ptrType = this;
      }

      //
      // Type_Pointer destructor
      //
      Type_Pointer::~Type_Pointer()
      {
         // Only nullify base's reference if this is the unqualified pointer.
         if(base->ptrType == this)
            base->ptrType = nullptr;
      }

      //
      // Type_RefL constructor
      //
      Type_RefL::Type_RefL(Type const *base_) : Super{base_}
      {
         base->lvrType = this;
      }

      //
      // Type_RefL destructor
      //
      Type_RefL::~Type_RefL()
      {
         // Only nullify base's reference if this is the unqualified pointer.
         if(base->lvrType == this)
            base->lvrType = nullptr;
      }

      //
      // Type_RefR constructor
      //
      Type_RefR::Type_RefR(Type const *base_) : Super{base_}
      {
         base->rvrType = this;
      }

      //
      // Type_RefR destructor
      //
      Type_RefR::~Type_RefR()
      {
         // Only nullify base's reference if this is the unqualified pointer.
         if(base->rvrType == this)
            base->rvrType = nullptr;
      }
   }
}

// EOF

