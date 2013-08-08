//-----------------------------------------------------------------------------
//
// Copyright (C) 2013 David Hill
//
// See COPYING for license information.
//
//-----------------------------------------------------------------------------
//
// ZDoom ACS IR code output.
//
//-----------------------------------------------------------------------------

#include "Info.hpp"

#include "GDCC/IR/Function.hpp"
#include "GDCC/IR/Glyph.hpp"

#include <iostream>


//----------------------------------------------------------------------------|
// Global Functions                                                           |
//

namespace Bytecode
{
   namespace ZDACS
   {
      //
      // Info::put
      //
      void Info::put(std::ostream &out)
      {
         // Put header.
         out.write("ACSE", 4);
         putWord(out, jumpPos);
         out.write("GDCC::BC", 8);

         // Put statements.
         for(auto &func : GDCC::IR::FunctionRange()) try
         {
            curFunc = &func.second;
            for(auto const &stmnt : func.second.block)
               putStmnt(out, stmnt);
            curFunc = nullptr;
         }
         catch(...) {curFunc = nullptr; throw;}

         // Put chunks.
         putChunk(out);
      }

      //
      // Info::putByte
      //
      void Info::putByte(std::ostream &out, GDCC::FastU i)
      {
         out.put(i & 0xFF);
      }

      //
      // Info::putExpWord
      //
      void Info::putExpWord(std::ostream &out, GDCC::IR::Exp const *exp)
      {
         putWord(out, ResolveValue(exp->getValue()));
      }

      //
      // Info::putHWord
      //
      void Info::putHWord(std::ostream &out, GDCC::FastU i)
      {
         out.put((i >> 0) & 0xFF);
         out.put((i >> 8) & 0xFF);
      }

      //
      // Info::putStmnt
      //
      void Info::putStmnt(std::ostream &out, GDCC::IR::Statement const &stmnt)
      {
         switch(stmnt.code)
         {
         case GDCC::IR::Code::Nop:
            putWord(out, 0);
            break;

         case GDCC::IR::Code::AddI_W:
         case GDCC::IR::Code::AddU_W:
            putWord(out, 14); // add
            break;

         case GDCC::IR::Code::AndU_W:
            putWord(out, 72); // andbitwise
            break;

         case GDCC::IR::Code::Call:
            putStmnt_Call(out, stmnt);
            break;

         case GDCC::IR::Code::Casm:
            for(auto const &arg : stmnt.args)
               putExpWord(out, arg.aLit.value);
            break;

         case GDCC::IR::Code::CmpI_EQ_W:
         case GDCC::IR::Code::CmpU_EQ_W:
            putWord(out, 19); // eq
            break;

         case GDCC::IR::Code::CmpI_GE_W:
            putWord(out, 24); // ge
            break;

         case GDCC::IR::Code::CmpI_GT_W:
            putWord(out, 22); // gt
            break;

         case GDCC::IR::Code::CmpI_LE_W:
            putWord(out, 23); // le
            break;

         case GDCC::IR::Code::CmpI_LT_W:
            putWord(out, 21); // lt
            break;

         case GDCC::IR::Code::CmpI_NE_W:
         case GDCC::IR::Code::CmpU_NE_W:
            putWord(out, 20); // ne
            break;

         case GDCC::IR::Code::Cnat:
            putWord(out, 351); // callfunc
            putExpWord(out, stmnt.args[0].aLit.value);
            putWord(out, stmnt.args.size() - 2);
            break;

         case GDCC::IR::Code::Cspe:
            putStmnt_Cspe(out, stmnt);
            break;

         case GDCC::IR::Code::DivI_W:
            putWord(out, 17); // divide
            break;

         case GDCC::IR::Code::DivX_W:
            putWord(out, 137); // fixeddiv
            break;

         case GDCC::IR::Code::InvU_W:
            putWord(out, 330); // negatebinary
            break;

         case GDCC::IR::Code::ModI_W:
            putWord(out, 18); // modulus
            break;

         case GDCC::IR::Code::Move_W:
            putStmnt_Move_W(out, stmnt);
            break;

         case GDCC::IR::Code::MulI_W:
            putWord(out, 16); // multiply
            break;

         case GDCC::IR::Code::MulX_W:
            putWord(out, 136); // fixedmul
            break;

         case GDCC::IR::Code::NegI_W:
            putWord(out, 78); // unaryminus
            break;

         case GDCC::IR::Code::NotU_W:
            putWord(out, 75); // negatelogical
            break;

         case GDCC::IR::Code::OrIU_W:
            putWord(out, 73); // orbitwise
            break;

         case GDCC::IR::Code::OrXU_W:
            putWord(out, 74); // eorbitwise
            break;

         case GDCC::IR::Code::Retn:
            putStmnt_Retn(out, stmnt);
            break;

         case GDCC::IR::Code::ShLU_W:
            putWord(out, 76); // lshift
            break;

         case GDCC::IR::Code::ShRI_W:
            putWord(out, 77); // rshift
            break;

         case GDCC::IR::Code::SubI_W:
         case GDCC::IR::Code::SubU_W:
            putWord(out, 15); // subtract
            break;

         case GDCC::IR::Code::Swap_W:
            putWord(out, 217); // swap
            break;

         default:
            std::cerr << "ERROR: " << stmnt.pos << ": cannot put Code for ZDACS: "
               << stmnt.code << '\n';
            throw EXIT_FAILURE;
         }
      }

