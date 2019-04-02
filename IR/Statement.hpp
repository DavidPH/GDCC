//-----------------------------------------------------------------------------
//
// Copyright (C) 2013-2018 David Hill
//
// See COPYING for license information.
//
//-----------------------------------------------------------------------------
//
// Intermediary Representation statements/instructions.
//
//-----------------------------------------------------------------------------

#ifndef GDCC__IR__Statement_H__
#define GDCC__IR__Statement_H__

#include "../IR/Arg.hpp"
#include "../IR/Code.hpp"

#include "../Core/Array.hpp"


//----------------------------------------------------------------------------|
// Types                                                                      |
//

namespace GDCC::IR
{
   //
   // Statement
   //
   class Statement
   {
   public:
      Statement();
      Statement(Statement const &) = delete;
      Statement(Statement &&stmnt);
      Statement(Statement &&stmnt, Statement *link, Code code);
      explicit Statement(Statement *head);
      ~Statement();

      Statement &operator = (Statement &&stmnt);

      Core::Origin pos;

      Statement *next, *prev;

      Core::Array<Arg>          args;
      Core::Array<Core::String> labs;
      Code                      code;
   };
}


//----------------------------------------------------------------------------|
// Extern Functions                                                           |
//

namespace GDCC::IR
{
   OArchive &operator << (OArchive &out, Statement const &in);

   IArchive &operator >> (IArchive &in, Statement &out);
}

#endif//GDCC__IR__Statement_H__

