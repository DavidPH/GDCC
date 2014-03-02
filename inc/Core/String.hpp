//-----------------------------------------------------------------------------
//
// Copyright (C) 2013-2014 David Hill
//
// See COPYING for license information.
//
//-----------------------------------------------------------------------------
//
// Master strings table.
//
//-----------------------------------------------------------------------------

#ifndef GDCC__Core__String_H__
#define GDCC__Core__String_H__

#include <cstddef>
#include <functional>
#include <ostream>
#include <memory>


//----------------------------------------------------------------------------|
// Types                                                                      |
//

namespace GDCC
{
   namespace Core
   {
      //
      // StringIndex
      //
      enum StringIndex
      {
         STRNULL,
         STR___VA_ARGS__, // Can't use the macro because __VA_ARGS__ is magic.
         #define GDCC_Core_StringList(name, str) STR_##name,
         #include "StringList.hpp"
         STRMAX
      };

      //
      // StringData
      //
      class StringData
      {
      public:
         explicit StringData(StringIndex num);
         StringData(char const *str, StringIndex num);
         StringData(char const *str, std::size_t len, std::size_t hash, std::size_t num);

         char const &operator [] (std::size_t i) const {return str[i];}

         char const &back() const {return str[len - 1];}

         char const *begin() const {return str;}

         bool empty() const {return !len;}

         char const *end() const {return str + len;}

         char const &front() const {return *str;}

         std::size_t size() const {return len;}

         char const *str;
         std::size_t len;
         std::size_t hash;
         std::size_t num;
         std::size_t next;
      };

      //
      // String
      //
      class String
      {
      public:
         String() = default;
         explicit String(std::size_t num_) : num{num_} {}
         constexpr String(StringIndex num_) : num{num_} {}

         explicit constexpr operator bool () const {return num != STRNULL;}
         explicit constexpr operator std::size_t () const {return num;}
         constexpr operator StringIndex () const
            {return num < STRMAX ? static_cast<StringIndex>(num) : STRNULL;}

         char const &operator [] (std::size_t i) const {return DataV[num].str[i];}

         constexpr bool operator == (String const &str) const {return str.num == num;}
         constexpr bool operator == (StringIndex num_) const {return num == num_;}
         constexpr bool operator != (String const &str) const {return str.num != num;}
         constexpr bool operator != (StringIndex num_) const {return num != num_;}

         String &operator = (StringIndex num_) {num = num_; return *this;}

         char const *begin() const {return DataV[num].str;}

         char const *data() const {return DataV[num].str;}

         char const *end() const {return DataV[num].end();}

         StringData const &getData() const {return DataV[num];}

         std::size_t size() const {return DataV[num].len;}


         static std::size_t       DataC;
         static StringData const *DataV;

      private:
         std::size_t num;
      };
   }
}

namespace std
{
   //
   // hash<::GDCC::Core::String>
   //
   template<> struct hash<::GDCC::Core::String>
   {
      constexpr size_t operator () (::GDCC::Core::String const &str) const
         {return static_cast<size_t>(str);}
   };

   //
   // less<::GDCC::Core::String>
   //
   template<> struct less<::GDCC::Core::String>
   {
      constexpr size_t operator () (::GDCC::Core::String const &l,
         ::GDCC::Core::String const &r) const
         {return static_cast<size_t>(l) < static_cast<size_t>(r);}
   };
}


//----------------------------------------------------------------------------|
// Global Functions                                                           |
//

namespace GDCC
{
   namespace Core
   {
      std::ostream &operator << (std::ostream &out, String in);

      constexpr bool operator == (StringIndex l, String const &r) {return r == l;}
      constexpr bool operator != (StringIndex l, String const &r) {return r != l;}

      String operator + (String      l, String      r);
      String operator + (String      l, StringIndex r);
      String operator + (StringIndex l, String      r);

      inline String &operator += (String &l, String      r) {return l = l + r;}
      inline String &operator += (String &l, StringIndex r) {return l = l + r;}

      String AddString(char const *str);
      String AddString(char const *str, std::size_t len, std::size_t hash);

      String FindString(char const *str);
      String FindString(char const *str, std::size_t len, std::size_t hash);

      std::size_t HashString(char const *str);
      std::size_t HashString(char const *str, std::size_t len, std::size_t hash = 0);

      void LenHashString(char const *str, std::size_t &len, std::size_t &hash);

      std::unique_ptr<char[]> StrDup(char const *str, std::size_t len);

      inline String operator + (String l, StringIndex r) {return l + String(r);}
      inline String operator + (StringIndex l, String r) {return String(l) + r;}
   }
}

#endif//GDCC__Core__String_H__

