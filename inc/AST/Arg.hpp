//-----------------------------------------------------------------------------
//
// Copyright (C) 2013 David Hill
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
         explicit Arg(Type const *type, Exp const *data = nullptr);
         Arg(Type const *type, IR::AddrBase base, Exp const *data = nullptr);
         ~Arg();

         TypeCRef const type;
         ExpCPtr  const data;
      };
   }
}

// EOF

