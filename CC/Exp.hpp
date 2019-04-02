//-----------------------------------------------------------------------------
//
// Copyright (C) 2014-2019 David Hill
//
// See COPYING for license information.
//
//-----------------------------------------------------------------------------
//
// C expressions.
//
//-----------------------------------------------------------------------------

#ifndef GDCC__CC__Exp_H__
#define GDCC__CC__Exp_H__

#include "../CC/Types.hpp"

#include "../Core/Counter.hpp"
#include "../Core/Number.hpp"


//----------------------------------------------------------------------------|
// Types                                                                      |
//

namespace GDCC::CC
{
   //
   // GenAssoc
   //
   class GenAssoc
   {
   public:
      GenAssoc(SR::Type const *type, SR::Exp const *exp);

      Core::CounterRef<SR::Type const> type;
      Core::CounterRef<SR::Exp const>  exp;
   };
}


//----------------------------------------------------------------------------|
// Extern Functions                                                           |
//

namespace GDCC::CC
{
   Core::FastU ExpToFastU(SR::Exp const *exp);
   Core::Integ ExpToInteg(SR::Exp const *exp);

   // Is exp an lvalue?
   bool IsLValue(SR::Exp const *exp);

   // Is exp a modifiable lvalue?
   bool IsModLValue(SR::Exp const *exp);
}

#endif//GDCC__CC__Exp_H__

