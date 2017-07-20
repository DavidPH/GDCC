//-----------------------------------------------------------------------------
//
// Copyright (C) 2013-2015 David Hill
//
// See COPYING for license information.
//
//-----------------------------------------------------------------------------
//
// Intermediary Representation input.
//
//-----------------------------------------------------------------------------

#include "IR/IArchive.hpp"

#include "Core/Origin.hpp"

#include <iostream>
#include <cstring>


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

   return {str, len};
}


//----------------------------------------------------------------------------|
// Extern Functions                                                           |
//

namespace GDCC
{
   namespace IR
   {
      //
      // IArchive constructor
      //
      IArchive::IArchive(std::istream &in)
      {
         std::size_t count = 0;
         for(int c; (c = in.get()), in; data.push_back(static_cast<char>(c)))
            if(!c) ++count;

         if(!count) return;

         str = Core::Array<char const *>(count);
         auto si = itr = str.begin();

         *si++ = data.data();
         for(auto ci = data.begin(), ce = data.end(); ci != ce; ++ci)
            if(!*ci && ci + 1 != ce) *si++ = &ci[1];
      }

      //
      // IArchive::operator >> Core::Float
      //
      IArchive &IArchive::operator >> (Core::Float &out)
      {
         if(auto s = get())
            out = Core::Float(s, mpf_get_default_prec(), 16);
         else
            out = 0;

         return *this;
      }

      //
      // IArchive::operator >> Core::Integ
      //
      IArchive &IArchive::operator >> (Core::Integ &out)
      {
         if(auto s = get())
            out = Core::Integ(s, 16);
         else
            out = 0;

         return *this;
      }

      //
      // IArchive::getHeader
      //
      IArchive &IArchive::getHeader()
      {
         if(std::strcmp(get(), "DGE_NTS")  ||
            std::strcmp(get(), "GDCC::IR") ||
            std::strcmp(get(), ""))
         {
            std::cerr << "not IR\n";
            throw EXIT_FAILURE;
         }

         getTablesString();

         return *this;
      }

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
      // operator IArchive >> Core::Origin
      //
      IArchive &operator >> (IArchive &in, Core::Origin &out)
      {
         return in >> out.file >> out.line >> out.col;
      }

      //
      // operator IArchive >> Core::String
      //
      IArchive &operator >> (IArchive &in, Core::String &out)
      {
         auto n = in.getNumber<std::size_t>();

         if(n < Core::STRMAX)
            out = static_cast<Core::String>(n);
         else if((n -= Core::STRMAX) < in.stab.size())
            out = in.stab[n];
         else
         {
            std::cerr << "invalid String: " << std::hex << n + Core::STRMAX
               << "/(" << Core::STRMAX << '+' << in.stab.size() << ")\n";
            throw EXIT_FAILURE;
         }

         return in;
      }

      //
      // operator IArchive >> Core::StringIndex
      //
      IArchive &operator >> (IArchive &in, Core::StringIndex &out)
      {
         auto n = in.getNumber<std::size_t>();

         if(n < Core::STRMAX)
            out = static_cast<Core::StringIndex>(n);
         else if((n -= Core::STRMAX) < in.stab.size())
            out = Core::STRNULL;
         else
         {
            std::cerr << "invalid StringIndex: " << std::hex << n + Core::STRMAX
               << "/(" << Core::STRMAX << '+' << in.stab.size() << ")\n";
            throw EXIT_FAILURE;
         }

         return in;
      }
   }
}

// EOF

