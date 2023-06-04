//-----------------------------------------------------------------------------
//
// Copyright (C) 2014-2023 David Hill
//
// See COPYING for license information.
//
//-----------------------------------------------------------------------------
//
// C arithmetic type promotion rules.
//
//-----------------------------------------------------------------------------

#include "CC/Exp.hpp"

#include "CC/Factory.hpp"
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
   // ExpPromo_Arith_Fixed
   //
   static std::tuple<SR::Type::CRef, SR::Exp::CRef, SR::Exp::CRef>
   ExpPromo_Arith_Fixed(Factory &fact, SR::Exp const *l, SR::Exp const *r,
      Core::Origin pos)
   {
      SR::Exp::CRef expL{l}, expR{r};
      auto typeL = expL->getType();
      auto typeR = expR->getType();

      // Promote to signed.
      auto signL = typeL->getSizeBitsS();
      auto signR = typeR->getSizeBitsS();
      if(signL && !signR)
      {
         expR  = fact.expConvert_Arith(typeR->getSignType(), expR, pos);
         typeR = expR->getType();
      }
      else if(!signL && signR)
      {
         expL  = fact.expConvert_Arith(typeL->getSignType(), expL, pos);
         typeL = expL->getType();
      }

      // Promote to sat.
      bool satuL = typeL->isTypeSaturate();
      bool satuR = typeR->isTypeSaturate();
      if(satuL && !satuR)
      {
         expR  = fact.expConvert_Arith(typeR->getSatuType(), expR, pos);
         typeR = expR->getType();
      }
      else if(!satuL && satuR)
      {
         expL  = fact.expConvert_Arith(typeL->getSatuType(), expL, pos);
         typeL = expL->getType();
      }

      // Convert to higher rank.
      auto rankL = typeL->getRankC();
      auto rankR = typeR->getRankC();
      if(rankL < rankR)
         return std::make_tuple(typeR, fact.expConvert_Arith(typeR, expL, pos), expR);
      if(rankL > rankR)
         return std::make_tuple(typeL, expL, fact.expConvert_Arith(typeL, expR, pos));

      // If same rank, they should be compatible types.
      return std::make_tuple(typeL, expL, expR);
   }

   //
   // ExpPromo_Arith_Float
   //
   static std::tuple<SR::Type::CRef, SR::Exp::CRef, SR::Exp::CRef>
   ExpPromo_Arith_Float(Factory &fact, SR::Exp const *l, SR::Exp const *r,
      Core::Origin pos)
   {
      SR::Exp::CRef expL{l}, expR{r};

      auto typeL = expL->getType()->getTypeQual();
      auto typeR = expR->getType()->getTypeQual();

      if(typeL == typeR) return std::make_tuple(typeL, expL, expR);

      auto cplxL = typeL->isTypeComplex();
      auto cplxR = typeR->isTypeComplex();

      auto imagL = typeL->isTypeImagin();
      auto imagR = typeR->isTypeImagin();

      // Determine domain of inputs and result.
      // 0 real, 1 complex, 2 imaginary
      int domL   = cplxL ? 1 : (imagL ? 2 : 0);
      int domR   = cplxR ? 1 : (imagR ? 2 : 0);
      int domRes = (imagL || imagR || cplxL || cplxR) + (imagL && imagR);

      // Common promotion code.
      auto promo = [&](std::array<SR::Type::CRef, 3> &&types)
      {
         if(typeL != types[domL]) expL = fact.expConvert_Arith(types[domL], expL, pos);
         if(typeR != types[domR]) expR = fact.expConvert_Arith(types[domR], expR, pos);

         return std::make_tuple(types[domRes], expL, expR);
      };

      // long double
      if(typeL == TypeFloatCSLL || typeL == TypeFloatISLL || typeL == TypeFloatRSLL ||
         typeR == TypeFloatCSLL || typeR == TypeFloatISLL || typeR == TypeFloatRSLL)
      {
         return promo({TypeFloatRSLL, TypeFloatCSLL, TypeFloatISLL});
      }

      // double
      if(typeL == TypeFloatCSL || typeL == TypeFloatISL || typeL == TypeFloatRSL ||
         typeR == TypeFloatCSL || typeR == TypeFloatISL || typeR == TypeFloatRSL)
      {
         return promo({TypeFloatRSL, TypeFloatCSL, TypeFloatISL});
      }

      // float
      if(typeL == TypeFloatCS || typeL == TypeFloatIS || typeL == TypeFloatRS ||
         typeR == TypeFloatCS || typeR == TypeFloatIS || typeR == TypeFloatRS)
      {
         return promo({TypeFloatRS, TypeFloatCS, TypeFloatIS});
      }

      Core::Error(pos, "cannot determine common floating type");
   }

   //
   // ExpPromo_Arith_Integ
   //
   static std::tuple<SR::Type::CRef, SR::Exp::CRef, SR::Exp::CRef>
   ExpPromo_Arith_Integ(Factory &fact, SR::Exp const *l, SR::Exp const *r,
      Core::Origin pos)
   {
      auto expL = fact.expPromo_Int(l, pos);
      auto expR = fact.expPromo_Int(r, pos);

      auto typeL = expL->getType()->getTypeQual();
      auto typeR = expR->getType()->getTypeQual();

      if(typeL == typeR) return std::make_tuple(typeL, expL, expR);

      SR::TypeRankC rankL, rankR;

      try
      {
         rankL = typeL->getRankC();
         rankR = typeR->getRankC();
      }
      catch(SR::TypeError const &)
      {
         Core::Error(pos, "expected ranked types");
      }

      // If one has a higher rank, convert to that.
      if(rankL < rankR)
         return std::make_tuple(typeR, fact.expConvert_Arith(typeR, expL, pos), expR);
      if(rankL > rankR)
         return std::make_tuple(typeL, expL, fact.expConvert_Arith(typeL, expR, pos));

      // Try to promote to unsigned.
      if(typeL->isCTypeIntegU())
         return std::make_tuple(typeL, expL, fact.expConvert_Arith(typeL, expR, pos));
      if(typeR->isCTypeIntegU())
         return std::make_tuple(typeR, fact.expConvert_Arith(typeR, expL, pos), expR);

      // Same rank and both signed, should mean they're compatible.
      return std::make_tuple(typeL, expL, expR);
   }
}


