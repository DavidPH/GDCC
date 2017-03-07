//-----------------------------------------------------------------------------
//
// Copyright (C) 2014-2016 David Hill
//
// See COPYING for license information.
//
//-----------------------------------------------------------------------------
//
// Compare statement handling.
//
//-----------------------------------------------------------------------------

#include "BC/ZDACS/Info.hpp"

#include "BC/ZDACS/Code.hpp"

#include "Core/Exception.hpp"

#include "IR/Function.hpp"
#include "IR/Glyph.hpp"

#include <sstream>


//----------------------------------------------------------------------------|
// Extern Functions                                                           |
//

namespace GDCC
{
   namespace BC
   {
      namespace ZDACS
      {
         //
         // Info::genStmnt_CmpI_W
         //
         void Info::genStmnt_CmpI_W()
         {
            if(stmnt->op.size == 1)
               numChunkCODE += 4;
            else
               genStmntCall(1);
         }

         //
         // Info::genStmnt_CmpU_EQ_W
         //
         void Info::genStmnt_CmpU_EQ_W()
         {
            if(!stmnt->op.size) return;

            if(stmnt->args[1].a != IR::ArgBase::Stk ||
               stmnt->args[1].a != IR::ArgBase::Stk)
            {
               numChunkCODE += stmnt->op.size * 8 - 4 +
                  lenPushArg(stmnt->args[1], 0, stmnt->op.size) +
                  lenPushArg(stmnt->args[2], 0, stmnt->op.size);
            }
            else if(stmnt->op.size == 1)
               numChunkCODE += 4;
            else
               genStmntCall(1);
         }

         //
         // Info::preStmnt_CmpF_EQ_W
         //
         void Info::preStmnt_CmpF_EQ_W()
         {
            addFunc_CmpF_EQ_W(stmnt->op.size);
         }

         //
         // Info::preStmnt_CmpF_GE_W
         //
         void Info::preStmnt_CmpF_GE_W()
         {
            addFunc_CmpF_GE_W(stmnt->op.size);
         }

         //
         // Info::preStmnt_CmpF_GT_W
         //
         void Info::preStmnt_CmpF_GT_W()
         {
            addFunc_CmpF_GT_W(stmnt->op.size);
         }

         //
         // Info::preStmnt_CmpF_LE_W
         //
         void Info::preStmnt_CmpF_LE_W()
         {
            addFunc_CmpF_LE_W(stmnt->op.size);
         }

         //
         // Info::preStmnt_CmpF_LT_W
         //
         void Info::preStmnt_CmpF_LT_W()
         {
            addFunc_CmpF_LT_W(stmnt->op.size);
         }

         //
         // Info::preStmnt_CmpF_NE_W
         //
         void Info::preStmnt_CmpF_NE_W()
         {
            addFunc_CmpF_NE_W(stmnt->op.size);
         }

         //
         // Info::preStmnt_CmpI_GE_W
         //
         void Info::preStmnt_CmpI_GE_W()
         {
            if(stmnt->op.size < 2)
               return;

            addFunc_CmpI_GE_W(stmnt->op.size);
         }

         //
         // Info::preStmnt_CmpI_GT_W
         //
         void Info::preStmnt_CmpI_GT_W()
         {
            if(stmnt->op.size < 2)
               return;

            addFunc_CmpI_GT_W(stmnt->op.size);
         }

         //
         // Info::preStmnt_CmpI_LE_W
         //
         void Info::preStmnt_CmpI_LE_W()
         {
            if(stmnt->op.size < 2)
               return;

            addFunc_CmpI_LE_W(stmnt->op.size);
         }

         //
         // Info::preStmnt_CmpI_LT_W
         //
         void Info::preStmnt_CmpI_LT_W()
         {
            if(stmnt->op.size < 2)
               return;

            addFunc_CmpI_LT_W(stmnt->op.size);
         }

         //
         // Info::preStmnt_CmpU_EQ_W
         //
         void Info::preStmnt_CmpU_EQ_W(IR::Code codeCmp)
         {
            if(stmnt->op.size < 2)
               return;

            if(codeCmp == IR::Code::CmpU_EQ_W)
               addFunc_CmpU_EQ_W(stmnt->op.size);
            else
               addFunc_CmpU_NE_W(stmnt->op.size);
         }

         //
         // Info::preStmnt_CmpU_GE_W
         //
         void Info::preStmnt_CmpU_GE_W()
         {
            if(stmnt->op.size == 0)
               return;

            if(stmnt->op.size == 1)
               preStmnt_CmpU_W1(IR::Code::CmpU_GE_W, IR::Code::CmpI_GE_W, true, false);
            else
               addFunc_CmpU_GE_W(stmnt->op.size);
         }

         //
         // Info::preStmnt_CmpU_GT_W
         //
         void Info::preStmnt_CmpU_GT_W()
         {
            if(stmnt->op.size == 0)
               return;

            if(stmnt->op.size == 1)
               preStmnt_CmpU_W1(IR::Code::CmpU_GT_W, IR::Code::CmpI_GT_W, true, false);
            else
               addFunc_CmpU_GT_W(stmnt->op.size);
         }

         //
         // Info::preStmnt_CmpU_LE_W
         //
         void Info::preStmnt_CmpU_LE_W()
         {
            if(stmnt->op.size == 0)
               return;

            if(stmnt->op.size == 1)
               preStmnt_CmpU_W1(IR::Code::CmpU_LE_W, IR::Code::CmpI_LE_W, false, true);
            else
               addFunc_CmpU_LE_W(stmnt->op.size);
         }

