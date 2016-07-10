//-----------------------------------------------------------------------------
//
// Copyright (C) 2013-2016 David Hill
//
// See COPYING for license information.
//
//-----------------------------------------------------------------------------
//
// C types.
//
//
// C/C++ type classification.
//
// Where semantic differences between languages exist, the line is marked by
// that language, indicating that the type has the properties of that
// relative position in the chart only for that language. For example, enum
// is an integer type in C, but merely a scalar type in C++.
//
// This chart includes extended types, except for alternative spellings like
// __fixed and __str. (__str is an alternative spelling for __str_ent*.)
//
//   function types
//   +-member function types (C++)
//   +-static function types
//   object types
//   +-accum types
//   | +-signed accum types
//   | | +-short _Accum
//   | | +-_Accum
//   | | +-long _Accum
//   | | +-_Sat short _Accum
//   | | +-_Sat _Accum
//   | | +-_Sat long _Accum
//   | +-unsigned accum types
//   |   +-unsigned short _Accum
//   |   +-unsigned _Accum
//   |   +-unsigned long _Accum
//   |   +-_Sat unsigned short _Accum
//   |   +-_Sat unsigned _Accum
//   |   +-_Sat unsigned long _Accum
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
//   | | +-_Sat short _Fract
//   | | +-_Sat _Fract
//   | | +-_Sat long _Fract
//   | +-unsigned fract types
//   |   +-unsigned short _Fract
//   |   +-unsigned _Fract
//   |   +-unsigned long _Fract
//   |   +-_Sat unsigned short _Fract
//   |   +-_Sat unsigned _Fract
//   |   +-_Sat unsigned long _Fract
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
//   | | | | | +-short _Fract
//   | | | | | +-_Fract
//   | | | | | +-long _Fract
//   | | | | +-primary unsigned fixed-point types
//   | | | |   +-unsigned short _Accum
//   | | | |   +-unsigned _Accum
//   | | | |   +-unsigned long _Accum
//   | | | |   +-unsigned short _Fract
//   | | | |   +-unsigned _Fract
//   | | | |   +-unsigned long _Fract
//   | | | +-saturating fixed-point types
//   | | |   +-saturating signed fixed-point types
//   | | |   | +-_Sat short _Accum
//   | | |   | +-_Sat _Accum
//   | | |   | +-_Sat long _Accum
//   | | |   | +-_Sat short _Fract
//   | | |   | +-_Sat _Fract
//   | | |   | +-_Sat long _Fract
//   | | |   +-saturating unsigned fixed-point types
//   | | |     +-_Sat unsigned short _Accum
//   | | |     +-_Sat unsigned _Accum
//   | | |     +-_Sat unsigned long _Accum
//   | | |     +-_Sat unsigned short _Fract
//   | | |     +-_Sat unsigned _Fract
//   | | |     +-_Sat unsigned long _Fract
//   | | +-floating types
//   | | | +-complex types
//   | | | | +-float _Complex
//   | | | | +-double _Complex
//   | | | | +-long double _Complex
//   | | | +-imaginary types
//   | | | | +-float _Imaginary
//   | | | | +-double _Imaginary
//   | | | | +-long double _Imaginary
//   | | | +-real floating types
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
//   | +-pointer to member types (C++)
//   | +-std::nullptr_t (C++)
//   | +-__fastbool
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
//   __str_ent
//
//-----------------------------------------------------------------------------

#ifndef GDCC__CC__Type_H__
#define GDCC__CC__Type_H__

#include "../Core/Counter.hpp"


//----------------------------------------------------------------------------|
// Macros                                                                     |
//

