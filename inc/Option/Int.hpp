//-----------------------------------------------------------------------------
//
// Copyright (C) 2014 David Hill
//
// See COPYING for license information.
//
//-----------------------------------------------------------------------------
//
// Integer option handling.
//
//-----------------------------------------------------------------------------

#ifndef GDCC__Option__Int_H__
#define GDCC__Option__Int_H__

#include "../Option/Base.hpp"


//----------------------------------------------------------------------------|
// Types                                                                      |
//

namespace GDCC
{
   namespace Option
   {
      //
      // Int
      //
      template<typename T>
      class Int : public Base
      {
      public:
         Int(Program *list_, Info const &info_, T *data_) :
            Base{list_, info_}, data{data_} {}

         T *data;

      protected:
         //
         // v_process
         //
         virtual std::size_t v_process(Args const &args)
         {
            if(args.optFalse) {*data = 0; return 0;}
            if(!args.argC) Exception::Error(args, "argument required");

            *data = 0;
            for(auto s = args.argV[0]; *s; ++s)
            {
               if(*s < '0' || *s > '9')
                  Exception::Error(args, "integer required");

               *data = *data * 10 + (*s - '0');
            }

            return 1;
         }
      };
   }
}

#endif//GDCC__Option__Int_H__

