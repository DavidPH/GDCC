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
      void Info::put(std::ostream &out)
      {
         // Put header.
         out.write("ACSE", 4);
         putWord(out, jumpPos);
         out.write("GDCC::BC", 8);

         // Put statements.
         for(auto &func : GDCC::IR::FunctionRange()) try
         {
            curFunc = &func.second;
            for(auto const &stmnt : func.second.block)
               putStmnt(out, stmnt);
            curFunc = nullptr;
         }
         catch(...) {curFunc = nullptr; throw;}

         // Put chunks.
         putChunk(out);
      }

      //
      // Info::putByte
      //
      void Info::putByte(std::ostream &out, GDCC::FastU i)
      {
         out.put(i & 0xFF);
      }

      //
      // Info::putExpWord
      //
      void Info::putExpWord(std::ostream &out, GDCC::IR::Exp const *exp)
      {
         putWord(out, ResolveValue(exp->getValue()));
      }

      //
      // Info::putHWord
      //
      void Info::putHWord(std::ostream &out, GDCC::FastU i)
      {
         out.put((i >> 0) & 0xFF);
         out.put((i >> 8) & 0xFF);
      }

      //
      // Info::putString
      //
      void Info::putString(std::ostream &out, GDCC::String str)
      {
         auto const &data = str.getData();

         for(auto i = data.str, e = i + data.len; i != e; ++i) switch(*i)
         {
         case '\0':
            if('0' <= i[1] && i[1] <= '7')
               out.write("\\000", 4);
            else
               out.write("\\0", 2);
            break;

         case '\\':
            out.write("\\\\", 2);
            break;

         default:
            out.put(*i);
            break;
         }

         out.put('\0');
      };

      //
      // Info::putWord
      //
      void Info::putWord(std::ostream &out, GDCC::FastU i)
      {
         out.put((i >>  0) & 0xFF);
         out.put((i >>  8) & 0xFF);
         out.put((i >> 16) & 0xFF);
         out.put((i >> 24) & 0xFF);
      }
   }
}

// EOF

