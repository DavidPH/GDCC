//-----------------------------------------------------------------------------
//
// Copyright (C) 2013 David Hill
//
// See COPYING for license information.
//
//-----------------------------------------------------------------------------
//
// Master strings table.
//
//-----------------------------------------------------------------------------

#ifndef GDCC__String_H__
#define GDCC__String_H__

#include <cstddef>
#include <ostream>


//----------------------------------------------------------------------------|
// Types                                                                      |
//

namespace GDCC
{
   class StringData;

   //
   // StringIndex
   //
   enum StringIndex
   {
      STRNULL,
      #define GDCC_StringList(name, str) STR_##name,
      #include "StringList.hpp"
      STRMAX
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

      explicit constexpr operator bool () {return num != STRNULL;}
      explicit constexpr operator std::size_t () {return num;}
      explicit constexpr operator StringIndex ()
         {return num < STRMAX ? static_cast<StringIndex>(num) : STRNULL;}

      constexpr bool operator == (StringIndex num_) {return num == num_;}
      constexpr bool operator != (StringIndex num_) {return num != num_;}

      String &operator = (StringIndex num_) {num = num_; return *this;}

      StringData const &getData() const {return GetData(num);}

   private:
      std::size_t num;


      static StringData const &GetData(std::size_t num);
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

      char const *str;
      std::size_t len;
      std::size_t hash;
      std::size_t num;
      std::size_t next;
   };
}


//----------------------------------------------------------------------------|
// Global Functions                                                           |
//

namespace GDCC
{
   std::ostream &operator << (std::ostream &out, String in);

   constexpr bool operator == (StringIndex l, String const &r) {return r == l;}
   constexpr bool operator != (StringIndex l, String const &r) {return r != l;}

   String AddString(char const *str);
   String AddString(char const *str, std::size_t len, std::size_t hash);

   String FindString(char const *str);
   String FindString(char const *str, std::size_t len, std::size_t hash);

   std::size_t HashString(char const *str);
   std::size_t HashString(char const *str, std::size_t len, std::size_t hash = 0);

   void LenHashString(char const *str, std::size_t &len, std::size_t &hash);
}

#endif//GDCC__String_H__

