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

#include "Core/String.hpp"

#include "IR/IArchive.hpp"
#include "IR/OArchive.hpp"

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
// Static Functions                                                           |
//

//
// AddStringNTS
//
// Adds a string that may contain \xC0\x80 nulls.
//
static GDCC::Core::String AddStringNTS(char const *str)
{
   GDCC::Core::Array<char> tmp;
   std::size_t             len    = 0;
   std::size_t             hash   = 0;
   bool                    hasNul = false;

   for(auto s = str; *s; ++s, ++len)
   {
      if(s[0] == '\xC0' && s[1] == '\x80')
         hasNul = true, ++s;
   }

   if(hasNul)
   {
      tmp = GDCC::Core::Array<char>(len);
      for(auto s = tmp.begin(), e = tmp.end(); s != e; ++s, ++str)
      {
         if(str[0] == '\xC0' && str[1] == '\x80')
            *s = '\0', ++str;
         else
            *s = *str;
      }

      str = tmp.data();
   }

   hash = GDCC::Core::HashString(str, len, hash);
   return GDCC::Core::AddString(str, len, hash);
}


//----------------------------------------------------------------------------|
// Global Functions                                                           |
//

namespace GDCC
{
   namespace Core
   {
      //
      // String::GetData
      //
      StringData const &String::GetData(std::size_t num)
      {
         return StringTable[num];
      }

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
      StringData::StringData(char const *str_, StringIndex num_) : str{str_},
         len{0}, hash{0}, num{num_}, next{0}
      {
         LenHashString(str, len, hash);

         auto hashMod = hash % StringTableHashC;

         next = StringTableHash[hashMod];
         StringTableHash[hashMod] = num;
      }

      //
      // StringData constructor
      //
      StringData::StringData(char const *str_, std::size_t len_, std::size_t hash_,
         std::size_t num_) : str{str_}, len{len_}, hash{hash_}, num{num_}, next{0}
      {
         auto hashMod = hash % StringTableHashC;

         next = StringTableHash[hashMod];
         StringTableHash[hashMod] = num;
      }

      //
      // operator IR::OArchive << String
      //
      IR::OArchive &operator << (IR::OArchive &out, String in)
      {
         return out << static_cast<std::size_t>(in);
      }

      //
      // operator IR::OArchive << StringIndex
      //
      IR::OArchive &operator << (IR::OArchive &out, StringIndex in)
      {
         return out << static_cast<std::size_t>(in);
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
      // operator IR::IArchive >> String
      //
      IR::IArchive &operator >> (IR::IArchive &in, String &out)
      {
         auto n = in.getNumber<std::size_t>();

         if(n < STRMAX)
            out = static_cast<String>(n);
         else if((n -= STRMAX) < in.stab.size())
            out = in.stab[n];
         else
         {
            std::cerr << "invalid String: " << std::hex << n + STRMAX << "/("
               << STRMAX << '+' << in.stab.size() << ")\n";
            throw EXIT_FAILURE;
         }

         return in;
      }

      //
      // operator IR::IArchive >> StringIndex
      //
      IR::IArchive &operator >> (IR::IArchive &in, StringIndex &out)
      {
         out = static_cast<StringIndex>(IR::GetIR<String>(in));
         return in;
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
   }

   namespace IR
   {
      //
      // IArchive::getTablesString
      //
      IArchive &IArchive::getTablesString()
      {
         auto count = getNumber<std::size_t>();

         for(auto &s : (stab = Core::Array<Core::String>(count)))
            s = AddStringNTS(get());

         return *this;
      }

      //
      // OArchive::putTablesString
      //
      OArchive &OArchive::putTablesString()
      {
         auto begin = StringTable.begin();
         auto end   = StringTable.end();

         std::advance(begin, Core::STRMAX);
         *this << std::distance(begin, end);

         for(auto itr = begin; itr != end; ++itr)
         {
            for(auto ci = itr->str, ce = ci + itr->len; ci != ce; ++ci)
            {
               if(*ci)
                  out << *ci;
               else
                  out << '\xC0' << '\x80';
            }

            out << '\0';
         }

         return *this;
      }
   }
}

// EOF

