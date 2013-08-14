//-----------------------------------------------------------------------------
//
// Copyright (C) 2013 David Hill
//
// See COPYING for license information.
//
//-----------------------------------------------------------------------------
//
// ZDoom ACS IR code output.
//
//-----------------------------------------------------------------------------

#include "Info.hpp"

#include "GDCC/IR/Function.hpp"

#include <iostream>


//----------------------------------------------------------------------------|
// Global Functions                                                           |
//

namespace Bytecode
{
   namespace ZDACS
   {
      //
      // Info::put
      //
      void Info::put(std::ostream &out_)
      {
         out = &out_;

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
            putWord(0);
            putWord(0);
         }

         out = nullptr;
      }

      //
      // Info::putExpWord
      //
      void Info::putExpWord(GDCC::IR::Exp const *exp)
      {
         putWord(ResolveValue(exp->getValue()));
      }

      //
      // Info::putHWord
      //
      void Info::putHWord(GDCC::FastU i)
      {
         out->put((i >> 0) & 0xFF);
         out->put((i >> 8) & 0xFF);
      }

      //
      // Info::putString
      //
      void Info::putString(GDCC::String str)
      {
         auto const &data = str.getData();

         for(auto i = data.str, e = i + data.len; i != e; ++i) switch(*i)
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
      };

      //
      // Info::putString
      //
      // Handles STRE "encrypted" strings.
      //
      void Info::putString(GDCC::String str, GDCC::FastU key)
      {
         std::size_t idx = 0;

         //
         // putChar
         //
         auto putChar = [&](unsigned char c)
         {
            putByte(c ^ (idx++ / 2 + key));
         };

         auto const &data = str.getData();

         for(auto i = data.str, e = i + data.len; i != e; ++i) switch(*i)
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
      };

      //
      // Info::putWord
      //
      void Info::putWord(GDCC::FastU i)
      {
         out->put((i >>  0) & 0xFF);
         out->put((i >>  8) & 0xFF);
         out->put((i >> 16) & 0xFF);
         out->put((i >> 24) & 0xFF);
      }
   }
}

// EOF

