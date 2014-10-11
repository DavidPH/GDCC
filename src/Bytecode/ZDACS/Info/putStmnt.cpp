//-----------------------------------------------------------------------------
//
// Copyright (C) 2013-2014 David Hill
//
// See COPYING for license information.
//
//-----------------------------------------------------------------------------
//
// ZDoom ACSE statement output.
//
//-----------------------------------------------------------------------------

#include "Bytecode/ZDACS/Info.hpp"

#include "Bytecode/ZDACS/Code.hpp"

#include "Core/Exception.hpp"
#include "Core/Range.hpp"

#include "IR/CallType.hpp"
#include "IR/Function.hpp"
#include "IR/Glyph.hpp"

#include <iostream>


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
         // Info::putStmnt
         //
         void Info::putStmnt()
         {
            switch(stmnt->code)
            {
            case IR::Code::Nop:
               putCode(Code::Nop);
               break;

            case IR::Code::AddI_W:
            case IR::Code::AddU_W:
               putCode(Code::AddU);
               break;

            case IR::Code::AndU_W:
               putCode(Code::AndU);
               break;

            case IR::Code::Call:
               putStmnt_Call();
               break;

            case IR::Code::Casm:
               for(auto const &arg : stmnt->args)
                  putWord(GetWord(arg.aLit.value));
               break;

            case IR::Code::Cjmp_Nil:
               putCode(Code::Cjmp_Nil);
               putWord(GetWord(stmnt->args[1].aLit.value));
               break;

            case IR::Code::Cjmp_Tru:
               putCode(Code::Cjmp_Tru);
               putWord(GetWord(stmnt->args[1].aLit.value));
               break;

            case IR::Code::CmpI_EQ_W:
            case IR::Code::CmpU_EQ_W:
               putCode(Code::CmpU_EQ);
               break;

            case IR::Code::CmpI_GE_W:
               putCode(Code::CmpI_GE);
               break;

            case IR::Code::CmpI_GT_W:
               putCode(Code::CmpI_GT);
               break;

            case IR::Code::CmpI_LE_W:
               putCode(Code::CmpI_LE);
               break;

            case IR::Code::CmpI_LT_W:
               putCode(Code::CmpI_LT);
               break;

            case IR::Code::CmpI_NE_W:
            case IR::Code::CmpU_NE_W:
               putCode(Code::CmpU_NE);
               break;

            case IR::Code::CmpU_GE_W:
               putCode(Code::Call_Lit);
               putWord(GetWord(resolveGlyph("___GDCC__CmpU_GE_W")));
               break;

            case IR::Code::CmpU_GT_W:
               putCode(Code::Call_Lit);
               putWord(GetWord(resolveGlyph("___GDCC__CmpU_GT_W")));
               break;

            case IR::Code::CmpU_LE_W:
               putCode(Code::Call_Lit);
               putWord(GetWord(resolveGlyph("___GDCC__CmpU_LE_W")));
               break;

            case IR::Code::CmpU_LT_W:
               putCode(Code::Call_Lit);
               putWord(GetWord(resolveGlyph("___GDCC__CmpU_LT_W")));
               break;

            case IR::Code::Cnat:
               putCode(Code::Cnat);
               putWord(GetWord(stmnt->args[0].aLit.value));
               putWord(stmnt->args.size() - 2);
               break;

            case IR::Code::Cspe:
               putStmnt_Cspe();
               break;

            case IR::Code::DivI_W:
               putCode(Code::DivI);
               break;

            case IR::Code::DivX_W:
               putCode(Code::DivX);
               break;

            case IR::Code::InvU_W:
               putCode(Code::InvU);
               break;

            case IR::Code::Jump:
               putCode(Code::Jump_Lit);
               putWord(GetWord(stmnt->args[0].aLit.value));
               break;

            case IR::Code::ModI_W:
               putCode(Code::ModI);
               break;

            case IR::Code::Move_W:
               putStmnt_Move_W();
               break;

            case IR::Code::MulI_W:
            case IR::Code::MulU_W:
               putCode(Code::MulU);
               break;

            case IR::Code::MulX_W:
               putCode(Code::MulX);
               break;

            case IR::Code::NegI_W:
               putCode(Code::NegI);
               break;

            case IR::Code::NotU_W:
               putCode(Code::NotU);
               break;

            case IR::Code::OrIU_W:
               putCode(Code::OrIU);
               break;

            case IR::Code::OrXU_W:
               putCode(Code::OrXU);
               break;

            case IR::Code::Plsa:
               putCode(Code::Push_Lit);
               putWord(0);
               putCode(Code::Push_Lit);
               putWord(GetWord(stmnt->args[0].aLit.value));
               putCode(Code::Call_Lit);
               putWord(GetWord(resolveGlyph("___GDCC__alloc")));
               putCode(Code::Drop_LocReg);
               putWord(getStkPtrIdx());
               break;

            case IR::Code::Plsf:
               putCode(Code::Push_LocReg);
               putWord(getStkPtrIdx());
               putCode(Code::Push_Lit);
               putWord(0);
               putCode(Code::Call_Nul);
               putWord(GetWord(resolveGlyph("___GDCC__alloc")));
               break;

            case IR::Code::Pltn:
               putCode(Code::Push_LocReg);
               putWord(getStkPtrIdx());
               putCode(Code::AddU);
               break;

            case IR::Code::Retn:
               putStmnt_Retn();
               break;

            case IR::Code::ShLU_W:
               putCode(Code::ShLU);
               break;

            case IR::Code::ShRI_W:
               putCode(Code::ShRI);
               break;

            case IR::Code::ShRU_W:
               putStmnt_ShRU_W();
               break;

            case IR::Code::SubI_W:
            case IR::Code::SubU_W:
               putCode(Code::SubU);
               break;

            case IR::Code::Swap_W:
               putCode(Code::Swap);
               break;

            default:
               std::cerr << "ERROR: " << stmnt->pos
                  << ": cannot put Code for ZDACS: " << stmnt->code << '\n';
               throw EXIT_FAILURE;
            }
         }

         //
         // Info::putStmnt_Call
         //
         void Info::putStmnt_Call()
         {
            auto ret = stmnt->args[1].aLit.value->getValue().getFastU();

            switch(stmnt->args[0].a)
            {
            case IR::ArgBase::Lit:
               if(ret == 0)
                  putCode(Code::Call_Nul);
               else
                  putCode(Code::Call_Lit);

               putWord(GetWord(stmnt->args[0].aLit.value));

               if(ret)
                  putStmntPushRetn(ret);

               break;

            case IR::ArgBase::Stk:
               putCode(Code::Call_Stk);

               if(ret == 0)
                  putCode(Code::Drop_Nul);
               else
                  putStmntPushRetn(ret);

               break;

            default:
               std::cerr << "ERROR: " << stmnt->pos << ": bad Call\n";
               throw EXIT_FAILURE;
            }
         }

         //
         // Info::putStmnt_Cspe
         //
         void Info::putStmnt_Cspe()
         {
            auto ret = stmnt->args[1].aLit.value->getValue().getFastU();

            IR::ArgBase a;
            if(stmnt->args.size() == 2)
               a = ret ? IR::ArgBase::Stk : IR::ArgBase::Lit;
            else
               a = stmnt->args[2].a;

            switch(a)
            {
            case IR::ArgBase::Lit:
               if(ret)
               {
                  for(auto const &arg : Core::MakeRange(stmnt->args.begin() + 2, stmnt->args.end()))
                  {
                     putCode(Code::Push_Lit);
                     putWord(GetWord(arg.aLit.value));
                  }

                  putCode(Code::Cspe_5R1); break;
                  putWord(GetWord(stmnt->args[0].aLit.value));

                  break;
               }

               switch(stmnt->args.size() - 2)
               {
               case 0: putCode(Code::Cspe_1L); break;
               case 1: putCode(Code::Cspe_1L); break;
               case 2: putCode(Code::Cspe_2L); break;
               case 3: putCode(Code::Cspe_3L); break;
               case 4: putCode(Code::Cspe_4L); break;
               case 5: putCode(Code::Cspe_5L); break;
               }

               putWord(GetWord(stmnt->args[0].aLit.value));

               // Dummy arg.
               if(stmnt->args.size() == 2)
                  putWord(0);

               for(auto const &arg : Core::MakeRange(stmnt->args.begin() + 2, stmnt->args.end()))
                  putWord(GetWord(arg.aLit.value));

               break;

            case IR::ArgBase::Stk:
               if(ret)
               {
                  switch(stmnt->args.size() - 2)
                  {
                  case 0: putCode(Code::Push_Lit); putWord(0);
                  case 1: putCode(Code::Push_Lit); putWord(0);
                  case 2: putCode(Code::Push_Lit); putWord(0);
                  case 3: putCode(Code::Push_Lit); putWord(0);
                  case 4: putCode(Code::Push_Lit); putWord(0);
                  case 5: putCode(Code::Cspe_5R1); break;
                  }
               }
               else
               {
                  switch(stmnt->args.size() - 2)
                  {
                  case 0: putCode(Code::Push_Lit); putWord(0);
                  case 1: putCode(Code::Cspe_1); break;
                  case 2: putCode(Code::Cspe_2); break;
                  case 3: putCode(Code::Cspe_3); break;
                  case 4: putCode(Code::Cspe_4); break;
                  case 5: putCode(Code::Cspe_5); break;
                  }
               }

               putWord(GetWord(stmnt->args[0].aLit.value));

               break;

            default:
               std::cerr << "ERROR: " << stmnt->pos << ": bad Cspe\n";
               throw EXIT_FAILURE;
            }
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

            case IR::ArgBase::GblReg:
               putStmnt_Move_W__Stk_Reg(stmnt->args[1].aGblReg, Code::Push_GblReg);
               break;

            case IR::ArgBase::Lit:
               putStmnt_Move_W__Stk_Lit(stmnt->args[1].aLit.value);
               break;

            case IR::ArgBase::Loc:
               putCode(Code::Push_LocReg);
               putWord(getStkPtrIdx());
               putCode(Code::AddU);

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

            case IR::ArgBase::LocReg:
               putStmnt_Move_W__Stk_Reg(stmnt->args[1].aLocReg, Code::Push_LocReg);
               break;

            case IR::ArgBase::MapArr:
               putStmnt_Move_W__Stk_Arr(stmnt->args[1].aMapArr, Code::Push_MapArr);
               break;

            case IR::ArgBase::MapReg:
               putStmnt_Move_W__Stk_Reg(stmnt->args[1].aMapReg, Code::Push_MapReg);
               break;

            case IR::ArgBase::WldArr:
               putStmnt_Move_W__Stk_Arr(stmnt->args[1].aWldArr, Code::Push_WldArr);
               break;

            case IR::ArgBase::WldReg:
               putStmnt_Move_W__Stk_Reg(stmnt->args[1].aWldReg, Code::Push_WldReg);
               break;

            default:
               std::cerr << "bad Code::Move_W(Stk, ?)\n";
               throw EXIT_FAILURE;
            }

            // drop_?
            else if(stmnt->args[1].a == IR::ArgBase::Stk) switch(stmnt->args[0].a)
            {
            case IR::ArgBase::GblArr:
               putStmnt_Move_W__Arr_Stk(stmnt->args[0].aGblArr, Code::Drop_GblArr);
               break;

            case IR::ArgBase::GblReg:
               putStmnt_Move_W__Reg_Stk(stmnt->args[0].aGblReg, Code::Drop_GblReg);
               break;

            case IR::ArgBase::Loc:
               putCode(Code::Push_LocReg);
               putWord(getStkPtrIdx());
               putCode(Code::AddU);

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

            case IR::ArgBase::LocReg:
               putStmnt_Move_W__Reg_Stk(stmnt->args[0].aLocReg, Code::Drop_LocReg);
               break;

            case IR::ArgBase::MapArr:
               putStmnt_Move_W__Arr_Stk(stmnt->args[0].aMapArr, Code::Drop_MapArr);
               break;

            case IR::ArgBase::MapReg:
               putStmnt_Move_W__Reg_Stk(stmnt->args[0].aMapReg, Code::Drop_MapReg);
               break;

            case IR::ArgBase::Nul:
               putCode(Code::Drop_Nul);
               break;

            case IR::ArgBase::WldArr:
               putStmnt_Move_W__Arr_Stk(stmnt->args[0].aWldArr, Code::Drop_WldArr);
               break;

            case IR::ArgBase::WldReg:
               putStmnt_Move_W__Reg_Stk(stmnt->args[0].aWldReg, Code::Drop_WldReg);
               break;

            default:
               std::cerr << "bad Code::Move_W(?, Stk)\n";
               throw EXIT_FAILURE;
            }

            // ???
            else
            {
               std::cerr << "bad Code::Move_W\n";
               throw EXIT_FAILURE;
            }
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
         // Info::putStmnt_Move_W__Reg_Stk
         //
         void Info::putStmnt_Move_W__Reg_Stk(IR::ArgPtr1 const &reg, Code code)
         {
            putCode(code);
            putWord(GetWord(IR::ExpCreate_AddPtrRaw(reg.idx->aLit.value, reg.off)));
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
         // Info::putStmnt_Move_W__Stk_Lit
         //
         void Info::putStmnt_Move_W__Stk_Lit(IR::Exp const *exp)
         {
            auto val = exp->getValue();

            switch(val.v)
            {
            case IR::ValueBase::Funct:
               if(val.vFunct.vtype.callT == IR::CallType::ScriptS)
                  putStmntPushStrEn(val.vFunct.value);
               else
                  putStmntPushFunct(val.vFunct.value);
               break;

            case IR::ValueBase::StrEn:
               putStmntPushStrEn(val.vStrEn.value);
               break;

            default:
               putCode(Code::Push_Lit);
               putWord(GetWord(exp));
               break;
            }
         }

         //
         // Info::putStmnt_Move_W__Stk_Reg
         //
         void Info::putStmnt_Move_W__Stk_Reg(IR::ArgPtr1 const &reg, Code code)
         {
            putCode(code);
            putWord(GetWord(IR::ExpCreate_AddPtrRaw(reg.idx->aLit.value, reg.off)));
         }

         //
         // Info::putStmnt_Retn
         //
         void Info::putStmnt_Retn()
         {
            switch(func->ctype)
            {
            case IR::CallType::StdCall:
            case IR::CallType::StkCall:
               if(stmnt->args.size() == 0)
                  putCode(Code::Retn_Nul);
               else
               {
                  putStmntDropRetn(stmnt->args.size());
                  putCode(Code::Retn_Stk);
               }
               break;

            case IR::CallType::ScriptI:
            case IR::CallType::ScriptS:
               if(stmnt->args.size() == 0)
               {
                  putCode(Code::Rscr);
               }
               else if(stmnt->args.size() == 1)
               {
                  putCode(Code::Drop_ScrRet);
                  putCode(Code::Rscr);
               }
               else
               {
                  std::cerr << "STUB: " __FILE__ << ':' << __LINE__ << '\n';
                  throw EXIT_FAILURE;
               }
               break;

            default:
               std::cerr << "ERROR: " << stmnt->pos << ": bad Code::Retn\n";
               throw EXIT_FAILURE;
            }
         }

         //
         // Info::putStmnt_ShRU_W
         //
         void Info::putStmnt_ShRU_W()
         {
            auto CaseXReg = [this](Code code)
            {
               auto idx = IR::ExpCreate_AddPtrRaw(
                  stmnt->args[2].aLocReg.idx->aLit.value,
                  stmnt->args[2].aLocReg.off);

               putCode(code);
               putWord(GetWord(idx));

               // If shift is 0, jump to end.
               putCode(Code::Cjmp_Lit);
               putWord(0);
               putWord(putPos + 48);

               putCode(Code::ShRI);
               putCode(Code::Push_Lit);
               putWord(0x80000000);
               putCode(code);
               putWord(GetWord(idx));
               putCode(Code::Push_Lit);
               putWord(1);
               putCode(Code::SubU);
               putCode(Code::ShRI);
               putCode(Code::InvU);
               putCode(Code::AndU);
            };

            Core::FastU val;

            switch(stmnt->args[2].a)
            {
            case IR::ArgBase::GblReg:
               CaseXReg(Code::Push_GblReg);
               break;

            case IR::ArgBase::Lit:
               val = stmnt->args[2].aLit.value->getValue().getFastU();

               putCode(Code::Push_Lit);
               putWord(val);
               putCode(Code::ShRI);
               putCode(Code::Push_Lit);
               putWord(val < 32 ? 0xFFFFFFFFU >> val : 0xFFFFFFFF);
               putCode(Code::AndU);
               break;

            case IR::ArgBase::LocReg:
               CaseXReg(Code::Push_LocReg);
               break;

            case IR::ArgBase::MapReg:
               CaseXReg(Code::Push_MapReg);
               break;

            case IR::ArgBase::Stk:
               // If shift is 0, jump to end.
               putCode(Code::Cjmp_Lit);
               putWord(0);
               putWord(putPos + 60);

               putCode(Code::Drop_LocReg);
               putWord(func->localReg + 0);
               putCode(Code::Push_LocReg);
               putWord(func->localReg + 0);
               putCode(Code::ShRI);
               putCode(Code::Push_Lit);
               putWord(0x80000000);
               putCode(Code::DecU_LocReg);
               putWord(func->localReg + 0);
               putCode(Code::Push_LocReg);
               putWord(func->localReg + 0);
               putCode(Code::ShRI);
               putCode(Code::InvU);
               putCode(Code::AndU);
               break;

            case IR::ArgBase::WldReg:
               CaseXReg(Code::Push_WldReg);
               break;

            default:
               throw Core::ExceptStr(stmnt->pos, "bad put Code::ShRU_W");
            }
         }

         //
         // Info::putStmntDropRetn
         //
         void Info::putStmntDropRetn(Core::FastU ret)
         {
            if(ret) for(Core::FastU i = 0; ++i != ret;)
            {
               putCode(Code::Push_Lit);
               putWord(-i);
               putCode(Code::Swap);
               putCode(Code::Drop_GblArr);
               putWord(LocArsArray);
            }
         }

         //
         // Info::putStmntPushFunct
         //
         void Info::putStmntPushFunct(Core::FastU value)
         {
            if((value & 0xFFFFFFFF) == 0)
               putCode(Code::Push_Lit);
            else
               putCode(Code::Pfun_Lit);

            putWord(value);
         }

         //
         // Info::putStmntPushRetn
         //
         void Info::putStmntPushRetn(Core::FastU ret)
         {
            if(ret) while(ret-- != 1)
            {
               putCode(Code::Push_Lit);
               putWord(-ret);
               putCode(Code::Push_GblArr);
               putWord(LocArsArray);
            }
         }

         //
         // Info::putStmntPushStrEn
         //
         void Info::putStmntPushStrEn(Core::FastU value)
         {
            putCode(Code::Push_Lit);
            putWord(value);

            if((value & 0xFFFFFFFF) == 0xFFFFFFFF)
               putCode(Code::Nop);
            else
               putCode(Code::Pstr_Stk);
         }
      }
   }
}

// EOF

