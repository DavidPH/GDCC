//-----------------------------------------------------------------------------
//
// Copyright (C) 2014-2018 David Hill
//
// See COPYING for license information.
//
//-----------------------------------------------------------------------------
//
// Function-calling option handling.
//
//-----------------------------------------------------------------------------

#ifndef GDCC__Option__Function_H__
#define GDCC__Option__Function_H__

#include "../Option/Base.hpp"


//----------------------------------------------------------------------------|
// Types                                                                      |
//

namespace GDCC::Option
{
   //
   // Function
   //
   class Function : public Base
   {
   public:
      using Processor = std::size_t(*)(Base *opt, Args const &args);


      // constructor
      Function(Program *program, Info const &optInfo, Processor processor) :
         Base{program, optInfo}, fn{processor} {}

   protected:
      //
      // v_process
      //
      virtual std::size_t v_process(Args const &args)
      {
         return fn(this, args);
      }

   private:
      Processor const fn;
   };
}

#endif//GDCC__Option__Function_H__

