//-----------------------------------------------------------------------------
//
// Copyright (C) 2013-2015 David Hill
//
// See COPYING for license information.
//
//-----------------------------------------------------------------------------
//
// Intermediary Representation expression type handling.
//
//-----------------------------------------------------------------------------

#include "IR/Type.hpp"

#include "IR/Addr.hpp"
#include "IR/CallType.hpp"
#include "IR/IArchive.hpp"
#include "IR/OArchive.hpp"

#include <iostream>


//----------------------------------------------------------------------------|
// Extern Functions                                                           |
//

namespace GDCC
{
   namespace IR
   {
      //
      // Type_Array copy constructor
      //
      Type_Array::Type_Array(Type_Array const &t) :
         elemT{new Type(*t.elemT)}, elemC{t.elemC}
      {
      }

      //
      // Type_Array constructor
      //
      Type_Array::Type_Array(Type const &t, Core::FastU c) :
         elemT{new Type(t)}, elemC{c}
      {
      }

      //
      // Type_Array constructor
      //
      Type_Array::Type_Array(Type &&t, Core::FastU c) :
         elemT{new Type(std::move(t))}, elemC{c}
      {
      }

      //
      // Type_Array constructor
      //
      Type_Array::Type_Array(IArchive &in) :
         elemT{new Type(in)}, elemC{GetIR(in, elemC)}
      {
      }

      //
      // Type_Array::operator = Type_Array
      //
      Type_Array &Type_Array::operator = (Type_Array const &t)
      {
         if(elemT)
            *elemT = *t.elemT;
         else
            elemT.reset(new Type(*t.elemT));

         elemC = t.elemC;

         return *this;
      }

      //
      // Type_Array::operator == Type_Array
      //
      bool Type_Array::operator == (Type_Array const &t) const
      {
         return elemC == t.elemC && *elemT == *t.elemT;
      }

      //
      // Type_Assoc constructor
      //
      Type_Assoc::Type_Assoc(IArchive &in) : assoc{GetIR(in, assoc)}
      {
      }

      //
      // Type_DJump constructor
      //
      Type_DJump::Type_DJump(IArchive &)
      {
      }

      //
      // Type_Empty constructor
      //
      Type_Empty::Type_Empty(IArchive &)
      {
      }

      //
      // Type_Fixed constructor
      //
      Type_Fixed::Type_Fixed(IArchive &in) : bitsI{GetIR(in, bitsI)},
         bitsF{GetIR(in, bitsF)}, bitsS{GetIR<bool>(in)}, satur{GetIR<bool>(in)}
      {
      }

      //
      // Type_Fixed::clamp
      //
      Core::Integ &Type_Fixed::clamp(Core::Integ &value)
      {
         Core::Integ max = 1; max <<= bitsF + bitsI; --max;

         if(bitsS)
         {
            Core::Integ min = -max; --min;

            if(satur)
            {
                    if(value > max) value = max;
               else if(value < min) value = min;
            }
            else if(value > max || value < min)
            {
               Core::FastU bit = bitsF + bitsI;

               max <<= 1; ++max;

               value &= max;

               if(mpz_tstbit(value.get_mpz_t(), bit))
               {
                  value ^= max;
                  ++value;
                  value = -value;
               }
            }
         }
         else
         {
            if(satur)
            {
                    if(value > max) value = max;
               else if(value < 0)   value = 0;
            }
            else
            {
               value &= max;
            }
         }

         return value;
      }

      //
      // Type_Fixed::Promote
      //
      Type_Fixed Type_Fixed::Promote(Type_Fixed const &l, Type_Fixed const &r)
      {
         return Type_Fixed(std::max(l.bitsI, r.bitsI),
            std::max(l.bitsF, r.bitsF), l.bitsS || r.bitsS, l.satur || r.satur);
      }

      //
      // Type_Float constructor
      //
      Type_Float::Type_Float(IArchive &in) : bitsI{GetIR(in, bitsI)},
         bitsF{GetIR(in, bitsF)}, bitsS{GetIR<bool>(in)}, satur{GetIR<bool>(in)}
      {
      }

      //
      // Type_Float::Promote
      //
      Type_Float Type_Float::Promote(Type_Float const &l, Type_Float const &r)
      {
         return Type_Float(std::max(l.bitsI, r.bitsI),
            std::max(l.bitsF, r.bitsF), l.bitsS || r.bitsS, l.satur || r.satur);
      }

      //
      // Type_Funct constructor
      //
      Type_Funct::Type_Funct(IArchive &in) : callT{GetIR(in, callT)}
      {
      }

      //
      // Type_Point constructor
      //
      Type_Point::Type_Point(IArchive &in) : reprB{GetIR(in, reprB)},
         reprN{GetIR(in, reprN)}, reprS{GetIR(in, reprS)}, reprW{GetIR(in, reprW)}
      {
      }

      //
      // Type_StrEn constructor
      //
      Type_StrEn::Type_StrEn(IArchive &)
      {
      }

      //
      // Type_Tuple constructor
      //
      Type_Tuple::Type_Tuple(IArchive &in) : types{GetIR(in, types)}
      {
      }

      //
      // Type_Union constructor
      //
      Type_Union::Type_Union(IArchive &in) : types{GetIR(in, types)}
      {
      }

