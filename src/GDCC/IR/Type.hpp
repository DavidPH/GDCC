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

#ifndef GDCC__IR__Type_H__
#define GDCC__IR__Type_H__

#include "../Array.hpp"
#include "../Number.hpp"
#include "../String.hpp"

#include <memory>


//----------------------------------------------------------------------------|
// Types                                                                      |
//

namespace GDCC
{
   namespace IR
   {
      enum class AddrBase;
      enum class CallType;
      class Type;

      //
      // TypeBase
      //
      enum class TypeBase
      {
         #define GDCC_IR_TypeList(name) name,
         #include "TypeList.hpp"
      };

      //
      // Type_Empty
      //
      struct Type_Empty
      {
         Type_Empty() = default;

         explicit Type_Empty(IArchive &in);
      };

      //
      // Type_Fixed
      //
      struct Type_Fixed
      {
         Type_Fixed() = default;

         Type_Fixed(FastU bitsI_, FastU bitsF_, bool bitsS_, bool satur_) :
            bitsI{bitsI_}, bitsF{bitsF_}, bitsS{bitsS_}, satur{satur_} {}

         explicit Type_Fixed(IArchive &in);

         Integ &clamp(Integ &value);

         FastU getBits() const {return bitsI + bitsF + bitsS;}

         FastU bitsI;
         FastU bitsF;
         bool  bitsS : 1;
         bool  satur : 1;
      };

      //
      // Type_Float
      //
      struct Type_Float
      {
         Type_Float() = default;

         Type_Float(FastU bitsI_, FastU bitsF_, bool bitsS_, bool satur_) :
            bitsI{bitsI_}, bitsF{bitsF_}, bitsS{bitsS_}, satur{satur_} {}

         explicit Type_Float(IArchive &in);

         FastU bitsI;
         FastU bitsF;
         bool  bitsS : 1;
         bool  satur : 1;
      };

      //
      // Type_Funct
      //
      struct Type_Funct
      {
         Type_Funct() = default;

         explicit Type_Funct(CallType callT_) : callT{callT_} {}

         explicit Type_Funct(IArchive &in);

         CallType callT;
      };

      //
      // Type_Multi
      //
      struct Type_Multi
      {
         Type_Multi() = default;

         explicit Type_Multi(Array<Type> const &types_) : types{types_} {}
         explicit Type_Multi(Array<Type> &&types_) : types{std::move(types_)} {}

         explicit Type_Multi(IArchive &in);

         Array<Type> types;
      };

      //
      // Type_Point
      //
      struct Type_Point
      {
         Type_Point() = default;

         Type_Point(AddrBase reprB_, FastU reprO_) : reprB{reprB_}, reprO{reprO_} {}
         explicit Type_Point(IArchive &in);

         AddrBase reprB;
         FastU    reprO;
      };

      //
      // Type_StrEn
      //
      struct Type_StrEn
      {
         Type_StrEn() = default;

         explicit Type_StrEn(IArchive &in);
      };

      //
      // Type
      //
      class Type
      {
      public:
         Type() : t{TypeBase::Empty}, tEmpty{} {}

         //
         // copy constructor
         //
         Type(Type const &type) : t{type.t}
         {
            switch(t)
            {
               #define GDCC_IR_TypeList(name) case TypeBase::name: \
                     new(&t##name) Type_##name(type.t##name); break;
               #include "TypeList.hpp"
            }
         }

         //
         // move constructor
         //
         Type(Type &&type) : t{type.t}
         {
            switch(t)
            {
               #define GDCC_IR_TypeList(name) case TypeBase::name: \
                     new(&t##name) Type_##name(std::move(type.t##name)); break;
               #include "TypeList.hpp"
            }
         }

         //
         // cast constructors
         //
         #define GDCC_IR_TypeList(name) \
            Type(Type_##name const &type) : t{TypeBase::name}, t##name(          type ) {} \
            Type(Type_##name      &&type) : t{TypeBase::name}, t##name(std::move(type)) {}
         #include "TypeList.hpp"

         explicit Type(IArchive &in);

         //
         // destructor
         //
         ~Type()
         {
            switch(t)
            {
               #define GDCC_IR_TypeList(name) \
                  case TypeBase::name: t##name.~Type_##name(); break;
               #include "TypeList.hpp"
            }
         }

         //
         // copy assignment
         //
         Type &operator = (Type const &type)
         {
            if(type.t == t) switch(t)
            {
               #define GDCC_IR_TypeList(name) \
                  case TypeBase::name: t##name = type.t##name; break;
               #include "TypeList.hpp"
            }
            else
               new((this->~Type(), this)) Type(type);

            return *this;
         }

         //
         // move assignment
         //
         Type &operator = (Type &&type)
         {
            if(type.t == t) switch(t)
            {
               #define GDCC_IR_TypeList(name) \
                  case TypeBase::name: t##name = std::move(type.t##name); break;
               #include "TypeList.hpp"
            }
            else
               new((this->~Type(), this)) Type(type);

            return *this;
         }

         TypeBase t;

         union
         {
            #define GDCC_IR_TypeList(name) Type_##name t##name;
            #include "TypeList.hpp"
         };
      };
   }
}


//----------------------------------------------------------------------------|
// Global Functions                                                           |
//

namespace GDCC
{
   namespace IR
   {
      OArchive &operator << (OArchive &out, TypeBase in);

      #define GDCC_IR_TypeList(name) \
         OArchive &operator << (OArchive &out, Type_##name const &in);
      #include "TypeList.hpp"

      OArchive &operator << (OArchive &out, Type const &in);

      IArchive &operator >> (IArchive &in, TypeBase &out);

      #define GDCC_IR_TypeList(name) \
         IArchive &operator >> (IArchive &in, Type_##name &out);
      #include "TypeList.hpp"

      IArchive &operator >> (IArchive &in, Type &out);
   }
}

#endif//GDCC__IR__Type_H__

