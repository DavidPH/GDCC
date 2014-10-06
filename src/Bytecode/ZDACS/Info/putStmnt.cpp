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
               putWord(0);
               break;

            case IR::Code::AddI_W:
            case IR::Code::AddU_W:
               putWord(14); // add
               break;

            case IR::Code::AndU_W:
               putWord(72); // andbitwise
               break;

            case IR::Code::Call:
               putStmnt_Call();
               break;

            case IR::Code::Casm:
               for(auto const &arg : stmnt->args)
                  putExpWord(arg.aLit.value);
               break;

            case IR::Code::Cjmp_Nil:
               putWord(79); // ifnotgoto
               putExpWord(stmnt->args[1].aLit.value);
               break;

            case IR::Code::Cjmp_Tru:
               putWord(53); // ifgoto
               putExpWord(stmnt->args[1].aLit.value);
               break;

            case IR::Code::CmpI_EQ_W:
            case IR::Code::CmpU_EQ_W:
               putWord(19); // eq
               break;

            case IR::Code::CmpI_GE_W:
               putWord(24); // ge
               break;

            case IR::Code::CmpI_GT_W:
               putWord(22); // gt
               break;

            case IR::Code::CmpI_LE_W:
               putWord(23); // le
               break;

            case IR::Code::CmpI_LT_W:
               putWord(21); // lt
               break;

            case IR::Code::CmpI_NE_W:
            case IR::Code::CmpU_NE_W:
               putWord(20); // ne
               break;

            case IR::Code::Cnat:
               putWord(351); // callfunc
               putExpWord(stmnt->args[0].aLit.value);
               putWord(stmnt->args.size() - 2);
               break;

            case IR::Code::Cspe:
               putStmnt_Cspe();
               break;

            case IR::Code::DivI_W:
               putWord(17); // divide
               break;

            case IR::Code::DivX_W:
               putWord(137); // fixeddiv
               break;

            case IR::Code::InvU_W:
               putWord(330); // negatebinary
               break;

            case IR::Code::Jump:
               putWord(52); // goto
               putExpWord(stmnt->args[0].aLit.value);
               break;

            case IR::Code::ModI_W:
               putWord(18); // modulus
               break;

            case IR::Code::Move_W:
               putStmnt_Move_W();
               break;

            case IR::Code::MulI_W:
            case IR::Code::MulU_W:
               putWord(16); // multiply
               break;

            case IR::Code::MulX_W:
               putWord(136); // fixedmul
               break;

            case IR::Code::NegI_W:
               putWord(78); // unaryminus
               break;

            case IR::Code::NotU_W:
               putWord(75); // negatelogical
               break;

            case IR::Code::OrIU_W:
               putWord(73); // orbitwise
               break;

            case IR::Code::OrXU_W:
               putWord(74); // eorbitwise
               break;

            case IR::Code::Plsa:
               putWord(3); // pushnumber
               putWord(0);
               putWord(3); // pushnumber
               putExpWord(stmnt->args[0].aLit.value);
               putWord(203); // call
               putExpWord(resolveGlyph("___GDCC__alloc"));
               putWord(25); // assignscriptvar
               putWord(getStkPtrIdx());
               break;

            case IR::Code::Plsf:
               putWord(28); // pushscriptvar
               putWord(getStkPtrIdx());
               putWord(3); // pushnumber
               putWord(0);
               putWord(204); // calldiscard
               putExpWord(resolveGlyph("___GDCC__alloc"));
               break;

            case IR::Code::Pltn:
               putWord(28); // pushscriptvar
               putWord(getStkPtrIdx());
               putWord(14); // add
               break;

            case IR::Code::Retn:
               putStmnt_Retn();
               break;

            case IR::Code::ShLU_W:
               putWord(76); // lshift
               break;

            case IR::Code::ShRI_W:
               putWord(77); // rshift
               break;

            case IR::Code::SubI_W:
            case IR::Code::SubU_W:
               putWord(15); // subtract
               break;

            case IR::Code::Swap_W:
               putWord(217); // swap
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
            auto ret = ResolveValue(stmnt->args[1].aLit.value->getValue());

            switch(stmnt->args[0].a)
            {
            case IR::ArgBase::Lit:
               if(ret == 0)
                  putWord(204); // calldiscard
               else
                  putWord(203); // call

               putExpWord(stmnt->args[0].aLit.value);

               if(ret)
                  putStmntPushRetn(ret);

               break;

            case IR::ArgBase::Stk:
               putWord(360); // callstack

               if(ret == 0)
                  putWord(54); // drop
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
            auto ret = ResolveValue(stmnt->args[1].aLit.value->getValue());

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
                     putWord(3); // pushnumber
                     putExpWord(arg.aLit.value);
                  }

                  putWord(263); break; // lspec5result
                  putExpWord(stmnt->args[0].aLit.value);

                  break;
               }

               switch(stmnt->args.size() - 2)
               {
               case 0: putWord( 9); break; // lspec1direct
               case 1: putWord( 9); break; // lspec1direct
               case 2: putWord(10); break; // lspec2direct
               case 3: putWord(11); break; // lspec3direct
               case 4: putWord(12); break; // lspec4direct
               case 5: putWord(13); break; // lspec5direct
               }

               putExpWord(stmnt->args[0].aLit.value);

               // Dummy arg.
               if(stmnt->args.size() == 2)
                  putWord(0);

               for(auto const &arg : Core::MakeRange(stmnt->args.begin() + 2, stmnt->args.end()))
                  putExpWord(arg.aLit.value);

               break;

            case IR::ArgBase::Stk:
               if(ret)
               {
                  switch(stmnt->args.size() - 2)
                  {
                  case 0: putWord(3); putWord(0); // pushnumber
                  case 1: putWord(3); putWord(0); // pushnumber
                  case 2: putWord(3); putWord(0); // pushnumber
                  case 3: putWord(3); putWord(0); // pushnumber
                  case 4: putWord(3); putWord(0); // pushnumber
                  case 5: putWord(263); break; // lspec5result
                  }
               }
               else
               {
                  switch(stmnt->args.size() - 2)
                  {
                  case 0: putWord(3); putWord(0); // pushnumber
                  case 1: putWord(4); break; // lspec1
                  case 2: putWord(5); break; // lspec2
                  case 3: putWord(6); break; // lspec3
                  case 4: putWord(7); break; // lspec4
                  case 5: putWord(8); break; // lspec5
                  }
               }

               putExpWord(stmnt->args[0].aLit.value);

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
               putStmnt_Move_W__Stk_Arr(stmnt->args[1].aGblArr, 235); // pushglobalarray
               break;

            case IR::ArgBase::GblReg:
               putStmnt_Move_W__Stk_Reg(stmnt->args[1].aGblReg, 182); // pushglobalvar
               break;

            case IR::ArgBase::Lit:
               putStmnt_Move_W__Stk_Lit(stmnt->args[1].aLit.value);
               break;

            case IR::ArgBase::Loc:
               putWord(28); // pushscriptvar
               putWord(getStkPtrIdx());
               putWord(14); // add

            case IR::ArgBase::LocArs:
               if(!IsExp0(stmnt->args[1].aLocArs.off))
               {
                  putWord(3); // pushnumber
                  putExpWord(stmnt->args[1].aLocArs.off);
                  putWord(14); // add
               }

               putWord(235); // pushglobalarray
               putWord(LocArsArray);
               break;

            case IR::ArgBase::LocReg:
               putStmnt_Move_W__Stk_Reg(stmnt->args[1].aLocReg, 28); // pushscriptvar
               break;

            case IR::ArgBase::MapArr:
               putStmnt_Move_W__Stk_Arr(stmnt->args[1].aMapArr, 207); // pushmaparray
               break;

            case IR::ArgBase::MapReg:
               putStmnt_Move_W__Stk_Reg(stmnt->args[1].aMapReg, 29); // pushmapvar
               break;

            case IR::ArgBase::WldArr:
               putStmnt_Move_W__Stk_Arr(stmnt->args[1].aWldArr, 226); // pushworldarray
               break;

            case IR::ArgBase::WldReg:
               putStmnt_Move_W__Stk_Reg(stmnt->args[1].aWldReg, 30); // pushworldvar
               break;

            default:
               std::cerr << "bad Code::Move_W(Stk, ?)\n";
               throw EXIT_FAILURE;
            }

            // drop_?
            else if(stmnt->args[1].a == IR::ArgBase::Stk) switch(stmnt->args[0].a)
            {
            case IR::ArgBase::GblArr:
               putStmnt_Move_W__Arr_Stk(stmnt->args[0].aGblArr, 236); // assignglobalarray
               break;

            case IR::ArgBase::GblReg:
               putStmnt_Move_W__Reg_Stk(stmnt->args[0].aGblReg, 181); // assignglobalvar
               break;

            case IR::ArgBase::Loc:
               putWord(28); // pushscriptvar
               putWord(getStkPtrIdx());
               putWord(14); // add

            case IR::ArgBase::LocArs:
               if(!IsExp0(stmnt->args[0].aLocArs.off))
               {
                  putWord(3); // pushnumber
                  putExpWord(stmnt->args[0].aLocArs.off);
                  putWord(14); // add
               }

               putWord(217); // swap
               putWord(236); // assignglobalarray
               putWord(LocArsArray);
               break;

            case IR::ArgBase::LocReg:
               putStmnt_Move_W__Reg_Stk(stmnt->args[0].aLocReg, 25); // assignscriptvar
               break;

            case IR::ArgBase::MapArr:
               putStmnt_Move_W__Arr_Stk(stmnt->args[0].aMapArr, 208); // assignmaparray
               break;

            case IR::ArgBase::MapReg:
               putStmnt_Move_W__Reg_Stk(stmnt->args[0].aMapReg, 26); // assignmapvar
               break;

            case IR::ArgBase::Nul:
               putWord(54); // drop
               break;

            case IR::ArgBase::WldArr:
               putStmnt_Move_W__Arr_Stk(stmnt->args[0].aWldArr, 227); // assignworldarray
               break;

            case IR::ArgBase::WldReg:
               putStmnt_Move_W__Reg_Stk(stmnt->args[0].aWldReg, 27); // assignworldvar
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
         void Info::putStmnt_Move_W__Arr_Stk(IR::ArgPtr2 const &arr, Core::FastU i)
         {
            if(!IsExp0(arr.off))
            {
               putWord(3); // pushnumber
               putExpWord(arr.off);
               putWord(14); // add
            }

            putWord(217); // swap
            putWord(i);
            putExpWord(arr.arr->aLit.value);
         }

         //
         // Info::putStmnt_Move_W__Reg_Stk
         //
         void Info::putStmnt_Move_W__Reg_Stk(IR::ArgPtr1 const &reg, Core::FastU i)
         {
            putWord(i);

            if(reg.off)
               putExpWord(IR::ExpCreate_AddPtrRaw(reg.idx->aLit.value, reg.off));
            else
               putExpWord(reg.idx->aLit.value);
         }

         //
         // Info::putStmnt_Move_W__Stk_Arr
         //
         void Info::putStmnt_Move_W__Stk_Arr(IR::ArgPtr2 const &arr, Core::FastU i)
         {
            if(!IsExp0(arr.off))
            {
               putWord(3); // pushnumber
               putExpWord(arr.off);
               putWord(14); // add
            }

            putWord(i);
            putExpWord(arr.arr->aLit.value);
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
               putWord(359); // pushfunction
               putWord(ResolveValue(val));
               break;

            case IR::ValueBase::StrEn:
               putWord(3); // pushnumber
               putWord(ResolveValue(val));
               putWord(225); // tagstring
               break;

            default:
               putWord(3); // pushnumber
               putWord(ResolveValue(val));
               break;
            }
         }

         //
         // Info::putStmnt_Move_W__Stk_Reg
         //
         void Info::putStmnt_Move_W__Stk_Reg(IR::ArgPtr1 const &reg, Core::FastU i)
         {
            putWord(i);

            if(reg.off)
               putExpWord(IR::ExpCreate_AddPtrRaw(reg.idx->aLit.value, reg.off));
            else
               putExpWord(reg.idx->aLit.value);
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
                  putWord(205); // returnvoid
               else
               {
                  putStmntDropRetn(stmnt->args.size());

                  putWord(206); // returnval
               }
               break;

            case IR::CallType::ScriptI:
            case IR::CallType::ScriptS:
               if(stmnt->args.size() == 0)
               {
                  putWord(1); // terminate
               }
               else if(stmnt->args.size() == 1)
               {
                  putWord(257); // setresultvalue
                  putWord(  1); // terminate
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
         // Info::putStmntDropRetn
         //
         void Info::putStmntDropRetn(Core::FastU ret)
         {
            if(ret) for(Core::FastU i = 0; ++i != ret;)
            {
               putWord(3); // pushnumber
               putWord(-i);
               putWord(217); // swap
               putWord(236); // assignglobalarray
               putWord(LocArsArray);
            }
         }

         //
         // Info::putStmntPushRetn
         //
         void Info::putStmntPushRetn(Core::FastU ret)
         {
            if(ret) while(ret-- != 1)
            {
               putWord(3); // pushnumber
               putWord(-ret);
               putWord(235); // pushglobalarray
               putWord(LocArsArray);
            }
         }
      }
   }
}

// EOF

