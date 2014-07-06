//-----------------------------------------------------------------------------
//
// Copyright (C) 2014 David Hill
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

#include "AST/Exp.hpp"
#include "AST/Type.hpp"

#include "Core/Exception.hpp"


//----------------------------------------------------------------------------|
// Static Functions                                                           |
//

//
// AddrPromo
//
static GDCC::IR::AddrSpace AddrPromo(GDCC::IR::AddrSpace addrL,
   GDCC::IR::AddrSpace addrR, GDCC::Core::Origin pos)
{
   using namespace GDCC;

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
         throw Core::ExceptStr(pos, "cannot promote disjoint address spaces");
   }
}

//
// QualPromo
//
static GDCC::AST::TypeQual QualPromo(GDCC::AST::TypeQual qualL,
   GDCC::AST::TypeQual qualR, GDCC::Core::Origin pos)
{
   // Merge qualifiers.
   qualL.space  = AddrPromo(qualL.space, qualR.space, pos);
   qualL.aAtom |= qualR.aAtom;
   qualL.aCons |= qualR.aCons;
   qualL.aRest |= qualR.aRest;
   qualL.aVola |= qualR.aVola;

   return qualL;
}


//----------------------------------------------------------------------------|
// Global Functions                                                           |
//

namespace GDCC
{
   namespace CC
   {
      //
      // ExpPromo_PtrEqu
      //
      std::tuple<AST::Type::CRef, AST::Exp::CRef, AST::Exp::CRef>
      ExpPromo_PtrEqu(AST::Exp const *l, AST::Exp const *r, Core::Origin pos)
      {
         AST::Exp::CRef expL{l}, expR{r};

         auto typeL = expL->getType();
         auto typeR = expR->getType();

         // Check if either operand is a null pointer constant.
         bool nulL = typeL->isCTypeInteg() && expL->isZero();
         bool nulR = typeR->isCTypeInteg() && expR->isZero();

         // Both operands must be pointers.
         if(!(nulL || typeL->isTypePointer()) || !(nulR || typeR->isTypePointer()))
            throw Core::ExceptStr(pos, "expected pointer");

         // One operand is a pointer and the other is a null pointer constant.
         if(nulL)
         {
            // This needs to fall under arithmetic comparison.
            if(nulR)
               throw Core::ExceptStr(pos, "unexpected two null pointer constants");

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
               throw Core::ExceptStr(pos, "expected pointer to object");

            auto qual = QualPromo(baseL->getQual(), baseR->getQual(), pos);
            auto type = AST::Type::Void->getTypeQual(qual)
               ->getTypeArrayQualAddr(qual.space)->getTypePointer();

            expL = ExpConvert_Pointer(type, expL, pos);
            expR = ExpConvert_Pointer(type, expR, pos);

            return std::make_tuple(type, expL, expR);
         }

         throw Core::ExceptStr(pos, "incompatible pointers");
      }

      //
      // ExpPromo_PtrRel
      //
      std::tuple<AST::Type::CRef, AST::Exp::CRef, AST::Exp::CRef>
      ExpPromo_PtrRel(AST::Exp const *l, AST::Exp const *r, Core::Origin pos)
      {
         AST::Exp::CRef expL{l}, expR{r};

         auto typeL = expL->getType();
         auto typeR = expR->getType();

         // Both operands must be pointers.
         if(!typeL->isTypePointer() || !typeR->isTypePointer())
            throw Core::ExceptStr(pos, "expected pointer");

         auto baseL = typeL->getBaseType();
         auto baseR = typeR->getBaseType();

         // Relational pointers must have compatible types.
         if(baseL->getTypeQual() != baseR->getTypeQual())
            throw Core::ExceptStr(pos, "incompatible pointers");

         // Relational pointers must refer to objects.
         if(!baseL->isCTypeObject() || !baseR->isCTypeObject())
            throw Core::ExceptStr(pos, "expected pointer to object");

         // Convert to common pointer type.
         auto qual = QualPromo(baseL->getQual(), baseR->getQual(), pos);
         auto type = baseL->getTypeQual(qual)
            ->getTypeArrayQualAddr(qual.space)->getTypePointer();

         expL = ExpConvert_Pointer(type, expL, pos);
         expR = ExpConvert_Pointer(type, expR, pos);

         return std::make_tuple(type, expL, expR);
      }
   }
}

// EOF