      //
      // Info::putStmnt_Cspe
      //
      void Info::putStmnt_Cspe(std::ostream &out, GDCC::IR::Statement const &stmnt)
      {
         auto ret = ResolveValue(stmnt.args[1].aLit.value->getValue());

         GDCC::IR::ArgBase a;
         if(stmnt.args.size() == 2)
            a = ret ? GDCC::IR::ArgBase::Stk : GDCC::IR::ArgBase::Lit;
         else
            a = stmnt.args[2].a;

         switch(a)
         {
         case GDCC::IR::ArgBase::Lit:
            if(ret)
            {
               for(auto const &arg : GDCC::MakeRange(stmnt.args.begin() + 2, stmnt.args.end()))
               {
                  putWord(out, 3); // pushnumber
                  putExpWord(out, arg.aLit.value);
               }

               putWord(out, 263); break; // lspec5result
               putExpWord(out, stmnt.args[0].aLit.value);

               break;
            }

            switch(stmnt.args.size() - 2)
            {
            case 0: putWord(out,  9); break; // lspec1direct
            case 1: putWord(out,  9); break; // lspec1direct
            case 2: putWord(out, 10); break; // lspec2direct
            case 3: putWord(out, 11); break; // lspec3direct
            case 4: putWord(out, 12); break; // lspec4direct
            case 5: putWord(out, 13); break; // lspec5direct
            }

            putExpWord(out, stmnt.args[0].aLit.value);

            // Dummy arg.
            if(stmnt.args.size() == 2)
               putWord(out, 0);

            for(auto const &arg : GDCC::MakeRange(stmnt.args.begin() + 2, stmnt.args.end()))
               putExpWord(out, arg.aLit.value);

            break;

         case GDCC::IR::ArgBase::Stk:
            if(ret)
            {
               switch(stmnt.args.size() - 2)
               {
               case 0: putWord(out, 3); putWord(out, 0); // pushnumber
               case 1: putWord(out, 3); putWord(out, 0); // pushnumber
               case 2: putWord(out, 3); putWord(out, 0); // pushnumber
               case 3: putWord(out, 3); putWord(out, 0); // pushnumber
               case 4: putWord(out, 3); putWord(out, 0); // pushnumber
               case 5: putWord(out, 263); break; // lspec5result
               }
            }
            else
            {
               switch(stmnt.args.size() - 2)
               {
               case 0: putWord(out, 3); putWord(out, 0); // pushnumber
               case 1: putWord(out, 4); break; // lspec1
               case 2: putWord(out, 5); break; // lspec2
               case 3: putWord(out, 6); break; // lspec3
               case 4: putWord(out, 7); break; // lspec4
               case 5: putWord(out, 8); break; // lspec5
               }
            }

            putExpWord(out, stmnt.args[0].aLit.value);

            break;

         default:
            std::cerr << "ERROR: " << stmnt.pos << ": bad Cspe\n";
            throw EXIT_FAILURE;
         }
      }

