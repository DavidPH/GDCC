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

#include "Addr.hpp"
#include "Function.hpp"
#include "OArchive.hpp"


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

      //
      // operator OArchive << TypeBase
      //
      OArchive &operator << (OArchive &out, TypeBase in)
      {
         switch(in)
         {
            #define GDCC_IR_TypeList(name) \
               case TypeBase::name: return out << STR_##name;
            #include "TypeList.hpp"
         }
      }

      //
      // operator OArchive << Type_Empty
      //
      OArchive &operator << (OArchive &out, Type_Empty const &)
      {
         return out;
      }

      //
      // operator OArchive << Type_Fixed
      //
      OArchive &operator << (OArchive &out, Type_Fixed const &in)
      {
         return out << in.bitsI << in.bitsF << in.bitsS << in.satur;
      }

      //
      // operator OArchive << Type_Float
      //
      OArchive &operator << (OArchive &out, Type_Float const &in)
      {
         return out << in.bitsI << in.bitsF << in.bitsS << in.satur;
      }

      //
      // operator OArchive << Type_Funct
      //
      OArchive &operator << (OArchive &out, Type_Funct const &in)
      {
         return out << in.callT;
      }

      //
      // operator OArchive << Type_Multi
      //
      OArchive &operator << (OArchive &out, Type_Multi const &in)
      {
         return out << in.types;
      }

      //
      // operator OArchive << Type_Point
      //
      OArchive &operator << (OArchive &out, Type_Point const &in)
      {
         return out << in.reprB << in.reprO;
      }

      //
      // operator OArchive << Type
      //
      OArchive &operator << (OArchive &out, Type const &in)
      {
         out << in.t;
         switch(in.t)
         {
            #define GDCC_IR_TypeList(name) \
               case TypeBase::name: return out << in.t##name;
            #include "TypeList.hpp"
         }
      }
   }
}

// EOF

