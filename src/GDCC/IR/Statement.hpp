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

#include "Arg.hpp"
#include "Code.hpp"

#include "../Array.hpp"


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
         Statement() : pos{STRNULL, 0}, next{this}, prev{this}, code{Code::None} {}
         Statement(Statement const &) = delete;
         Statement(Statement &&) = delete;
         Statement(Statement *head, Code code_) : pos{head->pos}, next{head},
            prev{head->prev}, args{std::move(head->args)}, code{code_}
            {next->prev = this; prev->next = this;}
         ~Statement() {next->prev = prev; prev->next = next;}

         Origin pos;

         Statement *next, *prev;

         Array<Arg> args;
         Code       code;
      };
   }
}

#endif//GDCC__IR__Statement_H__

