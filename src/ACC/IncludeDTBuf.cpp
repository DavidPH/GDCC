//-----------------------------------------------------------------------------
//
// Copyright (C) 2015 David Hill
//
// See COPYING for license information.
//
//-----------------------------------------------------------------------------
//
// #include directive token buffer.
//
//-----------------------------------------------------------------------------

#include "ACC/IncludeDTBuf.hpp"

#include "ACC/TStream.hpp"

#include "CPP/Macro.hpp"

#include "Core/Path.hpp"


//----------------------------------------------------------------------------|
// Global Functions                                                           |
//

namespace GDCC
{
   namespace ACC
   {
      //
      // IncludeDTBuf::doInc
      //
      void IncludeDTBuf::doInc(Core::String name,
         std::unique_ptr<std::streambuf> &&newStr)
      {
         macros.linePush(CPP::Macro::Stringize(name));

         str = std::move(newStr);
         inc.reset(new IncStream(*str, macros, pragma, name,
            Core::PathDirname(name)));
      }
   }
}

// EOF

