//-----------------------------------------------------------------------------
//
// Copyright (C) 2013-2018 David Hill
//
// See COPYING for license information.
//
//-----------------------------------------------------------------------------
//
// Master strings table and string utilities.
//
//-----------------------------------------------------------------------------

#ifndef GDCC__Core__String_H__
#define GDCC__Core__String_H__

#include "../Core/Types.hpp"

#include "../Option/StrUtil.hpp"

#include <ostream>


//----------------------------------------------------------------------------|
// Types                                                                      |
//

namespace GDCC::Core
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
      explicit StringData(StringIndex idx);

      template<std::size_t Len>
      StringData(char const (&str_)[Len], StringIndex idx_) :
         StringData{str_, Len - 1, Option::StrHash(str_, Len - 1),
            static_cast<std::size_t>(idx_)} {}

      StringData(char const *str, std::size_t len, std::size_t hash, std::size_t idx);

      char const &operator [] (std::size_t i) const {return str[i];}

      char const &back() const {return str[len - 1];}

      char const *begin() const {return str;}

      char const *data() const {return str;}

      bool empty() const {return !len;}

      char const *end() const {return str + len;}

      char const &front() const {return *str;}

      std::size_t getHash() const {return hash;}

      std::size_t getNext() const {return next;}

      std::size_t size() const {return len;}
      std::size_t size0() const {return len0;}
      std::size_t size16() const;
      std::size_t size32() const;


      friend class String;

      static std::size_t GetFirst(std::size_t hash);

   private:
      bool isLower() const;

      char const *const str;
      std::size_t const len;
      std::size_t const len0;
      std::size_t const hash;
      std::size_t const next;

      mutable std::size_t idxLower;

      mutable std::size_t len16;
      mutable std::size_t len32;
   };

   //
   // String
   //
   class String
   {
   public:
      String() = default;
      String(char const *str) : idx{Get(str).idx} {}
      String(char const *str, std::size_t len) : idx{Get(str, len).idx} {}
      String(char const *str, std::size_t len, std::size_t hash) :
         idx{Get(str, len, hash).idx} {}
      String(char const *first, char const *last) :
         idx{Get(first, last - first).idx} {}
      constexpr String(std::nullptr_t) : idx{STRNULL} {}
      explicit constexpr String(std::size_t idx_) : idx{idx_} {}
      constexpr String(StringIndex idx_) : idx{static_cast<std::size_t>(idx_)} {}

      explicit constexpr operator bool () const {return idx != STRNULL;}
      explicit constexpr operator std::size_t () const {return idx;}
      constexpr operator StringIndex () const
         {return idx < STRMAX ? static_cast<StringIndex>(idx) : STRNULL;}

      char const &operator [] (std::size_t i) const {return DataV[idx][i];}

      String &operator = (StringIndex idx_) {idx = idx_; return *this;}

      char const &back() const {return DataV[idx].back();}

      char const *begin() const {return DataV[idx].begin();}

      char const *data() const {return DataV[idx].data();}

      bool empty() const {return DataV[idx].empty();}

      char const *end() const {return DataV[idx].end();}

      char const &front() const {return DataV[idx].front();}

      std::size_t getHash() const {return DataV[idx].getHash();}

      String getLower() const;

      std::size_t size() const {return DataV[idx].size();}
      std::size_t size0() const {return DataV[idx].size0();}
      std::size_t size16() const {return DataV[idx].size16();}
      std::size_t size32() const {return DataV[idx].size32();}


      // String must not already exist in table.
      static String Add(char const *str, std::size_t len, std::size_t hash);
      static String Add(std::unique_ptr<char[]> &&str, std::size_t len,
         std::size_t hash);

      static String Find(char const *str);
      static String Find(char const *str, std::size_t len);
      static String Find(char const *str, std::size_t len, std::size_t hash);

      static String Get(char const *str);
      static String Get(char const *str, std::size_t len);
      static String Get(char const *str, std::size_t len, std::size_t hash);

      static std::size_t       GetDataC() {return DataC;}
      static StringData const *GetDataV() {return DataV;}

   private:
      std::size_t idx;


      static std::size_t       DataC;
      static StringData const *DataV;
   };
}

