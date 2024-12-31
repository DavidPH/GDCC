//-----------------------------------------------------------------------------
//
// Copyright (C) 2013-2024 David Hill
//
// See COPYING for license information.
//
//-----------------------------------------------------------------------------
//
// Master strings table and string utilities.
//
//-----------------------------------------------------------------------------

#include "Core/String.hpp"

#include <cctype>
#include <cstring>
#include <tuple>
#include <vector>


//----------------------------------------------------------------------------|
// Static Prototypes                                                          |
//

namespace GDCC::Core
{
   static std::vector<GDCC::Core::StringData> &StringTable();
}


//----------------------------------------------------------------------------|
// Static Objects                                                             |
//

namespace GDCC::Core
{
   static constexpr std::size_t StringTableHashC = 1024;
   static std::size_t StringTableHash[StringTableHashC];
}


//----------------------------------------------------------------------------|
// Extern Objects                                                             |
//

namespace GDCC::Core
{
   std::size_t       String::DataC = StringTable().size();
   StringData const *String::DataV = StringTable().data();
}


//----------------------------------------------------------------------------|
// Static Functions                                                           |
//

namespace GDCC::Core
{
   //
   // StringTable
   //
   static std::vector<StringData> &StringTable()
   {
      static std::vector<StringData> table
      {
         StringData(STRNULL),
         StringData("__VA_ARGS__", STR___VA_ARGS__),
         #define GDCC_Core_StringList(name, str) \
            StringData(str, STR_##name),
         #include "Core/StringList.hpp"
      };

      return table;
   }

   //
   // StringTableAlloc
   //
   static std::vector<std::unique_ptr<char[]>> &StringTableAlloc()
   {
      static std::vector<std::unique_ptr<char[]>> table;
      return table;
   }
}


//----------------------------------------------------------------------------|
// Extern Functions                                                           |
//

namespace GDCC::Core
{
   //
   // StringData constructor
   //
   StringData::StringData(StringIndex) :
      str     {""},
      len     {0},
      len0    {0},
      hash    {0},
      next    {0},
      idxLower{0},
      len16   {0},
      len32   {0}
   {
   }

   //
   // StringData constructor
   //
   StringData::StringData(char const *str_, std::size_t len_,
      std::size_t hash_, std::size_t idx) :
      str     {str_},
      len     {len_},
      len0    {std::strlen(str_)},
      hash    {hash_},
      next    {StringTableHash[hash % StringTableHashC]},
      idxLower{0},
      len16   {0},
      len32   {0}
   {
      StringTableHash[hash % StringTableHashC] = idx;
   }

   //
   // StringData::isLower
   //
   bool StringData::isLower() const
   {
      // TODO: Unicode support.

      for(char c : *this)
         if(std::isupper(c)) return false;

      return true;
   }

   //
   // StringData::size16
   //
   std::size_t StringData::size16() const
   {
      // Compute length, if needed.
      if(!len16)
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
      if(!len32)
      {
         for(auto itr = str, e = itr + len; itr != e; ++len32)
            std::tie(std::ignore, itr) = Str8To32(itr, e);
      }

      return len32;
   }

   //
   // StringData::GetFirst
   //
   std::size_t StringData::GetFirst(std::size_t hash)
   {
      return StringTableHash[hash % StringTableHashC];
   }

   //
   // String::getLower
   //
   String String::getLower() const
   {
      if(!StringTable()[idx].idxLower)
      {
         StringData &data = StringTable()[idx];

         // Check if string is already lowercase.
         if(data.isLower())
            return String(data.idxLower = idx);

         // TODO: Unicode support.

         // Calculate new length.
         std::size_t len = data.len;

         // Convert case into buffer.
         std::unique_ptr<char[]> str{new char[data.len + 1]};
         char *out = str.get();
         for(char c : data)
            *out++ = std::tolower(c);
         *out = '\0';

         // Calculate new hash.
         std::size_t hash = StrHash(str.get(), data.len);

         // Find or add new string.
         std::size_t idxLower;
         if(auto s = Find(str.get(), len, hash))
            idxLower = s.idx;
         else
            idxLower = Add(std::move(str), len, hash).idx;

         // The call to Add might invalidate data's address, so avoid it.
         StringTable()[idx].idxLower = idxLower;

         // Also set idxLower's idxLower to itself.
         StringTable()[idxLower].idxLower = idxLower;
      }

      return String(StringTable()[idx].idxLower);
   }

