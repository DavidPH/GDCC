//-----------------------------------------------------------------------------
//
// Copyright (C) 2013 David Hill
//
// See COPYING for license information.
//
//-----------------------------------------------------------------------------
//
// Number parsing utilities.
//
//-----------------------------------------------------------------------------

#include "Parse.hpp"


//----------------------------------------------------------------------------|
// Static Functions                                                           |
//

//
// IsNumberChar
//
static bool IsNumberChar(int c, int buf)
{
   if(std::isalnum(c)) return true;
   if(c == '.' || c == '_') return true;

   if(c == '+' || c == '-')
      return buf == 'E' || buf == 'e' || buf == 'P' || buf == 'p';

   return false;
}


//----------------------------------------------------------------------------|
// Global Functions                                                           |
//

namespace GDCC
{
   //
   // ReadNumberC
   //
   std::string ReadNumberC(std::istream &in)
   {
      std::string buf;

      for(int c = in.get();; c = in.get())
      {
         if(c == EOF) break;

         if(!IsNumberChar(c, buf.empty() ? EOF : buf.back()))
         {
            in.putback(c);
            break;
         }

         buf += static_cast<char>(c);
      }

      return buf;
   }
}

// EOF

