//-----------------------------------------------------------------------------
//
// Copyright (C) 2014 David Hill
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

namespace GDCC
{
   namespace Option
   {
      //
      // Bool
      //
      class Bool : public Base
      {
      public:
         Bool(Program *list_, Info const &info_, bool *data_) :
            Base{list_, info_}, data{data_} {}

         bool *data;

      protected:
         virtual std::size_t v_process(Args const &args)
            {*data = !args.optFalse; return 0;}
      };
   }
}

#endif//GDCC__Option__Bool_H__

