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

#ifndef GDCC__CC__Type_H__
#define GDCC__CC__Type_H__

#include "../Core/Counter.hpp"


//----------------------------------------------------------------------------|
// Global Variables                                                           |
//

namespace GDCC
{
   namespace CC
   {
      extern Core::CounterRef<AST::Type> TypeBool;

      extern Core::CounterRef<AST::Type> TypeChar;

      extern Core::CounterRef<AST::Type> TypeFixedPrS;
      extern Core::CounterRef<AST::Type> TypeFixedPrSH;
      extern Core::CounterRef<AST::Type> TypeFixedPrSL;
      extern Core::CounterRef<AST::Type> TypeFixedPrU;
      extern Core::CounterRef<AST::Type> TypeFixedPrUH;
      extern Core::CounterRef<AST::Type> TypeFixedPrUL;

      extern Core::CounterRef<AST::Type> TypeFixedSaS;
      extern Core::CounterRef<AST::Type> TypeFixedSaSH;
      extern Core::CounterRef<AST::Type> TypeFixedSaSL;
      extern Core::CounterRef<AST::Type> TypeFixedSaU;
      extern Core::CounterRef<AST::Type> TypeFixedSaUH;
      extern Core::CounterRef<AST::Type> TypeFixedSaUL;

      extern Core::CounterRef<AST::Type> TypeFloatCS;
      extern Core::CounterRef<AST::Type> TypeFloatCSL;
      extern Core::CounterRef<AST::Type> TypeFloatCSLL;

      extern Core::CounterRef<AST::Type> TypeFloatIS;
      extern Core::CounterRef<AST::Type> TypeFloatISL;
      extern Core::CounterRef<AST::Type> TypeFloatISLL;

      extern Core::CounterRef<AST::Type> TypeFloatRS;
      extern Core::CounterRef<AST::Type> TypeFloatRSL;
      extern Core::CounterRef<AST::Type> TypeFloatRSLL;

      extern Core::CounterRef<AST::Type> TypeFractPrS;
      extern Core::CounterRef<AST::Type> TypeFractPrSH;
      extern Core::CounterRef<AST::Type> TypeFractPrSL;
      extern Core::CounterRef<AST::Type> TypeFractPrU;
      extern Core::CounterRef<AST::Type> TypeFractPrUH;
      extern Core::CounterRef<AST::Type> TypeFractPrUL;

      extern Core::CounterRef<AST::Type> TypeFractSaS;
      extern Core::CounterRef<AST::Type> TypeFractSaSH;
      extern Core::CounterRef<AST::Type> TypeFractSaSL;
      extern Core::CounterRef<AST::Type> TypeFractSaU;
      extern Core::CounterRef<AST::Type> TypeFractSaUH;
      extern Core::CounterRef<AST::Type> TypeFractSaUL;

      extern Core::CounterRef<AST::Type> TypeIntegPrS;
      extern Core::CounterRef<AST::Type> TypeIntegPrSH;
      extern Core::CounterRef<AST::Type> TypeIntegPrSHH;
      extern Core::CounterRef<AST::Type> TypeIntegPrSL;
      extern Core::CounterRef<AST::Type> TypeIntegPrSLL;
      extern Core::CounterRef<AST::Type> TypeIntegPrU;
      extern Core::CounterRef<AST::Type> TypeIntegPrUH;
      extern Core::CounterRef<AST::Type> TypeIntegPrUHH;
      extern Core::CounterRef<AST::Type> TypeIntegPrUL;
      extern Core::CounterRef<AST::Type> TypeIntegPrULL;

      extern Core::CounterRef<AST::Type> TypeStr;

      extern Core::CounterRef<AST::Type> TypeVoid;
   }
}


//----------------------------------------------------------------------------|
// Global Functions                                                           |
//

namespace GDCC
{
   namespace CC
   {
      Core::CounterRef<AST::Type> GetTypeBool();

      Core::CounterRef<AST::Type> GetTypeChar();

      Core::CounterRef<AST::Type> GetTypeFixedPrS();
      Core::CounterRef<AST::Type> GetTypeFixedPrSH();
      Core::CounterRef<AST::Type> GetTypeFixedPrSL();
      Core::CounterRef<AST::Type> GetTypeFixedPrU();
      Core::CounterRef<AST::Type> GetTypeFixedPrUH();
      Core::CounterRef<AST::Type> GetTypeFixedPrUL();

      Core::CounterRef<AST::Type> GetTypeFixedSaS();
      Core::CounterRef<AST::Type> GetTypeFixedSaSH();
      Core::CounterRef<AST::Type> GetTypeFixedSaSL();
      Core::CounterRef<AST::Type> GetTypeFixedSaU();
      Core::CounterRef<AST::Type> GetTypeFixedSaUH();
      Core::CounterRef<AST::Type> GetTypeFixedSaUL();

      Core::CounterRef<AST::Type> GetTypeFloatCS();
      Core::CounterRef<AST::Type> GetTypeFloatCSL();
      Core::CounterRef<AST::Type> GetTypeFloatCSLL();

      Core::CounterRef<AST::Type> GetTypeFloatIS();
      Core::CounterRef<AST::Type> GetTypeFloatISL();
      Core::CounterRef<AST::Type> GetTypeFloatISLL();

      Core::CounterRef<AST::Type> GetTypeFloatRS();
      Core::CounterRef<AST::Type> GetTypeFloatRSL();
      Core::CounterRef<AST::Type> GetTypeFloatRSLL();

      Core::CounterRef<AST::Type> GetTypeFractPrS();
      Core::CounterRef<AST::Type> GetTypeFractPrSH();
      Core::CounterRef<AST::Type> GetTypeFractPrSL();
      Core::CounterRef<AST::Type> GetTypeFractPrU();
      Core::CounterRef<AST::Type> GetTypeFractPrUH();
      Core::CounterRef<AST::Type> GetTypeFractPrUL();

      Core::CounterRef<AST::Type> GetTypeFractSaS();
      Core::CounterRef<AST::Type> GetTypeFractSaSH();
      Core::CounterRef<AST::Type> GetTypeFractSaSL();
      Core::CounterRef<AST::Type> GetTypeFractSaU();
      Core::CounterRef<AST::Type> GetTypeFractSaUH();
      Core::CounterRef<AST::Type> GetTypeFractSaUL();

      Core::CounterRef<AST::Type> GetTypeIntegPrS();
      Core::CounterRef<AST::Type> GetTypeIntegPrSH();
      Core::CounterRef<AST::Type> GetTypeIntegPrSHH();
      Core::CounterRef<AST::Type> GetTypeIntegPrSL();
      Core::CounterRef<AST::Type> GetTypeIntegPrSLL();
      Core::CounterRef<AST::Type> GetTypeIntegPrU();
      Core::CounterRef<AST::Type> GetTypeIntegPrUH();
      Core::CounterRef<AST::Type> GetTypeIntegPrUHH();
      Core::CounterRef<AST::Type> GetTypeIntegPrUL();
      Core::CounterRef<AST::Type> GetTypeIntegPrULL();

      Core::CounterRef<AST::Type> GetTypeStr();

      Core::CounterRef<AST::Type> GetTypeVoid();
   }
}

#endif//GDCC__CC__Type_H__

