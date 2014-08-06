//-----------------------------------------------------------------------------
//
// Copyright (C) 2014 David Hill
//
// See COPYING for license information.
//
//-----------------------------------------------------------------------------
//
// String generator.
//
//-----------------------------------------------------------------------------

#include "Core/StringGen.hpp"

#include <cstring>


//----------------------------------------------------------------------------|
// Global Functions                                                           |
//

namespace GDCC
{
   namespace Core
   {
      //
      // StringGen copy constructor
      //
      StringGen::StringGen(StringGen const &gen) :
         buffer  {new char[gen.baseLen + NumLen + 1]},
         suffix  {&buffer[gen.baseLen]},
         baseHash{gen.baseHash},
         baseLen {gen.baseLen},
         number  {gen.number}
      {
         std::memcpy(buffer.get(), gen.buffer.get(), baseLen);
      }

      //
      // StringGen move constructor
      //
      StringGen::StringGen(StringGen &&gen) :
         buffer  {std::move(gen.buffer)},
         suffix  {gen.suffix},
         baseHash{gen.baseHash},
         baseLen {gen.baseLen},
         number  {gen.number}
      {
         gen.reset();
      }

      //
      // StringGen::operator (std::size_t)
      //
      String StringGen::operator () (std::size_t i)
      {
         std::size_t len = std::sprintf(suffix, "%zu", i);

         return {buffer.get(), len + baseLen, StrHash(suffix, len, baseHash)};
      }

      //
      // StringGen::operator = StringGen
      //
      StringGen &StringGen::operator = (StringGen const &gen)
      {
         buffer.reset(new char[gen.baseLen + NumLen + 1]);
         suffix   = &buffer[gen.baseLen];
         baseHash = gen.baseHash;
         baseLen  = gen.baseLen;
         number   = gen.number;

         std::memcpy(buffer.get(), gen.buffer.get(), baseLen);

         return *this;
      }

      //
      // StringGen::reset
      //
      void StringGen::reset()
      {
         buffer   = nullptr;
         suffix   = nullptr;
         baseHash = 0;
         baseLen  = 0;
         number   = 0;
      }

      //
      // StringGen::reset
      //
      void StringGen::reset(String baseStr)
      {
         // Allocate string buffer.
         // baseStr number \0
         baseLen = baseStr.size();
         buffer.reset(new char[baseLen + NumLen + 1]);

         // Initialize string buffer base.
         std::memcpy(buffer.get(), baseStr.data(), baseStr.size());

         // Precompute string buffer base hash.
         baseHash = baseStr.getHash();

         suffix = &buffer[baseLen];
         number = 0;
      }

      //
      // StringGen::reset
      //
      void StringGen::reset(String baseStr, char const *baseSuf)
      {
         std::size_t baseSuL = std::strlen(baseSuf);

         // Allocate string buffer.
         // baseStr baseSuf number \0
         baseLen = baseStr.size() + baseSuL;
         buffer.reset(new char[baseLen + NumLen + 1]);

         // Initialize string buffer base.
         std::memcpy(buffer.get(), baseStr.data(), baseStr.size());
         std::memcpy(&buffer[baseStr.size()], baseSuf, baseSuL);

         // Precompute string buffer base hash.
         baseHash = StrHash(baseSuf, baseSuL, baseStr.getHash());

         suffix = &buffer[baseLen];
         number = 0;
      }

      //
      // StringGen::swap
      //
      void StringGen::swap(StringGen &gen)
      {
         std::swap(buffer,   gen.buffer);
         std::swap(suffix,   gen.suffix);
         std::swap(baseHash, gen.baseHash);
         std::swap(baseLen,  gen.baseLen);
         std::swap(number,   gen.number);
      }
   }
}

// EOF

