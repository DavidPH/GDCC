//-----------------------------------------------------------------------------
//
// Copyright (C) 2013-2014 David Hill
//
// See COPYING for license information.
//
//-----------------------------------------------------------------------------
//
// Intermediary Representation statements/instructions.
//
//-----------------------------------------------------------------------------

#include "IR/Statement.hpp"

#include "IR/Code.hpp"
#include "IR/IArchive.hpp"
#include "IR/OArchive.hpp"


//----------------------------------------------------------------------------|
// Global Functions                                                           |
//

namespace GDCC
{
   namespace IR
   {
      //
      // Statement default constructor
      //
      Statement::Statement() :
         next{this},
         prev{this}
      {
      }

      //
      // Statement move constructor
      //
      Statement::Statement(Statement &&stmnt) :
         pos {std::move(stmnt.pos)},
         next{std::move(stmnt.next)},
         prev{std::move(stmnt.prev)},
         args{std::move(stmnt.args)},
         labs{std::move(stmnt.labs)},
         op  {std::move(stmnt.op)}
      {
         // If stmnt is in a list...
         if(stmnt.next != &stmnt)
         {
            // ... Unlink stmnt and relink this.
            next->prev = prev->next = this;
            stmnt.next = stmnt.prev = &stmnt;
         }
         else
            // Otherwise, just reset links.
            next = prev = this;
      }

      //
      // Statement constructor
      //
      Statement::Statement(Statement *head) :
         next{head},
         prev{head->prev}
      {
         next->prev = this;
         prev->next = this;
      }

      //
      // Statement constructor
      //
      Statement::Statement(Statement *head, Statement *link, OpCode op_) :
         pos {head->pos},
         next{link},
         prev{link->prev},
         args{std::move(head->args)},
         labs{std::move(head->labs)},
         op  {op_}
      {
         next->prev = this;
         prev->next = this;
      }

      //
      // Statement move assignment
      //
      Statement &Statement::operator = (Statement &&stmnt)
      {
         // Must avoid self-assignment.
         if(&stmnt == this) return *this;

         // Unlink from current list.
         next->prev = prev;
         prev->next = next;

         // Move values from stmnt.
         pos  = std::move(stmnt.pos);
         next = std::move(stmnt.next);
         prev = std::move(stmnt.prev);
         args = std::move(stmnt.args);
         labs = std::move(stmnt.labs);
         op   = std::move(stmnt.op);

         // If stmnt is in a list...
         if(stmnt.next != &stmnt)
         {
            // ... Unlink stmnt and relink this.
            next->prev = prev->next = this;
            stmnt.next = stmnt.prev = &stmnt;
         }
         else
            // Otherwise, just reset links.
            next = prev = this;

         return *this;
      }

      //
      // Statement destructor
      //
      Statement::~Statement()
      {
         next->prev = prev;
         prev->next = next;
      }

      //
      // operator OArchive << Statement
      //
      OArchive &operator << (OArchive &out, Statement const &in)
      {
         return out << in.pos << in.op << in.args << in.labs;
      }

      //
      // operator IArchive >> Statement
      //
      IArchive &operator >> (IArchive &in, Statement &out)
      {
         return in >> out.pos >> out.op >> out.args >> out.labs;
      }
   }
}

// EOF

