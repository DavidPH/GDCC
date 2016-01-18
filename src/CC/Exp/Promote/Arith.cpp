//-----------------------------------------------------------------------------
//
// Copyright (C) 2014-2016 David Hill
//
// See COPYING for license information.
//
//-----------------------------------------------------------------------------
//
// C arithmetic type promotion rules.
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
// ExpPromo_Arith_Fixed
//
static std::tuple<GDCC::AST::Type::CRef, GDCC::AST::Exp::CRef, GDCC::AST::Exp::CRef>
ExpPromo_Arith_Fixed(GDCC::AST::Exp const *l, GDCC::AST::Exp const *r,
   GDCC::Core::Origin pos)
{
   using namespace GDCC;

   AST::Exp::CRef expL{l}, expR{r};
   auto typeL = expL->getType();
   auto typeR = expR->getType();

   // Promote to signed.
   auto signL = typeL->getSizeBitsS();
   auto signR = typeR->getSizeBitsS();
   if(signL && !signR)
   {
      expR  = CC::ExpConvert_Arith(typeR->getSignType(), expR, pos);
      typeR = expR->getType();
   }
   else if(!signL && signR)
   {
      expL  = CC::ExpConvert_Arith(typeL->getSignType(), expL, pos);
      typeL = expL->getType();
   }

   // Promote to sat.
   bool satuL = typeL->isTypeSaturate();
   bool satuR = typeR->isTypeSaturate();
   if(satuL && !satuR)
   {
      expR  = CC::ExpConvert_Arith(typeR->getSatuType(), expR, pos);
      typeR = expR->getType();
   }
   else if(!satuL && satuR)
   {
      expL  = CC::ExpConvert_Arith(typeL->getSatuType(), expL, pos);
      typeL = expL->getType();
   }

   // Convert to higher rank.
   auto rankL = typeL->getRankC();
   auto rankR = typeR->getRankC();
   if(rankL < rankR)
      return std::make_tuple(typeR, CC::ExpConvert_Arith(typeR, expL, pos), expR);
   if(rankL > rankR)
      return std::make_tuple(typeL, expL, CC::ExpConvert_Arith(typeL, expR, pos));

   // If same rank, they should be compatible types.
   return std::make_tuple(typeL, expL, expR);
}

//
// ExpPromo_Arith_Float
//
static std::tuple<GDCC::AST::Type::CRef, GDCC::AST::Exp::CRef, GDCC::AST::Exp::CRef>
ExpPromo_Arith_Float(GDCC::AST::Exp const *l, GDCC::AST::Exp const *r,
   GDCC::Core::Origin pos)
{
   using namespace GDCC;
   using namespace CC;

   AST::Exp::CRef expL{l}, expR{r};

   auto typeL = expL->getType()->getTypeQual();
   auto typeR = expR->getType()->getTypeQual();

   auto cplxL = typeL->isTypeComplex();
   auto cplxR = typeR->isTypeComplex();

   auto imagL = typeL->isTypeImagin();
   auto imagR = typeR->isTypeImagin();

   // 0 real, 1 complex, 2 imaginary
   int resDomain = (imagL || imagR || cplxL || cplxR) + (imagL && imagR);

   // long double
   if(typeL == TypeFloatCSLL || typeL == TypeFloatISLL || typeL == TypeFloatRSLL ||
      typeR == TypeFloatCSLL || typeR == TypeFloatISLL || typeR == TypeFloatRSLL)
   {
           if(cplxL) expL = CC::ExpConvert_Arith(CC::TypeFloatCSLL, expL, pos);
      else if(imagL) expL = CC::ExpConvert_Arith(CC::TypeFloatISLL, expL, pos);
      else           expL = CC::ExpConvert_Arith(CC::TypeFloatRSLL, expL, pos);

           if(cplxR) expR = CC::ExpConvert_Arith(CC::TypeFloatCSLL, expR, pos);
      else if(imagR) expR = CC::ExpConvert_Arith(CC::TypeFloatISLL, expR, pos);
      else           expR = CC::ExpConvert_Arith(CC::TypeFloatRSLL, expR, pos);

      if(resDomain == 1) return std::make_tuple(CC::TypeFloatCSLL, expL, expR);
      if(resDomain == 2) return std::make_tuple(CC::TypeFloatISLL, expL, expR);
                         return std::make_tuple(CC::TypeFloatRSLL, expL, expR);
   }

   // double
   if(typeL == TypeFloatCSL || typeL == TypeFloatISL || typeL == TypeFloatRSL ||
      typeR == TypeFloatCSL || typeR == TypeFloatISL || typeR == TypeFloatRSL)
   {
           if(cplxL) expL = CC::ExpConvert_Arith(CC::TypeFloatCSL, expL, pos);
      else if(imagL) expL = CC::ExpConvert_Arith(CC::TypeFloatISL, expL, pos);
      else           expL = CC::ExpConvert_Arith(CC::TypeFloatRSL, expL, pos);

           if(cplxR) expR = CC::ExpConvert_Arith(CC::TypeFloatCSL, expR, pos);
      else if(imagR) expR = CC::ExpConvert_Arith(CC::TypeFloatISL, expR, pos);
      else           expR = CC::ExpConvert_Arith(CC::TypeFloatRSL, expR, pos);

      if(resDomain == 1) return std::make_tuple(CC::TypeFloatCSL, expL, expR);
      if(resDomain == 2) return std::make_tuple(CC::TypeFloatISL, expL, expR);
                         return std::make_tuple(CC::TypeFloatRSL, expL, expR);
   }

   // float
   if(typeL == TypeFloatCS || typeL == TypeFloatIS || typeL == TypeFloatRS ||
      typeR == TypeFloatCS || typeR == TypeFloatIS || typeR == TypeFloatRS)
   {
           if(cplxL) expL = CC::ExpConvert_Arith(CC::TypeFloatCS, expL, pos);
      else if(imagL) expL = CC::ExpConvert_Arith(CC::TypeFloatIS, expL, pos);
      else           expL = CC::ExpConvert_Arith(CC::TypeFloatRS, expL, pos);

           if(cplxR) expR = CC::ExpConvert_Arith(CC::TypeFloatCS, expR, pos);
      else if(imagR) expR = CC::ExpConvert_Arith(CC::TypeFloatIS, expR, pos);
      else           expR = CC::ExpConvert_Arith(CC::TypeFloatRS, expR, pos);

      if(resDomain == 1) return std::make_tuple(CC::TypeFloatCS, expL, expR);
      if(resDomain == 2) return std::make_tuple(CC::TypeFloatIS, expL, expR);
                         return std::make_tuple(CC::TypeFloatRS, expL, expR);
   }

   throw Core::ExceptStr(pos, "cannot determine common floating type");
}

