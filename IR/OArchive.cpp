//-----------------------------------------------------------------------------
//
// Copyright (C) 2013-2019 David Hill
//
// See COPYING for license information.
//
//-----------------------------------------------------------------------------
//
// Intermediary Representation output.
//
//-----------------------------------------------------------------------------

#include "IR/OArchive.hpp"

#include "Core/Exception.hpp"

#include "Target/Addr.hpp"
#include "Target/CallType.hpp"


//----------------------------------------------------------------------------|
// Extern Functions                                                           |
//

namespace GDCC::IR
{
   //
   // OArchive constructor
   //
   OArchive::OArchive(std::ostream &out_) :
      strUse{Core::Size, Core::String::GetDataC(), false},
      out{out_}
   {
   }

   //
   // OArchive::operator << Core::String
   //
   OArchive &OArchive::operator << (Core::String in)
   {
      auto idx = static_cast<std::size_t>(in);
      strUse[idx] = true;
      putU(idx);

      return *this;
   }

   //
   // OArchive::operator << Core::StringIndex
   //
   OArchive &OArchive::operator << (Core::StringIndex in)
   {
      auto idx = static_cast<std::size_t>(in);
      strUse[idx] = true;
      putU(idx);

      return *this;
   }

   //
   // operator OArchive << Target::AddrBase
   //
   OArchive &operator << (OArchive &out, Target::AddrBase in)
   {
      switch(in)
      {
         #define GDCC_Target_AddrList(name) \
            case Target::AddrBase::name: return out << Core::STR_##name;
         #include "Target/AddrList.hpp"
      }

      Core::Error({}, "invalid enum GDCC::Target::AddrBase");
   }

   //
   // operator OArchive << Target::AddrSpace
   //
   OArchive &operator << (OArchive &out, Target::AddrSpace in)
   {
      return out << in.base << in.name;
   }

   //
   // operator OArchive << Target::CallType
   //
   OArchive &operator << (OArchive &out, Target::CallType in)
   {
      switch(in)
      {
         #define GDCC_Target_CallTypeList(name) \
            case Target::CallType::name: return out << Core::STR_##name;
         #include "Target/CallTypeList.hpp"
      }

      Core::Error({}, "invalid enum GDCC::Target::CallType");
   }

   //
   // OArchive::putHead
   //
   void OArchive::putHead()
   {
      out.write("GDCC::IR\0\0\0\0\0\0\0", 16);
   }

   //
   // OArchive::putInteg
   //
   void OArchive::putInteg(Core::Integ in)
   {
      int sign = sgn(in);
      out.put(sign < 0);
      if(sign == 0) {out.put(0); return;}
      if(sign < 0) in = -in;

      std::size_t len = (mpz_size(in.get_mpz_t()) * sizeof(mp_limb_t) * CHAR_BIT + 6) / 7 + 1;
      std::unique_ptr<char[]> buf{new char[len]};
      char *ptr = &buf[len];

      *--ptr = static_cast<char>(in.get_ui() & 0x7F);
      while((in >>= 7))
         *--ptr = static_cast<char>(in.get_ui() & 0x7F) | 0x80;

      out.write(ptr, (&buf[len]) - ptr);
   }

   //
   // OArchive::putRatio
   //
   void OArchive::putRatio(Core::Ratio const &in)
   {
      putInteg(in.get_num());
      putInteg(in.get_den());
   }

   //
   // OArchive::putStrTab
   //
   void OArchive::putStrTab()
   {
      putU(strUse.size());

      auto str = Core::String::GetDataV();
      for(auto &use : strUse)
      {
         if(use)
         {
            putU(str->size());
            out.write(str->data(), str->size());
         }
         else
            out.put(0);

         ++str;
      }
   }

   //
   // OArchive::putTail
   //
   void OArchive::putTail()
   {
      std::size_t idx = out.tellp();

      putStrTab();

      // Write index to tail data.
      constexpr std::size_t idxLen = sizeof(idx) * CHAR_BIT / 8;
      static_assert(idxLen < 256, "pos_type too large");

      char idxBuf[idxLen];
      for(auto c = idxBuf + idxLen; c != idxBuf; idx = idx >> 8)
         *--c = idx & 0xFF;
      out.write(idxBuf, idxLen);
      out.put(idxLen);
   }

   //
   // operator OArchive << Core::Origin
   //
   OArchive &operator << (OArchive &out, Core::Origin const &in)
   {
      return out << in.file << in.line << in.col;
   }
}

// EOF