namespace std
{
   //
   // hash<::GDCC::Core::String>
   //
   template<> struct hash<::GDCC::Core::String>
   {
      size_t operator () (::GDCC::Core::String str) const
         {return str.getHash();}
   };
}


//----------------------------------------------------------------------------|
// Extern Functions                                                           |
//

namespace GDCC::Core
{
   using Option::StrCmp;
   using Option::StrDup;
   using Option::StrHash;
   using Option::StrLenHash;

   String operator + (String      l, char const *r);
   String operator + (String      l, String      r);
   String operator + (String      l, StringIndex r);
   String operator + (StringIndex l, String      r);

   std::ostream &operator << (std::ostream &out, String in);

   bool operator < (String      l, String      r);
   bool operator < (String      l, StringIndex r);
   bool operator < (StringIndex l, String      r);

   bool operator > (String      l, String      r);
   bool operator > (String      l, StringIndex r);
   bool operator > (StringIndex l, String      r);

   bool operator <= (String      l, String      r);
   bool operator <= (String      l, StringIndex r);
   bool operator <= (StringIndex l, String      r);

   bool operator >= (String      l, String      r);
   bool operator >= (String      l, StringIndex r);
   bool operator >= (StringIndex l, String      r);

   constexpr bool operator == (String      l, String      r);
   constexpr bool operator == (String      l, StringIndex r);
   constexpr bool operator == (StringIndex l, String      r);

   constexpr bool operator != (String      l, String      r);
   constexpr bool operator != (String      l, StringIndex r);
   constexpr bool operator != (StringIndex l, String      r);

   String &operator += (String &l, char const *r);
   String &operator += (String &l, String      r);
   String &operator += (String &l, StringIndex r);

   std::pair<char32_t, char const *> Str8To32(char const *itr, char const *end);

   inline String operator + (String l, StringIndex r) {return l + String(r);}
   inline String operator + (StringIndex l, String r) {return String(l) + r;}

   inline bool operator < (String l, StringIndex r) {return l < String(r);}
   inline bool operator < (StringIndex l, String r) {return String(l) < r;}

   inline bool operator > (String l, String r) {return r < l;}
   inline bool operator > (String l, StringIndex r) {return l > String(r);}
   inline bool operator > (StringIndex l, String r) {return String(l) > r;}

   inline bool operator <= (String l, String r) {return l == r || l < r;}
   inline bool operator <= (String l, StringIndex r) {return l <= String(r);}
   inline bool operator <= (StringIndex l, String r) {return String(l) <= r;}

   inline bool operator >= (String l, String r) {return l == r || r < l;}
   inline bool operator >= (String l, StringIndex r) {return l >= String(r);}
   inline bool operator >= (StringIndex l, String r) {return String(l) >= r;}

   constexpr bool operator == (String l, String r)
      {return static_cast<std::size_t>(l) == static_cast<std::size_t>(r);}
   constexpr bool operator == (String l, StringIndex r)
      {return static_cast<std::size_t>(l) == static_cast<std::size_t>(r);}
   constexpr bool operator == (StringIndex l, String r)
      {return static_cast<std::size_t>(l) == static_cast<std::size_t>(r);}

   constexpr bool operator != (String l, String r)
      {return static_cast<std::size_t>(l) != static_cast<std::size_t>(r);}
   constexpr bool operator != (String l, StringIndex r)
      {return static_cast<std::size_t>(l) != static_cast<std::size_t>(r);}
   constexpr bool operator != (StringIndex l, String r)
      {return static_cast<std::size_t>(l) != static_cast<std::size_t>(r);}

   inline String &operator += (String &l, char const *r) {return l = l + r;}
   inline String &operator += (String &l, String      r) {return l = l + r;}
   inline String &operator += (String &l, StringIndex r) {return l = l + r;}
}

#endif//GDCC__Core__String_H__

