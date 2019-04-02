//-----------------------------------------------------------------------------
//
// Copyright (C) 2018 David Hill
//
// See COPYING for license information.
//
//-----------------------------------------------------------------------------
//
// IR exceptions.
//
//-----------------------------------------------------------------------------

#ifndef GDCC__IR__Exception_H__
#define GDCC__IR__Exception_H__

#include "../IR/Statement.hpp"

#include "../Core/Exception.hpp"


//----------------------------------------------------------------------------|
// Extern Functions                                                           |
//

namespace GDCC::IR
{
   [[noreturn]]
   void ErrorCode(Core::Origin pos, Code code, char const *msg);
   [[noreturn]]
   void ErrorCode(Statement *stmnt, char const *msg);

   template<typename... Args>
   [[noreturn]]
   void ErrorCode(Statement *stmnt, Args const &...args)
   {
      std::ostringstream oss;
      oss << stmnt->code << ": ";
      ((oss << args), ...);
      Error(stmnt->pos, oss.str());
   }
}

#endif//GDCC__IR__Exception_H__

