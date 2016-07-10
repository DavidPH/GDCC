//-----------------------------------------------------------------------------
//
// Copyright (C) 2013-2016 David Hill
//
// See COPYING for license information.
//
//-----------------------------------------------------------------------------
//
// C types.
//
//-----------------------------------------------------------------------------

#include "CC/Type.hpp"

#include "SR/Type.hpp"


//----------------------------------------------------------------------------|
// Extern Objects                                                             |
//

namespace GDCC
{
   namespace CC
   {
      SR::Type::CRef TypeBool    {GetTypeBool()};
      SR::Type::CRef TypeBoolSoft{GetTypeBoolSoft()};

      SR::Type::CRef TypeChar{GetTypeChar()};

      SR::Type::CRef TypeFixedPrS {GetTypeFixedPrS ()};
      SR::Type::CRef TypeFixedPrSH{GetTypeFixedPrSH()};
      SR::Type::CRef TypeFixedPrSL{GetTypeFixedPrSL()};
      SR::Type::CRef TypeFixedPrU {GetTypeFixedPrU ()};
      SR::Type::CRef TypeFixedPrUH{GetTypeFixedPrUH()};
      SR::Type::CRef TypeFixedPrUL{GetTypeFixedPrUL()};

      SR::Type::CRef TypeFixedSaS {GetTypeFixedSaS ()};
      SR::Type::CRef TypeFixedSaSH{GetTypeFixedSaSH()};
      SR::Type::CRef TypeFixedSaSL{GetTypeFixedSaSL()};
      SR::Type::CRef TypeFixedSaU {GetTypeFixedSaU ()};
      SR::Type::CRef TypeFixedSaUH{GetTypeFixedSaUH()};
      SR::Type::CRef TypeFixedSaUL{GetTypeFixedSaUL()};

      SR::Type::CRef TypeFloatCS  {GetTypeFloatCS  ()};
      SR::Type::CRef TypeFloatCSL {GetTypeFloatCSL ()};
      SR::Type::CRef TypeFloatCSLL{GetTypeFloatCSLL()};

      SR::Type::CRef TypeFloatIS  {GetTypeFloatIS  ()};
      SR::Type::CRef TypeFloatISL {GetTypeFloatISL ()};
      SR::Type::CRef TypeFloatISLL{GetTypeFloatISLL()};

      SR::Type::CRef TypeFloatRS  {GetTypeFloatRS  ()};
      SR::Type::CRef TypeFloatRSL {GetTypeFloatRSL ()};
      SR::Type::CRef TypeFloatRSLL{GetTypeFloatRSLL()};

      SR::Type::CRef TypeFractPrS {GetTypeFractPrS ()};
      SR::Type::CRef TypeFractPrSH{GetTypeFractPrSH()};
      SR::Type::CRef TypeFractPrSL{GetTypeFractPrSL()};
      SR::Type::CRef TypeFractPrU {GetTypeFractPrU ()};
      SR::Type::CRef TypeFractPrUH{GetTypeFractPrUH()};
      SR::Type::CRef TypeFractPrUL{GetTypeFractPrUL()};

      SR::Type::CRef TypeFractSaS {GetTypeFractSaS ()};
      SR::Type::CRef TypeFractSaSH{GetTypeFractSaSH()};
      SR::Type::CRef TypeFractSaSL{GetTypeFractSaSL()};
      SR::Type::CRef TypeFractSaU {GetTypeFractSaU ()};
      SR::Type::CRef TypeFractSaUH{GetTypeFractSaUH()};
      SR::Type::CRef TypeFractSaUL{GetTypeFractSaUL()};

      SR::Type::CRef TypeIntegPrS  {GetTypeIntegPrS  ()};
      SR::Type::CRef TypeIntegPrSH {GetTypeIntegPrSH ()};
      SR::Type::CRef TypeIntegPrSHH{GetTypeIntegPrSHH()};
      SR::Type::CRef TypeIntegPrSL {GetTypeIntegPrSL ()};
      SR::Type::CRef TypeIntegPrSLL{GetTypeIntegPrSLL()};
      SR::Type::CRef TypeIntegPrU  {GetTypeIntegPrU  ()};
      SR::Type::CRef TypeIntegPrUH {GetTypeIntegPrUH ()};
      SR::Type::CRef TypeIntegPrUHH{GetTypeIntegPrUHH()};
      SR::Type::CRef TypeIntegPrUL {GetTypeIntegPrUL ()};
      SR::Type::CRef TypeIntegPrULL{GetTypeIntegPrULL()};
   }
}

// EOF