         //
         // Info::preStmnt_CmpU_LT_W
         //
         void Info::preStmnt_CmpU_LT_W()
         {
            if(stmnt->op.size == 0)
               return;

            if(stmnt->op.size == 1)
               preStmnt_CmpU_W1(IR::Code::CmpU_LT_W, IR::Code::CmpI_LT_W, false, true);
            else
               addFunc_CmpU_LT_W(stmnt->op.size);
         }

         //
         // Info::preStmnt_CmpU_W1
         //
         void Info::preStmnt_CmpU_W1(IR::Code code, IR::Code codeCmp, bool resGT, bool resLT)
         {
            // Unsigned word compare logic:
            //
            // if(l & 0x80000000) {if(!(r & 0x80000000)) return +1;}
            // else               {if(  r & 0x80000000 ) return -1;}
            // return cmp((int)l, (int)r)

            Core::String name = getCallName({code, 1});
            auto newFunc = preStmntCallDef(name, 1, 2, 2, __FILE__, __LINE__);

            if(!newFunc)
               return;

            IR::Glyph labelPos{prog, name + "$pos"};
            IR::Glyph labelCmp{prog, name + "$cmp"};

            IR::Arg_LocReg lop{IR::Arg_Lit(newFunc->block.getExp(0))};
            IR::Arg_LocReg rop{IR::Arg_Lit(newFunc->block.getExp(stmnt->op.size))};

            #define AS_Stmnt newFunc->block.addStatementArgs

            AS_Stmnt({IR::Code::BAnd_W,   1}, IR::Arg_Stk(), lop, 0x80000000);
            AS_Stmnt({IR::Code::Jcnd_Nil, 1}, IR::Arg_Stk(), labelPos);

            AS_Stmnt({IR::Code::BAnd_W,   1}, IR::Arg_Stk(), rop, 0x80000000);
            AS_Stmnt({IR::Code::Jcnd_Tru, 1}, IR::Arg_Stk(), labelCmp);
            AS_Stmnt({IR::Code::Retn,     1}, resGT);

            newFunc->block.addLabel(labelPos);
            AS_Stmnt({IR::Code::BAnd_W,   1}, IR::Arg_Stk(), rop, 0x80000000);
            AS_Stmnt({IR::Code::Jcnd_Nil, 1}, IR::Arg_Stk(), labelCmp);
            AS_Stmnt({IR::Code::Retn,     1}, resLT);

            newFunc->block.addLabel(labelCmp);
            AS_Stmnt({codeCmp,        1}, IR::Arg_Stk(), lop, rop);
            AS_Stmnt({IR::Code::Retn, 1}, IR::Arg_Stk());

            #undef AS_Stmnt

            throw ResetFunc();
         }

         //
         // Info::putStmnt_CmpI_W
         //
         void Info::putStmnt_CmpI_W(Code code)
         {
            if(stmnt->op.size == 1)
               putCode(code);
            else
               putStmntCall(1);
         }

         //
         // Info::putStmnt_CmpU_EQ_W
         //
         void Info::putStmnt_CmpU_EQ_W()
         {
            if(!stmnt->op.size) return;

            if(stmnt->args[1].a != IR::ArgBase::Stk ||
               stmnt->args[1].a != IR::ArgBase::Stk)
            {
               putStmntPushArg(stmnt->args[1], 0);
               putStmntPushArg(stmnt->args[2], 0);
               putCode(Code::CmpU_EQ);

               for(Core::FastU i = 1; i != stmnt->op.size; ++i)
               {
                  putStmntPushArg(stmnt->args[1], i);
                  putStmntPushArg(stmnt->args[2], i);
                  putCode(Code::CmpU_EQ);
                  putCode(Code::BAnd);
               }
            }
            else if(stmnt->op.size == 1)
               putCode(Code::CmpU_EQ);
            else
               putStmntCall(1);
         }

         //
         // Info::putStmnt_CmpU_NE_W
         //
         void Info::putStmnt_CmpU_NE_W()
         {
            if(!stmnt->op.size) return;

            if(stmnt->args[1].a != IR::ArgBase::Stk ||
               stmnt->args[1].a != IR::ArgBase::Stk)
            {
               putStmntPushArg(stmnt->args[1], 0);
               putStmntPushArg(stmnt->args[2], 0);
               putCode(Code::CmpU_NE);

               for(Core::FastU i = 1; i != stmnt->op.size; ++i)
               {
                  putStmntPushArg(stmnt->args[1], i);
                  putStmntPushArg(stmnt->args[2], i);
                  putCode(Code::CmpU_NE);
                  putCode(Code::BOrI);
               }
            }
            else if(stmnt->op.size == 1)
               putCode(Code::CmpU_NE);
            else
               putStmntCall(1);
         }

         //
         // Info::trStmnt_CmpU_EQ_W
         //
         void Info::trStmnt_CmpU_EQ_W()
         {
            CheckArgC(stmnt, 3);

            if(isPushArg(stmnt->args[1]) && isPushArg(stmnt->args[2]))
               moveArgStk_dst(stmnt->args[0], 1);
            else
               trStmntStk3(1, stmnt->op.size, false);
         }
      }
   }
}

// EOF

