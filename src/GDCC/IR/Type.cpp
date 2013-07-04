//-----------------------------------------------------------------------------
//
// Copyright (C) 2013 David Hill
//
// See COPYING for license information.
//
//-----------------------------------------------------------------------------
//
// Intermediary Representation expression type handling.
//
//-----------------------------------------------------------------------------

#include "Type.hpp"


//----------------------------------------------------------------------------|
// Global Functions                                                           |
//

namespace GDCC
{
   namespace IR
   {
      //
      // Type_Fixed::clamp
      //
      Integ &Type_Fixed::clamp(Integ &value)
      {
         Integ max = 1; max <<= bitsF + bitsI; --max;

         if(bitsS)
         {
            Integ min = -max - 1;

            if(satur)
            {
               if(value > max) value = max;
               if(value < min) value = min;
            }
            else
            {
               Integ rng = max - min;

               while(value > max) value -= rng;
               while(value < min) value += rng;
            }
         }
         else
         {
            if(satur)
            {
               if(value > max) value = max;
               if(value < 0)   value = 0;
            }
            else
            {
               while(value > max) value -= max;
               while(value < 0)   value += max;
            }
         }

         return value;
      }
   }
}

// EOF

