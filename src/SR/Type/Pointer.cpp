//-----------------------------------------------------------------------------
//
// Copyright (C) 2013-2016 David Hill
//
// See COPYING for license information.
//
//-----------------------------------------------------------------------------
//
// Source Representation expression pointer/reference type handling.
//
//-----------------------------------------------------------------------------

#include "SR/Type/Pointer.hpp"

#include "IR/CallType.hpp"
#include "IR/Type.hpp"

#include "Platform/Platform.hpp"


//----------------------------------------------------------------------------|
// Extern Functions                                                           |
//

namespace GDCC
{
   namespace SR
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
            return IR::Type_Funct(IR::GetCallTypeIR(base->getCallType()));

         if(base->isTypeLabel())
            return IR::Type_DJump();

         if(base->isTypeStrEnt())
            return IR::Type_StrEn();

         // Map Gen for IR.
         auto addr = base->getQualAddr();
         if(addr.base == IR::AddrBase::Gen)
            addr = IR::GetAddrGen();

         // If underlying type is incomplete, don't fail to generate IR type.
         Core::FastU sizePoint = 0;
         Core::FastU sizeShift = 0;
         try
         {
            // Check sizeShift first, because it affects representation.
            sizeShift = base->getSizeShift();
            sizePoint = base->getSizePoint();
         }
         catch(TypeError const &) {}

         return IR::Type_Point(addr.base, addr.name, sizePoint, sizeShift);
      }

      //
      // Type_PtrBase::getSizeAlign
      //
      Core::FastU Type_PtrBase::getSizeAlign() const
      {
         return Platform::GetWordAlign();
      }

      //
      // Type_PtrBase::getSizeBytes
      //
      Core::FastU Type_PtrBase::getSizeBytes() const
      {
         return getSizeWords() * Platform::GetWordBytes();
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
         return Platform::GetWordShift();
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

         if(base->isTypeStrEnt())
            return 1;

         if(base->isCTypeObject()) switch(base->getQualAddr().base)
         {
         case IR::AddrBase::Far:
         case IR::AddrBase::GblArs:
         case IR::AddrBase::HubArs:
         case IR::AddrBase::ModArs:
         case IR::AddrBase::StrArs:
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

