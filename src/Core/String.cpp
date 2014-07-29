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

#include "Core/String.hpp"

#include <cstring>
#include <iostream>
#include <memory>
#include <vector>


//----------------------------------------------------------------------------|
// Static Variables                                                           |
//

static std::vector<std::unique_ptr<char[]>> StringTableAlloc;

static constexpr std::size_t StringTableHashC = 256;
static std::size_t StringTableHash[StringTableHashC];

static std::vector<GDCC::Core::StringData> StringTable =
{
   GDCC::Core::StringData(GDCC::Core::STRNULL),
   GDCC::Core::StringData("__VA_ARGS__", GDCC::Core::STR___VA_ARGS__),
   #define GDCC_Core_StringList(name, str) \
      GDCC::Core::StringData(str, GDCC::Core::STR_##name),
   #include "Core/StringList.hpp"
};


//----------------------------------------------------------------------------|
// Global Variables                                                           |
//

namespace GDCC
{
   namespace Core
   {
      std::size_t       String::DataC = StringTable.size();
      StringData const *String::DataV = StringTable.data();
   }
}


//----------------------------------------------------------------------------|
// Global Functions                                                           |
//

namespace GDCC
{
   namespace Core
   {
      //
      // StringData constructor
      //
      StringData::StringData(StringIndex num_) : str{""}, len{0}, hash{0},
         num{num_}, next{0}
      {
      }

      //
      // StringData constructor
      //
      StringData::StringData(char const *str_, StringIndex num_) :
         str  {str_},
         len  {0},
         hash {0},
         num  {num_},
         next {0},
         len16{0},
         len32{0}
      {
         LenHashString(str, len, hash);

         auto hashMod = hash % StringTableHashC;

         next = StringTableHash[hashMod];
         StringTableHash[hashMod] = num;
      }

      //
      // StringData constructor
      //
      StringData::StringData(char const *str_, std::size_t len_,
         std::size_t hash_, std::size_t num_) :
         str  {str_},
         len  {len_},
         hash {hash_},
         num  {num_},
         next {0},
         len16{0},
         len32{0}
      {
         auto hashMod = hash % StringTableHashC;

         next = StringTableHash[hashMod];
         StringTableHash[hashMod] = num;
      }

      //
      // StringData::size16
      //
      std::size_t StringData::size16() const
      {
         // Compute length, if needed.
         if(!len16 && len)
         {
            for(auto itr = str, e = itr + len; itr != e;)
            {
               char32_t c;
               std::tie(c, itr) = Str8To32(itr, e);
               len16 += c > 0xFFFF ? 2 : 1;
            }
         }

         return len16;
      }

      //
      // StringData::size32
      //
      std::size_t StringData::size32() const
      {
         // Compute length, if needed.
         if(!len32 && len)
         {
            for(auto itr = str, e = itr + len; itr != e; ++len32)
               std::tie(std::ignore, itr) = Str8To32(itr, e);
         }

         return len32;
      }

      //
      // operator std::ostream << String
      //
      std::ostream &operator << (std::ostream &out, String in)
      {
         auto const &data = in.getData();
         return out.write(data.str, data.len);
      }

      //
      // operator String + String
      //
      String operator + (String l, String r)
      {
         auto const &ld = l.getData();
         auto const &rd = r.getData();

         std::size_t len  = ld.len + rd.len;
         std::size_t hash = HashString(rd.str, rd.len, ld.hash);

         for(auto num = StringTableHash[hash % StringTableHashC]; num;)
         {
            auto const &entry = StringTable[num];

            if(entry.hash == hash && entry.len == len &&
               !std::memcmp(entry.str,          ld.str, ld.len) &&
               !std::memcmp(entry.str + ld.len, rd.str, rd.len))
               return String(num);

            num = entry.next;
         }

         char *newstr;
         std::size_t num = StringTable.size();

         newstr = new char[len + 1];
         std::memcpy(newstr,          ld.str, ld.len);
         std::memcpy(newstr + ld.len, rd.str, rd.len);
         newstr[len] = '\0';

         StringTableAlloc.emplace_back(newstr);
         StringTable.emplace_back(newstr, len, hash, num);

         String::DataC = StringTable.size();
         String::DataV = StringTable.data();

         return String(num);
      }

      //
      // AddString
      //
      String AddString(char const *str)
      {
         std::size_t len = 0, hash = 0;
         LenHashString(str, len, hash);
         return AddString(str, len, hash);
      }

      //
      // AddString
      //
      String AddString(char const *str, std::size_t len, std::size_t hash)
      {
         if(auto s = FindString(str, len, hash)) return s;

         char *newstr;
         std::size_t num = StringTable.size();

         newstr = new char[len + 1];
         std::memcpy(newstr, str, len);
         newstr[len] = '\0';

         StringTableAlloc.emplace_back(newstr);
         StringTable.emplace_back(newstr, len, hash, num);

         String::DataC = StringTable.size();
         String::DataV = StringTable.data();

         return String(num);
      }

      //
      // FindString
      //
      String FindString(char const *str)
      {
         std::size_t len = 0, hash = 0;
         LenHashString(str, len, hash);
         return FindString(str, len, hash);
      }

      //
      // FindString
      //
      String FindString(char const *str, std::size_t len, std::size_t hash)
      {
         for(auto num = StringTableHash[hash % StringTableHashC]; num;)
         {
            auto const &entry = StringTable[num];

            if(entry.hash == hash && entry.len == len && !std::memcmp(entry.str, str, len))
               return String(num);

            num = entry.next;
         }

         return STRNULL;
      }

      //
      // HashString
      //
      std::size_t HashString(char const *s)
      {
         std::size_t hash = 0;

         while(*s) hash = (hash << 3) ^ static_cast<unsigned char>(*s++);

         return hash;
      }

      //
      // HashString
      //
      std::size_t HashString(char const *s, std::size_t len, std::size_t hash)
      {
         while(len--) hash = (hash << 3) ^ static_cast<unsigned char>(*s++);

         return hash;
      }

      //
      // LenHashString
      //
      void LenHashString(char const *s, std::size_t &len_, std::size_t &hash_)
      {
         std::size_t len = len_, hash = hash_;

         for(; *s; ++len) hash = (hash << 3) ^ static_cast<unsigned char>(*s++);

         len_ = len; hash_ = hash;
      }

      //
      // Str8To32
      //
      // This function converts UTF-8 to UTF-32. It does not perform validation
      // of the input, but should never result in undefined behavior for any
      // valid combination of pointers.
      //
      std::pair<char32_t, char const *> Str8To32(char const *itr, char const *end)
      {
         char32_t c;

              if(!(*itr & 0x80)) c = *itr++ & 0x7F;
         else if(!(*itr & 0x40)) c = *itr++ & 0x3F; // Not valid.
         else if(!(*itr & 0x20)) c = *itr++ & 0x1F;
         else if(!(*itr & 0x10)) c = *itr++ & 0x0F;
         else if(!(*itr & 0x08)) c = *itr++ & 0x07;
         else if(!(*itr & 0x04)) c = *itr++ & 0x03;
         else if(!(*itr & 0x02)) c = *itr++ & 0x01;
         else if(!(*itr & 0x01)) c = *itr++ & 0x00; // Not valid.
         else                    c = *itr++ & 0x00; // Not valid.

         while(itr != end && (*itr & 0xC0) == 0x80)
            c = (c << 6) | (*itr++ & 0x3F);

         return {c, itr};
      }

      //
      // StrDup
      //
      std::unique_ptr<char[]> StrDup(char const *str, std::size_t len)
      {
         std::unique_ptr<char[]> ptr{new char[len + 1]};
         std::memcpy(ptr.get(), str, len);
         ptr[len] = '\0';
         return ptr;
      }
   }
}

// EOF

