//-----------------------------------------------------------------------------
//
// Copyright (C) 2014 David Hill
//
// See COPYING for license information.
//
//-----------------------------------------------------------------------------
//
// Jump statement handling.
//
//-----------------------------------------------------------------------------

#include "Bytecode/ZDACS/Info.hpp"

#include "Bytecode/ZDACS/Code.hpp"

#include "IR/Statement.hpp"


//----------------------------------------------------------------------------|
// Global Functions                                                           |
//

namespace GDCC
{
   namespace Bytecode
   {
      namespace ZDACS
      {
         //
         // Info::genStmnt_Jcnd_Nil
         //
         void Info::genStmnt_Jcnd_Nil()
         {
            if(stmnt->op.size == 0)
            {
               numChunkCODE += 8;
               return;
            }

            numChunkCODE += stmnt->op.size * 4 + 4;
         }

         //
         // Info::genStmnt_Jcnd_Tru
         //
         void Info::genStmnt_Jcnd_Tru()
         {
            if(stmnt->op.size == 0) return;

            numChunkCODE += stmnt->op.size * 4 + 4;
         }

         //
         // Info::genStmnt_Jump
         //
         void Info::genStmnt_Jump()
         {
            numChunkCODE += 8;
         }

         //
         // Info::putStmnt_Jcnd_Nil
         //
         void Info::putStmnt_Jcnd_Nil()
         {
            if(stmnt->op.size == 0)
            {
               putCode(Code::Jump_Lit, GetWord(stmnt->args[1].aLit));
               return;
            }

            for(auto i = stmnt->op.size; --i;)
               putCode(Code::OrIU);

            putCode(Code::Jcnd_Nil, GetWord(stmnt->args[1].aLit));
         }

         //
         // Info::putStmnt_Jcnd_Tru
         //
         void Info::putStmnt_Jcnd_Tru()
         {
            if(stmnt->op.size == 0) return;

            for(auto i = stmnt->op.size; --i;)
               putCode(Code::OrIU);

            putCode(Code::Jcnd_Tru, GetWord(stmnt->args[1].aLit));
         }

         //
         // Info::putStmnt_Jump
         //
         void Info::putStmnt_Jump()
         {
            putCode(Code::Jump_Lit, GetWord(stmnt->args[0].aLit));
         }

         //
         // Info::trStmnt_Jcnd_Nil
         //
         void Info::trStmnt_Jcnd_Nil()
         {
            CheckArgC(stmnt, 2);
            CheckArgB(stmnt, 1, IR::ArgBase::Lit);
            moveArgStk_src(stmnt->args[0], stmnt->op.size);
         }

         //
         // Info::trStmnt_Jcnd_Tru
         //
         void Info::trStmnt_Jcnd_Tru()
         {
            CheckArgC(stmnt, 2);
            CheckArgB(stmnt, 1, IR::ArgBase::Lit);
            moveArgStk_src(stmnt->args[0], stmnt->op.size);
         }

         //
         // Info::trStmnt_Jump
         //
         void Info::trStmnt_Jump()
         {
            CheckArgC(stmnt, 1);
            CheckArgB(stmnt, 0, IR::ArgBase::Lit);
         }
      }
   }
}

// EOF

