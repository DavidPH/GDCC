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

         auto hash = HashString(suffix, len, baseHash);

         return AddString(buffer.get(), len + baseLen, hash);
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
         auto const &baseDat = baseStr.getData();

         // Allocate string buffer.
         // baseStr number \0
         baseLen = baseDat.len;
         buffer.reset(new char[baseLen + NumLen + 1]);

         // Initialize string buffer base.
         std::memcpy(buffer.get(), baseDat.str, baseDat.len);

         // Precompute string buffer base hash.
         baseHash = baseDat.hash;

         suffix = &buffer[baseLen];
         number = 0;
      }

      //
      // StringGen::reset
      //
      void StringGen::reset(String baseStr, char const *baseSuf)
      {
         auto const &baseDat = baseStr.getData();
         std::size_t baseSuL = std::strlen(baseSuf);

         // Allocate string buffer.
         // baseStr baseSuf number \0
         baseLen = baseDat.len + baseSuL;
         buffer.reset(new char[baseLen + NumLen + 1]);

         // Initialize string buffer base.
         std::memcpy(buffer.get(), baseDat.str, baseDat.len);
         std::memcpy(&buffer[baseDat.len], baseSuf, baseSuL);

         // Precompute string buffer base hash.
         baseHash = HashString(baseSuf, baseSuL, baseDat.hash);

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

