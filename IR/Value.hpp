//-----------------------------------------------------------------------------
//
// Copyright (C) 2013-2025 David Hill
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

#include "../IR/Type.hpp"


//----------------------------------------------------------------------------|
// Types                                                                      |
//

namespace GDCC::IR
{
   //
   // Value_Array
   //
   class Value_Array
   {
   public:
      Value_Array() = default;

      Value_Array(Core::Array<Value> const &value_, Type_Array const &vtype_) :
         vtype{vtype_}, value{value_} {}
      Value_Array(Core::Array<Value> const &value_, Type_Array &&vtype_) :
         vtype{std::move(vtype_)}, value{value_} {}
      Value_Array(Core::Array<Value> &&value_, Type_Array const &vtype_) :
         vtype{vtype_}, value{std::move(value_)} {}
      Value_Array(Core::Array<Value> &&value_, Type_Array &&vtype_) :
         vtype{std::move(vtype_)}, value{std::move(value_)} {}

      explicit Value_Array(IArchive &in);

      explicit operator bool () const {return !value.empty();}

      Type_Array         vtype;
      Core::Array<Value> value;
   };

   //
   // Value_Assoc
   //
   class Value_Assoc
   {
   public:
      Value_Assoc() = default;

      Value_Assoc(Core::Array<Value> const &value_, Type_Assoc const &vtype_) :
         vtype{vtype_}, value{value_} {}
      Value_Assoc(Core::Array<Value> const &value_, Type_Assoc &&vtype_) :
         vtype{std::move(vtype_)}, value{value_} {}
      Value_Assoc(Core::Array<Value> &&value_, Type_Assoc const &vtype_) :
         vtype{vtype_}, value{std::move(value_)} {}
      Value_Assoc(Core::Array<Value> &&value_, Type_Assoc &&vtype_) :
         vtype{std::move(vtype_)}, value{std::move(value_)} {}

      explicit Value_Assoc(IArchive &in);

      explicit operator bool () const {return !value.empty();}

      Type_Assoc         vtype;
      Core::Array<Value> value;
   };

   //
   // Value_DJump
   //
   class Value_DJump
   {
   public:
      Value_DJump() = default;

      Value_DJump(Value_Fixed const &value, Type_DJump const &vtype);

      Value_DJump(Core::FastU value_, Type_DJump vtype_) :
         vtype{vtype_}, value{value_} {}

      explicit Value_DJump(IArchive &in);

      explicit operator bool () const {return static_cast<bool>(value);}

      Type_DJump  vtype;
      Core::FastU value;
   };

   //
   // Value_Empty
   //
   class Value_Empty
   {
   public:
      Value_Empty() = default;

      Value_Empty(Type_Empty vtype_) : vtype{vtype_} {}

      explicit Value_Empty(IArchive &in);

      explicit operator bool () const {return false;}

      Type_Empty vtype;
   };

   //
   // Value_Fixed
   //
   class Value_Fixed
   {
   public:
      Value_Fixed() = default;

      Value_Fixed(Value_DJump const &value, Type_Fixed const &vtype);
      Value_Fixed(Value_Fixed const &value, Type_Fixed const &vtype);
      Value_Fixed(Value_Fixed      &&value, Type_Fixed const &vtype);
      Value_Fixed(Value_Float const &value, Type_Fixed const &vtype);
      Value_Fixed(Value_Funct const &value, Type_Fixed const &vtype);
      Value_Fixed(Value_Point const &value, Type_Fixed const &vtype);
      Value_Fixed(Value_StrEn const &value, Type_Fixed const &vtype);

      Value_Fixed(Core::Integ const &value_, Type_Fixed const &vtype_) :
         vtype{vtype_}, value{value_} {clamp();}
      Value_Fixed(Core::Integ &&value_, Type_Fixed const &vtype_) :
         vtype{vtype_}, value{std::move(value_)} {clamp();}

      explicit Value_Fixed(IArchive &in);

      explicit operator bool () const {return static_cast<bool>(value);}

      Value_Fixed &clamp() {vtype.clamp(value); return *this;}

      Core::FastU getFastU() const;

      Type_Fixed  vtype;
      Core::Integ value;


      static Value_Fixed const Bool0;
      static Value_Fixed const Bool1;
   };

   //
   // Value_Float
   //
   class Value_Float
   {
   public:
      Value_Float() = default;

