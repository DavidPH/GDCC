//-----------------------------------------------------------------------------
//
// Copyright (C) 2013-2014 David Hill
//
// See COPYING for license information.
//
//-----------------------------------------------------------------------------
//
// Abstract Syntax Tree expression type handling.
//
//
// C/C++ type classification.
//
// Where semantic differences between languages exist, the line is marked by
// that language, indicating that the type has the properties of that
// relative position in the chart only for that language. For example, enum
// is an integer type in C, but merely a scalar type in C++.
//
//   function types
//   +-member function types
//   +-static function types
//   object types
//   +-accum types
//   | +-signed accum types
//   | | +-short _Accum
//   | | +-_Accum
//   | | +-long _Accum
//   | | +-long long _Accum
//   | | +-_Sat short _Accum
//   | | +-_Sat _Accum
//   | | +-_Sat long _Accum
//   | | +-_Sat long long _Accum
//   | +-unsigned accum types
//   |   +-unsigned short _Accum
//   |   +-unsigned _Accum
//   |   +-unsigned long _Accum
//   |   +-unsigned long long _Accum
//   |   +-_Sat unsigned short _Accum
//   |   +-_Sat unsigned _Accum
//   |   +-_Sat unsigned long _Accum
//   |   +-_Sat unsigned long long _Accum
//   +-aggregate types
//   | +-array types
//   | +-structure types
//   | +-tuple types
//   +-character types
//   | +-char
//   | +-signed char
//   | +-unsigned char
//   +-fract types
//   | +-signed fract types
//   | | +-short _Fract
//   | | +-_Fract
//   | | +-long _Fract
//   | | +-long long _Fract
//   | | +-_Sat short _Fract
//   | | +-_Sat _Fract
//   | | +-_Sat long _Fract
//   | | +-_Sat long long _Fract
//   | +-unsigned fract types
//   |   +-unsigned short _Fract
//   |   +-unsigned _Fract
//   |   +-unsigned long _Fract
//   |   +-unsigned long long _Fract
//   |   +-_Sat unsigned short _Fract
//   |   +-_Sat unsigned _Fract
//   |   +-_Sat unsigned long _Fract
//   |   +-_Sat unsigned long long _Fract
//   +-real types
//   | +-fixed-point types
//   | +-integer types
//   | +-real floating types
//   +-scalar types
//   | +-arithmetic types
//   | | +-fixed-point types
//   | | | +-primary fixed-point types
//   | | | | +-primary signed fixed-point types
//   | | | | | +-short _Accum
//   | | | | | +-_Accum
//   | | | | | +-long _Accum
//   | | | | | +-long long _Accum
//   | | | | | +-short _Fract
//   | | | | | +-_Fract
//   | | | | | +-long _Fract
//   | | | | | +-long long _Fract
//   | | | | +-primary unsigned fixed-point types
//   | | | |   +-unsigned short _Accum
//   | | | |   +-unsigned _Accum
//   | | | |   +-unsigned long _Accum
//   | | | |   +-unsigned long long _Accum
//   | | | |   +-unsigned short _Fract
//   | | | |   +-unsigned _Fract
//   | | | |   +-unsigned long _Fract
//   | | | |   +-unsigned long long _Fract
//   | | | +-saturating fixed-point types
//   | | |   +-saturating signed fixed-point types
//   | | |   | +-_Sat short _Accum
//   | | |   | +-_Sat _Accum
//   | | |   | +-_Sat long _Accum
//   | | |   | +-_Sat long long _Accum
//   | | |   | +-_Sat short _Fract
//   | | |   | +-_Sat _Fract
//   | | |   | +-_Sat long _Fract
//   | | |   | +-_Sat long long _Fract
//   | | |   +-saturating unsigned fixed-point types
//   | | |     +-_Sat unsigned short _Accum
//   | | |     +-_Sat unsigned _Accum
//   | | |     +-_Sat unsigned long _Accum
//   | | |     +-_Sat unsigned long long _Accum
//   | | |     +-_Sat unsigned short _Fract
//   | | |     +-_Sat unsigned _Fract
//   | | |     +-_Sat unsigned long _Fract
//   | | |     +-_Sat unsigned long long _Fract
//   | | +-floating types
//   | | | +-complex types
//   | | | | +-short float _Complex
//   | | | | +-float _Complex
//   | | | | +-double _Complex
//   | | | | +-long double _Complex
//   | | | +-imaginary types
//   | | | | +-short float _Imaginary
//   | | | | +-float _Imaginary
//   | | | | +-double _Imaginary
//   | | | | +-long double _Imaginary
//   | | | +-real floating types
//   | | |   +-short float
//   | | |   +-float
//   | | |   +-double
//   | | |   +-long double
//   | | +-integer types
//   | |   +-bool (C++)
//   | |   +-char
//   | |   +-char16_t (C++)
//   | |   +-char32_t (C++)
//   | |   +-enumerated types (C)
//   | |   +-signed integer types
//   | |   | +-extended signed integer types
//   | |   | +-standard signed integer types
//   | |   |   +-signed char
//   | |   |   +-short int
//   | |   |   +-int
//   | |   |   +-long int
//   | |   |   +-long long int
//   | |   +-unsigned integer types
//   | |   | +-extended unsigned integer types
//   | |   | +-standard unsigned integer types
//   | |   |   +-unsigned char
//   | |   |   +-unsigned short int
//   | |   |   +-unsigned int
//   | |   |   +-unsigned long int
//   | |   |   +-unsigned long long int
//   | |   |   +-_Bool (C)
//   | |   +-wchar_t (C++)
//   | +-enumerated types (C++)
//   | | +-scoped enumeration types
//   | | +-unscoped enumeration types
//   | +-pointer types
//   | +-pointer to member types
//   | +-std::nullptr_t
//   | +-__fastbool
//   | +-__str_ent
//   +-extended integer types
//   | +-extended signed integer types
//   | +-extended unsigned integer types
//   +-standard integer types
//   | +-standard signed integer types
//   | +-standard unsigned integer types
//   +-union types
//   +-void
//   reference types
//   +-lvalue reference types
//   +-rvalue reference types
//   __label
//
//-----------------------------------------------------------------------------

