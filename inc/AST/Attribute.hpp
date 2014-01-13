//-----------------------------------------------------------------------------
//
// Copyright (C) 2013-2014 David Hill
//
// See COPYING for license information.
//
//-----------------------------------------------------------------------------
//
// Abstract Syntax Tree generalized attribute handling.
//
//-----------------------------------------------------------------------------

#ifndef GDCC__AST__Attribute_H__
#define GDCC__AST__Attribute_H__

#include "../Core/Array.hpp"
#include "../Core/Counter.hpp"
#include "../Core/String.hpp"


//----------------------------------------------------------------------------|
// Types                                                                      |
//

namespace GDCC
{
   namespace IR
   {
      enum class CallType;
      enum class Linkage;
   }

   namespace AST
   {
      class Type;

      //
      // Attribute
      //
      class Attribute
      {
      public:
         Attribute();
         ~Attribute();

         IR::CallType                 callt;
         IR::Linkage                  linka;
         Core::String                 name;
         Core::Array<Attribute>       param;
         Core::CounterPtr<Type const> type;

         bool funcInline   : 1;
         bool funcNoParam  : 1;
         bool funcNoReturn : 1;

         bool isTypedef : 1;

         bool storeAuto   : 1;
         bool storeExt    : 1;
         bool storeInt    : 1;
         bool storeReg    : 1;
         bool storeThread : 1;
      };
   }
}

#endif//GDCC__AST__Attribute_H__

