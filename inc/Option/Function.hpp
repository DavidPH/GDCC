//-----------------------------------------------------------------------------
//
// Copyright (C) 2014 David Hill
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

namespace GDCC
{
   namespace Option
   {
      //
      // Function
      //
      class Function : public Base
      {
      public:
         using Processor = std::size_t(*)(Base *opt, Args const &args);


         Function(Program *list_, Info const &info_, Processor processor_) :
            Base{list_, info_}, processor{processor_} {}

         Processor const processor;

      protected:
         virtual std::size_t v_process(Args const &args)
            {return processor(this, args);}
      };
   }
}

#endif//GDCC__Option__Function_H__

