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

#include <iostream>


//----------------------------------------------------------------------------|
// Global Functions                                                           |
//

namespace GDCC
{
   namespace IR
   {
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
      // Value_Multi constructor
      //
      Value_Multi::Value_Multi(IArchive &in) : vtype{in}, value{GetIR(in, value)}
      {
      }

      //
      // Value_Point constructor
      //
      Value_Point::Value_Point(IArchive &in) : vtype{in}, value{GetIR(in, value)},
         addrB{GetIR(in, addrB)}, addrN{GetIR(in, addrN)}
      {
      }

      //
      // Value_StrEn constructor
      //
      Value_StrEn::Value_StrEn(IArchive &in) : vtype{in}, value{GetIR(in, value)}
      {
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
      // operator OArchive << Value_StrEn
      //
      OArchive &operator << (OArchive &out, Value_StrEn const &in)
      {
         return out << in.vtype << in.value;
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
      // operator IArchive >> Value_Multi
      //
      IArchive &operator >> (IArchive &in, Value_Multi &out)
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