//----------------------------------------------------------------------------|
// Extern Functions                                                           |
//

namespace GDCC::CC
{
   //
   // Factory::expPromo_Arith
   //
   std::tuple<SR::Type::CRef, SR::Exp::CRef, SR::Exp::CRef>
   Factory::expPromo_Arith(SR::Exp const *l, SR::Exp const *r, Core::Origin pos)
   {
      SR::Exp::CRef expL{l}, expR{r};

      auto typeL = expL->getType();
      auto typeR = expR->getType();

      // Floating types.
      if(typeL->isCTypeFloat() || typeR->isCTypeFloat())
         return ExpPromo_Arith_Float(*this, expL, expR, pos);

      // Fixed-point types.
      if(typeL->isCTypeFixed())
      {
         if(typeR->isCTypeFixed())
            return ExpPromo_Arith_Fixed(*this, expL, expR, pos);

         else if(typeR->isCTypeInteg())
            return std::make_tuple(typeL, expL, expPromo_Int(expR, pos));
      }
      else if(typeR->isCTypeFixed() && typeL->isCTypeInteg())
         return std::make_tuple(typeR, expPromo_Int(expL, pos), expR);

      // Integer types.
      if(typeL->isCTypeInteg() && typeR->isCTypeInteg())
         return ExpPromo_Arith_Integ(*this, expL, expR, pos);

      Core::Error(pos, "cannot determine common real type");
   }

   //
   // Factory::expPromo_ArithAlways
   //
   std::tuple<SR::Type::CRef, SR::Exp::CRef, SR::Exp::CRef>
   Factory::expPromo_ArithAlways(SR::Exp const *l, SR::Exp const *r, Core::Origin pos)
   {
      auto [type, expL, expR] = expPromo_Arith(l, r, pos);

      if(expL->getType()->getTypeQual() != type)
         expL = expConvert_Arith(type, expL, pos);

      if(expR->getType()->getTypeQual() != type)
         expR = expConvert_Arith(type, expR, pos);

      return std::make_tuple(type, expL, expR);
   }
}

// EOF

