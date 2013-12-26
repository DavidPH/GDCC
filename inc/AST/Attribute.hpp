//-----------------------------------------------------------------------------
//
// Copyright (C) 2013 David Hill
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

#include "../Core/Counter.hpp"
#include "../Core/String.hpp"


//----------------------------------------------------------------------------|
// Types                                                                      |
//

namespace GDCC
{
   namespace IR
   {
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

         IR::Linkage                  linka;
         Core::String                 name;
         Core::CounterPtr<Type const> type;
      };
   }
}

#endif//GDCC__AST__Attribute_H__

