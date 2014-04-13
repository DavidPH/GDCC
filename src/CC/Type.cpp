//-----------------------------------------------------------------------------
//
// Copyright (C) 2013 David Hill
//
// See COPYING for license information.
//
//-----------------------------------------------------------------------------
//
// C types.
//
//-----------------------------------------------------------------------------

#include "CC/Type.hpp"

#include "AST/Type.hpp"


//----------------------------------------------------------------------------|
// Global Variables                                                           |
//

namespace GDCC
{
   namespace CC
   {
      AST::Type::CRef TypeBool    {GetTypeBool()};
      AST::Type::CRef TypeBoolSoft{GetTypeBoolSoft()};

      AST::Type::CRef TypeChar{GetTypeChar()};

      AST::Type::CRef TypeFixedPrS {GetTypeFixedPrS ()};
      AST::Type::CRef TypeFixedPrSH{GetTypeFixedPrSH()};
      AST::Type::CRef TypeFixedPrSL{GetTypeFixedPrSL()};
      AST::Type::CRef TypeFixedPrU {GetTypeFixedPrU ()};
      AST::Type::CRef TypeFixedPrUH{GetTypeFixedPrUH()};
      AST::Type::CRef TypeFixedPrUL{GetTypeFixedPrUL()};

      AST::Type::CRef TypeFixedSaS {GetTypeFixedSaS ()};
      AST::Type::CRef TypeFixedSaSH{GetTypeFixedSaSH()};
      AST::Type::CRef TypeFixedSaSL{GetTypeFixedSaSL()};
      AST::Type::CRef TypeFixedSaU {GetTypeFixedSaU ()};
      AST::Type::CRef TypeFixedSaUH{GetTypeFixedSaUH()};
      AST::Type::CRef TypeFixedSaUL{GetTypeFixedSaUL()};

      AST::Type::CRef TypeFloatCS  {GetTypeFloatCS  ()};
      AST::Type::CRef TypeFloatCSL {GetTypeFloatCSL ()};
      AST::Type::CRef TypeFloatCSLL{GetTypeFloatCSLL()};

      AST::Type::CRef TypeFloatIS  {GetTypeFloatIS  ()};
      AST::Type::CRef TypeFloatISL {GetTypeFloatISL ()};
      AST::Type::CRef TypeFloatISLL{GetTypeFloatISLL()};

      AST::Type::CRef TypeFloatRS  {GetTypeFloatRS  ()};
      AST::Type::CRef TypeFloatRSL {GetTypeFloatRSL ()};
      AST::Type::CRef TypeFloatRSLL{GetTypeFloatRSLL()};

      AST::Type::CRef TypeFractPrS {GetTypeFractPrS ()};
      AST::Type::CRef TypeFractPrSH{GetTypeFractPrSH()};
      AST::Type::CRef TypeFractPrSL{GetTypeFractPrSL()};
      AST::Type::CRef TypeFractPrU {GetTypeFractPrU ()};
      AST::Type::CRef TypeFractPrUH{GetTypeFractPrUH()};
      AST::Type::CRef TypeFractPrUL{GetTypeFractPrUL()};

      AST::Type::CRef TypeFractSaS {GetTypeFractSaS ()};
      AST::Type::CRef TypeFractSaSH{GetTypeFractSaSH()};
      AST::Type::CRef TypeFractSaSL{GetTypeFractSaSL()};
      AST::Type::CRef TypeFractSaU {GetTypeFractSaU ()};
      AST::Type::CRef TypeFractSaUH{GetTypeFractSaUH()};
      AST::Type::CRef TypeFractSaUL{GetTypeFractSaUL()};

      AST::Type::CRef TypeIntegPrS  {GetTypeIntegPrS  ()};
      AST::Type::CRef TypeIntegPrSH {GetTypeIntegPrSH ()};
      AST::Type::CRef TypeIntegPrSHH{GetTypeIntegPrSHH()};
      AST::Type::CRef TypeIntegPrSL {GetTypeIntegPrSL ()};
      AST::Type::CRef TypeIntegPrSLL{GetTypeIntegPrSLL()};
      AST::Type::CRef TypeIntegPrU  {GetTypeIntegPrU  ()};
      AST::Type::CRef TypeIntegPrUH {GetTypeIntegPrUH ()};
      AST::Type::CRef TypeIntegPrUHH{GetTypeIntegPrUHH()};
      AST::Type::CRef TypeIntegPrUL {GetTypeIntegPrUL ()};
      AST::Type::CRef TypeIntegPrULL{GetTypeIntegPrULL()};
   }
}

// EOF

