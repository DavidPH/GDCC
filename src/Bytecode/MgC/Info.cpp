//-----------------------------------------------------------------------------
//
// Copyright (C) 2013 David Hill
//
// See COPYING for license information.
//
//-----------------------------------------------------------------------------
//
// MageCraft target information and handling class.
//
//-----------------------------------------------------------------------------

#include "Info.hpp"

#include "GDCC/IR/Function.hpp"

#include "GDCC/IR/Exp/ValueGlyph.hpp"
#include "GDCC/IR/Exp/ValueRoot.hpp"

#include <iostream>


//----------------------------------------------------------------------------|
// Static Functions                                                           |
//


//----------------------------------------------------------------------------|
// Global Functions                                                           |
//

namespace Bytecode
{
   namespace MgC
   {
      //
      // Info::put
      //
      void Info::put(std::ostream &out)
      {
         out << std::hex << "MgC_NTS" << '\0' << "CODEDEFS" << '\0' << '\0';

         for(auto const &itr : GDCC::IR::FunctionRange())
         {
            if(!itr.second.exdef)
               putFunction(out, itr.second);
         }
      }

      //
      // Info::putExp
      //
      void Info::putExp(std::ostream &out, GDCC::IR::Exp const *exp)
      {
         switch(auto s = static_cast<GDCC::StringIndex>(exp->getName()))
         {
         case GDCC::STR_ValueGlyph:
            putGlyph(out, static_cast<GDCC::IR::Exp_ValueGlyph const *>(exp)->glyph);
            break;

         case GDCC::STR_ValueRoot:
            putValue(out, static_cast<GDCC::IR::Exp_ValueRoot const *>(exp)->value);
            break;

         default:
            std::cerr << "bad Exp\n";
            throw EXIT_FAILURE;
         }
      }

      //
      // Info::putFunction
      //
      void Info::putFunction(std::ostream &out, GDCC::IR::Function const &func)
      {
         // Put the function header.
         out << "function" << '\0' << func.glyph << '\0'
            << '{' << '\0'
               << "argCount" << '\0' << '=' << '\0' << func.param    << '\0' << ';' << '\0'
               << "label"    << '\0' << '=' << '\0' << func.label    << '\0' << ';' << '\0'
               << "regCount" << '\0' << '=' << '\0' << func.localReg << '\0' << ';' << '\0'
               << "retCount" << '\0' << '=' << '\0' << func.retrn    << '\0' << ';' << '\0'
            << '}' << '\0';

         // Put the function code.
         out << "code" << '\0' << func.label << '\0'
            << '{' << '\0';

         for(auto const &stmnt : func.block)
            putStatement(out, stmnt);

         out << '}' << '\0';
      }

      //
      // Info::putGlyph
      //
      void Info::putGlyph(std::ostream &out, GDCC::IR::Glyph glyph)
      {
         out << '$' << static_cast<GDCC::String>(glyph) << '\0';
      }

      //
      // Info::putStatement
      //
      void Info::putStatement(std::ostream &out, GDCC::IR::Statement const &stmnt)
      {
         // Put labels.
         for(auto lab : stmnt.labs)
            out << "label" << '\0' << '(' << '\0' << lab << '\0' << ')' << '\0';

         // Put code and args.
         switch(stmnt.code)
         {
         case GDCC::IR::Code::Nop:
            out << "nop" << '\0' << '(' << '\0' << ')' << '\0';
            break;

         case GDCC::IR::Code::AddU_W:
            out << "addu_stk" << '\0' << '(' << '\0' << ')' << '\0';
            break;

         case GDCC::IR::Code::Call:
            out << "call_stk" << '\0' << '(' << '\0';
               putExp(out, stmnt.args[1].aLit.value);
            out << ')' << '\0';
            break;

         case GDCC::IR::Code::Cjmp_Nil:
            out << "cjmp_stk_nil" << '\0' << '(' << '\0';
               putExp(out, stmnt.args[1].aLit.value);
            out << ')' << '\0';
            break;

         case GDCC::IR::Code::Cjmp_Tru:
            out << "cjmp_stk_tru" << '\0' << '(' << '\0';
               putExp(out, stmnt.args[1].aLit.value);
            out << ')' << '\0';
            break;

         case GDCC::IR::Code::CmpU_EQ_W:
            out << "cmpu_stk_eq" << '\0' << '(' << '\0' << ')' << '\0';
            break;

         case GDCC::IR::Code::CmpU_GE_W:
            out << "cmpu_stk_ge" << '\0' << '(' << '\0' << ')' << '\0';
            break;

         case GDCC::IR::Code::CmpU_GT_W:
            out << "cmpu_stk_gt" << '\0' << '(' << '\0' << ')' << '\0';
            break;

         case GDCC::IR::Code::CmpU_LE_W:
            out << "cmpu_stk_le" << '\0' << '(' << '\0' << ')' << '\0';
            break;

         case GDCC::IR::Code::CmpU_LT_W:
            out << "cmpu_stk_lt" << '\0' << '(' << '\0' << ')' << '\0';
            break;

         case GDCC::IR::Code::CmpU_NE_W:
            out << "cmpu_stk_ne" << '\0' << '(' << '\0' << ')' << '\0';
            break;

         case GDCC::IR::Code::Jump:
            putStatement_Jump(out, stmnt);
            break;

         case GDCC::IR::Code::Move_W:
            putStatement_Move_W(out, stmnt);
            break;

         case GDCC::IR::Code::NotU_W:
            out << "notu_stk" << '\0' << '(' << '\0' << ')' << '\0';
            break;

         case GDCC::IR::Code::Retn:
            out << "retn" << '\0' << '(' << '\0' << ')' << '\0';
            break;

         case GDCC::IR::Code::SubU_W:
            out << "subu_stk" << '\0' << '(' << '\0' << ')' << '\0';
            break;

         default:
            std::cerr << "bad Code\n";
            throw EXIT_FAILURE;
         }
      }

