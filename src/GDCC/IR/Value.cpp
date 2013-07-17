//-----------------------------------------------------------------------------
//
// Copyright (C) 2013 David Hill
//
// See COPYING for license information.
//
//-----------------------------------------------------------------------------
//
// Intermediary Representation expression value handling.
//
//-----------------------------------------------------------------------------

#include "Value.hpp"

#include "Addr.hpp"
#include "OArchive.hpp"


//----------------------------------------------------------------------------|
// Global Functions                                                           |
//

namespace GDCC
{
   namespace IR
   {
      //
      // Value_Fixed constructor
      //
      Value_Fixed::Value_Fixed(Value_Fixed const &value_, Type_Fixed const &vtype_) :
         vtype{vtype_}, value{value_.value}
      {
         if(vtype.bitsF > value_.vtype.bitsF)
            value <<= vtype.bitsF - value_.vtype.bitsF;
         else
            value >>= value_.vtype.bitsF - vtype.bitsF;

         clamp();
      }

      //
      // Value_Fixed constructor
      //
      Value_Fixed::Value_Fixed(Value_Fixed &&value_, Type_Fixed const &vtype_) :
         vtype{vtype_}, value{std::move(value_.value)}
      {
         if(vtype.bitsF > value_.vtype.bitsF)
            value <<= vtype.bitsF - value_.vtype.bitsF;
         else
            value >>= value_.vtype.bitsF - vtype.bitsF;

         clamp();
      }

      //
      // operator OArchive << Value_Empty
      //
      OArchive &operator << (OArchive &out, Value_Empty const &in)
      {
         return out << in.vtype;
      }

      //
      // operator OArchive << Value_Fixed
      //
      OArchive &operator << (OArchive &out, Value_Fixed const &in)
      {
         return out << in.vtype << in.value;
      }

      //
      // operator OArchive << Value_Float
      //
      OArchive &operator << (OArchive &out, Value_Float const &in)
      {
         return out << in.vtype << in.value;
      }

      //
      // operator OArchive << Value_Funct
      //
      OArchive &operator << (OArchive &out, Value_Funct const &in)
      {
         return out << in.vtype << in.value;
      }

      //
      // operator OArchive << Value_Multi
      //
      OArchive &operator << (OArchive &out, Value_Multi const &in)
      {
         return out << in.vtype << in.value;
      }

      //
      // operator OArchive << Value_Point
      //
      OArchive &operator << (OArchive &out, Value_Point const &in)
      {
         return out << in.vtype << in.value << in.addrB << in.addrN;
      }

      //
      // operator OArchive << Value
      //
      OArchive &operator << (OArchive &out, Value const &in)
      {
         out << in.v;
         switch(in.v)
         {
            #define GDCC_IR_TypeList(name) \
               case ValueBase::name: return out << in.v##name;
            #include "TypeList.hpp"
         }
      }
   }
}

// EOF