//
// ExpPromo_Arith_Integ
//
static std::tuple<GDCC::AST::Type::CRef, GDCC::AST::Exp::CRef, GDCC::AST::Exp::CRef>
ExpPromo_Arith_Integ(GDCC::AST::Exp const *l, GDCC::AST::Exp const *r,
   GDCC::Core::Origin pos)
{
   using namespace GDCC;

   auto expL = CC::ExpPromo_Int(l, pos);
   auto expR = CC::ExpPromo_Int(r, pos);

   auto typeL = expL->getType()->getTypeQual();
   auto typeR = expR->getType()->getTypeQual();

   if(typeL == typeR) return std::make_tuple(typeL, expL, expR);

   AST::TypeRankC rankL, rankR;

   try
   {
      rankL = typeL->getRankC();
      rankR = typeR->getRankC();
   }
   catch(AST::TypeError const &)
   {
      throw Core::ExceptStr(pos, "expected ranked types");
   }

   // If one has a higher rank, convert to that.
   if(rankL < rankR)
      return std::make_tuple(typeR, CC::ExpConvert_Arith(typeR, expL, pos), expR);
   if(rankL > rankR)
      return std::make_tuple(typeL, expL, CC::ExpConvert_Arith(typeL, expR, pos));

   // Try to promote to unsigned.
   if(typeL->isCTypeIntegU())
      return std::make_tuple(typeL, expL, CC::ExpConvert_Arith(typeL, expR, pos));
   if(typeR->isCTypeIntegU())
      return std::make_tuple(typeR, CC::ExpConvert_Arith(typeR, expL, pos), expR);

   // Same rank and both signed, should mean they're compatible.
   return std::make_tuple(typeL, expL, expR);
}


//----------------------------------------------------------------------------|
// Global Functions                                                           |
//

namespace GDCC
{
   namespace CC
   {
      //
      // ExpPromo_Arith
      //
      std::tuple<AST::Type::CRef, AST::Exp::CRef, AST::Exp::CRef>
      ExpPromo_Arith(AST::Exp const *l, AST::Exp const *r, Core::Origin pos)
      {
         AST::Exp::CRef expL{l}, expR{r};

         auto typeL = expL->getType();
         auto typeR = expR->getType();

         // Floating types.
         if(typeL->isCTypeFloat() || typeR->isCTypeFloat())
            return ExpPromo_Arith_Float(expL, expR, pos);

         // Fixed-point types.
         if(typeL->isCTypeFixed())
         {
            if(typeR->isCTypeFixed())
               return ExpPromo_Arith_Fixed(expL, expR, pos);

            else if(typeR->isCTypeInteg())
               return std::make_tuple(typeL, expL, ExpPromo_Int(expR, pos));
         }
         else if(typeR->isCTypeFixed() && typeL->isCTypeInteg())
            return std::make_tuple(typeR, ExpPromo_Int(expL, pos), expR);

         // Integer types.
         if(typeL->isCTypeInteg() && typeR->isCTypeInteg())
            return ExpPromo_Arith_Integ(expL, expR, pos);

         throw Core::ExceptStr(pos, "cannot determine common real type");
      }

      //
      // ExpPromo_ArithAlways
      //
      std::tuple<AST::Type::CRef, AST::Exp::CRef, AST::Exp::CRef>
      ExpPromo_ArithAlways(AST::Exp const *l, AST::Exp const *r, Core::Origin pos)
      {
         AST::Exp::CRef  expL{l}, expR{r};
         AST::Type::CRef type{AST::Type::None};

         std::tie(type, expL, expR) = ExpPromo_Arith(expL, expR, pos);

         if(expL->getType()->getTypeQual() != type)
            expL = ExpConvert_Arith(type, expL, pos);

         if(expR->getType()->getTypeQual() != type)
            expR = ExpConvert_Arith(type, expR, pos);

         return std::make_tuple(type, expL, expR);
      }
   }
}

// EOF

