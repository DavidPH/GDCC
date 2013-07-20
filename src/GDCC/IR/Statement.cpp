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

#include "Statement.hpp"

#include "IArchive.hpp"
#include "OArchive.hpp"


//----------------------------------------------------------------------------|
// Global Functions                                                           |
//

namespace GDCC
{
   namespace IR
   {
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
         code = std::move(stmnt.code);

         // Unlink stmnt and relink this.
         next->prev = prev->next = this;
         stmnt.next = stmnt.prev = &stmnt;

         return *this;
      }

      //
      // operator OArchive << Statement
      //
      OArchive &operator << (OArchive &out, Statement const &in)
      {
         return out << in.pos << in.code << in.args << in.labs;
      }

      //
      // operator IArchive >> Statement
      //
      IArchive &operator >> (IArchive &in, Statement &out)
      {
         return in >> out.pos >> out.code >> out.args >> out.labs;
      }
   }
}

// EOF

