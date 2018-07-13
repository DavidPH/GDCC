//-----------------------------------------------------------------------------
//
// Copyright (C) 2014-2018 David Hill
//
// See COPYING for license information.
//
//-----------------------------------------------------------------------------
//
// Boolean/flag option handling.
//
//-----------------------------------------------------------------------------

#ifndef GDCC__Option__Bool_H__
#define GDCC__Option__Bool_H__

#include "../Option/Base.hpp"


//----------------------------------------------------------------------------|
// Types                                                                      |
//

namespace GDCC::Option
{
   //
   // Bool
   //
   class Bool : public Base
   {
   public:
      // constructor
      Bool(Program *program, Info const &optInfo, bool *ptr) :
         Base{program, optInfo}, dptr{ptr} {}

   protected:
      //
      // v_process
      //
      virtual std::size_t v_process(Args const &args)
      {
         *dptr = !args.optFalse;
         return 0;
      }

   private:
      bool *const dptr;
   };
}

#endif//GDCC__Option__Bool_H__

