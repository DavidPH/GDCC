//-----------------------------------------------------------------------------
//
// Copyright (C) 2014-2016 David Hill
//
// See COPYING for license information.
//
//-----------------------------------------------------------------------------
//
// Addition and subtraction statement handling.
//
//-----------------------------------------------------------------------------

#include "BC/ZDACS/Info.hpp"

#include "BC/ZDACS/Code.hpp"

#include "Core/Exception.hpp"

#include "IR/Program.hpp"


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
         // Info::genStmnt_AdXU_W
         //
         void Info::genStmnt_AdXU_W()
         {
            Core::FastU lop = stmnt->args.size() == 3 ? 1 : 2, rop = lop + 1;
            Core::FastU lenAdXU_W1 =
               lenPushArg(stmnt->args[lop], 0) * 2 +
               lenPushArg(stmnt->args[rop], 0) + 16;

            // No carry.
            if(stmnt->args.size() == 3)
               numChunkCODE += lenAdXU_W1;

            // With carry.
            else
               numChunkCODE += lenAdXU_W1 * 2 + 56;
         }

         //
         // Info::genStmnt_AddU_W
         //
         void Info::genStmnt_AddU_W()
         {
            if(stmnt->op.size != 1)
            {
               genStmntCall(stmnt->op.size);
               return;
            }

            if(stmnt->args[0].a == IR::ArgBase::Stk)
               numChunkCODE += 4;
            else
               numChunkCODE += 8;
         }

         //
         // Info::genStmnt_SuXU_W
         //
         void Info::genStmnt_SuXU_W()
         {
            Core::FastU lop = stmnt->args.size() == 3 ? 1 : 2, rop = lop + 1;
            Core::FastU lenSuXU_W1 =
               lenPushArg(stmnt->args[lop], 0) * 2 +
               lenPushArg(stmnt->args[rop], 0) + 20;

            // No carry.
            if(stmnt->args.size() == 3)
               numChunkCODE += lenSuXU_W1;

            // With carry.
            else
               numChunkCODE += lenSuXU_W1 * 2 + 64;
         }

         //
         // Info::preStmnt_AdXU_W
         //
         void Info::preStmnt_AdXU_W()
         {
            preStmnt_CmpU_W1(IR::Code::CmpU_LT_W, IR::Code::CmpI_LT_W, false, true);
         }

         //
         // Info::preStmnt_AddF_W
         //
         void Info::preStmnt_AddF_W()
         {
            if(stmnt->op.size == 0)
               return;

            addFunc_AddF_W(stmnt->op.size);
         }

         //
         // Info::preStmnt_AddU_W
         //
         void Info::preStmnt_AddU_W()
         {
            if(stmnt->op.size <= 1)
               return;

            addFunc_AddU_W(stmnt->op.size);
         }

         //
         // Info::preStmnt_SuXU_W
         //
         void Info::preStmnt_SuXU_W()
         {
            preStmnt_CmpU_W1(IR::Code::CmpU_GT_W, IR::Code::CmpI_GT_W, true, false);
         }

         //
         // Info::preStmnt_SubF_W
         //
         void Info::preStmnt_SubF_W()
         {
            if(stmnt->op.size == 0)
               return;

            Core::String name = getCallName();
            auto newFunc = preStmntCallDef(name, 1, stmnt->op.size * 2,
               stmnt->op.size * 3 + 2, __FILE__, __LINE__);

            if(!newFunc)
               return;

            FloatInfo fi = getFloatInfo(stmnt->op.size);

            IR::Glyph labelLPos{prog, name + "$lpos"};
            IR::Glyph labelRPos{prog, name + "$rpos"};

            IR::Glyph labelLEMax{prog, name + "$lemax"};
            IR::Glyph labelLEMin{prog, name + "$lemin"};
            IR::Glyph labelREMax{prog, name + "$remax"};
            IR::Glyph labelREMin{prog, name + "$remin"};

            IR::Glyph labelLREMax{prog, name + "$lremax"};

            IR::Glyph labelLGTR{prog, name + "$lgtr"};
            IR::Glyph labelLLTR{prog, name + "$lltr"};

            IR::Glyph labelNeg0{prog, name + "$neg0"};
            IR::Glyph labelPos0{prog, name + "$pos0"};

            IR::Arg_LocReg lop {IR::Arg_Lit(newFunc->block.getExp(stmnt->op.size * 0))};
            IR::Arg_LocReg lhi {IR::Arg_Lit(newFunc->block.getExp(stmnt->op.size * 1 - 1))};
            IR::Arg_LocReg rop {IR::Arg_Lit(newFunc->block.getExp(stmnt->op.size * 1))};
            IR::Arg_LocReg rhi {IR::Arg_Lit(newFunc->block.getExp(stmnt->op.size * 2 - 1))};
            IR::Arg_LocReg tmp {IR::Arg_Lit(newFunc->block.getExp(stmnt->op.size * 2))};
            IR::Arg_LocReg thi {IR::Arg_Lit(newFunc->block.getExp(stmnt->op.size * 3 - 1))};
            IR::Arg_LocReg expL{IR::Arg_Lit(newFunc->block.getExp(stmnt->op.size * 3 + 0))};
            IR::Arg_LocReg expR{IR::Arg_Lit(newFunc->block.getExp(stmnt->op.size * 3 + 1))};

            IR::Arg_Nul nul{};
            IR::Arg_Stk stk{};

            #define AS_Stmnt newFunc->block.addStatementArgs

            // +0 - +0 = +0
            AS_Stmnt({IR::Code::BOrI_W,   stmnt->op.size}, stk, lop, rop);
            AS_Stmnt({IR::Code::Jcnd_Nil, stmnt->op.size}, stk, labelPos0);

            // -0 - -0 = +0
            AS_Stmnt({IR::Code::BOrI_W,   stmnt->op.size}, stk, lop, rop);
            AS_Stmnt({IR::Code::CmpU_NE_W, 1},             stk, stk, fi.maskSig);
            AS_Stmnt({IR::Code::Jcnd_Nil, stmnt->op.size}, stk, labelPos0);

            // Is l negative? l - r = -(-l + r)
            AS_Stmnt({IR::Code::BAnd_W,   1},              stk, lhi, fi.maskSig);
            AS_Stmnt({IR::Code::Jcnd_Nil, 1},              stk, labelLPos);
            AS_Stmnt({IR::Code::NegF_W,   stmnt->op.size}, stk, lop);
            AS_Stmnt({IR::Code::Move_W,   stmnt->op.size}, stk, rop);
            AS_Stmnt({IR::Code::AddF_W,   stmnt->op.size}, stk, stk, stk);
            AS_Stmnt({IR::Code::NegF_W,   stmnt->op.size}, stk, stk);
            AS_Stmnt({IR::Code::Retn,     stmnt->op.size}, stk);
            newFunc->block.addLabel(labelLPos);

            // Is r negative? l - r = l + -r
            AS_Stmnt({IR::Code::BAnd_W,   1},              stk, rhi, fi.maskSig);
            AS_Stmnt({IR::Code::Jcnd_Nil, 1},              stk, labelRPos);
            AS_Stmnt({IR::Code::Move_W,   stmnt->op.size}, stk, lop);
            AS_Stmnt({IR::Code::NegF_W,   stmnt->op.size}, stk, rop);
            AS_Stmnt({IR::Code::AddF_W,   stmnt->op.size}, stk, stk, stk);
            AS_Stmnt({IR::Code::Retn,     stmnt->op.size}, stk);
            newFunc->block.addLabel(labelRPos);

            // Does l have specal exponent?
            AS_Stmnt({IR::Code::BAnd_W,   1}, stk, lhi, fi.maskExp);
            AS_Stmnt({IR::Code::Jcnd_Tab, 1}, stk, fi.maskExp, labelLEMax, 0, labelLEMin);
            AS_Stmnt({IR::Code::ShRI_W,   1}, expL, stk, fi.bitsMan);

            // Does r have specal exponent?
            AS_Stmnt({IR::Code::BAnd_W,   1}, stk, rhi, fi.maskExp);
            AS_Stmnt({IR::Code::Jcnd_Tab, 1}, stk, fi.maskExp, labelREMax, 0, labelREMin);
            AS_Stmnt({IR::Code::ShRI_W,   1}, expR, stk, fi.bitsMan);

            // Both are normalized.

            // Is l > r?
            AS_Stmnt({IR::Code::CmpI_GT_W, stmnt->op.size}, stk, lop, rop);
            AS_Stmnt({IR::Code::Jcnd_Tru,  1},              stk, labelLGTR);

            // Is l < r?
            AS_Stmnt({IR::Code::CmpI_LT_W, stmnt->op.size}, stk, lop, rop);
            AS_Stmnt({IR::Code::Jcnd_Tru,  1},              stk, labelLLTR);

            // l == r, return +0.
            AS_Stmnt({IR::Code::Retn, stmnt->op.size}, 0);

            // l > r

            newFunc->block.addLabel(labelLGTR);

            // Calculate exponent difference.
            AS_Stmnt({IR::Code::SubU_W, 1}, tmp, expL, expR);

            // If difference >= total mantissa bits, r is too small to affect l.
            AS_Stmnt({IR::Code::CmpI_GE_W, 1}, stk, tmp, fi.bitsManFull + 1);
            AS_Stmnt({IR::Code::Jcnd_Tru,  1}, stk, labelREMin);

            // tmp = l.manfull - (r.manfull >> difference);
            AS_Stmnt({IR::Code::Move_W, stmnt->op.size}, stk, lop);
            AS_Stmnt({IR::Code::BAnd_W, 1},              stk, stk, fi.maskMan);
            AS_Stmnt({IR::Code::BOrI_W, 1},              stk, stk, fi.maskMan + 1);
            AS_Stmnt({IR::Code::Move_W, stmnt->op.size}, stk, rop);
            AS_Stmnt({IR::Code::BAnd_W, 1},              stk, stk, fi.maskMan);
            AS_Stmnt({IR::Code::BOrI_W, 1},              stk, stk, fi.maskMan + 1);
            AS_Stmnt({IR::Code::ShRI_W, stmnt->op.size}, stk, stk, tmp);
            AS_Stmnt({IR::Code::SubU_W, stmnt->op.size}, tmp, stk, stk);

            // Adjust mantissa to cover hidden bit.
            AS_Stmnt({IR::Code::Bclz_W, stmnt->op.size}, expR, tmp);
            AS_Stmnt({IR::Code::SubU_W, 1},              expR, expR, fi.bitsExp);
            AS_Stmnt({IR::Code::SubU_W, 1},              expL, expL, expR);
            AS_Stmnt({IR::Code::ShLU_W, stmnt->op.size}, tmp,  tmp,  expR);

            // If exponent <= 0, return +0.
            AS_Stmnt({IR::Code::CmpI_LE_W, 1}, stk, expL, 0);
            AS_Stmnt({IR::Code::Jcnd_Tru,  1}, stk, labelPos0);

            // Otherwise, combine exponent and mantissa to form result.
            AS_Stmnt({IR::Code::Move_W, stmnt->op.size}, stk, tmp);
            AS_Stmnt({IR::Code::BAnd_W, 1},              stk, stk, fi.maskMan);
            AS_Stmnt({IR::Code::ShLU_W, 1},              stk, expL, fi.bitsMan);
            AS_Stmnt({IR::Code::BOrI_W, 1},              stk, stk, stk);
            AS_Stmnt({IR::Code::Retn,   stmnt->op.size}, stk);

            // l < r

            newFunc->block.addLabel(labelLLTR);

            // Calculate exponent difference.
            AS_Stmnt({IR::Code::SubU_W, 1}, tmp, expR, expL);

            // If difference >= total mantissa bits, l is too small to affect r.
            AS_Stmnt({IR::Code::CmpI_GE_W, 1}, stk, tmp, fi.bitsManFull + 1);
            AS_Stmnt({IR::Code::Jcnd_Tru,  1}, stk, labelLEMin);

            // tmp = -(r.manfull - (l.manfull >> difference));
            AS_Stmnt({IR::Code::Move_W, stmnt->op.size}, stk, rop);
            AS_Stmnt({IR::Code::BAnd_W, 1},              stk, stk, fi.maskMan);
            AS_Stmnt({IR::Code::BOrI_W, 1},              stk, stk, fi.maskMan + 1);
            AS_Stmnt({IR::Code::Move_W, stmnt->op.size}, stk, lop);
            AS_Stmnt({IR::Code::BAnd_W, 1},              stk, stk, fi.maskMan);
            AS_Stmnt({IR::Code::BOrI_W, 1},              stk, stk, fi.maskMan + 1);
            AS_Stmnt({IR::Code::ShRI_W, stmnt->op.size}, stk, stk, tmp);
            AS_Stmnt({IR::Code::SubU_W, stmnt->op.size}, tmp, stk, stk);

            // Adjust mantissa to cover hidden bit.
            AS_Stmnt({IR::Code::Bclz_W, stmnt->op.size}, expL, tmp);
            AS_Stmnt({IR::Code::SubU_W, 1},              expL, expL, fi.bitsExp);
            AS_Stmnt({IR::Code::SubU_W, 1},              expR, expR, expL);
            AS_Stmnt({IR::Code::ShLU_W, stmnt->op.size}, tmp,  tmp,  expL);

            // If exponent <= 0, return -0.
            AS_Stmnt({IR::Code::CmpI_LE_W, 1}, stk, expR, 0);
            AS_Stmnt({IR::Code::Jcnd_Tru,  1}, stk, labelNeg0);

            // Otherwise, combine exponent and mantissa to form result.
            AS_Stmnt({IR::Code::Move_W, stmnt->op.size}, stk, tmp);
            AS_Stmnt({IR::Code::BAnd_W, 1},              stk, stk, fi.maskMan);
            AS_Stmnt({IR::Code::ShLU_W, 1},              stk, expR, fi.bitsMan);
            AS_Stmnt({IR::Code::BOrI_W, 1},              stk, stk, stk);
            AS_Stmnt({IR::Code::NegF_W, stmnt->op.size}, stk, stk);
            AS_Stmnt({IR::Code::Retn,   stmnt->op.size}, stk);

            // l has max exponent. It is either INF or NaN.
            newFunc->block.addLabel(labelLEMax);
            // Check r for max exponent.
            AS_Stmnt({IR::Code::BAnd_W,   1}, stk, rhi, fi.maskExp);
            AS_Stmnt({IR::Code::Jcnd_Tab, 1}, stk, fi.maskExp, labelLREMax);
            AS_Stmnt({IR::Code::Move_W,   1}, nul, stk);

            // r has min exponent. Therefore, r == 0 and the result is l.
            newFunc->block.addLabel(labelREMin);
            AS_Stmnt({IR::Code::Retn, stmnt->op.size}, lop);

            // r has max exponent. It is either INF or NaN.
            newFunc->block.addLabel(labelREMax);
            // l has min exponent. Therefore, l == 0 and the result is -r.
            newFunc->block.addLabel(labelLEMin);
            AS_Stmnt({IR::Code::NegF_W, stmnt->op.size}, stk, rop);
            AS_Stmnt({IR::Code::Retn,   stmnt->op.size}, stk);

            // l and r have max exponent.
            newFunc->block.addLabel(labelLREMax);

            // Is l NaN? If so, return l.
            AS_Stmnt({IR::Code::Move_W, stmnt->op.size},   stk, lop);
            AS_Stmnt({IR::Code::BAnd_W, 1},                stk, stk, fi.maskMan);
            AS_Stmnt({IR::Code::Jcnd_Tru, stmnt->op.size}, stk, labelREMin);

            // Is r NaN? If so, return r. (Sign inversion is fine.)
            AS_Stmnt({IR::Code::Move_W, stmnt->op.size},   stk, rop);
            AS_Stmnt({IR::Code::BAnd_W, 1},                stk, stk, fi.maskMan);
            AS_Stmnt({IR::Code::Jcnd_Tru, stmnt->op.size}, stk, labelLEMin);

            // +INF - +INF = NaN.
            for(auto n = stmnt->op.size - 1; n--;)
               AS_Stmnt({IR::Code::Move_W, 1},              stk, 0xFFFFFFFF);
            AS_Stmnt({IR::Code::Move_W,    1},              stk, fi.maskExp | fi.maskMan);
            AS_Stmnt({IR::Code::Retn,      stmnt->op.size}, stk);

            // Return +0.
            newFunc->block.addLabel(labelPos0);
            AS_Stmnt({IR::Code::Retn, stmnt->op.size}, 0);

            // Return -0.
            newFunc->block.addLabel(labelNeg0);
            for(auto n = stmnt->op.size - 1; n--;)
               AS_Stmnt({IR::Code::Move_W, 1},              stk, 0);
            AS_Stmnt({IR::Code::Move_W,    1},              stk, fi.maskSig);
            AS_Stmnt({IR::Code::Retn,      stmnt->op.size}, stk);

            #undef AS_Stmnt

            throw ResetFunc();
         }

         //
         // Info::preStmnt_SubU_W
         //
         void Info::preStmnt_SubU_W()
         {
            if(stmnt->op.size <= 1)
               return;

            addFunc_SubU_W(stmnt->op.size);
         }

         //
         // Info::putStmnt_AdXU_W
         //
         void Info::putStmnt_AdXU_W()
         {
            Core::FastU lop = stmnt->args.size() == 3 ? 1 : 2, rop = lop + 1;

            //
            // putAdXU_W1
            //
            auto putAdXU_W1 = [&]()
            {
               putStmntPushArg(stmnt->args[lop], 0);
               putStmntPushArg(stmnt->args[rop], 0);
               putCode(Code::AddU);
               putCode(Code::Copy);
               putStmntPushArg(stmnt->args[lop], 0);
               putStmntCall("___GDCC__CmpU_LT_W1", 1);
            };

            // No carry.
            if(stmnt->args.size() == 3)
               putAdXU_W1();

            // With carry.
            else
            {
               Core::FastU lenAdXU_W1 =
                  lenPushArg(stmnt->args[lop], 0) * 2 +
                  lenPushArg(stmnt->args[rop], 0) + 16;

               Core::FastU lenCarry0 = lenAdXU_W1 +  8;
               Core::FastU lenCarry1 = lenAdXU_W1 + 40;

               putCode(Code::Jcnd_Tru, putPos + lenCarry0 + 8);

               putAdXU_W1();
               putCode(Code::Jump_Lit, putPos + lenCarry1 + 8);

               putAdXU_W1();
               putCode(Code::Drop_LocReg, func->localReg + 0);
               putCode(Code::Push_Lit,    1);
               putCode(Code::AddU);
               putCode(Code::Copy);
               putCode(Code::LNot);
               putCode(Code::Push_LocReg, func->localReg + 0);
               putCode(Code::BOrI);
            }
         }

         //
         // Info::putStmnt_AddU_W
         //
         void Info::putStmnt_AddU_W()
         {
            if(stmnt->op.size != 1)
            {
               putStmntCall(stmnt->op.size);
               return;
            }

            //
            // putReg
            //
            auto putReg = [this](IR::ArgPtr1 const &a, Code add, Code inc)
            {
               putCode(stmnt->args[2].a == IR::ArgBase::Lit ? inc : add);
               putWord(GetWord(a.idx->aLit) + a.off);
            };

            switch(stmnt->args[0].a)
            {
            case IR::ArgBase::GblReg:
               putReg(stmnt->args[0].aGblReg, Code::AddU_GblReg, Code::IncU_GblReg);
               break;

            case IR::ArgBase::HubReg:
               putReg(stmnt->args[0].aHubReg, Code::AddU_HubReg, Code::IncU_HubReg);
               break;

            case IR::ArgBase::LocReg:
               putReg(stmnt->args[0].aLocReg, Code::AddU_LocReg, Code::IncU_LocReg);
               break;

            case IR::ArgBase::ModReg:
               putReg(stmnt->args[0].aModReg, Code::AddU_ModReg, Code::IncU_ModReg);
               break;

            default:
               putCode(Code::AddU);
               break;
            }
         }

         //
         // Info::putStmnt_SuXU_W
         //
         void Info::putStmnt_SuXU_W()
         {
            Core::FastU lop = stmnt->args.size() == 3 ? 1 : 2, rop = lop + 1;

            //
            // putSuXU_W1
            //
            auto putSuXU_W1 = [&]()
            {
               putStmntPushArg(stmnt->args[lop], 0);
               putStmntPushArg(stmnt->args[rop], 0);
               putCode(Code::SubU);
               putCode(Code::Copy);
               putStmntPushArg(stmnt->args[lop], 0);
               putStmntCall("___GDCC__CmpU_GT_W1", 1);
               putCode(Code::NegI);
            };

            // No carry.
            if(stmnt->args.size() == 3)
               putSuXU_W1();

            // With carry.
            else
            {
               Core::FastU lenSuXU_W1 =
                  lenPushArg(stmnt->args[lop], 0) * 2 +
                  lenPushArg(stmnt->args[rop], 0) + 20;

               Core::FastU lenCarry0 = lenSuXU_W1 +  8;
               Core::FastU lenCarry1 = lenSuXU_W1 + 48;

               putCode(Code::Jcnd_Tru, putPos + lenCarry0 + 8);

               putSuXU_W1();
               putCode(Code::Jump_Lit, putPos + lenCarry1 + 8);

               putSuXU_W1();
               putCode(Code::Drop_LocReg, func->localReg + 0);
               putCode(Code::Push_Lit,    1);
               putCode(Code::SubU);
               putCode(Code::Copy);
               putCode(Code::BNot);
               putCode(Code::LNot);
               putCode(Code::NegI);
               putCode(Code::Push_LocReg, func->localReg + 0);
               putCode(Code::BOrI);
            }
         }

         //
         // Info::putStmnt_SubU_W
         //
         void Info::putStmnt_SubU_W()
         {
            if(stmnt->op.size != 1)
            {
               putStmntCall(stmnt->op.size);
               return;
            }

            //
            // putReg
            //
            auto putReg = [this](IR::ArgPtr1 const &a, Code sub, Code dec)
            {
               putCode(stmnt->args[2].a == IR::ArgBase::Lit ? dec : sub);
               putWord(GetWord(a.idx->aLit) + a.off);
            };

            switch(stmnt->args[0].a)
            {
            case IR::ArgBase::GblReg:
               putReg(stmnt->args[0].aGblReg, Code::SubU_GblReg, Code::DecU_GblReg);
               break;

            case IR::ArgBase::HubReg:
               putReg(stmnt->args[0].aHubReg, Code::SubU_HubReg, Code::DecU_HubReg);
               break;

            case IR::ArgBase::LocReg:
               putReg(stmnt->args[0].aLocReg, Code::SubU_LocReg, Code::DecU_LocReg);
               break;

            case IR::ArgBase::ModReg:
               putReg(stmnt->args[0].aModReg, Code::SubU_ModReg, Code::DecU_ModReg);
               break;

            default:
               putCode(Code::SubU);
               break;
            }
         }

         //
         // Info::trStmnt_AdXU_W
         //
         void Info::trStmnt_AdXU_W()
         {
            CheckArgC(stmnt, 3);

            if(stmnt->op.size != 1)
               throw Core::ExceptStr(stmnt->pos, "unsupported AdXU_W size");

            moveArgStk_dst(stmnt->args[0], 2);

            if(stmnt->args.size() > 3)
            {
               func->setLocalTmp(1);
               moveArgStk_src(stmnt->args[1], 1);
            }
         }

         //
         // Info::trStmnt_AddU_W
         //
         void Info::trStmnt_AddU_W()
         {
            if(stmnt->op.size != 1)
            {
               trStmntStk3(stmnt->op.size, stmnt->op.size, false);
               return;
            }

            if(stmnt->args[0] == stmnt->args[1]) switch(stmnt->args[0].a)
            {
            case IR::ArgBase::GblReg:
            case IR::ArgBase::HubReg:
            case IR::ArgBase::LocReg:
            case IR::ArgBase::ModReg:
               if(stmnt->args[2].a != IR::ArgBase::Lit ||
                  !stmnt->args[2].aLit.value->isValue() ||
                  GetWord(stmnt->args[2].aLit) != 1)
               {
                  moveArgStk_src(stmnt->args[2], stmnt->op.size);
               }

               return;

            default:
               break;
            }
            else if(stmnt->args[0] == stmnt->args[2]) switch(stmnt->args[0].a)
            {
            case IR::ArgBase::GblReg:
            case IR::ArgBase::HubReg:
            case IR::ArgBase::LocReg:
            case IR::ArgBase::ModReg:
               std::swap(stmnt->args[1], stmnt->args[2]);

               if(stmnt->args[2].a != IR::ArgBase::Lit ||
                  !stmnt->args[2].aLit.value->isValue() ||
                  GetWord(stmnt->args[2].aLit) != 1)
               {
                  moveArgStk_src(stmnt->args[2], stmnt->op.size);
               }

               return;

            default:
               break;
            }

            trStmntStk3(stmnt->op.size, stmnt->op.size, false);
         }

         //
         // Info::trStmnt_SuXU_W
         //
         void Info::trStmnt_SuXU_W()
         {
            CheckArgC(stmnt, 3);

            if(stmnt->op.size != 1)
               throw Core::ExceptStr(stmnt->pos, "unsupported SuXU_W size");

            moveArgStk_dst(stmnt->args[0], 2);

            if(stmnt->args.size() > 3)
            {
               func->setLocalTmp(1);
               moveArgStk_src(stmnt->args[1], 1);
            }
         }

         //
         // Info::trStmnt_SubU_W
         //
         void Info::trStmnt_SubU_W()
         {
            if(stmnt->op.size != 1)
            {
               trStmntStk3(stmnt->op.size, stmnt->op.size, true);
               return;
            }

            if(stmnt->args[0] == stmnt->args[1]) switch(stmnt->args[0].a)
            {
            case IR::ArgBase::GblReg:
            case IR::ArgBase::HubReg:
            case IR::ArgBase::LocReg:
            case IR::ArgBase::ModReg:
               if(stmnt->args[2].a != IR::ArgBase::Lit ||
                  !stmnt->args[2].aLit.value->isValue() ||
                  GetWord(stmnt->args[2].aLit) != 1)
               {
                  moveArgStk_src(stmnt->args[2], stmnt->op.size);
               }

               return;

            default:
               break;
            }

            trStmntStk3(stmnt->op.size, stmnt->op.size, true);
         }
      }
   }
}

// EOF

