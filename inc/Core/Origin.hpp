//-----------------------------------------------------------------------------
//
// Copyright (C) 2013-2018 David Hill
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

#include "../Core/String.hpp"


//----------------------------------------------------------------------------|
// Types                                                                      |
//

namespace GDCC::Core
{
   //
   // Origin
   //
   class Origin
   {
   public:
      Origin() = default;
      constexpr Origin(String file_, std::size_t line_ = 0, std::size_t col_ = 0) :
         file{file_}, line{line_}, col{col_} {}

      constexpr explicit operator bool () const {return file || line || col;}

      constexpr bool operator == (Origin const &pos) const
         {return file == pos.file && line == pos.line && col == pos.col;}

      constexpr bool operator != (Origin const &pos) const
         {return file != pos.file || line != pos.line || col != pos.col;}

      String      file;
      std::size_t line;
      std::size_t col;
   };

   //
   // OriginSource
   //
   class OriginSource
   {
   public:
      virtual ~OriginSource() {}

      Origin getOrigin() const {return v_getOrigin();}

   protected:
      virtual Origin v_getOrigin() const = 0;
   };
}


//----------------------------------------------------------------------------|
// Extern Functions                                                           |
//

namespace GDCC::Core
{
   std::ostream &operator << (std::ostream &out, Origin const &in);
}

#endif//GDCC__Core__Origin_H__

