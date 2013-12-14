//-----------------------------------------------------------------------------
//
// Copyright (C) 2013 David Hill
//
// See COPYING for license information.
//
//-----------------------------------------------------------------------------
//
// Source position/origin information.
//
//-----------------------------------------------------------------------------

#ifndef GDCC__Core__Origin_H__
#define GDCC__Core__Origin_H__

#include "String.hpp"


//----------------------------------------------------------------------------|
// Types                                                                      |
//

namespace GDCC
{
   namespace Core
   {
      //
      // Origin
      //
      struct Origin
      {
         Origin() = default;
         constexpr Origin(String file_, std::size_t line_) : file{file_}, line{line_} {}

         constexpr bool operator == (Origin const &pos) const
            {return file == pos.file && line == pos.line;}

         constexpr bool operator != (Origin const &pos) const
            {return file != pos.file || line != pos.line;}

         String      file;
         std::size_t line;
      };
   }
}


//----------------------------------------------------------------------------|
// Global Functions                                                           |
//

namespace GDCC
{
   namespace Core
   {
      IR::OArchive &operator << (IR::OArchive &out, Origin const &in);
      std::ostream &operator << (std::ostream &out, Origin const &in);

      IR::IArchive &operator >> (IR::IArchive &in, Origin &out);
   }
}

#endif//GDCC__Core__Origin_H__

