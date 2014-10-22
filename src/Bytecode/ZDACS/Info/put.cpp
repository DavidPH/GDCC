//-----------------------------------------------------------------------------
//
// Copyright (C) 2013-2014 David Hill
//
// See COPYING for license information.
//
//-----------------------------------------------------------------------------
//
// ZDoom ACS IR code output.
//
//-----------------------------------------------------------------------------

#include "Bytecode/ZDACS/Info.hpp"

#include "IR/Function.hpp"

#include <iostream>


//----------------------------------------------------------------------------|
// Global Functions                                                           |
//

namespace GDCC
{
   namespace Bytecode
   {
      namespace ZDACS
      {
         //
         // Info::put
         //
         void Info::put()
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
               putWord(0);
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
         void Info::putCode(Code code)
         {
            putWord(static_cast<Core::FastU>(code));
         }

         //
         // Info::putCode
         //
         void Info::putCode(Code code, Core::FastU arg0)
         {
            putCode(code);
            putWord(arg0);
         }

         //
         // Info::putData
         //
         void Info::putData(char const *s, std::size_t len)
         {
            out->write(s, len);

            putPos += len;
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
   }
}

// EOF
