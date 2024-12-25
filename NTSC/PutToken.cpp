//-----------------------------------------------------------------------------
//
// Copyright (C) 2019-2024 David Hill
//
// See COPYING for license information.
//
//-----------------------------------------------------------------------------
//
// NTS token output.
//
//-----------------------------------------------------------------------------

#include "NTSC/PutToken.hpp"

#include "../Core/Exception.hpp"
#include "../Core/Parse.hpp"
#include "../Core/Token.hpp"

#include <cstring>


//----------------------------------------------------------------------------|
// Extern Functions                                                           |
//

namespace GDCC::NTSC
{
   //
   // PutToken
   //
   void PutToken(std::ostream &out, Core::Token const &tok)
   {
      switch(tok.tok)
      {
      case Core::TOK_Number: return PutToken_Number(out, tok);
      default:
         out << tok.str << '\0';
         break;
      }
   }

   //
   // PutToken_Number
   //
   void PutToken_Number(std::ostream &out, Core::Token const &tok)
   {
      unsigned    base;
      char const *itr = tok.str.begin(), *end = tok.str.end();
      Core::Integ i;

      std::tie(itr, base)           = Core::ParseNumberBaseC(itr);
      std::tie(itr, i, std::ignore) = Core::ParseNumberInteg(itr, base);
      if(itr != end)
         Core::ErrorExpect("number", tok);
      Core::WriteNumberHex(out << '0', i) << '\0';
   }
}

// EOF