#define GDCC_CC_Type_ImplGet(type, class, ...) \
   SR::Type::CRef GetType##type() \
      {static SR::Type::CRef t{new Type_##class(__VA_ARGS__)}; return t;}


//----------------------------------------------------------------------------|
// Types                                                                      |
//

namespace GDCC
{
   namespace SR
   {
      class Type;
   }
}


//----------------------------------------------------------------------------|
// Global Variables                                                           |
//

namespace GDCC
{
   namespace CC
   {
      extern Core::CounterRef<SR::Type const> TypeBool;
      extern Core::CounterRef<SR::Type const> TypeBoolSoft;

      extern Core::CounterRef<SR::Type const> TypeChar;

      extern Core::CounterRef<SR::Type const> TypeFixedPrS;
      extern Core::CounterRef<SR::Type const> TypeFixedPrSH;
      extern Core::CounterRef<SR::Type const> TypeFixedPrSL;
      extern Core::CounterRef<SR::Type const> TypeFixedPrU;
      extern Core::CounterRef<SR::Type const> TypeFixedPrUH;
      extern Core::CounterRef<SR::Type const> TypeFixedPrUL;

      extern Core::CounterRef<SR::Type const> TypeFixedSaS;
      extern Core::CounterRef<SR::Type const> TypeFixedSaSH;
      extern Core::CounterRef<SR::Type const> TypeFixedSaSL;
      extern Core::CounterRef<SR::Type const> TypeFixedSaU;
      extern Core::CounterRef<SR::Type const> TypeFixedSaUH;
      extern Core::CounterRef<SR::Type const> TypeFixedSaUL;

      extern Core::CounterRef<SR::Type const> TypeFloatCS;
      extern Core::CounterRef<SR::Type const> TypeFloatCSL;
      extern Core::CounterRef<SR::Type const> TypeFloatCSLL;

      extern Core::CounterRef<SR::Type const> TypeFloatIS;
      extern Core::CounterRef<SR::Type const> TypeFloatISL;
      extern Core::CounterRef<SR::Type const> TypeFloatISLL;

      extern Core::CounterRef<SR::Type const> TypeFloatRS;
      extern Core::CounterRef<SR::Type const> TypeFloatRSL;
      extern Core::CounterRef<SR::Type const> TypeFloatRSLL;

      extern Core::CounterRef<SR::Type const> TypeFractPrS;
      extern Core::CounterRef<SR::Type const> TypeFractPrSH;
      extern Core::CounterRef<SR::Type const> TypeFractPrSL;
      extern Core::CounterRef<SR::Type const> TypeFractPrU;
      extern Core::CounterRef<SR::Type const> TypeFractPrUH;
      extern Core::CounterRef<SR::Type const> TypeFractPrUL;

      extern Core::CounterRef<SR::Type const> TypeFractSaS;
      extern Core::CounterRef<SR::Type const> TypeFractSaSH;
      extern Core::CounterRef<SR::Type const> TypeFractSaSL;
      extern Core::CounterRef<SR::Type const> TypeFractSaU;
      extern Core::CounterRef<SR::Type const> TypeFractSaUH;
      extern Core::CounterRef<SR::Type const> TypeFractSaUL;

      extern Core::CounterRef<SR::Type const> TypeIntegPrS;
      extern Core::CounterRef<SR::Type const> TypeIntegPrSH;
      extern Core::CounterRef<SR::Type const> TypeIntegPrSHH;
      extern Core::CounterRef<SR::Type const> TypeIntegPrSL;
      extern Core::CounterRef<SR::Type const> TypeIntegPrSLL;
      extern Core::CounterRef<SR::Type const> TypeIntegPrU;
      extern Core::CounterRef<SR::Type const> TypeIntegPrUH;
      extern Core::CounterRef<SR::Type const> TypeIntegPrUHH;
      extern Core::CounterRef<SR::Type const> TypeIntegPrUL;
      extern Core::CounterRef<SR::Type const> TypeIntegPrULL;
   }
}


//----------------------------------------------------------------------------|
// Global Functions                                                           |
//

namespace GDCC
{
   namespace CC
   {
      Core::CounterRef<SR::Type const> GetTypeBool();
      Core::CounterRef<SR::Type const> GetTypeBoolSoft();

      Core::CounterRef<SR::Type const> GetTypeChar();

      Core::CounterRef<SR::Type const> GetTypeFixedPrS();
      Core::CounterRef<SR::Type const> GetTypeFixedPrSH();
      Core::CounterRef<SR::Type const> GetTypeFixedPrSL();
      Core::CounterRef<SR::Type const> GetTypeFixedPrU();
      Core::CounterRef<SR::Type const> GetTypeFixedPrUH();
      Core::CounterRef<SR::Type const> GetTypeFixedPrUL();

      Core::CounterRef<SR::Type const> GetTypeFixedSaS();
      Core::CounterRef<SR::Type const> GetTypeFixedSaSH();
      Core::CounterRef<SR::Type const> GetTypeFixedSaSL();
      Core::CounterRef<SR::Type const> GetTypeFixedSaU();
      Core::CounterRef<SR::Type const> GetTypeFixedSaUH();
      Core::CounterRef<SR::Type const> GetTypeFixedSaUL();

      Core::CounterRef<SR::Type const> GetTypeFloatCS();
      Core::CounterRef<SR::Type const> GetTypeFloatCSL();
      Core::CounterRef<SR::Type const> GetTypeFloatCSLL();

      Core::CounterRef<SR::Type const> GetTypeFloatIS();
      Core::CounterRef<SR::Type const> GetTypeFloatISL();
      Core::CounterRef<SR::Type const> GetTypeFloatISLL();

      Core::CounterRef<SR::Type const> GetTypeFloatRS();
      Core::CounterRef<SR::Type const> GetTypeFloatRSL();
      Core::CounterRef<SR::Type const> GetTypeFloatRSLL();

      Core::CounterRef<SR::Type const> GetTypeFractPrS();
      Core::CounterRef<SR::Type const> GetTypeFractPrSH();
      Core::CounterRef<SR::Type const> GetTypeFractPrSL();
      Core::CounterRef<SR::Type const> GetTypeFractPrU();
      Core::CounterRef<SR::Type const> GetTypeFractPrUH();
      Core::CounterRef<SR::Type const> GetTypeFractPrUL();

      Core::CounterRef<SR::Type const> GetTypeFractSaS();
      Core::CounterRef<SR::Type const> GetTypeFractSaSH();
      Core::CounterRef<SR::Type const> GetTypeFractSaSL();
      Core::CounterRef<SR::Type const> GetTypeFractSaU();
      Core::CounterRef<SR::Type const> GetTypeFractSaUH();
      Core::CounterRef<SR::Type const> GetTypeFractSaUL();

      Core::CounterRef<SR::Type const> GetTypeIntegPrS();
      Core::CounterRef<SR::Type const> GetTypeIntegPrSH();
      Core::CounterRef<SR::Type const> GetTypeIntegPrSHH();
      Core::CounterRef<SR::Type const> GetTypeIntegPrSL();
      Core::CounterRef<SR::Type const> GetTypeIntegPrSLL();
      Core::CounterRef<SR::Type const> GetTypeIntegPrU();
      Core::CounterRef<SR::Type const> GetTypeIntegPrUH();
      Core::CounterRef<SR::Type const> GetTypeIntegPrUHH();
      Core::CounterRef<SR::Type const> GetTypeIntegPrUL();
      Core::CounterRef<SR::Type const> GetTypeIntegPrULL();
   }
}

#endif//GDCC__CC__Type_H__

