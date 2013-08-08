//-----------------------------------------------------------------------------
//
// Copyright (C) 2013 David Hill
//
// See COPYING for license information.
//
//-----------------------------------------------------------------------------
//
// ZDoom ACSE statement output.
//
//-----------------------------------------------------------------------------

#include "Info.hpp"

#include "GDCC/IR/Statement.hpp"

#include <iostream>


//----------------------------------------------------------------------------|
// Global Functions                                                           |
//

namespace Bytecode
{
   namespace ZDACS
   {
      //
      // Info::putStmnt_Call
      //
      void Info::putStmnt_Call(std::ostream &out, GDCC::IR::Statement const &stmnt)
      {
         auto ret = ResolveValue(stmnt.args[1].aLit.value->getValue());

         switch(stmnt.args[0].a)
         {
         case GDCC::IR::ArgBase::Lit:
            if(ret == 0)
               putWord(out, 204); // calldiscard

            else if(stmnt.args.size() == 1)
               putWord(out, 203); // call

            else
            {
               std::cerr << "STUB: " __FILE__ << ':' << __LINE__ << '\n';
               throw EXIT_FAILURE;
            }

            putExpWord(out, stmnt.args[0].aLit.value);
            break;

         case GDCC::IR::ArgBase::Stk:
            putWord(out, 360); // callstack

            if(ret == 0)
               putWord(out, 54); // drop

            else if(ret == 1) {}

            else
            {
               std::cerr << "STUB: " __FILE__ << ':' << __LINE__ << '\n';
               throw EXIT_FAILURE;
            }

            break;

         default:
            std::cerr << "ERROR: " << stmnt.pos << ": bad Call\n";
            throw EXIT_FAILURE;
         }
      }

      //
      // Info::putStmnt_Move_W__Stk_Lit
      //
      void Info::putStmnt_Move_W__Stk_Lit(std::ostream &out, GDCC::IR::Exp const *exp)
      {
         auto val = exp->getValue();

         switch(val.v)
         {
         case GDCC::IR::ValueBase::Funct:
            putWord(out, 359); // pushfunction
            putWord(out, ResolveValue(val));
            break;

         default:
            putWord(out, 3); // pushnumber
            putWord(out, ResolveValue(val));
            break;
         }
      }
   }
}

// EOF

