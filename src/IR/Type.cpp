//-----------------------------------------------------------------------------
//
// Copyright (C) 2013-2014 David Hill
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
// Global Functions                                                           |
//

namespace GDCC
{
   namespace IR
   {
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
            Core::Integ min = -max - 1;

            if(satur)
            {
               if(value > max) value = max;
               if(value < min) value = min;
            }
            else
            {
               Core::Integ rng = max - min;

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
      // Type_Multi constructor
      //
      Type_Multi::Type_Multi(IArchive &in) : types{GetIR(in, types)}
      {
      }

      //
      // Type_Point constructor
      //
      Type_Point::Type_Point(IArchive &in) : reprB{GetIR(in, reprB)},
         reprS{GetIR(in, reprS)}, reprW{GetIR(in, reprW)}
      {
      }

      //
      // Type_StrEn constructor
      //
      Type_StrEn::Type_StrEn(IArchive &)
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
         return out << in.reprB << in.reprS << in.reprW;
      }

      //
      // operator OArchive << Type_StrEn
      //
      OArchive &operator << (OArchive &out, Type_StrEn const &)
      {
         return out;
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

         std::cerr << "invalid enum GDCC::IR::Type\n";
         throw EXIT_FAILURE;
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
            std::cerr << "invalid TypeBase\n";
            throw EXIT_FAILURE;
         }
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
      // operator IArchive >> Type_Multi
      //
      IArchive &operator >> (IArchive &in, Type_Multi &out)
      {
         return in >> out.types;
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

         std::cerr << "invalid enum GDCC::IR::Type\n";
         throw EXIT_FAILURE;
      }
   }
}

// EOF