      //
      // Info::putStmnt_Move_W
      //
      void Info::putStmnt_Move_W(std::ostream &out, GDCC::IR::Statement const &stmnt)
      {
         // push_?
         if(stmnt.args[0].a == GDCC::IR::ArgBase::Stk) switch(stmnt.args[1].a)
         {
         case GDCC::IR::ArgBase::GblArr:
            putStmnt_Move_W__Stk_Arr(out, stmnt.args[1].aGblArr, 235); // pushglobalarray
            break;

         case GDCC::IR::ArgBase::GblReg:
            putWord(out, 182); // pushglobalvar
            putExpWord(out, stmnt.args[1].aGblReg.idx->aLit.value);
            break;

         case GDCC::IR::ArgBase::Lit:
            putStmnt_Move_W__Stk_Lit(out, stmnt.args[1].aLit.value);
            break;

         case GDCC::IR::ArgBase::LocReg:
            putWord(out, 28); // pushscriptvar
            putExpWord(out, stmnt.args[1].aLocReg.idx->aLit.value);
            break;

         case GDCC::IR::ArgBase::MapArr:
            putStmnt_Move_W__Stk_Arr(out, stmnt.args[1].aMapArr, 207); // pushmaparray
            break;

         case GDCC::IR::ArgBase::MapReg:
            putWord(out, 29); // pushmapvar
            putExpWord(out, stmnt.args[1].aMapReg.idx->aLit.value);
            break;

         case GDCC::IR::ArgBase::WldArr:
            putStmnt_Move_W__Stk_Arr(out, stmnt.args[1].aWldArr, 226); // pushworldarray
            break;

         case GDCC::IR::ArgBase::WldReg:
            putWord(out, 30); // pushworldvar
            putExpWord(out, stmnt.args[1].aWldReg.idx->aLit.value);
            break;

         default:
            std::cerr << "bad Code::Move_W(Stk, ?)\n";
            throw EXIT_FAILURE;
         }

         // drop_?
         else if(stmnt.args[1].a == GDCC::IR::ArgBase::Stk) switch(stmnt.args[0].a)
         {
         case GDCC::IR::ArgBase::GblArr:
            putStmnt_Move_W__Arr_Stk(out, stmnt.args[1].aGblArr, 236); // assignglobalarray
            break;

         case GDCC::IR::ArgBase::GblReg:
            putWord(out, 181); // assignglobalvar
            putExpWord(out, stmnt.args[0].aGblReg.idx->aLit.value);
            break;

         case GDCC::IR::ArgBase::LocReg:
            putWord(out, 25); // assignscriptvar
            putExpWord(out, stmnt.args[0].aLocReg.idx->aLit.value);
            break;

         case GDCC::IR::ArgBase::MapArr:
            putStmnt_Move_W__Arr_Stk(out, stmnt.args[1].aMapArr, 208); // assignmaparray
            break;

         case GDCC::IR::ArgBase::MapReg:
            putWord(out, 26); // assignmapvar
            putExpWord(out, stmnt.args[0].aMapReg.idx->aLit.value);
            break;

         case GDCC::IR::ArgBase::Nul:
            putWord(out, 54); // drop
            break;

         case GDCC::IR::ArgBase::WldArr:
            putStmnt_Move_W__Arr_Stk(out, stmnt.args[1].aWldArr, 227); // assignworldarray
            break;

         case GDCC::IR::ArgBase::WldReg:
            putWord(out, 27); // assignworldvar
            putExpWord(out, stmnt.args[0].aWldReg.idx->aLit.value);
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
      void Info::putStmnt_Move_W__Arr_Stk(std::ostream &out,
         GDCC::IR::ArgPtr2 const &arr, GDCC::FastU i)
      {
         if(!IsExp0(arr.off))
         {
            putWord(out, 3); // pushnumber
            putExpWord(out, arr.off);
            putWord(out, 14); // add
         }

         putWord(out, 217); // swap
         putWord(out, i);
         putExpWord(out, arr.arr->aLit.value);
      }

      //
      // Info::putStmnt_Move_W__Stk_Arr
      //
      void Info::putStmnt_Move_W__Stk_Arr(std::ostream &out,
         GDCC::IR::ArgPtr2 const &arr, GDCC::FastU i)
      {
         if(!IsExp0(arr.off))
         {
            putWord(out, 3); // pushnumber
            putExpWord(out, arr.off);
            putWord(out, 14); // add
         }

         putWord(out, i);
         putExpWord(out, arr.arr->aLit.value);
      }

      //
      // Info::putStmnt_Retn
      //
      void Info::putStmnt_Retn(std::ostream &out, GDCC::IR::Statement const &stmnt)
      {
         switch(curFunc->ctype)
         {
         case GDCC::IR::CallType::LangACS:
            if(stmnt.args.size() == 0)
               putWord(out, 205); // returnvoid

            else if(stmnt.args.size() == 1)
               putWord(out, 206); // returnval

            else
            {
               std::cerr << "STUB: " __FILE__ << ':' << __LINE__ << '\n';
               throw EXIT_FAILURE;
            }
            break;

         case GDCC::IR::CallType::Script:
         case GDCC::IR::CallType::ScriptI:
         case GDCC::IR::CallType::ScriptS:
            if(stmnt.args.size() == 0)
            {
               putWord(out, 1); // terminate
            }
            else if(stmnt.args.size() == 1)
            {
               putWord(out, 257); // setresultvalue
               putWord(out,   1); // terminate
            }
            else
            {
               std::cerr << "STUB: " __FILE__ << ':' << __LINE__ << '\n';
               throw EXIT_FAILURE;
            }
            break;

         default:
            std::cerr << "ERROR: " << stmnt.pos << ": bad Code::Retn\n";
            throw EXIT_FAILURE;
         }
      }

      //
      // Info::putWord
      //
      void Info::putWord(std::ostream &out, GDCC::FastU i)
      {
         out.put((i >>  0) & 0xFF);
         out.put((i >>  8) & 0xFF);
         out.put((i >> 16) & 0xFF);
         out.put((i >> 24) & 0xFF);
      }
   }
}

// EOF

