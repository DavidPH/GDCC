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
#include "../Core/Origin.hpp"

#include "../IR/Addr.hpp"


//----------------------------------------------------------------------------|
// Types                                                                      |
//

namespace GDCC
{
   namespace Core
   {
      class Token;
   }

   namespace IR
   {
      enum class CallType;
      enum class Linkage;

      class Exp;
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
         Attribute(Attribute const &attr);
         Attribute(Attribute &&attr);
         ~Attribute();

         void setName(Core::Token const &tok);

         Core::CounterPtr<IR::Exp const> addrI;
         Core::String                    addrS;
         IR::CallType                    callt;
         IR::Linkage                     linka;
         Core::String                    name;
         Core::Origin                    namePos;
         Core::Array<Attribute>          param;
         IR::AddrSpace                   space;
         Core::CounterPtr<Type const>    type;

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