   //
   // String::Add
   //
   String String::Add(char const *str, std::size_t len, std::size_t hash)
   {
      std::size_t idx = StringTable().size();

      StringTable().emplace_back(str, len, hash, idx);

      String::DataC = StringTable().size();
      String::DataV = StringTable().data();

      return String(idx);
   }

   //
   // String::Add
   //
   String String::Add(std::unique_ptr<char[]> &&str, std::size_t len,
      std::size_t hash)
   {
      StringTableAlloc().emplace_back(std::move(str));
      return Add(StringTableAlloc().back().get(), len, hash);
   }

   //
   // String::Find
   //
   String String::Find(char const *str)
   {
      if(!str) return STRNULL;

      auto lh = StrLenHash(str);
      return Find(str, lh.first, lh.second);
   }

   //
   // String::Find
   //
   String String::Find(char const *str, std::size_t len)
   {
      if(!str) return STRNULL;

      return Find(str, len, StrHash(str, len));
   }

   //
   // String::Find
   //
   String String::Find(char const *str, std::size_t len, std::size_t hash)
   {
      if(!str) return STRNULL;

      for(auto idx = StringTableHash[hash % StringTableHashC]; idx;)
      {
         auto const &entry = StringTable()[idx];

         if(entry.hash == hash && entry.len == len && !std::memcmp(entry.str, str, len))
            return String(idx);

         idx = entry.next;
      }

      return STRNULL;
   }

   //
   // String::Get
   //
   String String::Get(char const *str)
   {
      if(!str) return STRNULL;

      auto lh = StrLenHash(str);
      return Get(str, lh.first, lh.second);
   }

   //
   // String::Get
   //
   String String::Get(char const *str, std::size_t len)
   {
      if(!str) return STRNULL;

      return Get(str, len, StrHash(str, len));
   }

   //
   // String::Get
   //
   String String::Get(char const *str, std::size_t len, std::size_t hash)
   {
      if(!str) return STRNULL;

      if(auto s = Find(str, len, hash)) return s;

      return Add(StrDup(str, len), len, hash);
   }

   //
   // operator std::ostream << String
   //
   std::ostream &operator << (std::ostream &out, String in)
   {
      return out.write(in.data(), in.size());
   }

   //
   // operator String + char const *
   //
   String operator + (String l, char const *r)
   {
      std::size_t rl, rh;
      std::tie(rl, rh) = StrLenHash(r);

      std::size_t len  = l.size() + rl;
      std::size_t hash = StrHash(r, rl, l.getHash());

      for(auto idx = StringData::GetFirst(hash); idx;)
      {
         auto const &entry = String::GetDataV()[idx];

         if(entry.getHash() == hash && entry.size() == len &&
            !std::memcmp(entry.data(),            l.data(), l.size()) &&
            !std::memcmp(entry.data() + l.size(), r,        rl))
            return String(idx);

         idx = entry.getNext();
      }

      std::unique_ptr<char[]> newstr{new char[len + 1]};
      std::memcpy(newstr.get(),            l.data(), l.size());
      std::memcpy(newstr.get() + l.size(), r,        rl);
      newstr[len] = '\0';

      return String::Add(std::move(newstr), len, hash);
   }

   //
   // operator String + String
   //
   String operator + (String l, String r)
   {
      std::size_t len  = l.size() + r.size();
      std::size_t hash = StrHash(r.data(), r.size(), l.getHash());

      for(auto idx = StringData::GetFirst(hash); idx;)
      {
         auto const &entry = String::GetDataV()[idx];

         if(entry.getHash() == hash && entry.size() == len &&
            !std::memcmp(entry.data(),            l.data(), l.size()) &&
            !std::memcmp(entry.data() + l.size(), r.data(), r.size()))
            return String(idx);

         idx = entry.getNext();
      }

      std::unique_ptr<char[]> newstr{new char[len + 1]};
      std::memcpy(newstr.get(),            l.data(), l.size());
      std::memcpy(newstr.get() + l.size(), r.data(), r.size());
      newstr[len] = '\0';

      return String::Add(std::move(newstr), len, hash);
   }

   //
   // operator String < String
   //
   bool operator < (String l, String r)
   {
      if(l == r) return false;

      if(l.size() < r.size())
         return std::memcmp(l.data(), r.data(), l.size()) <= 0;
      else
         return std::memcmp(l.data(), r.data(), r.size()) < 0;
   }

   //
   // Str8To32
   //
   // This function converts UTF-8 to UTF-32. It does not perform validation of
   // the input, but should never result in undefined behavior for any valid
   // combination of pointers.
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
}

// EOF

