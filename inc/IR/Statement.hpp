//-----------------------------------------------------------------------------
//
// Copyright (C) 2013 David Hill
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

namespace GDCC
{
   namespace IR
   {
      //
      // Statement
      //
      class Statement
      {
      public:
         Statement() : next{this}, prev{this} {}
         Statement(Statement const &) = delete;
         Statement(Statement &&stmnt);
         explicit Statement(Statement *head) : next{head}, prev{head->prev}
            {next->prev = prev->next = this;}
         Statement(Statement *head, Statement *link, Code code_) :
            pos{head->pos}, next{link}, prev{link->prev},
            args{std::move(head->args)}, labs{std::move(head->labs)},
            code{code_} {next->prev = this; prev->next = this;}
         ~Statement() {next->prev = prev; prev->next = next;}

         Statement &operator = (Statement &&stmnt);

         Core::Origin pos;

         Statement *next, *prev;

         Core::Array<Arg>          args;
         Core::Array<Core::String> labs;
         Code                      code;
      };
   }
}


//----------------------------------------------------------------------------|
// Global Functions                                                           |
//

namespace GDCC
{
   namespace IR
   {
      OArchive &operator << (OArchive &out, Statement const &in);

      IArchive &operator >> (IArchive &in, Statement &out);
   }
}

#endif//GDCC__IR__Statement_H__