      Value_Float(Value_Fixed const &value, Type_Float const &vtype);
      Value_Float(Value_Float const &value, Type_Float const &vtype);
      Value_Float(Value_Float      &&value, Type_Float const &vtype);

      Value_Float(Core::Float const &value_, Type_Float const &vtype_) :
         vtype{vtype_}, value{value_} {clamp();}
      Value_Float(Core::Float &&value_, Type_Float const &vtype_) :
         vtype{vtype_}, value{std::move(value_)} {clamp();}

      explicit Value_Float(IArchive &in);

      explicit operator bool () const {return static_cast<bool>(value);}

      Value_Float &clamp() {return *this;}

      Core::FastU getFastU() const;

      Type_Float  vtype;
      Core::Float value;
   };

   //
   // Value_Funct
   //
   class Value_Funct
   {
   public:
      Value_Funct() = default;

      Value_Funct(Value_Fixed const &value, Type_Funct const &vtype);
      Value_Funct(Value_Funct const &value, Type_Funct const &vtype);
      Value_Funct(Value_StrEn const &value, Type_Funct const &vtype);

      Value_Funct(Core::FastU value_, Type_Funct const &vtype_) :
         vtype{vtype_}, value{value_} {}

      explicit Value_Funct(IArchive &in);

      explicit operator bool () const;

      Type_Funct  vtype;
      Core::FastU value;
   };

   //
   // Value_Point
   //
   class Value_Point
   {
   public:
      Value_Point() = default;

      Value_Point(Value_Point const &value, Type_Point const &vtype);
      Value_Point(Value_Fixed const &value, Type_Point const &vtype);

      Value_Point(Core::FastU value_, AddrBase addrB_, Core::String addrN_,
         Type_Point const &vtype_) :
         vtype{vtype_}, value{value_}, addrB{addrB_}, addrN{addrN_} {}

      explicit Value_Point(IArchive &in);

      explicit operator bool () const;

      Type_Point   vtype;
      Core::FastU  value;
      AddrBase     addrB;
      Core::String addrN;
   };

   //
   // Value_StrEn
   //
   class Value_StrEn
   {
   public:
      Value_StrEn() = default;

      Value_StrEn(Value_Fixed const &value, Type_StrEn const &vtype);
      Value_StrEn(Value_Funct const &value, Type_StrEn const &vtype);

      Value_StrEn(Core::FastU value_, Type_StrEn vtype_) :
         vtype{vtype_}, value{value_} {}

      explicit Value_StrEn(IArchive &in);

      explicit operator bool () const;

      Type_StrEn  vtype;
      Core::FastU value;
   };

   //
   // Value_Tuple
   //
   class Value_Tuple
   {
   public:
      Value_Tuple() = default;

      Value_Tuple(Core::Array<Value> const &value_, Type_Tuple const &vtype_) :
         vtype{vtype_}, value{value_} {}
      Value_Tuple(Core::Array<Value> const &value_, Type_Tuple &&vtype_) :
         vtype{std::move(vtype_)}, value{value_} {}
      Value_Tuple(Core::Array<Value> &&value_, Type_Tuple const &vtype_) :
         vtype{vtype_}, value{std::move(value_)} {}
      Value_Tuple(Core::Array<Value> &&value_, Type_Tuple &&vtype_) :
         vtype{std::move(vtype_)}, value{std::move(value_)} {}

      explicit Value_Tuple(IArchive &in);

      explicit operator bool () const {return !value.empty();}

      Type_Tuple         vtype;
      Core::Array<Value> value;
   };

   //
   // Value_Union
   //
   class Value_Union
   {
   public:
      Value_Union() = default;

      Value_Union(Value_Union const &v);
      Value_Union(Value_Union &&v) = default;

      Value_Union(Value const &value, Type_Union const &vtype);
      Value_Union(Value const &value, Type_Union &&vtype);
      Value_Union(Value &&value, Type_Union const &vtype);
      Value_Union(Value &&value, Type_Union &&vtype);

      explicit Value_Union(IArchive &in);

      explicit operator bool () const;

      Value_Union &operator = (Value_Union const &v);
      Value_Union &operator = (Value_Union &&v) = default;

      Type_Union             vtype;
      std::unique_ptr<Value> value;
   };

   //
   // Value
   //
   class Value
   {
   public:
      Value() : v{ValueBase::Empty}, vEmpty{} {}

      explicit Value(IArchive &in);

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

      Value const &operator + () const {return *this;}

