//-----------------------------------------------------------------------------
//
// Copyright (C) 2013-2023 David Hill
//
// See COPYING for license information.
//
//-----------------------------------------------------------------------------
//
// Source Representation storage arguments.
//
// This is the SR equivalent to IR::Arg, used to designate run-time data
// sources and sinks.
//
//-----------------------------------------------------------------------------

#ifndef GDCC__SR__Arg_H__
#define GDCC__SR__Arg_H__

#include "../SR/Types.hpp"

#include "../Core/Counter.hpp"


//----------------------------------------------------------------------------|
// Types                                                                      |
//

namespace GDCC::SR
{
   //
   // Arg
   //
   class Arg
   {
   protected:
      using ExpCPtr  = Core::CounterPtr<Exp const>;
      using TypeCRef = Core::CounterRef<Type const>;

   public:
      explicit Arg(Type const *type);
      Arg(Type const *type, Exp const *data);
      Arg(Type const *type, IR::AddrBase base, Exp const *data = nullptr);

      IR::Arg getIRArg(IR::Program &prog) const;

      IR::Arg_Stk getIRArgStk() const;

      bool isIRArg() const;

      TypeCRef type;
      ExpCPtr  data;
   };
}

#endif//GDCC__SR__Arg_H__

