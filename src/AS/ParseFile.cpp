//-----------------------------------------------------------------------------
//
// Copyright (C) 2015-2016 David Hill
//
// See COPYING for license information.
//
//-----------------------------------------------------------------------------
//
// Assembly file parsing.
//
//-----------------------------------------------------------------------------

#include "AS/Parse.hpp"

#include "AS/Macro.hpp"
#include "AS/TStream.hpp"

#include "Core/File.hpp"
#include "Core/Token.hpp"


//----------------------------------------------------------------------------|
// Extern Functions                                                           |
//

namespace GDCC
{
   namespace AS
   {
      //
      // ParseFile
      //
      void ParseFile(char const *inName, IR::Program &prog)
      {
         auto buf = Core::FileOpenStream(inName, std::ios_base::in);

         TStream   in    {*buf, inName};
         MacroMap  macros{};
         ParserCtx ctx   {in, macros, prog};

         while(!ctx.in.peek(Core::TOK_EOF))
            AS::ParseDeclaration(ctx);
      }
   }
}

// EOF

