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
         putData("ACSE", 4);
         putWord(16);
         putData("GDCC::BC", 8);

         // Put chunks.
         putChunk();

         out = nullptr;
      }

      //
      // Info::putByte
      //
      void Info::putByte(GDCC::FastU i)
      {
         out->put(i & 0xFF);
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

