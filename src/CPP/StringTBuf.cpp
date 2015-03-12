//-----------------------------------------------------------------------------
//
// Copyright (C) 2013-2015 David Hill
//
// See COPYING for license information.
//
//-----------------------------------------------------------------------------
//
// String parsing token buffer.
//
//-----------------------------------------------------------------------------

#include "CPP/StringTBuf.hpp"

#include "Core/Exception.hpp"
#include "Core/Parse.hpp"


//----------------------------------------------------------------------------|
// Global Functions                                                           |
//

namespace GDCC
{
   namespace CPP
   {
      //
      // StringTBuf::underflow
      //
      void StringTBuf::underflow()
      {
         if(tptr() != tend()) return;

         switch((buf[0] = src.get()).tok)
         {
         case Core::TOK_Charac:
            // Prefix?
            if(buf[0].str[0] != '\'')
            {
               auto s = buf[0].str.data();

                    if(s[0] == 'L' && s[1] == '\'')
                  buf[0].tok = Core::TOK_ChrWid;
               else if(s[0] == 'U' && s[1] == '\'')
                  buf[0].tok = Core::TOK_ChrU32;
               else if(s[0] == 'u' && s[1] == '\'')
                  buf[0].tok = Core::TOK_ChrU16;
               else
                  throw Core::ParseExceptExpect(buf[0], "character-literal", false, false);

               buf[0].str = Core::ParseStringC(buf[0].str, 1, '\'', escaper);
            }
            else
               buf[0].str = Core::ParseStringC(buf[0].str, '\'', escaper);

            break;

         case Core::TOK_String:
            // Prefix?
            if(buf[0].str[0] != '"')
            {
               std::size_t o;
               auto        s = buf[0].str.data();

                    if(s[0] == 'L' && s[1] == '"')
                  buf[0].tok = Core::TOK_StrWid, o = 1;
               else if(s[0] == 'U' && s[1] == '"')
                  buf[0].tok = Core::TOK_StrU32, o = 1;
               else if(s[0] == 'u' && s[1] == '"')
                  buf[0].tok = Core::TOK_StrU16, o = 1;
               else if(s[0] == 'u' && s[1] == '8' && s[2] == '"')
                  buf[0].tok = Core::TOK_StrU08, o = 2;
               else if(s[0] == 'c' && s[1] == '"')
                  buf[0].tok = Core::TOK_StrChr, o = 1;
               else if(s[0] == 's' && s[1] == '"')
                  buf[0].tok = Core::TOK_StrIdx, o = 1;
               else
                  throw Core::ParseExceptExpect(buf[0], "string-literal", false, false);

               buf[0].str = Core::ParseStringC(buf[0].str, o, '"', escaper);
            }
            else
               buf[0].str = Core::ParseStringC(buf[0].str, '"', escaper);

            break;

         default: break;
         }

         sett(buf, buf, buf + 1);
      }
   }
}

// EOF

