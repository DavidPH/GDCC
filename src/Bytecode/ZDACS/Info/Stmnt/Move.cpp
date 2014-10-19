//-----------------------------------------------------------------------------
//
// Copyright (C) 2014 David Hill
//
// See COPYING for license information.
//
//-----------------------------------------------------------------------------
//
// Move statement handling.
//
//-----------------------------------------------------------------------------

#include "Bytecode/ZDACS/Info.hpp"

#include "Bytecode/ZDACS/Code.hpp"

#include "Core/Exception.hpp"

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
         // Info::genStmnt_Move_W
         //
         void Info::genStmnt_Move_W()
         {
            // push_?
            if(stmnt->args[0].a == IR::ArgBase::Stk) switch(stmnt->args[1].a)
            {
            case IR::ArgBase::GblArr:
               genStmnt_Move_W__Stk_Arr(stmnt->args[1].aGblArr);
               break;

            case IR::ArgBase::Loc:
               numChunkCODE += IsExp0(stmnt->args[1].aLoc.off) ? 20 : 32;
               break;

            case IR::ArgBase::LocArs:
               numChunkCODE += IsExp0(stmnt->args[1].aLocArs.off) ? 8 : 20;
               break;

            case IR::ArgBase::MapArr:
               genStmnt_Move_W__Stk_Arr(stmnt->args[1].aMapArr);
               break;

            case IR::ArgBase::WldArr:
               genStmnt_Move_W__Stk_Arr(stmnt->args[1].aWldArr);
               break;

            default:
               genStmntPushArg(stmnt->args[1], 0);
               break;
            }

            // drop_?
            else if(stmnt->args[1].a == IR::ArgBase::Stk) switch(stmnt->args[0].a)
            {
            case IR::ArgBase::Nul: numChunkCODE += 4; break;

            case IR::ArgBase::GblArr:
               genStmnt_Move_W__Arr_Stk(stmnt->args[0].aGblArr);
               break;

            case IR::ArgBase::Loc:
               numChunkCODE += IsExp0(stmnt->args[0].aLoc.off) ? 24 : 36;
               break;

            case IR::ArgBase::LocArs:
               numChunkCODE += IsExp0(stmnt->args[0].aLocArs.off) ? 12 : 24;
               break;

            case IR::ArgBase::MapArr:
               genStmnt_Move_W__Arr_Stk(stmnt->args[0].aMapArr);
               break;

            case IR::ArgBase::WldArr:
               genStmnt_Move_W__Arr_Stk(stmnt->args[0].aWldArr);
               break;

            default:
               genStmntDropArg(stmnt->args[0], 0);
               break;
            }

            // ???
            else
               throw Core::ExceptStr(stmnt->pos, "bad gen Move_W");
         }

         //
         // Info::genStmnt_Move_W__Arr_Stk
         //
         void Info::genStmnt_Move_W__Arr_Stk(IR::ArgPtr2 const &arr)
         {
            numChunkCODE += IsExp0(arr.off) ? 12 : 24;
         }

         //
         // Info::genStmnt_Move_W__Stk_Arr
         //
         void Info::genStmnt_Move_W__Stk_Arr(IR::ArgPtr2 const &arr)
         {
            numChunkCODE += IsExp0(arr.off) ? 8 : 20;
         }

         //
         // Info::genStmnt_Move_Wx
         //
         void Info::genStmnt_Move_Wx(Core::FastU x)
         {
            if(stmnt->args[0].a != IR::ArgBase::Stk)
               genStmntDropArg(stmnt->args[0], 0, x);

            if(stmnt->args[1].a != IR::ArgBase::Stk)
               genStmntPushArg(stmnt->args[1], 0, x);
         }

         //
         // Info::putStmnt_Move_W
         //
         void Info::putStmnt_Move_W()
         {
            // push_?
            if(stmnt->args[0].a == IR::ArgBase::Stk) switch(stmnt->args[1].a)
            {
            case IR::ArgBase::GblArr:
               putStmnt_Move_W__Stk_Arr(stmnt->args[1].aGblArr, Code::Push_GblArr);
               break;

            case IR::ArgBase::Loc:
               putCode(Code::Push_LocReg);
               putWord(getStkPtrIdx());
               putCode(Code::AddU);

               if(!IsExp0(stmnt->args[1].aLoc.off))
               {
                  putCode(Code::Push_Lit);
                  putWord(GetWord(stmnt->args[1].aLoc.off));
                  putCode(Code::AddU);
               }

               putCode(Code::Push_GblArr);
               putWord(LocArsArray);
               break;

            case IR::ArgBase::LocArs:
               if(!IsExp0(stmnt->args[1].aLocArs.off))
               {
                  putCode(Code::Push_Lit);
                  putWord(GetWord(stmnt->args[1].aLocArs.off));
                  putCode(Code::AddU);
               }

               putCode(Code::Push_GblArr);
               putWord(LocArsArray);
               break;

            case IR::ArgBase::MapArr:
               putStmnt_Move_W__Stk_Arr(stmnt->args[1].aMapArr, Code::Push_MapArr);
               break;

            case IR::ArgBase::WldArr:
               putStmnt_Move_W__Stk_Arr(stmnt->args[1].aWldArr, Code::Push_WldArr);
               break;

            default:
               putStmntPushArg(stmnt->args[1], 0);
               break;
            }

            // drop_?
            else if(stmnt->args[1].a == IR::ArgBase::Stk) switch(stmnt->args[0].a)
            {
            case IR::ArgBase::GblArr:
               putStmnt_Move_W__Arr_Stk(stmnt->args[0].aGblArr, Code::Drop_GblArr);
               break;

            case IR::ArgBase::Loc:
               putCode(Code::Push_LocReg);
               putWord(getStkPtrIdx());
               putCode(Code::AddU);

               if(!IsExp0(stmnt->args[0].aLoc.off))
               {
                  putCode(Code::Push_Lit);
                  putWord(GetWord(stmnt->args[0].aLoc.off));
                  putCode(Code::AddU);
               }

               putCode(Code::Swap);
               putCode(Code::Drop_GblArr);
               putWord(LocArsArray);
               break;

            case IR::ArgBase::LocArs:
               if(!IsExp0(stmnt->args[0].aLocArs.off))
               {
                  putCode(Code::Push_Lit);
                  putWord(GetWord(stmnt->args[0].aLocArs.off));
                  putCode(Code::AddU);
               }

               putCode(Code::Swap);
               putCode(Code::Drop_GblArr);
               putWord(LocArsArray);
               break;

            case IR::ArgBase::MapArr:
               putStmnt_Move_W__Arr_Stk(stmnt->args[0].aMapArr, Code::Drop_MapArr);
               break;

            case IR::ArgBase::Nul:
               putCode(Code::Drop_Nul);
               break;

            case IR::ArgBase::WldArr:
               putStmnt_Move_W__Arr_Stk(stmnt->args[0].aWldArr, Code::Drop_WldArr);
               break;

            default:
               putStmntDropArg(stmnt->args[0], 0);
               break;
            }

            // ???
            else
               throw Core::ExceptStr(stmnt->pos, "bad put Move_W");
         }

         //
         // Info::putStmnt_Move_W__Arr_Stk
         //
         void Info::putStmnt_Move_W__Arr_Stk(IR::ArgPtr2 const &arr, Code code)
         {
            if(!IsExp0(arr.off))
            {
               putCode(Code::Push_Lit);
               putWord(GetWord(arr.off));
               putCode(Code::AddU);
            }

            putCode(Code::Swap);
            putCode(code);
            putWord(GetWord(arr.arr->aLit.value));
         }

         //
         // Info::putStmnt_Move_W__Stk_Arr
         //
         void Info::putStmnt_Move_W__Stk_Arr(IR::ArgPtr2 const &arr, Code code)
         {
            if(!IsExp0(arr.off))
            {
               putCode(Code::Push_Lit);
               putWord(GetWord(arr.off));
               putCode(Code::AddU);
            }

            putCode(code);
            putWord(GetWord(arr.arr->aLit.value));
         }

         //
         // putStmnt_Move_Wx
         //
         void Info::putStmnt_Move_Wx(Core::FastU x)
         {
            if(stmnt->args[0].a != IR::ArgBase::Stk)
            {
               if(stmnt->args[1].a != IR::ArgBase::Stk)
               {
                  for(Core::FastU w = 0; w != x; ++w)
                  {
                     putStmntPushArg(stmnt->args[1], w);
                     putStmntDropArg(stmnt->args[0], w);
                  }
               }
               else
                  putStmntDropArg(stmnt->args[0], 0, x);
            }
            else if(stmnt->args[1].a != IR::ArgBase::Stk)
               putStmntPushArg(stmnt->args[1], 0, x);
         }

         //
         // Info::trStmnt_Move_W
         //
         void Info::trStmnt_Move_W()
         {
            CheckArgC(stmnt, 2);
            CheckArg(stmnt->args[0], stmnt->pos);
            CheckArg(stmnt->args[1], stmnt->pos);

            #define moveIdx(name, n) \
               moveArgStk_W_src(*stmnt->args[n].a##name.idx, IR::Code::Move_W)

            // Push to stack?
            if(stmnt->args[0].a == IR::ArgBase::Stk) switch(stmnt->args[1].a)
            {
            case IR::ArgBase::GblReg: break;
            case IR::ArgBase::Lit:    break;
            case IR::ArgBase::LocReg: break;
            case IR::ArgBase::MapReg: break;
            case IR::ArgBase::WldReg: break;

            case IR::ArgBase::GblArr: moveIdx(GblArr, 1); break;
            case IR::ArgBase::Loc:    moveIdx(Loc,    1); break;
            case IR::ArgBase::LocArs: moveIdx(LocArs, 1); break;
            case IR::ArgBase::MapArr: moveIdx(MapArr, 1); break;
            case IR::ArgBase::WldArr: moveIdx(WldArr, 1); break;

            default:
               throw Core::ExceptStr(stmnt->pos, "bad tr Move_W push");
            }

            // Drop from stack?
            else if(stmnt->args[1].a == IR::ArgBase::Stk) switch(stmnt->args[0].a)
            {
            case IR::ArgBase::GblReg: break;
            case IR::ArgBase::LocReg: break;
            case IR::ArgBase::MapReg: break;
            case IR::ArgBase::Nul:    break;
            case IR::ArgBase::WldReg: break;

            case IR::ArgBase::GblArr: moveIdx(GblArr, 0); break;
            case IR::ArgBase::Loc:    moveIdx(Loc,    0); break;
            case IR::ArgBase::LocArs: moveIdx(LocArs, 0); break;
            case IR::ArgBase::MapArr: moveIdx(MapArr, 0); break;
            case IR::ArgBase::WldArr: moveIdx(WldArr, 0); break;

            default:
               throw Core::ExceptStr(stmnt->pos, "bad tr Move_W drop");
            }

            // Neither stack, split move and rescan.
            else
               moveArgStk_W_src(stmnt->args[1], IR::Code::Move_W);

            #undef moveIdx
         }

         //
         // trStmnt_Move_Wx
         //
         void Info::trStmnt_Move_Wx()
         {
            CheckArgC(stmnt, 2);
            CheckArg(stmnt->args[0], stmnt->pos);
            CheckArg(stmnt->args[1], stmnt->pos);
         }
      }
   }
}

// EOF

