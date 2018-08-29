//-----------------------------------------------------------------------------
//
// Copyright (C) 2013-2018 David Hill
//
// See COPYING for license information.
//
//-----------------------------------------------------------------------------
//
// Intermediary Representation input.
//
//-----------------------------------------------------------------------------

#include "IR/IArchive.hpp"

#include "Core/Exception.hpp"

#include <cstring>


//----------------------------------------------------------------------------|
// Extern Functions                                                           |
//

namespace GDCC::IR
{
   //
   // IArchive constructor
   //
   IArchive::IArchive(std::istream &in_) :
      prog{nullptr},
      in{in_}
   {
      // Check header.
      if(char buf[16]; !in.read(buf, 16) || std::memcmp(buf, "GDCC::IR\0\0\0\0\0\0\0", 16))
         Core::Error({}, "not IR");

      // Read start of table index.
      in.seekg(-1, std::ios_base::end);
      std::size_t idxLen = in.get();
      in.seekg(-static_cast<std::istream::off_type>(idxLen), std::ios_base::cur);

      std::size_t idx = 0;
      for(auto i = idxLen; --i;)
         idx = (idx << 8) + in.get();

      // Read tables.
      if(!in.seekg(idx))
         Core::Error({}, "bad IR idx");

      getStrTab();

      in.seekg(16);
   }

   //
   // IArchive::operator >> Core::String
   //
   IArchive &IArchive::operator >> (Core::String &out)
   {
      auto idx = getU<std::size_t>();

      if(idx < strTab.size())
         out = strTab[idx];
      else
         Core::Error({}, "invalid String: ", idx, '/', strTab.size());

      return *this;
   }

   //
   // IArchive::operator >> Core::StringIndex
   //
   IArchive &IArchive::operator >> (Core::StringIndex &out)
   {
      auto idx = getU<std::size_t>();

      if(idx < strTab.size())
         out = static_cast<Core::StringIndex>(strTab[idx]);
      else
         Core::Error({}, "invalid String: ", idx, '/', strTab.size());

      return *this;
   }

   //
   // IArchive::getBool
   //
   bool IArchive::getBool()
   {
      return !!in.get();
   }

   //
   // IArchive::getInteg
   //
   Core::Integ IArchive::getInteg()
   {
      bool sign = getBool();
      Core::Integ out = 0;

      unsigned char c;
      while(((c = in.get()) & 0x80) && in)
         out <<= 7, out += (c & 0x7F);
      out <<= 7, out += c;

      if(sign) out = -out;
      return out;
   }

   //
   // IArchive::getRatio
   //
   Core::Ratio IArchive::getRatio()
   {
      Core::Integ num = getInteg();
      Core::Integ den = getInteg();
      return {num, den};
   }

   //
   // IArchive::getStrTab
   //
   void IArchive::getStrTab()
   {
      std::vector<char> strBuf;

      for(auto &str : strTab = {Core::Size, getU<std::size_t>()})
      {
         strBuf.resize(getU<std::size_t>());
         if(!in.read(strBuf.data(), strBuf.size()))
            Core::Error({}, "bad IR str");
         str = {strBuf.data(), strBuf.size()};
      }
   }

   //
   // operator IArchive >> Core::Origin
   //
   IArchive &operator >> (IArchive &in, Core::Origin &out)
   {
      return in >> out.file >> out.line >> out.col;
   }
}

// EOF