      //
      // Info::putStatement_Jump
      //
      void Info::putStatement_Jump(std::ostream &out, GDCC::IR::Statement const &stmnt)
      {
         switch(stmnt.args[0].a)
         {
         case GDCC::IR::ArgBase::Lit:
            out << "jump_imm" << '\0' << '(' << '\0';
               putExp(out, stmnt.args[0].aLit.value);
            out << ')' << '\0';
            break;

         case GDCC::IR::ArgBase::Stk:
            out << "jump_stk" << '\0' << '(' << '\0' << ')' << '\0';
            break;

         default:
            std::cerr << "bad Code::Jump(?)\n";
            throw EXIT_FAILURE;
         }
      }

      //
      // Info::putStatement_Move_W
      //
      void Info::putStatement_Move_W(std::ostream &out, GDCC::IR::Statement const &stmnt)
      {
         // push_?
         if(stmnt.args[0].a == GDCC::IR::ArgBase::Stk) switch(stmnt.args[1].a)
         {
         case GDCC::IR::ArgBase::Lit:
            out << "push_imm" << '\0' << '(' << '\0';
               putExp(out, stmnt.args[1].aLit.value);
            out << ')' << '\0';
            break;

         case GDCC::IR::ArgBase::LocArs:
            switch(stmnt.args[1].aLocArs.idx->a)
            {
            case GDCC::IR::ArgBase::Lit:
               out << "push_dat" << '\0' << '(' << '\0';
                  out << '+' << '\0';
                     putExp(out, stmnt.args[1].aLocArs.off);
                     putExp(out, stmnt.args[1].aLocArs.idx->aLit.value);
               out << ')' << '\0';
               break;

            case GDCC::IR::ArgBase::Stk:
               out << "push_ptr" << '\0' << '(' << '\0';
                  putExp(out, stmnt.args[1].aLocArs.off);
               out << ')' << '\0';
               break;

            default:
               std::cerr << "bad Code::Move_W(Stk, LocArs(?))\n";
               throw EXIT_FAILURE;
            }
            break;

         case GDCC::IR::ArgBase::LocReg:
            out << "push_reg" << '\0' << '(' << '\0';
               putExp(out, stmnt.args[1].aLocReg.idx->aLit.value);
            out << ')' << '\0';
            break;

         default:
            std::cerr << "bad Code::Move_W(Stk, ?)\n";
            throw EXIT_FAILURE;
         }

         // drop_?
         else if(stmnt.args[1].a == GDCC::IR::ArgBase::Stk) switch(stmnt.args[0].a)
         {
         case GDCC::IR::ArgBase::Nul:
            out << "drop_nul" << '\0' << '(' << '\0' << ')' << '\0';
            break;

         case GDCC::IR::ArgBase::LocArs:
            switch(stmnt.args[0].aLocArs.idx->a)
            {
            case GDCC::IR::ArgBase::Lit:
               out << "drop_dat" << '\0' << '(' << '\0';
                  out << '+' << '\0';
                     putExp(out, stmnt.args[0].aLocArs.off);
                     putExp(out, stmnt.args[0].aLocArs.idx->aLit.value);
               out << ')' << '\0';
               break;

            case GDCC::IR::ArgBase::Stk:
               out << "drop_ptr" << '\0' << '(' << '\0';
                  putExp(out, stmnt.args[0].aLocArs.off);
               out << ')' << '\0';
               break;

            default:
               std::cerr << "bad Code::Move_W(LocArs(?), Stk)\n";
               throw EXIT_FAILURE;
            }
            break;

         case GDCC::IR::ArgBase::LocReg:
            out << "drop_reg" << '\0' << '(' << '\0';
               putExp(out, stmnt.args[0].aLocReg.idx->aLit.value);
            out << ')' << '\0';
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
      // Info::putValue
      //
      void Info::putValue(std::ostream &out, GDCC::IR::Value const &val)
      {
         switch(val.v)
         {
         case GDCC::IR::ValueBase::Fixed:
            out << val.vFixed.value << '\0';
            break;

         default:
            std::cerr << "bad Value\n";
            throw EXIT_FAILURE;
         }
      }
   }
}

// EOF