      //
      // Type constructor
      //
      Type::Type(IArchive &in)
      {
         switch(in >> t, t)
         {
            #define GDCC_IR_TypeList(name) \
               case TypeBase::name: new(&t##name) Type_##name(in); return;
            #include "IR/TypeList.hpp"
         }
      }

      //
      // operator OArchive << Type_Array
      //
      OArchive &operator << (OArchive &out, Type_Array const &in)
      {
         return out << *in.elemT << in.elemC;
      }

      //
      // operator OArchive << Type_Assoc
      //
      OArchive &operator << (OArchive &out, Type_Assoc const &in)
      {
         return out << in.assoc;
      }

      //
      // operator OArchive << Type_DJump
      //
      OArchive &operator << (OArchive &out, Type_DJump const &)
      {
         return out;
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
      // operator OArchive << Type_Point
      //
      OArchive &operator << (OArchive &out, Type_Point const &in)
      {
         return out << in.reprB << in.reprN << in.reprS << in.reprW;
      }

      //
      // operator OArchive << Type_StrEn
      //
      OArchive &operator << (OArchive &out, Type_StrEn const &)
      {
         return out;
      }

      //
      // operator OArchive << Type_Tuple
      //
      OArchive &operator << (OArchive &out, Type_Tuple const &in)
      {
         return out << in.types;
      }

      //
      // operator OArchive << Type_Union
      //
      OArchive &operator << (OArchive &out, Type_Union const &in)
      {
         return out << in.types;
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
            #include "IR/TypeList.hpp"
         }

         std::cerr << "invalid GDCC::IR::Type\n";
         throw EXIT_FAILURE;
      }

      //
      // operator OArchive << TypeBase
      //
      OArchive &operator << (OArchive &out, TypeBase in)
      {
         switch(in)
         {
            #define GDCC_IR_TypeList(name) \
               case TypeBase::name: return out << Core::STR_##name;
            #include "IR/TypeList.hpp"
         }

         std::cerr << "invalid enum GDCC::IR::TypeBase\n";
         throw EXIT_FAILURE;
      }

      //
      // operator OArchive << TypeAssoc
      //
      OArchive &operator << (OArchive &out, TypeAssoc const &in)
      {
         return out << in.type << in.name << in.addr;
      }

      //
      // operator std::ostream << TypeBase
      //
      std::ostream &operator << (std::ostream &out, TypeBase in)
      {
         switch(in)
         {
            #define GDCC_IR_TypeList(name) \
               case TypeBase::name: return out << #name;
            #include "IR/TypeList.hpp"
         }

         std::cerr << "invalid enum GDCC::IR::TypeBase\n";
         throw EXIT_FAILURE;
      }

      //
      // operator IArchive >> Type_Array
      //
      IArchive &operator >> (IArchive &in, Type_Array &out)
      {
         if(out.elemT)
            return in >> *out.elemT >> out.elemC;

         out.elemT.reset(new Type(in));
         return in >> out.elemC;
      }

      //
      // operator IArchive >> Type_Assoc
      //
      IArchive &operator >> (IArchive &in, Type_Assoc &out)
      {
         return in >> out.assoc;
      }

      //
      // operator IArchive >> Type_DJump
      //
      IArchive &operator >> (IArchive &in, Type_DJump &)
      {
         return in;
      }

      //
      // operator IArchive >> Type_Empty
      //
      IArchive &operator >> (IArchive &in, Type_Empty &)
      {
         return in;
      }

      //
      // operator IArchive >> Type_Fixed
      //
      IArchive &operator >> (IArchive &in, Type_Fixed &out)
      {
         in >> out.bitsI >> out.bitsF;
         out.bitsS = GetIR<bool>(in);
         out.satur = GetIR<bool>(in);
         return in;
      }

      //
      // operator IArchive >> Type_Float
      //
      IArchive &operator >> (IArchive &in, Type_Float &out)
      {
         in >> out.bitsI >> out.bitsF;
         out.bitsS = GetIR<bool>(in);
         out.satur = GetIR<bool>(in);
         return in;
      }

      //
      // operator IArchive >> Type_Funct
      //
      IArchive &operator >> (IArchive &in, Type_Funct &out)
      {
         return in >> out.callT;
      }

      //
      // operator IArchive >> Type_Point
      //
      IArchive &operator >> (IArchive &in, Type_Point &out)
      {
         return in >> out.reprB >> out.reprS >> out.reprW;
      }

      //
      // operator IArchive >> Type_StrEn
      //
      IArchive &operator >> (IArchive &in, Type_StrEn &)
      {
         return in;
      }

      //
      // operator IArchive >> Type_Tuple
      //
      IArchive &operator >> (IArchive &in, Type_Tuple &out)
      {
         return in >> out.types;
      }

      //
      // operator IArchive >> Type_Union
      //
      IArchive &operator >> (IArchive &in, Type_Union &out)
      {
         return in >> out.types;
      }

      //
      // operator IArchive >> Type
      //
      IArchive &operator >> (IArchive &in, Type &out)
      {
         switch(GetIR<TypeBase>(in))
         {
            #define GDCC_IR_TypeList(name) \
               case TypeBase::name: out = Type_##name(in); return in;
            #include "IR/TypeList.hpp"
         }

         std::cerr << "invalid GDCC::IR::Type\n";
         throw EXIT_FAILURE;
      }

      //
      // operator IArchive >> TypeBase
      //
      IArchive &operator >> (IArchive &in, TypeBase &out)
      {
         switch(GetIR<Core::StringIndex>(in))
         {
            #define GDCC_IR_TypeList(name) \
               case Core::STR_##name: out = TypeBase::name; return in;
            #include "IR/TypeList.hpp"

         default:
            std::cerr << "invalid enum GDCC::IR::TypeBase\n";
            throw EXIT_FAILURE;
         }
      }

      //
      // operator IArchive >> TypeAssoc
      //
      IArchive &operator >> (IArchive &in, TypeAssoc &out)
      {
         return in >> out.type >> out.name >> out.addr;
      }
   }
}

// EOF

