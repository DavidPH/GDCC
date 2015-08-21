//-----------------------------------------------------------------------------
//
// Copyright (C) 2013-2014 David Hill
//
// See COPYING for license information.
//
//-----------------------------------------------------------------------------
//
// Intermediary Representation expression value handling.
//
//-----------------------------------------------------------------------------

#include "IR/Value.hpp"

#include "IR/Addr.hpp"
#include "IR/IArchive.hpp"
#include "IR/OArchive.hpp"

#include "Platform/Platform.hpp"

#include <iostream>


//----------------------------------------------------------------------------|
// Global Variables                                                           |
//

namespace GDCC
{
   namespace IR
   {
      Value_Fixed const Value_Fixed::Bool0{0, {1, 0, 0, 0}};
      Value_Fixed const Value_Fixed::Bool1{1, {1, 0, 0, 0}};
   }
}


//----------------------------------------------------------------------------|
// Global Functions                                                           |
//

namespace GDCC
{
   namespace IR
   {
      //
      // Value_Array constructor
      //
      Value_Array::Value_Array(IArchive &in) : vtype{in}, value{GetIR(in, value)}
      {
      }

      //
      // Value_Assoc constructor
      //
      Value_Assoc::Value_Assoc(IArchive &in) : vtype{in}, value{GetIR(in, value)}
      {
      }

      //
      // Value_DJump constructor
      //
      Value_DJump::Value_DJump(IArchive &in) : vtype{in}, value{GetIR(in, value)}
      {
      }

      //
      // Value_Empty constructor
      //
      Value_Empty::Value_Empty(IArchive &in) : vtype{in}
      {
      }

      //
      // Value_Fixed constructor
      //
      Value_Fixed::Value_Fixed(IArchive &in) : vtype{in}, value{GetIR(in, value)}
      {
      }

      //
      // Value_Fixed::getFastU
      //
      Core::FastU Value_Fixed::getFastU() const
      {
         if(vtype.bitsF)
         {
            if(vtype.bitsS)
               return Core::NumberCast<Core::FastI>(value >> vtype.bitsF);
            else
               return Core::NumberCast<Core::FastU>(value >> vtype.bitsF);
         }
         else
         {
            if(vtype.bitsS)
               return Core::NumberCast<Core::FastI>(value);
            else
               return Core::NumberCast<Core::FastU>(value);
         }
      }

      //
      // Value_Float constructor
      //
      Value_Float::Value_Float(IArchive &in) : vtype{in}, value{GetIR(in, value)}
      {
      }

      //
      // Value_Funct constructor
      //
      Value_Funct::Value_Funct(IArchive &in) : vtype{in}, value{GetIR(in, value)}
      {
      }

      //
      // Value_Funct::operator bool
      //
      Value_Funct::operator bool () const
      {
         if(Platform::IsZeroNull_Funct(vtype.callT))
            return value != 0;
         else
            return value != static_cast<Core::FastU>(-1);
      }

      //
      // Value_Point constructor
      //
      Value_Point::Value_Point(IArchive &in) : vtype{in}, value{GetIR(in, value)},
         addrB{GetIR(in, addrB)}, addrN{GetIR(in, addrN)}
      {
      }

      //
      // Value_Point::operator bool
      //
      Value_Point::operator bool () const
      {
         if(Platform::IsZeroNull_Point(vtype.reprB))
            return value != 0;
         else
            return value != static_cast<Core::FastU>(-1);
      }

      //
      // Value_StrEn constructor
      //
      Value_StrEn::Value_StrEn(IArchive &in) : vtype{in}, value{GetIR(in, value)}
      {
      }

      //
      // Value_StrEn::operator bool
      //
      Value_StrEn::operator bool () const
      {
         if(Platform::IsZeroNull_StrEn())
            return value != 0;
         else
            return value != static_cast<Core::FastU>(-1);
      }

      //
      // Value_Tuple constructor
      //
      Value_Tuple::Value_Tuple(IArchive &in) : vtype{in}, value{GetIR(in, value)}
      {
      }

      //
      // Value_Union copy constructor
      //
      Value_Union::Value_Union(Value_Union const &v) :
         vtype{v.vtype}, value{new Value(*v.value)}
      {
      }

      //
      // Value_Union constructor
      //
      Value_Union::Value_Union(Value const &v, Type_Union const &t) :
         vtype{t}, value{new Value(v)}
      {
      }

      //
      // Value_Union constructor
      //
      Value_Union::Value_Union(Value const &v, Type_Union &&t) :
         vtype{std::move(t)}, value{new Value(v)}
      {
      }

      //
      // Value_Union constructor
      //
      Value_Union::Value_Union(Value &&v, Type_Union const &t) :
         vtype{t}, value{new Value(std::move(v))}
      {
      }

      //
      // Value_Union constructor
      //
      Value_Union::Value_Union(Value &&v, Type_Union &&t) :
         vtype{std::move(t)}, value{new Value(std::move(v))}
      {
      }

      //
      // Value_Union constructor
      //
      Value_Union::Value_Union(IArchive &in) : vtype{in}, value{new Value(in)}
      {
      }

      //
      // Value_Union::operator bool
      //
      Value_Union::operator bool () const
      {
         return static_cast<bool>(*value);
      }

