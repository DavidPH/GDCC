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

#ifndef GDCC__IR__Value_H__
#define GDCC__IR__Value_H__

#include "Type.hpp"


//----------------------------------------------------------------------------|
// Types                                                                      |
//

namespace GDCC
{
   namespace IR
   {
      class Value;

      typedef TypeBase ValueBase;

      //
      // Value_Empty
      //
      struct Value_Empty
      {
         Type_Empty vtype;
      };

      //
      // Value_Fixed
      //
      struct Value_Fixed
      {
         Value_Fixed() = default;

         Value_Fixed(Value_Fixed const &value, Type_Fixed const &vtype);
         Value_Fixed(Value_Fixed &&value, Type_Fixed const &vtype);

         Value_Fixed(Integ const &value_, Type_Fixed const &vtype_) :
            vtype{vtype_}, value{value_} {clamp();}
         Value_Fixed(Integ &&value_, Type_Fixed const &vtype_) :
            vtype{vtype_}, value{std::move(value_)} {clamp();}

         Value_Fixed &clamp() {vtype.clamp(value); return *this;}

         Type_Fixed vtype;
         Integ      value;
      };

      //
      // Value_Float
      //
      struct Value_Float
      {
         Type_Float vtype;
         Float      value;
      };

      //
      // Value_Funct
      //
      struct Value_Funct
      {
         Type_Funct vtype;
         FastU      value;
      };

      //
      // Value_Multi
      //
      struct Value_Multi
      {
         Value_Multi() = default;

         Value_Multi(Array<Value> const &value_, Type_Multi const &vtype_) :
            vtype{vtype_}, value{value_} {}
         Value_Multi(Array<Value> const &value_, Type_Multi &&vtype_) :
            vtype{std::move(vtype_)}, value{value_} {}
         Value_Multi(Array<Value> &&value_, Type_Multi const &vtype_) :
            vtype{vtype_}, value{std::move(value_)} {}
         Value_Multi(Array<Value> &&value_, Type_Multi &&vtype_) :
            vtype{std::move(vtype_)}, value{std::move(value_)} {}

         Type_Multi   vtype;
         Array<Value> value;
      };

      //
      // Value_Point
      //
      struct Value_Point
      {
         Type_Point vtype;
         FastU      value;
         AddrBase   addrB;
         String     addrN;
      };

      //
      // Value
      //
      class Value
      {
      public:
         Value() : v{ValueBase::Empty}, vEmpty{} {}

         //
         // copy constructor
         //
         Value(Value const &val) : v{val.v}
         {
            switch(v)
            {
               #define GDCC_IR_TypeList(name) case ValueBase::name: \
                     new(&v##name) Value_##name(val.v##name); break;
               #include "TypeList.hpp"
            }
         }

         //
         // move constructor
         //
         Value(Value &&val) : v{val.v}
         {
            switch(v)
            {
               #define GDCC_IR_TypeList(name) case ValueBase::name: \
                     new(&v##name) Value_##name(std::move(val.v##name)); break;
               #include "TypeList.hpp"
            }
         }

         //
         // cast constructors
         //
         #define GDCC_IR_TypeList(name) \
            Value(Value_##name const &val) : v{ValueBase::name}, v##name(          val ) {} \
            Value(Value_##name      &&val) : v{ValueBase::name}, v##name(std::move(val)) {}
         #include "TypeList.hpp"

         //
         // destructor
         //
         ~Value()
         {
            switch(v)
            {
               #define GDCC_IR_TypeList(name) \
                  case ValueBase::name: v##name.~Value_##name(); break;
               #include "TypeList.hpp"
            }
         }

         //
         // copy assignment
         //
         Value &operator = (Value const &val)
         {
            if(val.v == v) switch(v)
            {
               #define GDCC_IR_TypeList(name) \
                  case ValueBase::name: v##name = val.v##name; break;
               #include "TypeList.hpp"
            }
            else
               new((this->~Value(), this)) Value(val);

            return *this;
         }

         //
         // move assignment
         //
         Value &operator = (Value &&val)
         {
            if(val.v == v) switch(v)
            {
               #define GDCC_IR_TypeList(name) \
                  case ValueBase::name: v##name = std::move(val.v##name); break;
               #include "TypeList.hpp"
            }
            else
               new((this->~Value(), this)) Value(std::move(val));

            return *this;
         }

         //
         // getType
         //
         Type getType() const
         {
            switch(v)
            {
               #define GDCC_IR_TypeList(name) \
                  case ValueBase::name: return v##name.vtype;
               #include "TypeList.hpp"
            }

            return Type();
         }

         ValueBase v;

         union
         {
            #define GDCC_IR_TypeList(name) Value_##name v##name;
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
      OArchive &operator << (OArchive &out, ValueBase in);

      #define GDCC_IR_TypeList(name) \
         OArchive &operator << (OArchive &out, Value_##name const &in);
      #include "TypeList.hpp"

      OArchive &operator << (OArchive &out, Value const &in);
   }
}

#endif//GDCC__IR__Value_H__

