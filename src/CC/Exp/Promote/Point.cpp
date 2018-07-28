//-----------------------------------------------------------------------------
//
// Copyright (C) 2014-2018 David Hill
//
// See COPYING for license information.
//
//-----------------------------------------------------------------------------
//
// C type promotion rules.
//
//-----------------------------------------------------------------------------

#include "CC/Exp.hpp"

#include "CC/Type.hpp"

#include "Core/Exception.hpp"

#include "SR/Exp.hpp"
#include "SR/Type.hpp"


//----------------------------------------------------------------------------|
// Static Functions                                                           |
//

namespace GDCC::CC
{
   //
   // AddrPromo
   //
   static IR::AddrSpace AddrPromo(IR::AddrSpace addrL, IR::AddrSpace addrR,
      Core::Origin pos)
   {
      if(IR::IsAddrEnclosed(addrL, addrR))
      {
         // If both enclose the other, they are either the same address space, or
         // one is the generic address space and the other is the implementation
         // of the generic address space. In the latter case, return the other
         // address space in keeping with producing the most-qualified type.
         if(IR::IsAddrEnclosed(addrR, addrL))
            return addrL.base == IR::AddrBase::Gen ? addrR : addrL;
         else
            return addrL;
      }
      else
      {
         if(IR::IsAddrEnclosed(addrR, addrL))
            return addrR;
         else
            Core::Error(pos, "cannot promote disjoint address spaces");
      }
   }

   //
   // QualPromo
   //
   static SR::TypeQual QualPromo(SR::TypeQual qualL, SR::TypeQual qualR,
      Core::Origin pos)
   {
      // Merge qualifiers.
      qualL.space  = AddrPromo(qualL.space, qualR.space, pos);
      qualL.aAtom |= qualR.aAtom;
      qualL.aCons |= qualR.aCons;
      qualL.aRest |= qualR.aRest;
      qualL.aVola |= qualR.aVola;

      return qualL;
   }
}


//----------------------------------------------------------------------------|
// Extern Functions                                                           |
//

namespace GDCC::CC
{
   //
   // ExpPromo_PtrEqu
   //
   std::tuple<SR::Type::CRef, SR::Exp::CRef, SR::Exp::CRef>
   ExpPromo_PtrEqu(SR::Exp const *l, SR::Exp const *r, Core::Origin pos)
   {
      SR::Exp::CRef expL{l}, expR{r};

      auto typeL = expL->getType();
      auto typeR = expR->getType();

      // Check if either operand is a null pointer constant.
      bool nulL = typeL->isCTypeInteg() && expL->isZero();
      bool nulR = typeR->isCTypeInteg() && expR->isZero();

      // Both operands must be pointers.
      if(!(nulL || typeL->isTypePointer()) || !(nulR || typeR->isTypePointer()))
         Core::Error(pos, "expected pointer");

      // One operand is a pointer and the other is a null pointer constant.
      if(nulL)
      {
         // This needs to fall under arithmetic comparison.
         if(nulR)
            Core::Error(pos, "unexpected two null pointer constants");

         expL = ExpConvert_PtrArith(typeR, expL, pos);
         return std::make_tuple(typeR, expL, expR);
      }
      else if(nulR)
      {
         expR = ExpConvert_PtrArith(typeL, expR, pos);
         return std::make_tuple(typeL, expL, expR);
      }

      auto baseL = typeL->getBaseType();
      auto baseR = typeR->getBaseType();

      // Both operands are pointers to qualified or unqualified versions of
      // compatible types.
      if(baseL->getTypeQual() == baseR->getTypeQual())
      {
         auto qual = QualPromo(baseL->getQual(), baseR->getQual(), pos);
         auto type = baseL->getTypeQual(qual)
            ->getTypeArrayQualAddr(qual.space)->getTypePointer();

         expL = ExpConvert_Pointer(type, expL, pos);
         expR = ExpConvert_Pointer(type, expR, pos);

         return std::make_tuple(type, expL, expR);
      }

      // One operand is a pointer to an object type and the other is a
      // pointer to qualified or unqualified void type.
      if(baseL->isTypeVoid() || baseR->isTypeVoid())
      {
         if(!baseL->isCTypeObject() || !baseR->isCTypeObject())
            Core::Error(pos, "expected pointer to object");

         auto qual = QualPromo(baseL->getQual(), baseR->getQual(), pos);
         auto type = SR::Type::Void->getTypeQual(qual)
            ->getTypeArrayQualAddr(qual.space)->getTypePointer();

         expL = ExpConvert_Pointer(type, expL, pos);
         expR = ExpConvert_Pointer(type, expR, pos);

         return std::make_tuple(type, expL, expR);
      }

      Core::Error(pos, "incompatible pointers");
   }

   //
   // ExpPromo_PtrRel
   //
   std::tuple<SR::Type::CRef, SR::Exp::CRef, SR::Exp::CRef>
   ExpPromo_PtrRel(SR::Exp const *l, SR::Exp const *r, Core::Origin pos)
   {
      SR::Exp::CRef expL{l}, expR{r};

      auto typeL = expL->getType();
      auto typeR = expR->getType();

      // Both operands must be pointers.
      if(!typeL->isTypePointer() || !typeR->isTypePointer())
         Core::Error(pos, "expected pointer");

      auto baseL = typeL->getBaseType();
      auto baseR = typeR->getBaseType();

      // Relational pointers must have compatible types.
      if(baseL->getTypeQual() != baseR->getTypeQual())
         Core::Error(pos, "incompatible pointers");

      // Relational pointers must refer to objects.
      if(!baseL->isCTypeObject() || !baseR->isCTypeObject())
         Core::Error(pos, "expected pointer to object");

      // Convert to common pointer type.
      auto qual = QualPromo(baseL->getQual(), baseR->getQual(), pos);
      auto type = baseL->getTypeQual(qual)
         ->getTypeArrayQualAddr(qual.space)->getTypePointer();

      expL = ExpConvert_Pointer(type, expL, pos);
      expR = ExpConvert_Pointer(type, expR, pos);

      return std::make_tuple(type, expL, expR);
   }
}

// EOF