      //
      // Value_Union::operator = Value_Union
      //
      Value_Union &Value_Union::operator = (Value_Union const &v)
      {
         vtype = v.vtype;

         if(value)
            *value = *v.value;
         else
            value.reset(new Value(*v.value));

         return *this;
      }

      //
      // Value constructor
      //
      Value::Value(IArchive &in)
      {
         switch(in >> v, v)
         {
            #define GDCC_IR_TypeList(name) \
               case ValueBase::name: new(&v##name) Value_##name(in); return;
            #include "IR/TypeList.hpp"
         }
      }

      //
      // Value::getFastU
      //
      Core::FastU Value::getFastU() const
      {
         switch(v)
         {
         case ValueBase::DJump:
            return vDJump.value;

         case ValueBase::Fixed:
            return vFixed.getFastU();

         case ValueBase::Funct:
            return vFunct.value;

         case ValueBase::Point:
            return vPoint.value;

         case ValueBase::StrEn:
            return vStrEn.value;

         default:
            std::cerr << "ERROR: Bad Value::getFastU: " << v << '\n';
            throw EXIT_FAILURE;
         }
      }

      //
      // operator OArchive << Value_Array
      //
      OArchive &operator << (OArchive &out, Value_Array const &in)
      {
         return out << in.vtype << in.value;
      }

      //
      // operator OArchive << Value_Assoc
      //
      OArchive &operator << (OArchive &out, Value_Assoc const &in)
      {
         return out << in.vtype << in.value;
      }

      //
      // operator OArchive << Value_DJump
      //
      OArchive &operator << (OArchive &out, Value_DJump const &in)
      {
         return out << in.vtype << in.value;
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
      // operator OArchive << Value_Point
      //
      OArchive &operator << (OArchive &out, Value_Point const &in)
      {
         return out << in.vtype << in.value << in.addrB << in.addrN;
      }

      //
      // operator OArchive << Value_StrEn
      //
      OArchive &operator << (OArchive &out, Value_StrEn const &in)
      {
         return out << in.vtype << in.value;
      }

      //
      // operator OArchive << Value_Tuple
      //
      OArchive &operator << (OArchive &out, Value_Tuple const &in)
      {
         return out << in.vtype << in.value;
      }

      //
      // operator OArchive << Value_Union
      //
      OArchive &operator << (OArchive &out, Value_Union const &in)
      {
         return out << in.vtype << *in.value;
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
            #include "IR/TypeList.hpp"
         }

         std::cerr << "invalid enum GDCC::IR::Value\n";
         throw EXIT_FAILURE;
      }

      //
      // operator IArchive >> Value_Array
      //
      IArchive &operator >> (IArchive &in, Value_Array &out)
      {
         return in >> out.vtype >> out.value;
      }

      //
      // operator IArchive >> Value_Assoc
      //
      IArchive &operator >> (IArchive &in, Value_Assoc &out)
      {
         return in >> out.vtype >> out.value;
      }

      //
      // operator IArchive >> Value_DJump
      //
      IArchive &operator >> (IArchive &in, Value_DJump &out)
      {
         return in >> out.vtype >> out.value;
      }

      //
      // operator IArchive >> Value_Empty
      //
      IArchive &operator >> (IArchive &in, Value_Empty &out)
      {
         return in >> out.vtype;
      }

      //
      // operator IArchive >> Value_Fixed
      //
      IArchive &operator >> (IArchive &in, Value_Fixed &out)
      {
         return in >> out.vtype >> out.value;
      }

      //
      // operator IArchive >> Value_Float
      //
      IArchive &operator >> (IArchive &in, Value_Float &out)
      {
         return in >> out.vtype >> out.value;
      }

      //
      // operator IArchive >> Value_Funct
      //
      IArchive &operator >> (IArchive &in, Value_Funct &out)
      {
         return in >> out.vtype >> out.value;
      }

      //
      // operator IArchive >> Value_Point
      //
      IArchive &operator >> (IArchive &in, Value_Point &out)
      {
         return in >> out.vtype >> out.value >> out.addrB >> out.addrN;
      }

      //
      // operator IArchive >> Value_StrEn
      //
      IArchive &operator >> (IArchive &in, Value_StrEn &out)
      {
         return in >> out.vtype >> out.value;
      }

      //
      // operator IArchive >> Value_Tuple
      //
      IArchive &operator >> (IArchive &in, Value_Tuple &out)
      {
         return in >> out.vtype >> out.value;
      }

      //
      // operator IArchive >> Value_Union
      //
      IArchive &operator >> (IArchive &in, Value_Union &out)
      {
         in >> out.vtype;

         if(out.value)
            return in >> *out.value;

         out.value.reset(new Value(in));

         return in;
      }

      //
      // operator IArchive >> Value
      //
      IArchive &operator >> (IArchive &in, Value &out)
      {
         switch(GetIR<ValueBase>(in))
         {
            #define GDCC_IR_TypeList(name) \
               case ValueBase::name: out = Value_##name(in); return in;
            #include "IR/TypeList.hpp"
         }

         std::cerr << "invalid enum GDCC::IR::Value\n";
         throw EXIT_FAILURE;
      }
   }
}

// EOF