      //
      // operator (bool)
      //
      explicit operator bool () const
      {
         switch(v)
         {
            #define GDCC_IR_TypeList(name) \
               case ValueBase::name: return static_cast<bool>(v##name);
            #include "TypeList.hpp"
         }

         return false;
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

      Core::FastU getFastU() const;

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


//----------------------------------------------------------------------------|
// Extern Functions                                                           |
//

namespace GDCC::IR
{
   Value         operator - (Value       const &e);
   Value_Fixed   operator - (Value_Fixed const &e);
   Value_Fixed &&operator - (Value_Fixed      &&e);
   Value_Float   operator - (Value_Float const &e);
   Value_Float &&operator - (Value_Float      &&e);

   Value         operator ~ (Value       const &e);
   Value_Fixed   operator ~ (Value_Fixed const &e);
   Value_Fixed &&operator ~ (Value_Fixed      &&e);

   Value         operator * (Value       const &l, Value       const &r);
   Value_Fixed   operator * (Value_Fixed const &l, Value_Fixed const &r);
   Value_Fixed &&operator * (Value_Fixed      &&l, Value_Fixed const &r);
   Value_Float   operator * (Value_Float const &l, Value_Float const &r);
   Value_Float &&operator * (Value_Float      &&l, Value_Float const &r);
   Value         operator / (Value       const &l, Value       const &r);
   Value_Fixed   operator / (Value_Fixed const &l, Value_Fixed const &r);
   Value_Fixed &&operator / (Value_Fixed      &&l, Value_Fixed const &r);
   Value_Float   operator / (Value_Float const &l, Value_Float const &r);
   Value_Float &&operator / (Value_Float      &&l, Value_Float const &r);
   Value         operator % (Value       const &l, Value       const &r);
   Value_Fixed   operator % (Value_Fixed const &l, Value_Fixed const &r);
   Value_Fixed &&operator % (Value_Fixed      &&l, Value_Fixed const &r);
   Value_Float   operator % (Value_Float const &l, Value_Float const &r);
   Value_Float &&operator % (Value_Float      &&l, Value_Float const &r);

   Value         operator + (Value       const &l, Value       const &r);
   Value_Fixed   operator + (Value_Fixed const &l, Value_Fixed const &r);
   Value_Fixed &&operator + (Value_Fixed      &&l, Value_Fixed const &r);
   Value_Point   operator + (Value_Fixed const &l, Value_Point const &r);
   Value_Point &&operator + (Value_Fixed const &l, Value_Point      &&r);
   Value_Float   operator + (Value_Float const &l, Value_Float const &r);
   Value_Float &&operator + (Value_Float      &&l, Value_Float const &r);
   Value_Point   operator + (Value_Point const &l, Value_Fixed const &r);
   Value_Point &&operator + (Value_Point      &&l, Value_Fixed const &r);
   Value         operator - (Value       const &l, Value       const &r);
   Value_Fixed   operator - (Value_Fixed const &l, Value_Fixed const &r);
   Value_Fixed &&operator - (Value_Fixed      &&l, Value_Fixed const &r);
   Value_Float   operator - (Value_Float const &l, Value_Float const &r);
   Value_Float &&operator - (Value_Float      &&l, Value_Float const &r);
   Value_Point   operator - (Value_Point const &l, Value_Fixed const &r);
   Value_Point &&operator - (Value_Point      &&l, Value_Fixed const &r);
   Value_Fixed   operator - (Value_Point const &l, Value_Point const &r);

   Value         operator << (Value       const &l, Value       const &r);
   Value_Fixed   operator << (Value_Fixed const &l, Value_Fixed const &r);
   Value_Fixed &&operator << (Value_Fixed      &&l, Value_Fixed const &r);
   Value         operator >> (Value       const &l, Value       const &r);
   Value_Fixed   operator >> (Value_Fixed const &l, Value_Fixed const &r);
   Value_Fixed &&operator >> (Value_Fixed      &&l, Value_Fixed const &r);

   bool operator <  (Value       const &l, Value       const &r);
   bool operator <  (Value_Fixed const &l, Value_Fixed const &r);
   bool operator <  (Value_Float const &l, Value_Float const &r);
   bool operator >  (Value       const &l, Value       const &r);
   bool operator >  (Value_Fixed const &l, Value_Fixed const &r);
   bool operator >  (Value_Float const &l, Value_Float const &r);
   bool operator <= (Value       const &l, Value       const &r);
   bool operator <= (Value_Fixed const &l, Value_Fixed const &r);
   bool operator <= (Value_Float const &l, Value_Float const &r);
   bool operator >= (Value       const &l, Value       const &r);
   bool operator >= (Value_Fixed const &l, Value_Fixed const &r);
   bool operator >= (Value_Float const &l, Value_Float const &r);

   bool operator == (Value       const &l, Value       const &r);
   bool operator == (Value_Fixed const &l, Value_Fixed const &r);
   bool operator == (Value_Float const &l, Value_Float const &r);
   bool operator == (Value_Point const &l, Value_Point const &r);
   bool operator != (Value       const &l, Value       const &r);
   bool operator != (Value_Fixed const &l, Value_Fixed const &r);
   bool operator != (Value_Float const &l, Value_Float const &r);
   bool operator != (Value_Point const &l, Value_Point const &r);

   Value         operator & (Value       const &l, Value       const &r);
   Value_Fixed   operator & (Value_Fixed const &l, Value_Fixed const &r);
   Value_Fixed &&operator & (Value_Fixed      &&l, Value_Fixed const &r);

   Value         operator ^ (Value       const &l, Value       const &r);
   Value_Fixed   operator ^ (Value_Fixed const &l, Value_Fixed const &r);
   Value_Fixed &&operator ^ (Value_Fixed      &&l, Value_Fixed const &r);

   Value         operator | (Value       const &l, Value       const &r);
   Value_Fixed   operator | (Value_Fixed const &l, Value_Fixed const &r);
   Value_Fixed &&operator | (Value_Fixed      &&l, Value_Fixed const &r);

   Value       &operator *=  (Value       &l, Value       const &r);
   Value_Fixed &operator *=  (Value_Fixed &l, Value_Fixed const &r);
   Value_Float &operator *=  (Value_Float &l, Value_Float const &r);
   Value       &operator /=  (Value       &l, Value       const &r);
   Value_Fixed &operator /=  (Value_Fixed &l, Value_Fixed const &r);
   Value_Float &operator /=  (Value_Float &l, Value_Float const &r);
   Value       &operator %=  (Value       &l, Value       const &r);
   Value_Fixed &operator %=  (Value_Fixed &l, Value_Fixed const &r);
   Value_Float &operator %=  (Value_Float &l, Value_Float const &r);
   Value       &operator +=  (Value       &l, Value       const &r);
   Value_Fixed &operator +=  (Value_Fixed &l, Value_Fixed const &r);
   Value_Float &operator +=  (Value_Float &l, Value_Float const &r);
   Value_Point &operator +=  (Value_Point &l, Value_Fixed const &r);
   Value       &operator -=  (Value       &l, Value       const &r);
   Value_Fixed &operator -=  (Value_Fixed &l, Value_Fixed const &r);
   Value_Float &operator -=  (Value_Float &l, Value_Float const &r);
   Value_Point &operator -=  (Value_Point &l, Value_Fixed const &r);
   Value       &operator <<= (Value       &l, Value       const &r);
   Value_Fixed &operator <<= (Value_Fixed &l, Value_Fixed const &r);
   Value       &operator >>= (Value       &l, Value       const &r);
   Value_Fixed &operator >>= (Value_Fixed &l, Value_Fixed const &r);
   Value       &operator &=  (Value       &l, Value       const &r);
   Value_Fixed &operator &=  (Value_Fixed &l, Value_Fixed const &r);
   Value       &operator ^=  (Value       &l, Value       const &r);
   Value_Fixed &operator ^=  (Value_Fixed &l, Value_Fixed const &r);
   Value       &operator |=  (Value       &l, Value       const &r);
   Value_Fixed &operator |=  (Value_Fixed &l, Value_Fixed const &r);

   #define GDCC_IR_TypeList(name) \
      OArchive &operator << (OArchive &out, Value_##name const &in);
   #include "TypeList.hpp"

   OArchive &operator << (OArchive &out, Value const &in);
   OArchive &operator << (OArchive &out, ValueBase    in);

   #define GDCC_IR_TypeList(name) \
      IArchive &operator >> (IArchive &in, Value_##name &out);
   #include "TypeList.hpp"

   IArchive &operator >> (IArchive &in, Value     &out);
   IArchive &operator >> (IArchive &in, ValueBase &out);
}

#endif//GDCC__IR__Value_H__

