//-----------------------------------------------------------------------------
//
// Copyright (C) 2013-2024 David Hill
//
// See COPYING for license information.
//
//-----------------------------------------------------------------------------
//
// ZDoom ACS IR code output.
//
//-----------------------------------------------------------------------------

#include "BC/ZDACS/Info.hpp"

#include "BC/ZDACS/Code.hpp"
#include "BC/ZDACS/Module.hpp"

#include "IR/Function.hpp"
#include "IR/Program.hpp"

#include "Target/CallType.hpp"
#include "Target/Info.hpp"


//----------------------------------------------------------------------------|
// Extern Functions                                                           |
//

namespace GDCC::BC::ZDACS
{
   //
   // Info::put
   //
   void Info::put()
   {
      if(Target::FormatCur == Target::Format::ACS0)
         putACS0();
      else
         putACSE();
   }

   //
   // Info::putACS0
   //
   void Info::putACS0()
   {
      putData("ACS\0", 4);
      putWord(16 + module->chunkCODE.getPos());

      // <shamelessplug>
      putData("GDCC::BC", 8);
      // </shamelessplug>

      putACS0_Code();
      putACS0_Scripts();
      putACS0_Strings();
   }

   //
   // Info::putACS0_Code
   //
   void Info::putACS0_Code()
   {
      putCode();
   }

   //
   // Info::putACS0_Scripts
   //
   void Info::putACS0_Scripts()
   {
      // Write script count.
      putWord(module->chunkSPTR.size());

      // Write script headers.
      for(auto const &elem : module->chunkSPTR)
      {
         // Write entry.
         putWord(elem.value + elem.stype * 1000);
         putWord(getWord(elem.entry));
         putWord(elem.param);
      }
   }

   //
   // Info::putACS0_Strings
   //
   void Info::putACS0_Strings()
   {
      // Write string count.
      auto elemC = module->chunkSTRL.size();
      putWord(elemC);

      // Write string offsets.
      std::size_t off = putPos + elemC * 4;
      for(auto const &elem : module->chunkSTRL)
      {
         putWord(off);
         off += lenString(elem.value);
      }

      // Write strings.
      for(auto const &elem : module->chunkSTRL)
         putString(elem.value);
   }

   //
   // Info::putACSE
   //
   void Info::putACSE()
   {
      // Put header.
      if(UseFakeACS0)
      {
         putData("ACS\0", 4);
         putWord(24 + lenChunk());
      }
      else
      {
         putData("ACSE", 4);
         putWord(16);
      }

      // <shamelessplug>
      putData("GDCC::BC", 8);
      // </shamelessplug>

      // Put chunks.
      putChunk();

      // Put (real) header.
      if(UseFakeACS0)
      {
         putWord(16);
         putData("ACSE", 4);
         putACS0_Scripts();
         putWord(0);
      }
   }

   //
   // Info::putByte
   //
   void Info::putByte(Core::FastU i)
   {
      out->put(i & 0xFF);

      putPos += 1;
   }

   //
   // Info::putCode
   //
   void Info::putCode()
   {
      for(auto &code : module->chunkCODE) switch(static_cast<Code>(code.code))
      {
      case Code::Jcnd_Tab:
         putWord(code.code);
         putWord(getWord(code.args[0]));

         // Sort and write case data.
         {
            using Case = std::pair<Core::FastU, Core::FastU>;

            Core::Array<Case> cases{code.args.size() / 2};
            auto const *arg = &code.args[1];
            for(auto &c : cases)
            {
               c.first  = getWord(*arg++);
               c.second = getWord(*arg++);
            }

            // Sort by value as signed.
            std::sort(cases.begin(), cases.end(),
               [](Case const &l, Case const &r)
               {
                  if(l.first & 0x80000000)
                     return r.first & 0x80000000 ? l.first < r.first : true;
                  else
                     return r.first & 0x80000000 ? false : l.first < r.first;
               });

            for(auto const &c : cases)
            {
               putWord(c.first);
               putWord(c.second);
            }
         }

         break;

      default:
         putWord(code.code);
         for(auto const &arg : code.args)
            putWord(getWord(arg));
         break;
      }
   }

   //
   // Info::putHWord
   //
   void Info::putHWord(Core::FastU i)
   {
      out->put((i >> 0) & 0xFF);
      out->put((i >> 8) & 0xFF);

      putPos += 2;
   }

   //
   // Info::putString
   //
   void Info::putString(Core::String s)
   {
      for(auto i = s.begin(), e = s.end(); i != e; ++i) switch(*i)
      {
      case '\0':
         if('0' <= i[1] && i[1] <= '7')
            putData("\\000", 4);
         else
            putData("\\0", 2);
         break;

      case '\\':
         putData("\\\\", 2);
         break;

      default:
         putByte(*i);
         break;
      }

      putByte('\0');
   }

   //
   // Info::putString
   //
   // Handles STRE "encrypted" strings.
   //
   void Info::putString(Core::String s, Core::FastU key)
   {
      std::size_t idx = 0;

      //
      // putChar
      //
      auto putChar = [&](unsigned char c)
      {
         putByte(c ^ (idx++ / 2 + key));
      };

      for(auto i = s.begin(), e = s.end(); i != e; ++i) switch(*i)
      {
      case '\0':
         putChar('\\');
         putChar('0');

         if('0' <= i[1] && i[1] <= '7')
         {
            putChar('0');
            putChar('0');
         }

         break;

      case '\\':
         putChar('\\');
         putChar('\\');
         break;

      default:
         putChar(*i);
         break;
      }

      putChar('\0');
   }

   //
   // Info::putWord
   //
   void Info::putWord(Core::FastU i)
   {
      out->put((i >>  0) & 0xFF);
      out->put((i >>  8) & 0xFF);
      out->put((i >> 16) & 0xFF);
      out->put((i >> 24) & 0xFF);

      putPos += 4;
   }
}

// EOF

