//-----------------------------------------------------------------------------
//
// Copyright (C) 2014 David Hill
//
// See COPYING for license information.
//
//-----------------------------------------------------------------------------
//
// String utilities.
//
//-----------------------------------------------------------------------------

#include "Option/StrUtil.hpp"

#include <cstring>


//----------------------------------------------------------------------------|
// Global Functions                                                           |
//

namespace GDCC
{
   namespace Option
   {
      ///
      /// Compares strings as in std::strcmp, except that a null string is
      /// considered equal to a null string and less than any non-null string.
      ///
      /// @param l Left/first string.
      /// @param r Right/second string.
      ///
      /// @return As in std::strcmp.
      ///
      int StrCmp(char const *l, char const *r)
      {
         if(l) return r ? std::strcmp(l, r) : 1;
         else  return r ? -1 : 0;
      }

      ///
      /// Creates a new string with the length and contents of s, up to and
      /// including the first null character.
      ///
      /// @param s Source string.
      ///
      /// @return The new string.
      ///
      std::unique_ptr<char[]> StrDup(char const *s)
      {
         return StrDup(s, std::strlen(s));
      }

      ///
      /// Creates a new string with the length of len+1 and the first len bytes
      /// of s with a terminating null character added.
      ///
      /// @param s Source string.
      /// @param len Number of characters to copy from s.
      ///
      /// @return The new string.
      ///
      std::unique_ptr<char[]> StrDup(char const *s, std::size_t len)
      {
         std::unique_ptr<char[]> dup{new char[len + 1]};
         std::memcpy(dup.get(), s, len);
         dup[len] = '\0';

         return dup;
      }

      ///
      /// If s is null, returns 0. Otherwise, calculates a hash as if by
      /// calling StrHash(s, std::strlen(s), 0).
      ///
      /// @param s Source string.
      ///
      /// @return Calculated hash.
      ///
      std::size_t StrHash(char const *s)
      {
         std::size_t hash = 0;

         if(s) while(*s)
            hash = hash * 5 + static_cast<unsigned char>(*s++);

         return hash;
      }

      ///
      /// Calculates a hash as if by calling StrHash(s, len, 0).
      ///
      /// @param s Source string.
      /// @param len Number of characters to read from s.
      ///
      /// @return Calculated hash.
      ///
      std::size_t StrHash(char const *s, std::size_t len)
      {
         std::size_t hash = 0;

         while(len--)
            hash = hash * 5 + static_cast<unsigned char>(*s++);

         return hash;
      }

      ///
      /// Calculates a hash using the first len bytes of s, starting with a
      /// value of hash.
      ///
      /// The starting hash value can be used to calculate the hash for a
      /// string in multiple parts by passing the hash of the first part to the
      /// second part.
      ///
      /// Like std::hash, this function is meant for hash tables and is
      /// unsuitable for cryptographic uses.
      ///
      /// @param s Source string.
      /// @param len Number of characters to return from s.
      /// @param hash Starting hash value.
      ///
      /// @return Calculated hash.
      ///
      std::size_t StrHash(char const *s, std::size_t len, std::size_t hash)
      {
         while(len--)
            hash = hash * 5 + static_cast<unsigned char>(*s++);

         return hash;
      }

      ///
      /// Calculates the length and hash of s. If s is null, returned length
      /// and hash are both 0. Otherwise, length is calculated as if by calling
      /// std::strlen(s), and hash is calculated as if by calling StrHash(s).
      ///
      /// @param s Source string.
      ///
      /// @return Length, hash.
      ///
      std::pair<std::size_t, std::size_t> StrLenHash(char const *s)
      {
         std::size_t len = 0, hash = 0;

         if(s) for(; *s; ++len)
            hash = hash * 5 + static_cast<unsigned char>(*s++);

         return {len, hash};
      }
   }
}

// EOF