#include "AST/Type.hpp"

#include "AST/Exp.hpp"

#include "Core/Array.hpp"

#include "IR/Type.hpp"


//----------------------------------------------------------------------------|
// Global Variables                                                           |
//

namespace GDCC
{
   namespace AST
   {
      Type::CRef const Type::None{GetNone()};
      Type::CRef const Type::Size{GetSize()};

      TypeSet::Ref const TypeSet::Head{new TypeSet(false)};
      TypeSet::Ref const TypeSet::HeadV{new TypeSet(true)};
   }
}


//----------------------------------------------------------------------------|
// Global Functions                                                           |
//

namespace GDCC
{
   namespace AST
   {
      //
      // Type constructor
      //
      Type::Type() : qualNone{this}, qualNext{this}, qualPrev{this},
         arrType{nullptr}, arrType0{nullptr}, avmType{nullptr},
         avmType0{nullptr}, lvrType{nullptr}, ptrType{nullptr},
         rvrType{nullptr}
      {
         // We hold a reference to this, and so must not count that reference.
         // During destruction, the reference count will be decremented one last
         // time. Because the count will already be at 0, the decrement result
         // will not be 0.
         --refCount;
      }

      //
      // Type copy constructor
      //
      Type::Type(Type const &type) : Super{type}, qualNone{type.qualNone},
         qualNext{type.qualNext}, qualPrev{qualNext->qualPrev},
         arrType{nullptr}, arrType0{nullptr}, avmType{nullptr},
         avmType0{nullptr}, lvrType{nullptr}, ptrType{nullptr},
         rvrType{nullptr}
      {
         qualNext->qualPrev = this;
         qualPrev->qualNext = this;
      }

      //
      // Type destructor
      //
      Type::~Type()
      {
         qualNext->qualPrev = qualPrev;
         qualPrev->qualNext = qualNext;
      }

      //
      // Type::getTypeQual
      //
      Type::CRef Type::getTypeQual(TypeQual newQuals) const
      {
         if(quals == newQuals) return static_cast<CRef>(this);

         for(auto type = qualNext; type != this; type = type->qualNext)
            if(type->quals == newQuals) return static_cast<CRef>(type);

         auto type = clone();
         type->quals = newQuals;
         return type;
      }

      //
      // Type::getTypeQualAddr
      //
      Type::CRef Type::getTypeQualAddr(IR::AddrSpace addr) const
      {
         auto newQuals = quals;
         newQuals.space = addr;
         return Type::getTypeQual(newQuals);
      }

      //
      // Type::getIRType
      //
      IR::Type Type::getIRType() const
      {
         throw TypeError();
      }

      //
      // Type::getSizeBytesVM
      //
      Exp::CRef Type::getSizeBytesVM() const
      {
         return ExpCreate_Size(getSizeBytes());
      }

      //
      // Type::getSizePointVM
      //
      Exp::CRef Type::getSizePointVM() const
      {
         return ExpCreate_Size(getSizePoint());
      }

      //
      // Type::getSizeWordsVM
      //
      Exp::CRef Type::getSizeWordsVM() const
      {
         return ExpCreate_Size(getSizeWords());
      }

      //
      // TypeSet constructor
      //
      TypeSet::TypeSet(bool varia_) : next{this}, prev{this}, tbeg{nullptr},
         tend{nullptr}, varia{varia_}
      {
      }

      //
      // TypeSet constructor
      //
      TypeSet::TypeSet(TypeSet *head, Type::CRef const *tbeg_,
         Type::CRef const *tend_, bool varia_) : next{head}, prev{head->prev},
         tbeg{tbeg_}, tend{tend_}, varia{varia_}
      {
         next->prev = this;
         prev->next = this;
      }

      //
      // TypeSet destructor
      //
      TypeSet::~TypeSet()
      {
         next->prev = prev;
         prev->next = next;

         Core::Array<Type::CRef>::Del(tbeg, tend);
      }

      //
      // TypeSet::Get
      //
      TypeSet::CRef TypeSet::Get(bool varia)
      {
         return varia ? HeadV : Head;
      }

      //
      // TypeSet::Get
      //
      TypeSet::CRef TypeSet::Get(Type::CRef const *typev, std::size_t typec, bool varia)
      {
         TypeSet *head = varia ? HeadV : Head;
         if(!typec) return static_cast<CRef>(head);

         for(auto set = head->next; set != head; set = set->next)
         {
            if(set->size() == typec && std::equal(set->begin(), set->end(), typev))
               return static_cast<CRef>(set);
         }

         auto tbeg = Core::Array<Type::CRef>::Cpy(typev, typev + typec);
         return static_cast<CRef>(new TypeSet(head, tbeg, tbeg + typec, varia));
      }
   }
}

// EOF

