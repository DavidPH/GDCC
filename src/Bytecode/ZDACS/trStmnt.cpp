//-----------------------------------------------------------------------------
//
// Copyright (C) 2013 David Hill
//
// See COPYING for license information.
//
//-----------------------------------------------------------------------------
//
// ZDoom ACS IR statement translation.
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
      // Info::trStmnt_Call
      //
      void Info::trStmnt_Call(GDCC::IR::Statement &stmnt)
      {
         CheckArgC(stmnt, 2);
         CheckArgB(stmnt, 1, GDCC::IR::ArgBase::Lit);
         for(auto n = stmnt.args.size(); --n != 1;)
            CheckArgB(stmnt, n, GDCC::IR::ArgBase::Stk);

         auto ret = ResolveValue(stmnt.args[1].aLit.value->getValue());

         switch(stmnt.args[0].a)
         {
         case GDCC::IR::ArgBase::Lit:
            if(ret == 0)
               jumpPos += 8;

            else if(ret == 1)
               jumpPos += 8;

            else
            {
               std::cerr << "STUB: " __FILE__ << ':' << __LINE__ << '\n';
               throw EXIT_FAILURE;
            }

            break;

         case GDCC::IR::ArgBase::Stk:
            if(ret == 0)
               jumpPos += 8;

            else if(ret == 1)
               jumpPos += 4;

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
      // Info::trStmnt_Move_W__Stk_Lit
      //
      void Info::trStmnt_Move_W__Stk_Lit(GDCC::IR::Statement &, GDCC::IR::Exp const *exp)
      {
         auto type = exp->getType();

         switch(type.t)
         {
         case GDCC::IR::TypeBase::Funct: jumpPos += 8; break;
         case GDCC::IR::TypeBase::StrEn: jumpPos += 12; break;

         default: jumpPos += 8; break;
         }
      }
   }
}

// EOF

