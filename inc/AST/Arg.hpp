//-----------------------------------------------------------------------------
//
// Copyright (C) 2013-2014 David Hill
//
// See COPYING for license information.
//
//-----------------------------------------------------------------------------
//
// Abstract Syntax Tree storage arguments.
//
// This is the AST equivalent to IR::Arg, used to designate run-time data
// sources and sinks.
//
//-----------------------------------------------------------------------------

#ifndef GDCC__AST__Arg_H__
#define GDCC__AST__Arg_H__

#include "../Core/Counter.hpp"


//----------------------------------------------------------------------------|
// Types                                                                      |
//

namespace GDCC
{
   namespace IR
   {
      enum class AddrBase;
   }

   namespace AST
   {
      class Exp;
      class Type;

      //
      // Arg
      //
      class Arg
      {
      protected:
         using ExpCPtr  = Core::CounterPtr<Exp const>;
         using TypeCRef = Core::CounterRef<Type const>;

      public:
         Arg(Arg const &arg);
         explicit Arg(Type const *type);
         Arg(Type const *type, Exp const *data);
         Arg(Type const *type, IR::AddrBase base, Exp const *data = nullptr);
         ~Arg();

         TypeCRef const type;
         ExpCPtr  const data;
      };
   }
}

#endif//GDCC__AST__Arg_H__

